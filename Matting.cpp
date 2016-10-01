#include "Matting.h"
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <vector>
#include "Eigen\Eigen"
#include "Eigen\Dense"
#include "Eigen\Sparse"
using namespace std;
using namespace Eigen;

void createTrimap(IplImage *input, IplImage *canny, int k){
	for(int i = 0; i < input->height; ++i){
		for (int j = 0; j < input->width; ++j){
			if (CV_IMAGE_ELEM(canny, uchar, i, j)){
				cvCircle(input, cvPoint(j, i), k / 2, cvScalarAll(127), k / 2);
			}
		}
	}
}

IplImage* downSample(IplImage *I){
	int w = 0, h = 0;
	double kernel[] =
	{
		1, 4, 6, 4, 1,
		4, 16, 24, 16, 4,
		6, 24, 36, 24, 6,
		4, 16, 24, 16, 4,
		1, 4, 6, 4, 1
	};
	int dir[][2] =
	{
		{ -2, -2 }, { -2, -1 }, { -2, 0 }, { -2, 1 }, { -2, 2 },
		{ -1, -2 }, { -1, -1 }, { -1, 0 }, { -1, 1 }, { -1, 2 },
		{ 0, -2 }, { 0, -1 }, { 0, 0 }, { 0, 1 }, { 0, 2 },
		{ 1, -2 }, { 1, -1 }, { 1, 0 }, { 1, 1 }, { 1, 2 },
		{ 2, -2 }, { 2, -1 }, { 2, 0 }, { 2, 1 }, { 2, 2 }
	};
	for (int i = 2; i <= I->height - 3; i += 2) ++h;
	for (int i = 2; i <= I->width - 3; i += 2) ++w;
	IplImage *I2 = cvCreateImage(cvSize(w, h), I->depth, I->nChannels);
	w = h = 0;
	for (int i = 2; i <= I->height - 3; i += 2){
		w = 0;
		for (int j = 2; j <= I->width - 3; j += 2){
			double t[3] = { 0, 0, 0 };
			for (int k = 0; k < 25; ++k){
				int x = i + dir[k][0], y = j + dir[k][1];
				double *p = (double*)(I->imageData + x * I->widthStep);
				for (int c = 0; c < I->nChannels; ++c)
					t[c] += kernel[k] * p[c + y * I->nChannels];
			}
			double *p = (double*)(I2->imageData + h * I2->widthStep);
			for (int c = 0; c < I->nChannels; ++c)
				p[c + w * I2->nChannels] = t[c] / 256.0;
			w++;
		}
		h++;
	}
	return I2;
}

IplImage* round(IplImage *I){
	for (int i = 0; i < I->height; ++i){
		for (int j = 0; j < I->width; ++j){
			if (CV_IMAGE_ELEM(I, double, i, j) - 0.5 >= 1e-5)
				CV_IMAGE_ELEM(I, double, i, j) = 1;
			else
				CV_IMAGE_ELEM(I, double, i, j) = 0;
		}
	}
	return I;
}

