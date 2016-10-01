#include "qhairprocessing.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QHairProcessing w;
	w.show();
	return a.exec();
}
