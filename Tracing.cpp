#include "Tracing.h"
#include <vector>
using namespace std;
#define max(a,b) (a > b ? a : b)
#define min(a,b) (a < b ? a : b)

double PI = atan2(0, -1);
void Tracing::calKernel(){
	double sigmau = 1.8;
	double sigmav = 2.4;
	double lambda = 4;
	double sum = 0;
	for (int ri = 0; ri < 32; ++ri){
		sum = 0;
		double theta = PI / 32 * ri;
		for (int i = 0; i < wins; ++i){
			for (int j = 0; j < wins; ++j){
				double x = (j - wins / 2) * 1;
				double y = (i - wins / 2) * 1;
				double tx = x * cos(theta) + y * sin(theta);
				double ty = -x * sin(theta) + y * cos(theta);
				double t1 = exp(-(pow(tx, 2) / pow(sigmau, 2) + pow(ty, 2) / pow(sigmav, 2)) / 2);
				double t2 = cos(2 * PI * tx / lambda);
				kernel[ri][i][j] = t1 * t2;
				sum += kernel[ri][i][j];
			}
		}
	}
}

IplImage *Tracing::gaborFilter(IplImage *I, IplImage *mask, int ri){
	//IplImage *rI = cvCreateImage(cvSize(I->width, I->height), 8, 1);
	IplImage *tmp = cvCreateImage(cvSize(I->width, I->height), IPL_DEPTH_64F, 1);
	cvScale(tmp, tmp, 0);
	int cnt = 0;
	for (int i = 0; i < I->height; ++i){
		for (int j = 0; j < I->width; ++j){
			if (CV_IMAGE_ELEM(mask, uchar, i, j) == 0) continue;
			double a = 0;
			for (int ii = -wins / 2; ii <= wins / 2; ++ii){
				for (int jj = -wins / 2; jj <= wins / 2; ++jj){
					int x = j + ii, y = i + jj;
					int tx = wins / 2 + ii, ty = wins / 2 + jj;
					if (x < 0 || y < 0 || x >= I->width || y >= I->height || CV_IMAGE_ELEM(mask, uchar, y, x) == 0){
						a += kernel[ri][tx][ty] * CV_IMAGE_ELEM(I, double, i, j);
					}
					else
						a += kernel[ri][tx][ty] * CV_IMAGE_ELEM(I, double, y, x);
				}
			}
			CV_IMAGE_ELEM(tmp, double, i, j) = a;
		}
	}
	//cout << minm << ' ' << maxm << endl;
	return tmp;
}

CvScalar Tracing::calColor(int i){
	CvScalar t;
	t.val[0] = t.val[1] = t.val[2] = 0;
	if (i >= 0 && i < 30){
		int ti = i + 30;
		t.val[1] = 255 - 255.0 / 30 * ti;
		t.val[2] = 255;
	}
	else if (i < 60){
		int ti = i - 30;
		t.val[2] = 255;
		t.val[0] = 255.0 / 30 * ti;
	}
	else if (i < 90){
		int ti = i - 30;
		t.val[2] = 255 - 255.0 / 30 * ti;
		t.val[0] = 255;
	}
	else if (i < 120){
		int ti = i - 90;
		t.val[0] = 255;
		t.val[1] = 255.0 / 30 * ti;
	}
	else if (i < 150){
		int ti = i - 120;
		t.val[0] = 255 - 255.0 / 30 * ti;
		t.val[1] = 255;
	}
	else if (i < 180){
		int ti = i - 150;

		t.val[1] = 255;// -255.0 / 60 * i;
		t.val[2] = 255.0 / 30 * ti;
	}
	return t;
}

