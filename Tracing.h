#ifndef __TRACING_H__
#define __TRACING_H__
#include <cv.h>
#include <highgui.h>
#define wins 11

class Tracing{
	double kernel[32][wins][wins];
	IplImage *res;
	IplImage *weight;
	IplImage *thetaMap;
	IplImage *trace;
	IplImage *r[32];
private:
	IplImage *gaborFilter(IplImage *I, IplImage *mask, int ri);
	CvScalar calColor(int i);
	void calKernel();
public:
	Tracing(){
		calKernel();
	}
	void gaborProcess(IplImage *I, IplImage *mask);
	IplImage *getGaborResult(){ return res; }
	IplImage *getWeight() { return weight; }
	IplImage *getTrace() { return trace; }
	void tracing(IplImage *mask);
	IplImage *showResult(IplImage *input, IplImage *mask);
};


#endif