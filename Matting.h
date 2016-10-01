#ifndef __MATTING_H__
#define __MATTING_H__
#include <cv.h>
#include "Eigen\Eigen"
void createTrimap(IplImage *input, IplImage *canny, int k = 10);
Eigen::VectorXd solveAlpha(IplImage *I, IplImage *constMap, IplImage *constVal, int level, int activeLevel);
#endif