void Tracing::gaborProcess(IplImage *I, IplImage *mask){

	for (int i = 0; i < 32; i += 1){
		r[i] = gaborFilter(I, mask, i);
	}
	res = cvCreateImage(cvSize(I->width, I->height), 8, 3);
	thetaMap = cvCreateImage(cvSize(I->width, I->height), 8, 1);
	cvScale(res, res, 0);
	cvScale(thetaMap, thetaMap, 0);
	for (int i = 0; i < I->height; ++i){
		for (int j = 0; j < I->width; ++j){
			if (CV_IMAGE_ELEM(mask, uchar, i, j) == 0) continue;
			uchar *p = (uchar*)(res->imageData + i * res->widthStep);
			double m = -1e10;
			int theta = 0;
			for (int ri = 0; ri < 32; ++ri){
				if (m < CV_IMAGE_ELEM(r[ri], double, i, j)){
					m = CV_IMAGE_ELEM(r[ri], double, i, j);
					theta = ri;
				}
			}
			theta *= 5;
			CvScalar t = calColor(theta);
			for (int c = 0; c < 3; ++c)
				p[c + j * res->nChannels] = t.val[c];
			CV_IMAGE_ELEM(thetaMap, uchar, i, j) = theta;
		}
	}

	
	double minm, maxm;
	weight = cvCreateImage(cvSize(I->width, I->height), IPL_DEPTH_64F, 1);

	minm = 1e10, maxm = -1e10;
	for (int i = 0; i < I->height; ++i){
		for (int j = 0; j < I->width; ++j){
			if (CV_IMAGE_ELEM(mask, uchar, i, j) == 0) continue;

			double sum = 0;
			for (int ri = 0; ri < 32; ++ri){
				int theta = ri * 5;
				int thetaHat = CV_IMAGE_ELEM(thetaMap, uchar, i, j);
				double a = max(theta, thetaHat);
				double b = min(theta, thetaHat);
				double d = min(fabs(a - b), fabs(b + 180 - a)) * PI / 180;
				int ti = thetaHat / 5;
				double dF = CV_IMAGE_ELEM(r[ri], double, i, j) - CV_IMAGE_ELEM(r[ti], double, i, j);
				sum += sqrt(d * dF * dF);
			}
			CV_IMAGE_ELEM(weight, double, i, j) = sum;
			minm = min(sum, minm);
			maxm = max(sum, maxm);
		}
	}
	cvScale(weight, weight, 1 / maxm);
}

int round1(double a){
	if (a - int(a) < 0.5) return (int)a;
	else return (int)a + 1;
}

