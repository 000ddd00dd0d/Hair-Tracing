#include "qhairprocessing.h"
#include <qfiledialog.h>
#include <qstring.h>
#include <string>
#include <qimage.h>
#include "Matting.h"

CvScalar paintColor[2] = { CV_RGB(0, 0, 255), CV_RGB(255, 0, 0) };
IplImage *origin;
IplImage *paint;
IplImage *rough;
IplImage *mask;
IplImage *gray;
IplImage *trimap;

IplImage *qWatershed;
IplImage *qTrimap;
IplImage *qRough;
IplImage *qOrigin;
IplImage *qHair;
IplImage *qA;
IplImage *qInput1;
IplImage *qGabor1;
IplImage *qInput2;
IplImage *qGabor2;
IplImage *qTracing;
IplImage *qResult;

IplImage *res;
IplImage *I;
IplImage *M;
IplImage *alphaImage;
int currentMode;
Eigen::VectorXd alpha;

QHairProcessing::QHairProcessing(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	initialize();
	currentMode = 0;
	createActions();
	createMenus();
	
}

QHairProcessing::~QHairProcessing()
{
	cvDestroyAllWindows();
	cvReleaseImage(&origin);
	cvReleaseImage(&paint);

	cvReleaseImage(&qTrimap);
	cvReleaseImage(&qWatershed);
	cvReleaseImage(&qRough);
	cvReleaseImage(&qOrigin);
	cvReleaseImage(&qA);
	cvReleaseImage(&qHair);
}

void QHairProcessing::initialize(){
	ui.foreButton->setDisabled(true);
	ui.backButton->setDisabled(true);
	ui.resetButton->setDisabled(true);
	ui.mattingButton->setDisabled(true);
	ui.gaborButton->setDisabled(true);
	ui.tracingButton->setDisabled(true);
}

void QHairProcessing::createMenus(){
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newAction);
}

void QHairProcessing::createActions(){
	newAction = new QAction(tr("Load &New Image"), this);
	connect(newAction, SIGNAL(triggered()), this, SLOT(newImage()));
}

void QHairProcessing::setForeground(){
	currentMode = 0;
}
void QHairProcessing::setBackground(){
	currentMode = 1;
}
void QHairProcessing::reset(){
	cvReleaseImage(&paint);
	paint = cvCloneImage(origin);
	ls.reset();
	currentMode = 0;
	cvShowImage("origin", paint);
}

void onOriginMouse(int event, int x, int y, int flags, void *param){
	QHairProcessing* win = (QHairProcessing*)param;
	if (event == CV_EVENT_LBUTTONUP){

		win->ui.mattingButton->setDisabled(false);
		if (win->ls.doLazySnapping() == false) {
			return;
		}
		mask = win->ls.getImageMask();
		gray = cvCloneImage(mask);
		cvCanny(gray, gray, 50, 150, 3);
		rough = cvCloneImage(origin);
		IplImage *imgShow = cvCloneImage(paint);
		for (int h = 0; h < origin->height; ++h){
			unsigned char *pgray = (unsigned char*)gray->imageData + gray->widthStep * h;
			unsigned char *pimage = (unsigned char*)imgShow->imageData + imgShow->widthStep * h;
			unsigned char *pmask = (unsigned char*)mask->imageData + mask->widthStep * h;
			unsigned char *phair = (unsigned char*)rough->imageData + rough->widthStep * h;
			for (int w = 0; w < origin->width; ++w){
				if (*pgray++ != 0){
					pimage[0] = 0;
					pimage[1] = 255;
					pimage[2] = 0;
				}
				if (*pmask++ == 0){
					phair[0] = phair[1] = phair[2] = 0;
				}
				phair += 3;
				pimage += 3;
			}
		}

		trimap = cvCloneImage(mask);
		createTrimap(trimap, gray, 15);
		cvShowImage("origin", imgShow);

		cvReleaseImage(&qRough);
		qRough = cvCreateImage(cvSize(rough->width, rough->height), rough->depth, 4);
		cvCvtColor(rough, qRough, CV_RGB2RGBA);
		QImage img = QImage((const unsigned char*)(qRough->imageData), qRough->width, qRough->height, QImage::Format_RGB32);
		win->ui.labelRough->setPixmap(QPixmap::fromImage(img));
		win->ui.labelRough->resize(win->ui.labelRough->pixmap()->size());
		win->ui.labelRough->show();

		cvReleaseImage(&qWatershed);
		qWatershed = cvCreateImage(cvSize(trimap->width, trimap->height), trimap->depth, 4);
		cvCvtColor(win->ls.getWatershed(), qWatershed, CV_RGB2RGBA);
		QImage img2 = QImage((const unsigned char*)(qWatershed->imageData), qWatershed->width, qWatershed->height, QImage::Format_RGB32);
		win->ui.labelWatershed->setPixmap(QPixmap::fromImage(img2));
		win->ui.labelWatershed->resize(win->ui.labelWatershed->pixmap()->size());
		win->ui.labelWatershed->show();

		cvReleaseImage(&qTrimap);
		qTrimap = cvCreateImage(cvSize(trimap->width, trimap->height), trimap->depth, 4);
		cvCvtColor(trimap, qTrimap, CV_GRAY2RGBA);
		QImage img3 = QImage((const unsigned char*)(qTrimap->imageData), qTrimap->width, qTrimap->height, QImage::Format_RGB32);
		win->ui.labelTrimap->setPixmap(QPixmap::fromImage(img3));
		win->ui.labelTrimap->resize(win->ui.labelTrimap->pixmap()->size());
		win->ui.labelTrimap->show();

		

		cvReleaseImage(&mask);
		cvReleaseImage(&imgShow);
		cvReleaseImage(&gray);
	}
	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON)){
		CvPoint pt = cvPoint(x, y);
		if (!(x < 0 || y < 0 || x > paint->width || y > paint->height)){
			cvCircle(paint, pt, 1, paintColor[currentMode]);
			if (currentMode == 0) win->ls.addForePt(x, y);
			else if (currentMode == 1) win->ls.addBackPt(x, y);
			win->ls.addSeeds(x, y);
		}
		cvShowImage("origin", paint);
	}
	
}

