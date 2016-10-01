#ifndef __LASYSNAPPING_H__
#define __LASYSNAPPING_H__

#include <cv.h>
#include <highgui.h>
#include <vector>
#include <string>
#include <set>
#include "graph.h"

typedef Graph<float, float, float> GraphType;

class LazySnapping{
public:
	LazySnapping(){ color = 0; }
	~LazySnapping(){
		//if (graph) delete graph;
		//if (markerMask) cvReleaseImage(&markerMask);
		//if (color) delete[] color;
		//if (cnt) delete[] cnt;
	}
	void setInput(IplImage *image);
	void setForeBackCluster(std::vector<CvPoint> fore, std::vector<CvPoint> back);
	int runMaxflow();
	bool doLazySnapping();
	IplImage* getImageMask();
	void reset();
	void addForePt(int x, int y);
	void addBackPt(int x, int y);
	void addSeeds(int x, int y);
	IplImage* getWatershed(){
		return wshed;
	}
private:
	void watershed();
	float minDistance(double *color, std::set<int> cluster);
	void getE1(double *color, float *energy);
	float getE2(double *color1, double *color2);
private:
	std::vector<CvPoint> forePts;
	std::vector<CvPoint> backPts;
	std::set<int> foreCluster;
	std::set<int> backCluster;
	IplImage *image;
	IplImage *markerMask;
	IplImage *marker;
	IplImage *wshed;
	GraphType *graph;
	int count;
	CvScalar *color;
	int *cnt;
};


#endif