void Tracing::tracing(IplImage *mask){
	double whigh = 0.1;
	double wlow = 0.02;
	double epsilon = 0.4;
	double delta = 0.7;
	double step1 = 3, step2 = 1;
	double dis = 0.7;
	int len = 10;
	IplImage *w = cvCreateImage(cvSize(mask->width, mask->height), 8, 1);
	cvScale(w, w, 0);
	for (int i = 0; i < weight->height; ++i){
		for (int j = 0; j < weight->width; ++j){
			if (CV_IMAGE_ELEM(mask, uchar, i, j) == 0) continue;
			if (CV_IMAGE_ELEM(weight, double, i, j) <= whigh) continue;
			int theta = CV_IMAGE_ELEM(thetaMap, uchar, i, j);
			int theta2;
			if (theta < 90){
				theta2 = 90 + theta;
			}
			else{
				theta2 = theta - 90;
			}
			double a = cos(theta2 * PI / 180);
			double b = sin(theta2 * PI / 180);
			int x1, y1, x2, y2;
			x1 = round1(j + a * delta);
			y1 = round1(i - b * delta);
			x2 = round1(j - a * delta);
			y2 = round1(i + b * delta);;


			double t1 = 0, t2 = 0;

			if (y1 >= 0 && x1 >= 0 && y1 < mask->height && x1 < mask->width && CV_IMAGE_ELEM(mask, uchar, y1, x1))
				t1 = CV_IMAGE_ELEM(weight, double, y1, x1);
			if (y2 >= 0 && x2 >= 0 && y2 < mask->height && x2 < mask->width && CV_IMAGE_ELEM(mask, uchar, y2, x2))
				t2 = CV_IMAGE_ELEM(weight, double, y2, x2);
			double t3 = CV_IMAGE_ELEM(weight, double, i, j);
			if ((t3 - max(t1, t2)) / t3 > epsilon)
				CV_IMAGE_ELEM(w, uchar, i, j) = 255;
		}
	}
	cvShowImage("w", w);
	CvVideoWriter *pVW = cvCreateVideoWriter("tmp.avi", CV_FOURCC('P', 'I', 'M', '1'), 25, cvSize(mask->width, mask->height), 0);
	trace = cvCloneImage(w);
	cvScale(trace, trace, 0, 0);
	for (int i = 0; i < w->height; ++i){
		for (int j = 0; j < w->width; ++j){
			if (CV_IMAGE_ELEM(w, uchar, i, j) == 0) continue;
			int hp = len;
			int bi = i, bj = j;
			int ni = i, nj = j;
			int certain = 1;
			vector<int> t;
			double pi = i, pj = j;
			int bef = CV_IMAGE_ELEM(thetaMap, uchar, i, j) + 180;
			while (hp){
				//CV_IMAGE_ELEM(trace, uchar, ni, nj) = 255;
				cvLine(trace, cvPoint(nj, ni), cvPoint(bj, bi), cvScalarAll(255));
				//CV_IMAGE_ELEM(w, uchar, ni, nj) = 0;
				cvLine(w, cvPoint(nj, ni), cvPoint(bj, bi), cvScalarAll(0));
				int theta;
				if (certain){
					theta = CV_IMAGE_ELEM(thetaMap, uchar, ni, nj);
					double x1 = cos(theta * PI / 180), y1 = sin(theta * PI / 180);
					double x2 = cos(bef * PI / 180), y2 = sin(bef * PI / 180);
					double mul = x1 * x2 + y1 * y2;
					if (mul < 0) theta = (theta + 180) % 360;
					bef = theta;
					t.push_back(min((theta - bef + 360) % 360, (bef - theta + 360) % 360));
					if (t.size() > 10) t.erase(t.begin());
				}
				else{
					hp--;
					double t1 = 0, t2 = 0, t3 = 0, t4 = 0;
					for (int i = 0; i< t.size(); ++i)
					{
						t1 += i * i;
						t2 += i;
						t3 += i * t[i];
						t4 += t[i];
					}
					double a = (t3*t.size() - t2*t4) / (t1*t.size() - t2*t2);
					//b = (t4 - a*t2) / x.size();  
					double b = (t1*t4 - t2*t3) / (t1*t.size() - t2*t2);
					double y = a * 10 + b;
					theta = (int)(bef + y + 360) % 360;
					bef = theta;
				}
				bi = ni;
				bj = nj;

				int theta2 = theta % 180;
				if (theta2 < 90){
					theta2 = 90 + theta2;
				}
				else{
					theta2 = theta2 - 90;
				}

				double a = cos(theta * PI / 180) * step1;
				double b = sin(theta * PI / 180) * step1;
				pi = ni - b * delta;
				pj = nj + a * delta;

				a = cos(theta2 * PI / 180) * step2;
				b = sin(theta2 * PI / 180) * step2;
				int x1 = round1(pj + a * delta);
				int y1 = round1(pi - b * delta);
				int x2 = round1(pj - a * delta);
				int y2 = round1(pi + b * delta);
				double px = pj + a * delta;
				double py = pi - b * delta;
				double qx = pj - a * delta;
				double qy = pi + b * delta;
				double dist = sqrt(pow(px - qx, 2) + pow(py - qy, 2));

				if (x1 > x2) swap(x1, x2);
				if (y1 > y2) swap(y1, y2);

				double tw = -10000;
				for (int jj = x1; jj <= x2; ++jj){
					for (int ii = y1; ii <= y2; ++ii){
						if (jj < 0 || ii < 0 || jj >= mask->width || ii >= mask->height) continue;
						double tx1 = jj - px;
						double ty1 = ii - py;
						double tx2 = qx - px;
						double ty2 = qy - py;
						double d = fabs(tx1 * ty2 - tx2 * ty1);
						d /= dist;
						if (d > dis) continue;
						double ttw = CV_IMAGE_ELEM(weight, double, ii, jj);
						if (tw < ttw){
							tw = ttw;
							ni = ii;
							nj = jj;
						}
					}
				}


				if (ni < 0 || nj < 0 || ni >= w->height || nj >= w->width || CV_IMAGE_ELEM(mask, uchar, ni, nj) == 0)
					break;


				theta = CV_IMAGE_ELEM(thetaMap, uchar, ni, nj);
				double xx1 = cos(theta * PI / 180), xy1 = sin(theta * PI / 180);
				double xx2 = cos(bef * PI / 180), xy2 = sin(bef * PI / 180);
				double mul = xx1 * xx2 + xy1 * xy2;
				if (mul < 0) theta = (theta + 180) % 360;


				if (CV_IMAGE_ELEM(weight, double, ni, nj) < wlow)
					certain = 0;
				else if (min((theta - bef + 360) % 360, (bef - theta + 360) % 360) > 30)
					certain = 0;
				else
					certain = 1;

				if (CV_IMAGE_ELEM(mask, uchar, ni, nj) == 0){
					break;
				}
				if (CV_IMAGE_ELEM(trace, uchar, ni, nj) == 255){
					break;
				}
			}

			bef = CV_IMAGE_ELEM(thetaMap, uchar, i, j);
			hp = len;
			bi = i, bj = j;
			ni = i, nj = j;
			certain = 1;
			while (hp){
				//CV_IMAGE_ELEM(trace, uchar, ni, nj) = 255;
				cvLine(trace, cvPoint(nj, ni), cvPoint(bj, bi), cvScalarAll(255));
				//CV_IMAGE_ELEM(w, uchar, ni, nj) = 0;
				cvLine(w, cvPoint(nj, ni), cvPoint(bj, bi), cvScalarAll(0));
				int theta;
				if (certain){
					theta = CV_IMAGE_ELEM(thetaMap, uchar, ni, nj);
					double x1 = cos(theta * PI / 180), y1 = sin(theta * PI / 180);
					double x2 = cos(bef * PI / 180), y2 = sin(bef * PI / 180);
					double mul = x1 * x2 + y1 * y2;
					if (mul < 0) theta = (theta + 180) % 360;
					bef = theta;
					t.push_back(min((theta - bef + 360) % 360, (bef - theta + 360) % 360));
					if (t.size() > 10) t.erase(t.begin());
				}
				else{
					hp--;
					double t1 = 0, t2 = 0, t3 = 0, t4 = 0;
					for (int i = 0; i< t.size(); ++i)
					{
						t1 += i * i;
						t2 += i;
						t3 += i * t[i];
						t4 += t[i];
					}
					double a = (t3*t.size() - t2*t4) / (t1*t.size() - t2*t2);
					//b = (t4 - a*t2) / x.size();  
					double b = (t1*t4 - t2*t3) / (t1*t.size() - t2*t2);
					double y = a * 10 + b;
					theta = (int)(bef + y + 360) % 360;
					bef = theta;
				}
				bi = ni;
				bj = nj;

				int theta2 = theta % 180;
				if (theta2 < 90){
					theta2 = 90 + theta2;
				}
				else{
					theta2 = theta2 - 90;
				}

				double a = cos(theta * PI / 180) * step1;
				double b = sin(theta * PI / 180) * step1;
				pi = ni - b * delta;
				pj = nj + a * delta;

				a = cos(theta2 * PI / 180) * step2;
				b = sin(theta2 * PI / 180) * step2;
				int x1 = round1(pj + a * delta);
				int y1 = round1(pi - b * delta);
				int x2 = round1(pj - a * delta);
				int y2 = round1(pi + b * delta);
				double px = pj + a * delta;
				double py = pi - b * delta;
				double qx = pj - a * delta;
				double qy = pi + b * delta;
				double dist = sqrt(pow(px - qx, 2) + pow(py - qy, 2));

				if (x1 > x2) swap(x1, x2);
				if (y1 > y2) swap(y1, y2);

				double tw = -10000;
				for (int jj = x1; jj <= x2; ++jj){
					for (int ii = y1; ii <= y2; ++ii){
						if (jj < 0 || ii < 0 || jj >= mask->width || ii >= mask->height) continue;
						double tx1 = jj - px;
						double ty1 = ii - py;
						double tx2 = qx - px;
						double ty2 = qy - py;
						double d = fabs(tx1 * ty2 - tx2 * ty1);
						d /= dist;
						if (d > dis) continue;
						double ttw = CV_IMAGE_ELEM(weight, double, ii, jj);
						if (tw < ttw){
							tw = ttw;
							ni = ii;
							nj = jj;
						}
					}
				}


				if (ni < 0 || nj < 0 || ni >= w->height || nj >= w->width || CV_IMAGE_ELEM(mask, uchar, ni, nj) == 0)
					break;


				theta = CV_IMAGE_ELEM(thetaMap, uchar, ni, nj);
				double xx1 = cos(theta * PI / 180), xy1 = sin(theta * PI / 180);
				double xx2 = cos(bef * PI / 180), xy2 = sin(bef * PI / 180);
				double mul = xx1 * xx2 + xy1 * xy2;
				if (mul < 0) theta = (theta + 180) % 360;


				if (CV_IMAGE_ELEM(weight, double, ni, nj) < wlow)
					certain = 0;
				else if (min((theta - bef + 360) % 360, (bef - theta + 360) % 360) > 30)
					certain = 0;
				else
					certain = 1;
				if (CV_IMAGE_ELEM(mask, uchar, ni, nj) == 0){
					break;
				}
				if (CV_IMAGE_ELEM(trace, uchar, ni, nj) == 255){
					break;
				}
			}
			for (int www = 0; www < 3; ++www)
				cvWriteFrame(pVW, trace);
		}
	}
	cvReleaseVideoWriter(&pVW);
}

IplImage *Tracing::showResult(IplImage *input, IplImage *mask){
	IplImage *ret = cvCloneImage(input);
	cvScale(ret, ret, 0);
	for (int i = 0; i < input->height; ++i){
		for (int j = 0; j < input->width; ++j){
			if (CV_IMAGE_ELEM(trace, uchar, i, j) == 0) continue;
			uchar *p = (uchar*)(ret->imageData + i * ret->widthStep);
			uchar *p2 = (uchar*)(input->imageData + i * input->widthStep);
			double alpha = CV_IMAGE_ELEM(mask, uchar, i, j) / 255.0;
			for (int c = 0; c < ret->nChannels; ++c)
				p[c + j * ret->nChannels] = p2[c + j * input->nChannels] * alpha;
		}
	}
	return ret;
}