IplImage* getLinearCoeff(VectorXd dAlpha, IplImage *dI){
	double epsilon = 1e-7;
	MatrixXd coeff(dI->width * dI->height, 4);
	Matrix<double, 12, 4> winI;
	VectorXd tcoeff;
	Matrix<double, 12, 1> talpha;
	talpha.setZero();
	winI.setZero();
	coeff.setZero();
	double *p = 0;
	for (int i = 0; i < 9; ++i)
		winI(i, 3) = 1;
	for (int i = 0; i < 3; ++i){
		winI(i + 9, i) = sqrt(epsilon);
	}
	for (int j = 1; j < dI->width - 1; ++j){
		for (int i = 1; i < dI->height - 1; ++i){
			int id = 0;
			for (int tj = j - 1; tj <= j + 1; ++tj){
				for (int ti = i - 1; ti <= i + 1; ++ti){
					p = (double*)(dI->imageData + ti * dI->widthStep);
					winI(id, 0) = p[0 + tj * dI->nChannels];
					winI(id, 1) = p[1 + tj * dI->nChannels];
					winI(id, 2) = p[2 + tj * dI->nChannels];
					talpha(id, 0) = dAlpha(ti + tj * dI->height);
					++id;
				}
			}
			tcoeff = (winI.transpose() * winI).inverse() * winI.transpose() * talpha;
			for (int ti = 0; ti < 4; ++ti){
				coeff(i + j * dI->height, ti) = tcoeff(ti);
			}
		}
	}
	for (int i = 0; i < dI->width; ++i){
		for (int j = 0; j < 4; ++j){
			coeff(i * dI->height, j) = coeff(1 + i * dI->height, j);
			coeff(i * dI->height + dI->height - 1, j) = coeff(i * dI->height + dI->height - 2, j);
		}
	}
	for (int i = 0; i < dI->height; ++i){
		for (int j = 0; j < 4; ++j){
			coeff(i, j) = coeff(i + dI->height, j);
			coeff(i + (dI->width - 1) * dI->height, j) = coeff(i + (dI->width - 2) * dI->height, j);
		}
	}
	IplImage *calpha = cvCreateImage(cvSize(dI->width, dI->height), IPL_DEPTH_64F, 4);
	for (int j = 0; j < calpha->width; ++j){
		for (int i = 0; i < calpha->height; ++i){
			int id = i + j * calpha->height;
			double *p = (double*)(calpha->imageData + i * calpha->widthStep);
			p[0 + j * calpha->nChannels] = coeff(id, 0);
			p[1 + j * calpha->nChannels] = coeff(id, 1);
			p[2 + j * calpha->nChannels] = coeff(id, 2);
			p[3 + j * calpha->nChannels] = coeff(id, 3);
		}
	}
	return calpha;
}

IplImage* upSample(IplImage *I, int h, int w){
	IplImage *I2 = cvCreateImage(cvSize(w, h), I->depth, I->nChannels);

	for (int i = 2, oldI = 0; oldI < I->height; ++oldI, i += 2){
		for (int j = 2, oldJ = 0; oldJ < I->width; ++oldJ, j += 2){
			double *p1 = (double *)(I2->imageData + i * I2->widthStep);
			double *p2 = (double *)(I->imageData + oldI * I->widthStep);
			for (int ti = 0; ti < 4; ++ti){
				p1[ti + j * I2->nChannels] = p2[ti + oldJ * I->nChannels];
			}
		}
	}
	for (int j = 2; j < I2->width; j += 2){
		for (int i = 0; i < I2->height; ++i){
			if (i < 2){
				double *p1 = (double *)(I2->imageData + i * I2->widthStep);
				double *p2 = (double *)(I2->imageData + 2 * I2->widthStep);
				for (int ti = 0; ti < 4; ++ti){
					p1[ti + j * I2->nChannels] = p2[ti + j * I2->nChannels];
				}
			}
			else if (i > I->height * 2){
				double *p1 = (double *)(I2->imageData + i * I2->widthStep);
				double *p2 = (double *)(I2->imageData + I->height * 2 * I2->widthStep);
				for (int ti = 0; ti < 4; ++ti){
					p1[ti + j * I2->nChannels] = p2[ti + j * I2->nChannels];
				}
			}
			else if (i % 2 == 0){
				continue;
			}
			else{
				double *p1 = (double *)(I2->imageData + i * I2->widthStep);
				double *p2 = (double *)(I2->imageData + (i - 1) * I2->widthStep);
				double *p3 = (double *)(I2->imageData + (i + 1) * I2->widthStep);
				for (int ti = 0; ti < 4; ++ti){
					p1[ti + j * I2->nChannels] = (p2[ti + j * I2->nChannels] + p3[ti + j * I2->nChannels]) / 2;
				}
			}
		}
	}
	for (int i = 0; i < I2->height; ++i){
		for (int j = 0; j < I2->width; ++j){
			if (j < 2){
				double *p1 = (double *)(I2->imageData + i * I2->widthStep);
				double *p2 = (double *)(I2->imageData + i * I2->widthStep);
				for (int ti = 0; ti < 4; ++ti){
					p1[ti + j * I2->nChannels] = p2[ti + 2 * I2->nChannels];
				}
			}
			else if (j > I->width * 2){
				double *p1 = (double *)(I2->imageData + i * I2->widthStep);
				double *p2 = (double *)(I2->imageData + i * I2->widthStep);
				for (int ti = 0; ti < 4; ++ti){
					p1[ti + j * I2->nChannels] = p2[ti + 2 * I->width * I2->nChannels];
				}
			}
			else if (j % 2 == 0){
				continue;
			}
			else{
				double *p1 = (double *)(I2->imageData + i * I2->widthStep);
				double *p2 = (double *)(I2->imageData + i * I2->widthStep);
				double *p3 = (double *)(I2->imageData + i * I2->widthStep);
				for (int ti = 0; ti < 4; ++ti){
					p1[ti + j * I2->nChannels] = (p2[ti + (j - 1) * I2->nChannels] + p3[ti + (j + 1) * I2->nChannels]) / 2;
				}
			}
		}
	}
	return I2;
}