void QHairProcessing::newImage(){
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"),
		".", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
	if (fileName == "") return;

	ui.foreButton->setDisabled(false);
	ui.backButton->setDisabled(false);
	ui.resetButton->setDisabled(false);


	origin = cvLoadImage(fileName.toLatin1().data(), 1);
	if (origin == NULL) return;
	ls.setInput(origin);
	paint = cvCloneImage(origin);
	cvNamedWindow("origin");
	cvSetMouseCallback("origin", onOriginMouse, this);
	cvShowImage("origin", paint);
	
	qOrigin = cvCreateImage(cvSize(origin->width, origin->height), origin->depth, 4);
	cvCvtColor(origin, qOrigin, CV_RGB2RGBA);
	QImage img = QImage((const unsigned char*)(qOrigin->imageData), qOrigin->width, qOrigin->height, QImage::Format_RGB32);
	ui.labelOrigin->setPixmap(QPixmap::fromImage(img));
	ui.labelOrigin->resize(ui.labelOrigin->pixmap()->size());
	ui.labelOrigin->show();

	cvWaitKey(0);

}

void onMattingMouse(int pos, void *param){
	QHairProcessing* win = (QHairProcessing*)param;
	res = cvCloneImage(I);
	M = cvCloneImage(res);
	cvReleaseImage(&qHair);
	qHair = cvCreateImage(cvSize(I->width, I->height), origin->depth, 4);
	for (int j = 0; j < res->width; ++j){
		for (int i = 0; i < res->height; ++i){
			double *p2 = (double*)(M->imageData + i * M->widthStep);
			double *p = (double*)(res->imageData + i * res->widthStep);
			uchar *p3 = (uchar*)(qHair->imageData + i * qHair->widthStep);
			p3[3 + j * qHair->nChannels] = 255;
			if (alpha(i + j * res->height) * 100 < pos){

				for (int c = 0; c < res->nChannels; ++c){
					p3[c + j * qHair->nChannels] = 0;
					if ((i / 32 + j / 32) % 2 == 0)
						p2[c + j * M->nChannels] = p[c + j * res->nChannels] = 0.5;
					else
						p2[c + j * M->nChannels] = p[c + j * res->nChannels] = 1;
				}
			}
			else{
				double color;
				if ((i / 32 + j / 32) % 2 == 0)
					color = 0.5;
				else
					color = 1;
				for (int c = 0; c < res->nChannels; ++c){
					p2[c + j * M->nChannels] = p[c + j * res->nChannels] * alpha(j * I->height + i) + color * (1 - alpha(j * I->height + i));
					p3[c + j * qHair->nChannels] = (uchar)(p[c + j * res->nChannels] * 255);
				}
			}
		}
	}
	QImage img = QImage((const unsigned char*)(qHair->imageData), qHair->width, qHair->height, QImage::Format_RGB32);
	win->ui.labelHair->setPixmap(QPixmap::fromImage(img));
	win->ui.labelHair->resize(win->ui.labelHair->pixmap()->size());
	win->ui.labelHair->show();
	//cvShowImage("M", M);
	cvShowImage("res", M);
}

