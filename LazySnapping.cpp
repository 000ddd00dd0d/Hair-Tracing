#include "LazySnapping.h"
#include <set>
using namespace std;

//private functions
float LazySnapping::minDistance(double *color, set<int> cluster){
	float distance = -1;
	for (set<int>::iterator i = cluster.begin(); i != cluster.end(); i++){
		CvScalar c = this->color[*i];
		float d = (color[0] - c.val[0]) * (color[0] - c.val[0]) +
			(color[1] - c.val[1]) * (color[1] - c.val[1]) +
			(color[2] - c.val[2]) * (color[2] - c.val[2]);
		if (distance < 0) distance = d;
		else if (distance > d) distance = d;
	}
	if (distance == -1) return 0;
	return distance;
}

void LazySnapping::getE1(double *color, float *energy){
	float df = minDistance(color, foreCluster);
	float db = minDistance(color, backCluster);
	energy[0] = df / (db + df);
	energy[1] = db / (db + df);
}

float LazySnapping::getE2(double *color1, double *color2){
	const float eps = 1;
	float lambda = 1;
	return lambda / (eps +
		(color1[0] - color2[0]) * (color1[0] - color2[0]) +
		(color1[1] - color2[1]) * (color1[1] - color2[1]) +
		(color1[2] - color2[2]) * (color1[2] - color2[2]));
}

//public functions

void LazySnapping::setForeBackCluster(vector<CvPoint> fore, vector<CvPoint> back){
	int *foreCnt = new int[count + 1];
	int *backCnt = new int[count + 1];
	memset(foreCnt, 0, sizeof(int) * (count + 1));
	memset(backCnt, 0, sizeof(int) * (count + 1));
	for (int i = 0; i < fore.size(); ++i){
		int idx = CV_IMAGE_ELEM(marker, int, fore[i].y, fore[i].x);
		if (idx <= 0 || idx > count) continue;
		foreCnt[idx]++;
	}
	for (int i = 0; i < back.size(); ++i){
		int idx = CV_IMAGE_ELEM(marker, int, back[i].y, back[i].x);
		if (idx <= 0 || idx > count) continue;
		backCnt[idx]++;
	}
	foreCluster.clear();
	backCluster.clear();
	for (int i = 1; i <= count; ++i){
		if (foreCnt[i] == 0 && backCnt[i] == 0) continue;
		if (foreCnt[i] >= backCnt[i]){
			foreCluster.insert(i);
		}
		else{
			backCluster.insert(i);
		}
	}
	delete[] foreCnt;
	delete[] backCnt;
}

int LazySnapping::runMaxflow(){
	const float INF = 1e10;
	for (int i = 1; i <= count; ++i){
		float e1[2] = { 0, 0 };
		if (foreCluster.count(i)){
			e1[0] = 0;
			e1[1] = INF;
		}
		else if (backCluster.count(i)){
			e1[0] = INF;
			e1[1] = 0;
		}
		else{
			getE1(color[i].val, e1);
		}
		graph->add_node();
		graph->add_tweights(i - 1, e1[0], e1[1]);
	}
	set<pair<int, int> > edges;
	for (int h = 1; h < marker->height; ++h){
		for (int w = 1; w < marker->width; ++w){
			int cur = CV_IMAGE_ELEM(marker, int, h, w);
			int bef = CV_IMAGE_ELEM(marker, int, h - 1, w);
			int down = CV_IMAGE_ELEM(marker, int, h, w - 1);
			if (cur == -1) continue;
			if (cur == bef && cur == down) continue;
			if (bef != -1 && cur != bef){
				int min = cur >= bef ? bef : cur;
				int max = cur >= bef ? cur : bef;
				edges.insert(pair<int, int>(min, max));
			}
			if (down != -1 && cur != down){
				int min = cur >= down ? down : cur;
				int max = cur >= down ? cur : down;
				edges.insert(pair<int, int>(min, max));
			}
		}
	}
	
	for (set<pair<int, int> >::iterator i = edges.begin(); i != edges.end(); i++){
		int p1 = (*i).first;
		int p2 = (*i).second;
		float e2 = getE2(color[p1].val, color[p2].val);
		graph->add_edge(p1 - 1, p2 - 1, e2, e2);
	}
	return graph->maxflow();
}

IplImage* LazySnapping::getImageMask(){
	IplImage *gray = cvCreateImage(cvGetSize(image), 8, 1);
	
	for (int h = 0; h < image->height; ++h){
		unsigned char *p = (unsigned char*)gray->imageData + h * gray->widthStep;
		for (int w = 0; w < image->width; ++w){
			int id = CV_IMAGE_ELEM(marker, int, h, w);
			if (graph->what_segment(id - 1) == GraphType::SOURCE)
				*p = 0;
			else
				*p = 255;
			p++;;
		}
	}
	graph->reset();
	return gray;
}

void LazySnapping::addForePt(int x, int y){
	forePts.push_back(cvPoint(x, y));
}
void LazySnapping::addBackPt(int x, int y){
	backPts.push_back(cvPoint(x, y));
}