VectorXd solveAlpha(IplImage *I, IplImage *constMap, IplImage *constVal){
	int imageSize = I->width * I->height;
	SparseMatrix<double> A(imageSize, imageSize);
	vector<Triplet<double> > triplets;
	Matrix<double, 3, 1> mu;
	Matrix<double, 9, 3> winI;
	Matrix<double, 3, 3> var;
	Matrix<double, 9, 9> tval;
	double epsilon = 1e-7;
	double *p = NULL;
	IplImage *constI = cvCloneImage(constMap);
	cvErode(constMap, constI);
	for (int j = 1; j < I->width - 1; ++j){
		for (int i = 1; i < I->height - 1; ++i){
			if (CV_IMAGE_ELEM(constI, double, i, j)) continue;
			mu.setZero();
			winI.setZero();
			int id = 0;
			for (int tj = j - 1; tj <= j + 1; ++tj){
				for (int ti = i - 1; ti <= i + 1; ++ti){
					p = (double*)(I->imageData + ti * I->widthStep);
					mu(2, 0) += p[0 + tj * I->nChannels];
					mu(1, 0) += p[1 + tj * I->nChannels];
					mu(0, 0) += p[2 + tj * I->nChannels];
					winI(id, 2) = p[0 + tj * I->nChannels];
					winI(id, 1) = p[1 + tj * I->nChannels];
					winI(id, 0) = p[2 + tj * I->nChannels];
					++id;
				}
			}
			mu /= 9;
			var = (winI.transpose() * winI / 9 - mu * mu.transpose() + epsilon / 9 * MatrixXd::Identity(3, 3)).inverse();
			for (int ti = 0; ti < 9; ++ti){
				for (int tj = 0; tj < 3; ++tj){
					winI(ti, tj) -= mu(tj, 0);
				}
			}
			tval = winI * var * winI.transpose();
			for (int ti = 0; ti < 9; ++ti){
				for (int tj = 0; tj < 9; ++tj){
					tval(ti, tj) = (tval(ti, tj) + 1) / 9.0;
				}
			}
			id = 0;
			for (int tj = j - 1; tj <= j + 1; ++tj){
				for (int ti = i - 1; ti <= i + 1; ++ti){
					int col = ti + I->height * tj;
					for (int ttj = j - 1; ttj <= j + 1; ++ttj){
						for (int tti = i - 1; tti <= i + 1; ++tti){
							int row = tti + I->height * ttj;
							triplets.emplace_back(row, col, -tval(id % 9, id / 9));
							++id;
						}
					}
				}
			}
		}
	}


	VectorXd sumA(imageSize);
	sumA.setZero();
	for (int i = 0; i < triplets.size(); ++i){
		sumA(triplets[i].row()) -= triplets[i].value();

	}
	int lambda = 100;
	for (int i = 0; i < imageSize; ++i){
		if (sumA(i)){
			triplets.emplace_back(i, i, sumA(i));
		}
		int ti = i % I->height;
		int tj = i / I->height;
		if (fabs(CV_IMAGE_ELEM(constMap, double, ti, tj) - 1) < 1e-5){
			triplets.emplace_back(i, i, lambda);
		}
	}
	A.setFromTriplets(triplets.begin(), triplets.end());
	VectorXd B(imageSize);
	B.setZero();
	for (int j = 0; j < I->width; ++j){
		for (int i = 0; i < I->height; ++i){
			int id = i + I->height * j;
			if (fabs(CV_IMAGE_ELEM(constVal, double, i, j) - 1) < 1e-5)
				B(id) = lambda;
			else{
				B(id) = 0;
			}
		}
	}

	SimplicialLDLT<SparseMatrix<double> > linearSolver;
	linearSolver.compute(A);
	VectorXd alpha = linearSolver.solve(B);
	for (int i = 0; i < imageSize; ++i){
		if (alpha(i) - 1 > -1e-5) alpha(i) = 1;
		if (alpha(i) < 1e-5) alpha(i) = 0;
	}
	/*IplImage *alphaImage = cvCloneImage(constMap);
	for (int i = 0; i < alphaImage->height; ++i){
	for (int j = 0; j < alphaImage->width; ++j){
	int id = i + alphaImage->height * j;
	CV_IMAGE_ELEM(alphaImage, double, i, j) = alpha(id);
	}
	}
	IplImage *a = cvCreateImage(cvSize(alphaImage->width * 8, alphaImage->height * 8), IPL_DEPTH_64F, 1);
	cvResize(alphaImage, a, CV_INTER_NN);
	cvShowImage("alpha", a);
	cvWaitKey(0);*/
	cvReleaseImage(&constI);
	return alpha;
}

