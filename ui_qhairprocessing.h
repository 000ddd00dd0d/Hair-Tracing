/********************************************************************************
** Form generated from reading UI file 'qhairprocessing.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QHAIRPROCESSING_H
#define UI_QHAIRPROCESSING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QHairProcessingClass
{
public:
    QAction *actionLoad_File;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QGridLayout *gridLayout;
    QPushButton *backButton;
    QPushButton *resetButton;
    QPushButton *mattingButton;
    QPushButton *foreButton;
    QSpacerItem *verticalSpacer;
    QPushButton *gaborButton;
    QPushButton *tracingButton;
    QTabWidget *tabWidget;
    QWidget *tabOrigin;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_7;
    QLabel *labelOrigin;
    QSpacerItem *horizontalSpacer_8;
    QWidget *tabTrimap;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *labelTrimap;
    QSpacerItem *horizontalSpacer_2;
    QWidget *tabRough;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QLabel *labelRough;
    QSpacerItem *horizontalSpacer_4;
    QWidget *tabWatershed;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_5;
    QLabel *labelWatershed;
    QSpacerItem *horizontalSpacer_6;
    QTabWidget *tabWidget_2;
    QWidget *tabHair;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_9;
    QLabel *labelHair;
    QSpacerItem *horizontalSpacer_10;
    QWidget *tabAlpha;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_11;
    QLabel *labelAlpha;
    QSpacerItem *horizontalSpacer_12;
    QTabWidget *tabWidget_3;
    QWidget *tabInput1;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_13;
    QLabel *labelInput1;
    QSpacerItem *horizontalSpacer_14;
    QWidget *tabGabor1;
    QHBoxLayout *horizontalLayout_9;
    QSpacerItem *horizontalSpacer_15;
    QLabel *labelGabor1;
    QSpacerItem *horizontalSpacer_16;
    QWidget *tabInput2;
    QHBoxLayout *horizontalLayout_10;
    QSpacerItem *horizontalSpacer_17;
    QLabel *labelInput2;
    QSpacerItem *horizontalSpacer_18;
    QWidget *tabGabor2;
    QHBoxLayout *horizontalLayout_11;
    QSpacerItem *horizontalSpacer_20;
    QLabel *labelGabor2;
    QSpacerItem *horizontalSpacer_19;
    QWidget *tabTracing;
    QHBoxLayout *horizontalLayout_12;
    QSpacerItem *horizontalSpacer_22;
    QLabel *labelTracing;
    QSpacerItem *horizontalSpacer_21;
    QWidget *tabResult;
    QHBoxLayout *horizontalLayout_13;
    QSpacerItem *horizontalSpacer_24;
    QLabel *labelResult;
    QSpacerItem *horizontalSpacer_23;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QHairProcessingClass)
    {
        if (QHairProcessingClass->objectName().isEmpty())
            QHairProcessingClass->setObjectName(QStringLiteral("QHairProcessingClass"));
        QHairProcessingClass->resize(839, 414);
        actionLoad_File = new QAction(QHairProcessingClass);
        actionLoad_File->setObjectName(QStringLiteral("actionLoad_File"));
        centralWidget = new QWidget(QHairProcessingClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        backButton = new QPushButton(centralWidget);
        backButton->setObjectName(QStringLiteral("backButton"));

        gridLayout->addWidget(backButton, 1, 0, 1, 1);

        resetButton = new QPushButton(centralWidget);
        resetButton->setObjectName(QStringLiteral("resetButton"));

        gridLayout->addWidget(resetButton, 2, 0, 1, 1);

        mattingButton = new QPushButton(centralWidget);
        mattingButton->setObjectName(QStringLiteral("mattingButton"));

        gridLayout->addWidget(mattingButton, 3, 0, 1, 1);

        foreButton = new QPushButton(centralWidget);
        foreButton->setObjectName(QStringLiteral("foreButton"));

        gridLayout->addWidget(foreButton, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 6, 0, 1, 1);

        gaborButton = new QPushButton(centralWidget);
        gaborButton->setObjectName(QStringLiteral("gaborButton"));

        gridLayout->addWidget(gaborButton, 4, 0, 1, 1);

        tracingButton = new QPushButton(centralWidget);
        tracingButton->setObjectName(QStringLiteral("tracingButton"));

        gridLayout->addWidget(tracingButton, 5, 0, 1, 1);


        horizontalLayout_2->addLayout(gridLayout);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setDocumentMode(false);
        tabWidget->setMovable(true);
        tabWidget->setTabBarAutoHide(false);
        tabOrigin = new QWidget();
        tabOrigin->setObjectName(QStringLiteral("tabOrigin"));
        horizontalLayout_5 = new QHBoxLayout(tabOrigin);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_7);

        labelOrigin = new QLabel(tabOrigin);
        labelOrigin->setObjectName(QStringLiteral("labelOrigin"));

        horizontalLayout_5->addWidget(labelOrigin);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_8);

        tabWidget->addTab(tabOrigin, QString());
        tabTrimap = new QWidget();
        tabTrimap->setObjectName(QStringLiteral("tabTrimap"));
        horizontalLayout = new QHBoxLayout(tabTrimap);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        labelTrimap = new QLabel(tabTrimap);
        labelTrimap->setObjectName(QStringLiteral("labelTrimap"));

        horizontalLayout->addWidget(labelTrimap);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        tabWidget->addTab(tabTrimap, QString());
        tabRough = new QWidget();
        tabRough->setObjectName(QStringLiteral("tabRough"));
        horizontalLayout_3 = new QHBoxLayout(tabRough);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        labelRough = new QLabel(tabRough);
        labelRough->setObjectName(QStringLiteral("labelRough"));

        horizontalLayout_3->addWidget(labelRough);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        tabWidget->addTab(tabRough, QString());
        tabWatershed = new QWidget();
        tabWatershed->setObjectName(QStringLiteral("tabWatershed"));
        horizontalLayout_4 = new QHBoxLayout(tabWatershed);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        labelWatershed = new QLabel(tabWatershed);
        labelWatershed->setObjectName(QStringLiteral("labelWatershed"));

        horizontalLayout_4->addWidget(labelWatershed);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);

        tabWidget->addTab(tabWatershed, QString());

        horizontalLayout_2->addWidget(tabWidget);

        tabWidget_2 = new QTabWidget(centralWidget);
        tabWidget_2->setObjectName(QStringLiteral("tabWidget_2"));
        tabWidget_2->setDocumentMode(false);
        tabWidget_2->setMovable(true);
        tabHair = new QWidget();
        tabHair->setObjectName(QStringLiteral("tabHair"));
        horizontalLayout_6 = new QHBoxLayout(tabHair);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_9);

        labelHair = new QLabel(tabHair);
        labelHair->setObjectName(QStringLiteral("labelHair"));

        horizontalLayout_6->addWidget(labelHair);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_10);

        tabWidget_2->addTab(tabHair, QString());
        tabAlpha = new QWidget();
        tabAlpha->setObjectName(QStringLiteral("tabAlpha"));
        horizontalLayout_7 = new QHBoxLayout(tabAlpha);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_11);

        labelAlpha = new QLabel(tabAlpha);
        labelAlpha->setObjectName(QStringLiteral("labelAlpha"));

        horizontalLayout_7->addWidget(labelAlpha);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_12);

        tabWidget_2->addTab(tabAlpha, QString());

        horizontalLayout_2->addWidget(tabWidget_2);

        tabWidget_3 = new QTabWidget(centralWidget);
        tabWidget_3->setObjectName(QStringLiteral("tabWidget_3"));
        tabWidget_3->setTabsClosable(false);
        tabWidget_3->setMovable(true);
        tabWidget_3->setTabBarAutoHide(false);
        tabInput1 = new QWidget();
        tabInput1->setObjectName(QStringLiteral("tabInput1"));
        horizontalLayout_8 = new QHBoxLayout(tabInput1);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_13);

        labelInput1 = new QLabel(tabInput1);
        labelInput1->setObjectName(QStringLiteral("labelInput1"));

        horizontalLayout_8->addWidget(labelInput1);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_14);

        tabWidget_3->addTab(tabInput1, QString());
        tabGabor1 = new QWidget();
        tabGabor1->setObjectName(QStringLiteral("tabGabor1"));
        horizontalLayout_9 = new QHBoxLayout(tabGabor1);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_15);

        labelGabor1 = new QLabel(tabGabor1);
        labelGabor1->setObjectName(QStringLiteral("labelGabor1"));

        horizontalLayout_9->addWidget(labelGabor1);

        horizontalSpacer_16 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_16);

        tabWidget_3->addTab(tabGabor1, QString());
        tabInput2 = new QWidget();
        tabInput2->setObjectName(QStringLiteral("tabInput2"));
        horizontalLayout_10 = new QHBoxLayout(tabInput2);
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        horizontalSpacer_17 = new QSpacerItem(67, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_17);

        labelInput2 = new QLabel(tabInput2);
        labelInput2->setObjectName(QStringLiteral("labelInput2"));

        horizontalLayout_10->addWidget(labelInput2);

        horizontalSpacer_18 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_18);

        tabWidget_3->addTab(tabInput2, QString());
        tabGabor2 = new QWidget();
        tabGabor2->setObjectName(QStringLiteral("tabGabor2"));
        horizontalLayout_11 = new QHBoxLayout(tabGabor2);
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        horizontalSpacer_20 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_20);

        labelGabor2 = new QLabel(tabGabor2);
        labelGabor2->setObjectName(QStringLiteral("labelGabor2"));

        horizontalLayout_11->addWidget(labelGabor2);

        horizontalSpacer_19 = new QSpacerItem(67, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_19);

        tabWidget_3->addTab(tabGabor2, QString());
        tabTracing = new QWidget();
        tabTracing->setObjectName(QStringLiteral("tabTracing"));
        horizontalLayout_12 = new QHBoxLayout(tabTracing);
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        horizontalSpacer_22 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_22);

        labelTracing = new QLabel(tabTracing);
        labelTracing->setObjectName(QStringLiteral("labelTracing"));

        horizontalLayout_12->addWidget(labelTracing);

        horizontalSpacer_21 = new QSpacerItem(63, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_21);

        tabWidget_3->addTab(tabTracing, QString());
        tabResult = new QWidget();
        tabResult->setObjectName(QStringLiteral("tabResult"));
        horizontalLayout_13 = new QHBoxLayout(tabResult);
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        horizontalSpacer_24 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_24);

        labelResult = new QLabel(tabResult);
        labelResult->setObjectName(QStringLiteral("labelResult"));

        horizontalLayout_13->addWidget(labelResult);

        horizontalSpacer_23 = new QSpacerItem(67, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_23);

        tabWidget_3->addTab(tabResult, QString());

        horizontalLayout_2->addWidget(tabWidget_3);

        QHairProcessingClass->setCentralWidget(centralWidget);
        tabWidget_3->raise();
        tabWidget->raise();
        tabWidget_2->raise();
        menuBar = new QMenuBar(QHairProcessingClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 839, 26));
        QHairProcessingClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QHairProcessingClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QHairProcessingClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QHairProcessingClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QHairProcessingClass->setStatusBar(statusBar);
        QWidget::setTabOrder(foreButton, backButton);
        QWidget::setTabOrder(backButton, resetButton);
        QWidget::setTabOrder(resetButton, mattingButton);
        QWidget::setTabOrder(mattingButton, gaborButton);
        QWidget::setTabOrder(gaborButton, tracingButton);
        QWidget::setTabOrder(tracingButton, tabWidget);
        QWidget::setTabOrder(tabWidget, tabWidget_2);
        QWidget::setTabOrder(tabWidget_2, tabWidget_3);

        retranslateUi(QHairProcessingClass);
        QObject::connect(backButton, SIGNAL(clicked()), QHairProcessingClass, SLOT(setBackground()));
        QObject::connect(foreButton, SIGNAL(clicked()), QHairProcessingClass, SLOT(setForeground()));
        QObject::connect(resetButton, SIGNAL(clicked()), QHairProcessingClass, SLOT(reset()));
        QObject::connect(mattingButton, SIGNAL(clicked()), QHairProcessingClass, SLOT(matting()));
        QObject::connect(gaborButton, SIGNAL(clicked()), QHairProcessingClass, SLOT(gaborFilter()));
        QObject::connect(tracingButton, SIGNAL(clicked()), QHairProcessingClass, SLOT(tracingProcess()));

        tabWidget->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(0);
        tabWidget_3->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(QHairProcessingClass);
    } // setupUi

    void retranslateUi(QMainWindow *QHairProcessingClass)
    {
        QHairProcessingClass->setWindowTitle(QApplication::translate("QHairProcessingClass", "QHairProcessing", 0));
        actionLoad_File->setText(QApplication::translate("QHairProcessingClass", "Load New File", 0));
        backButton->setText(QApplication::translate("QHairProcessingClass", "Background", 0));
        backButton->setShortcut(QApplication::translate("QHairProcessingClass", "B", 0));
        resetButton->setText(QApplication::translate("QHairProcessingClass", "Reset", 0));
        resetButton->setShortcut(QApplication::translate("QHairProcessingClass", "R", 0));
        mattingButton->setText(QApplication::translate("QHairProcessingClass", "Matting", 0));
        mattingButton->setShortcut(QApplication::translate("QHairProcessingClass", "M", 0));
        foreButton->setText(QApplication::translate("QHairProcessingClass", "Foreground", 0));
        foreButton->setShortcut(QApplication::translate("QHairProcessingClass", "F", 0));
        gaborButton->setText(QApplication::translate("QHairProcessingClass", "Gabor Filter", 0));
        tracingButton->setText(QApplication::translate("QHairProcessingClass", "Tracing", 0));
        labelOrigin->setText(QApplication::translate("QHairProcessingClass", "Origin", 0));
        tabWidget->setTabText(tabWidget->indexOf(tabOrigin), QApplication::translate("QHairProcessingClass", "Origin", 0));
        labelTrimap->setText(QApplication::translate("QHairProcessingClass", "Trimap", 0));
        tabWidget->setTabText(tabWidget->indexOf(tabTrimap), QApplication::translate("QHairProcessingClass", "Trimap", 0));
        labelRough->setText(QApplication::translate("QHairProcessingClass", "Rough", 0));
        tabWidget->setTabText(tabWidget->indexOf(tabRough), QApplication::translate("QHairProcessingClass", "Rough", 0));
        labelWatershed->setText(QApplication::translate("QHairProcessingClass", "Watershed", 0));
        tabWidget->setTabText(tabWidget->indexOf(tabWatershed), QApplication::translate("QHairProcessingClass", "Watershed", 0));
        labelHair->setText(QApplication::translate("QHairProcessingClass", "Hair", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tabHair), QApplication::translate("QHairProcessingClass", "Hair", 0));
        labelAlpha->setText(QApplication::translate("QHairProcessingClass", "Alpha", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tabAlpha), QApplication::translate("QHairProcessingClass", "Alpha", 0));
        labelInput1->setText(QApplication::translate("QHairProcessingClass", "Input1", 0));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tabInput1), QApplication::translate("QHairProcessingClass", "Input1", 0));
        labelGabor1->setText(QApplication::translate("QHairProcessingClass", "Gabor1", 0));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tabGabor1), QApplication::translate("QHairProcessingClass", "Gabor1", 0));
        labelInput2->setText(QApplication::translate("QHairProcessingClass", "Input2", 0));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tabInput2), QApplication::translate("QHairProcessingClass", "Input2", 0));
        labelGabor2->setText(QApplication::translate("QHairProcessingClass", "Gabor2", 0));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tabGabor2), QApplication::translate("QHairProcessingClass", "Gabor2", 0));
        labelTracing->setText(QApplication::translate("QHairProcessingClass", "Tracing", 0));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tabTracing), QApplication::translate("QHairProcessingClass", "Tracing", 0));
        labelResult->setText(QApplication::translate("QHairProcessingClass", "Result", 0));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tabResult), QApplication::translate("QHairProcessingClass", "Result", 0));
    } // retranslateUi

};

namespace Ui {
    class QHairProcessingClass: public Ui_QHairProcessingClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QHAIRPROCESSING_H