void LazySnapping::setInput(IplImage *image){
	this->image = image;
	graph = new GraphType(image->width * image->height,
		image->width * image->height * 2);
	markerMask = cvCreateImage(cvGetSize(image), 8, 1);
	marker = cvCreateImage(cvGetSize(image), IPL_DEPTH_32S, 1);
	cvZero(markerMask);
	cvZero(marker);
}


bool LazySnapping::doLazySnapping(){
	if (forePts.size() == 0 && backPts.size() == 0) return false;
	watershed();
	color = new CvScalar[count + 1];
	for (int i = 0; i < count + 1; ++i)
		memset(color[i].val, 0, sizeof(double) * 4);
	cnt = new int[count + 1];
	for (int i = 0; i < marker->height; ++i){
		for (int j = 0; j < marker->width; ++j){
			int idx = CV_IMAGE_ELEM(marker, int, i, j);
			uchar* dst = &CV_IMAGE_ELEM(image, uchar, i, j * 3);
			if (idx <= 0 || idx > count){
				continue;
			}
			cnt[idx]++;
			color[idx].val[0] += dst[0];
			color[idx].val[1] += dst[1];
			color[idx].val[2] += dst[2];
		}
	}
	for (int i = 0; i < count; ++i){
		color[i].val[0] /= 1.0 * cnt[i];
		color[i].val[1] /= 1.0 * cnt[i];
		color[i].val[2] /= 1.0 * cnt[i];
	}
	setForeBackCluster(forePts, backPts);
	runMaxflow();
	return true;
}


void LazySnapping::watershed(){
	//watershed using cv lib
	for (int i = 0; i < marker->height; i++)
	{
		for (int j = 0; j < marker->width; j++)
		{
			if (i % 64 == 0 && j % 64 == 0){
				cvCircle(markerMask, cvPoint(j, i), 1, cvScalarAll(255));
			}
		}
	}
	CvMemStorage *storage = cvCreateMemStorage(0);
	CvSeq *contours = NULL;
	cvFindContours(markerMask, storage, &contours, sizeof(CvContour),
		CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	cvZero(marker);
	
	count = 0;
	for (; contours != 0; contours = contours->h_next, count++){
		cvDrawContours(marker, contours, cvScalarAll(count + 1),
			cvScalarAll(count + 1), -1, -1, 8, cvPoint(0, 0));
	}
	cvWatershed(image, marker);
	
	//draw the watershed result
	CvMat *color_tab = cvCreateMat(1, count, CV_8UC3);
	CvRNG rng = cvRNG(-1);
	wshed = cvCloneImage(image);
	int dir[][2] = { { -1, 0 }, { 0, -1 }, { 0, 1 }, { 1, 0 },
		{ -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, -1 } };
	for (int i = 0; i < count; i++)
	{
		uchar* ptr = color_tab->data.ptr + i * 3;
		ptr[0] = (uchar)(cvRandInt(&rng) % 180 + 50);
		ptr[1] = (uchar)(cvRandInt(&rng) % 180 + 50);
		ptr[2] = (uchar)(cvRandInt(&rng) % 180 + 50);
	}
	for (int i = 0; i < marker->height; i++){
		for (int j = 0; j < marker->width; j++)
		{
			int* idpointer = &CV_IMAGE_ELEM(marker, int, i, j);
			int idx = *idpointer;
			uchar* dst = &CV_IMAGE_ELEM(wshed, uchar, i, j * 3);
			
			if (idx == -1){
				dst[0] = dst[1] = dst[2] = (uchar)255;

			}
			else if (idx <= 0 || idx > count)
				dst[0] = dst[1] = dst[2] = (uchar)0; // should not get here
			else
			{
				uchar* ptr = color_tab->data.ptr + (idx - 1) * 3;
				dst[0] = ptr[0];
			}

			//fill the blanks
			if (idx <= 0 || idx > count){
				for (int k = 0; k < 8; ++k){
					int ti = i + dir[k][0], tj = j + dir[k][1];
					if (ti > 0 && tj > 0 && ti < marker->height && tj < marker->width){
						int tidx = CV_IMAGE_ELEM(marker, int, ti, tj);
						if (tidx > 0 && tidx < count){
							*idpointer = tidx;
							break;
						}
					}
				}
			}
		}
	}
	CV_IMAGE_ELEM(marker, int, 0, 0) = CV_IMAGE_ELEM(marker, int, 0, 1);
	//cvShowImage("watershed", wshed);
}

void LazySnapping::addSeeds(int x, int y){
	cvCircle(markerMask, cvPoint(x, y), 1, cvScalarAll(255));
}

void LazySnapping::reset(){
	graph->reset();
	forePts.clear();
	backPts.clear();
	foreCluster.clear();
	backCluster.clear();
	cvZero(markerMask);
	cvZero(marker);
	//if (color) delete[] color;
	//if (cnt) delete[] cnt;
	count = 0;
}