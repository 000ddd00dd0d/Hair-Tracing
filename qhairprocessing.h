#ifndef QHAIRPROCESSING_H
#define QHAIRPROCESSING_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qaction.h>
#include "ui_qhairprocessing.h"
#include "LazySnapping.h"
#include "Tracing.h"
#include <cv.h>
#include <highgui.h>

class QHairProcessing : public QMainWindow
{
	Q_OBJECT

public:
	QHairProcessing(QWidget *parent = 0);
	~QHairProcessing();

private:
	
	void createMenus();
	void createActions();
	void initialize();

private slots:
	void setForeground();
	void setBackground();
	void reset();
	void newImage();
	void matting();
	void gaborFilter();
	void tracingProcess();


private:
	QMenu *fileMenu;
	QAction *newAction;
	Tracing tracing;

public:
	Ui::QHairProcessingClass ui;
	LazySnapping ls;

	

	
};

#endif // QHAIRPROCESSING_H