void QHairProcessing::matting(){
	I = cvCreateImage(cvSize(origin->width, origin->height), IPL_DEPTH_64F, origin->nChannels);
	IplImage *constMap = cvCreateImage(cvSize(origin->width, origin->height), IPL_DEPTH_64F, 1);
	IplImage *constVal = cvCreateImage(cvSize(origin->width, origin->height), IPL_DEPTH_64F, 1);
	cvConvertScale(origin, I, 1.0 / 255, 0);
	for (int i = 0; i < origin->height; ++i){
		for (int j = 0; j < origin->width; ++j){

			if (CV_IMAGE_ELEM(trimap, uchar, i, j) == 0){
				CV_IMAGE_ELEM(constMap, double, i, j) = 1;
				CV_IMAGE_ELEM(constVal, double, i, j) = 0;
			}
			else if (CV_IMAGE_ELEM(trimap, uchar, i, j) == 255){
				CV_IMAGE_ELEM(constMap, double, i, j) = 1;
				CV_IMAGE_ELEM(constVal, double, i, j) = 1;
			}
			else{
				CV_IMAGE_ELEM(constMap, double, i, j) = 0;
				CV_IMAGE_ELEM(constVal, double, i, j) = 0;

			}
		}
	}

	alpha = solveAlpha(I, constMap, constVal, 3, 1);
	alphaImage = cvCreateImage(cvSize(I->width, I->height), origin->depth, 1);
	for (int j = 0; j < I->width; ++j){
		for (int i = 0; i < I->height; ++i){
			CV_IMAGE_ELEM(alphaImage, uchar, i, j) = (uchar)(alpha(i + j * I->height) * 255);
		}
	}
	qA = cvCreateImage(cvSize(origin->width, origin->height), origin->depth, 4);
	cvCvtColor(alphaImage, qA, CV_GRAY2RGBA);
	QImage img = QImage((const unsigned char*)(qA->imageData), qA->width, qA->height, QImage::Format_RGB32);
	ui.labelAlpha->setPixmap(QPixmap::fromImage(img));
	ui.labelAlpha->resize(ui.labelAlpha->pixmap()->size());
	ui.labelAlpha->show();

	
	for (int j = 0; j < I->width; ++j){
		for (int i = 0; i < I->height; ++i){
			double *p = (double*)(I->imageData + i * I->widthStep);
			int id = i + j * I->height;
			for (int c = 0; c < I->nChannels; ++c){
				p[c + j * I->nChannels] = p[c + j * I->nChannels] * alpha(id);
			}
		}
	}

	ui.gaborButton->setDisabled(false);

	res = cvCloneImage(I);
	cvNamedWindow("res", 1);
	int threshold = 10;
	cvCreateTrackbar2("alpha", "res", &threshold, 100, onMattingMouse, this);
	onMattingMouse(10, this);
	
	cvWaitKey(0);
}

IplImage *showImage(IplImage *input){
	IplImage *ret = cvCloneImage(input);
	for (int i = 0; i < input->height; ++i){
		for (int j = 0; j < input->width; ++j){
			uchar *p = (uchar*)(ret->imageData + i * ret->widthStep);
			uchar *p2 = (uchar*)(input->imageData + i * input->widthStep);
			if (CV_IMAGE_ELEM(alphaImage, uchar, i, j) == 0){
				for (int c = 0; c < res->nChannels; ++c){
					if ((i / 16 + j / 16) % 2 == 0)
						p[c + j * ret->nChannels]  = 200;
					else
						p[c + j * ret->nChannels]  = 255;
				}
			}
			else{
				uchar color;
				if ((i / 16 + j / 16) % 2 == 0)
					color = 200;
				else
					color = 255;
				for (int c = 0; c < res->nChannels; ++c){
					p[c + j * ret->nChannels] = p2[c + j * ret->nChannels] * alpha(i + j * I->height) + color * (1 - alpha(i + j * I->height));
				}
			}
		}
	}
	return ret;
}