VectorXd solveAlpha(IplImage *I, IplImage *constMap, IplImage *constVal, int level, int activeLevel){
	activeLevel = max(activeLevel, 1);
	VectorXd alpha(I->width * I->height);
	if (level > 1){
		IplImage *dI = downSample(I);
		IplImage *dConstMap = round(downSample(constMap));
		IplImage *dConstVal = round(downSample(constVal));
		VectorXd dAlpha = solveAlpha(dI, dConstMap, dConstVal, level - 1, min(level - 1, activeLevel));
		IplImage* coeff = getLinearCoeff(dAlpha, dI);
		IplImage* bcoeff = upSample(coeff, I->height, I->width);

		for (int i = 0; i < bcoeff->width * bcoeff->height; ++i){
			int x = i % bcoeff->height;
			int y = i / bcoeff->height;
			double *p = (double*)(bcoeff->imageData + x * bcoeff->widthStep);
			double *pI = (double*)(I->imageData + x * I->widthStep);
			alpha(i) = p[3 + y * bcoeff->nChannels];
			for (int ti = 0; ti < 3; ++ti){
				alpha(i) += p[ti + y * bcoeff->nChannels] * pI[ti + y * I->nChannels];
			}
			if (alpha(i) < 0) alpha(i) = 0;
			if (alpha(i) > 1) alpha(i) = 1;
		}
		/*IplImage *alphaImage = cvCreateImage(cvSize(I->width, I->height), IPL_DEPTH_64F, 1);
		for (int j = 0; j < alphaImage->width; ++j){
			for (int i = 0; i < alphaImage->height ; ++i){
				int id = i + alphaImage->height * j;
				CV_IMAGE_ELEM(alphaImage, double, i, j) = alpha(id);
			}
		}
		cvShowImage("alpha", alphaImage);*/
		//cvWaitKey(0);
		cvReleaseImage(&dI);
		cvReleaseImage(&dConstMap);
		cvReleaseImage(&dConstVal);
		cvReleaseImage(&coeff);
		cvReleaseImage(&bcoeff);
	}
	if (activeLevel >= level)
		alpha = solveAlpha(I, constMap, constVal);

	return alpha;
}