void QHairProcessing::gaborFilter(){
	IplImage *gray = cvCloneImage(alphaImage);
	IplImage *input = cvCreateImage(cvSize(I->width, I->height), IPL_DEPTH_64F, 1);
	cvCvtColor(qHair, gray, CV_RGBA2GRAY);
	cvScale(gray, input, 1.0 / 255);


	IplImage *qInput1 = cvCreateImage(cvSize(I->width, I->height), 8, 4);
	cvCvtColor(gray, qInput1, CV_GRAY2RGBA);
	QImage img = QImage((const unsigned char*)(qInput1->imageData), qInput1->width, qInput1->height, QImage::Format_RGB32);
	ui.labelInput1->setPixmap(QPixmap::fromImage(img));
	ui.labelInput1->resize(ui.labelAlpha->pixmap()->size());
	ui.labelInput1->show();

	tracing.gaborProcess(input, alphaImage);
	qGabor1 = cvCreateImage(cvSize(I->width, I->height), 8, 4);
	IplImage *gabor1 = tracing.getGaborResult();
	gabor1 = showImage(gabor1);
	cvCvtColor(gabor1, qGabor1, CV_RGB2RGBA);
	QImage img2 = QImage((const unsigned char*)(qGabor1->imageData), qGabor1->width, qGabor1->height, QImage::Format_RGB32);
	ui.labelGabor1->setPixmap(QPixmap::fromImage(img2));
	ui.labelGabor1->resize(ui.labelAlpha->pixmap()->size());
	ui.labelGabor1->show();

	qInput2 = cvCreateImage(cvSize(I->width, I->height), 8, 4);
	IplImage *input2 = cvCreateImage(cvSize(I->width, I->height), 8, 1);
	cvScale(tracing.getWeight(), input2, 255);
	cvCvtColor(input2, qInput2, CV_GRAY2RGBA);
	QImage img3 = QImage((const unsigned char*)(qInput2->imageData), qInput2->width, qInput2->height, QImage::Format_RGB32);
	ui.labelInput2->setPixmap(QPixmap::fromImage(img3));
	ui.labelInput2->resize(ui.labelAlpha->pixmap()->size());
	ui.labelInput2->show();

	tracing.gaborProcess(tracing.getWeight(), alphaImage);
	qGabor2 = cvCreateImage(cvSize(I->width, I->height), 8, 4);
	IplImage *gabor2 = tracing.getGaborResult();
	gabor2 = showImage(gabor2);
	cvCvtColor(gabor2, qGabor2, CV_RGB2RGBA);
	QImage img4 = QImage((const unsigned char*)(qGabor2->imageData), qGabor2->width, qGabor2->height, QImage::Format_RGB32);
	ui.labelGabor2->setPixmap(QPixmap::fromImage(img4));
	ui.labelGabor2->resize(ui.labelAlpha->pixmap()->size());
	ui.labelGabor2->show();

	ui.tracingButton->setDisabled(false);
}

void QHairProcessing::tracingProcess(){
	tracing.tracing(alphaImage);
	qTracing = cvCreateImage(cvSize(I->width, I->height), 8, 4);
	IplImage *trace = tracing.getTrace();
	cvCvtColor(trace, qTracing, CV_GRAY2RGBA);
	QImage img = QImage((const unsigned char*)(qTracing->imageData), qTracing->width, qTracing->height, QImage::Format_RGB32);
	ui.labelTracing->setPixmap(QPixmap::fromImage(img));
	ui.labelTracing->resize(ui.labelAlpha->pixmap()->size());
	ui.labelTracing->show();
	
	qResult = cvCreateImage(cvSize(I->width, I->height), 8, 4);
	IplImage *result = tracing.showResult(origin, alphaImage);
	cvCvtColor(result, qResult, CV_RGB2RGBA);
	QImage img2 = QImage((const unsigned char*)(qResult->imageData), qResult->width, qResult->height, QImage::Format_RGB32);
	ui.labelResult->setPixmap(QPixmap::fromImage(img2));
	ui.labelResult->resize(ui.labelAlpha->pixmap()->size());
	ui.labelResult->show();
}