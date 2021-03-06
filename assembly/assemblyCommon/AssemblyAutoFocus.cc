/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <QFormLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QPixmap>
#include <QLabel>
#include <QApplication>

#include <TGraph.h>
#include <TCanvas.h>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include <QDir>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif

#include "AssemblyAutoFocus.h"

using namespace std;
using namespace cv;

AssemblyAutoFocus::AssemblyAutoFocus(AssemblyScanner* cmdr_zscan, QWidget *parent)
  : QWidget(parent)
{

  NQLog("AssemblyAutoFocus::AssemblyAutoFocus");
    
  QGridLayout *l = new QGridLayout(this);
  setLayout(l);

  QGridLayout *g0 = new QGridLayout(this);
  l->addLayout(g0,0,0);
    
    NQLog("AssemblyAutoFocus::AssemblyAutoFocus 2");


  QPalette palette;
  palette.setColor(QPalette::Background, QColor(220, 220, 220));
    
  imageView_1 = new AssemblyUEyeView();
  imageView_1->setMinimumSize(200,200);
  imageView_1->setPalette(palette);
  imageView_1->setBackgroundRole(QPalette::Background);
  imageView_1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageView_1->setScaledContents(true);
  imageView_1->setAlignment(Qt::AlignCenter);
  QApplication::processEvents();
  imageView_1->connectImageProducer(cmdr_zscan, SIGNAL(updateScanImage(cv::Mat)));
    
  scrollArea_1 = new QScrollArea(this);
  scrollArea_1->setMinimumSize(200, 200);
  scrollArea_1->setPalette(palette);
  scrollArea_1->setBackgroundRole(QPalette::Background);
  scrollArea_1->setAlignment(Qt::AlignCenter);
  scrollArea_1->setWidget(imageView_1);
  scrollArea_1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  g0->addWidget(scrollArea_1,0,0);

    
  imageView_2 = new AssemblyUEyeView();
  imageView_2->setMinimumSize(200,200);
  imageView_2->setPalette(palette);
  imageView_2->setBackgroundRole(QPalette::Background);
  imageView_2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageView_2->setScaledContents(true);
  imageView_2->setAlignment(Qt::AlignCenter);
  imageView_2->setZoomFactor(1.0);

  QApplication::processEvents();    
  imageView_2->connectImageProducer(this, SIGNAL(graph_made(const cv::Mat&)));

  scrollArea_2 = new QScrollArea(this);
  scrollArea_2->setMinimumSize(200, 200);
  scrollArea_2->setPalette(palette);
  scrollArea_2->setBackgroundRole(QPalette::Background);
  scrollArea_2->setAlignment(Qt::AlignCenter);
  scrollArea_2->setWidget(imageView_2);
  scrollArea_2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
  g0->addWidget(scrollArea_2,0,1);
    

  lE1 = new QLineEdit("0.1,10");
  g0->addWidget(lE1,1,0);

  button1 = new QPushButton("AutoFocus (step distance (mm), number of steps)", this);
  g0->addWidget(button1,2,0);
    
  checkbox = new QCheckBox("Track marker", this);
  g0->addWidget(checkbox,3,0);
    
  QFormLayout *g1 = new QFormLayout(this);
  g0->addLayout(g1,1,1);
    
  button2 = new QPushButton("Go to focal point", this);
  g1->addRow(button2);
    
  lE2 = new QLineEdit("Absolute focal point = ");
  g1->addRow(lE2);

    
  //make all the neccessary connections
  connect(button1, SIGNAL(clicked()), this, SLOT(configure_scan()));
  connect(this , SIGNAL(run_scan(double, int)), cmdr_zscan , SLOT(run_scan(double, int)));
  connect(button2, SIGNAL(clicked()), this, SLOT(go_to_focal_point()));

  NQLog("AssemblyAutoFocus::AssemblyAutoFocus 3");
    
    
}

void AssemblyAutoFocus::configure_scan()
{
    
    //parse lineEdit text to get target coordinates
    QString  parent_string = this->lE1->text();
    
    QStringList pieces = parent_string.split( "," );
    QString x = pieces.value( pieces.length() - 2);
    QString y = pieces.value( pieces.length() - 1);
    
    double x_d = x.toDouble();
    double y_d = y.toInt();
    
    NQLog("AssemblyAutoFocus::configure_scan(),  Scan range  = ") << x_d << ",   "<< y_d ;
    
    emit run_scan(x_d, y_d);
    
}


void AssemblyAutoFocus::make_graph(const string img_name){


  cv::Mat img = cv::imread(img_name, CV_LOAD_IMAGE_COLOR);
  emit graph_made(img);
        
}


void AssemblyAutoFocus::updateText(double z){

  NQLog("AssemblyAutoFocus::updateText");

  std::ostringstream strs;
  strs.clear();
  strs << z;
//  strs << ",";
  std::string str = strs.str();
  QString qstr = "";
  qstr = QString::fromStdString(str);
  qstr = "Absolute focal point (measured)  = " + qstr;
  lE2->setText(qstr);
}




void AssemblyAutoFocus::go_to_focal_point(){
    
    NQLog("AssemblyAutoFocus::go_to_focal_point()");
    
}


void AssemblyAutoFocus::updateImage(int stage, std::string filename)
{
  NQLog("AssemblyAutoFocus") << ":updateImage()  " + filename;

  cv::Mat img_gs = cv::imread(filename, CV_LOAD_IMAGE_COLOR);

  if (stage == 1 ){
    imageView_1->setZoomFactor(0.2);
    imageView_1->setImage(img_gs);
    
  }
}


void AssemblyAutoFocus::connectImageProducer(const QObject* sender,
                                                   const char* signal)
{
  NQLog("AssemblyAutoFocus") << ":connectImageProducer";

  imageView_1->connectImageProducer(sender, signal);

  connect(sender, signal,
          this, SLOT(imageAcquired(const cv::Mat&)));
}

void AssemblyAutoFocus::disconnectImageProducer(const QObject* sender,
                                                      const char* signal)
{
  NQLog("AssemblyAutoFocus") << ":disconnectImageProducer";

  imageView_1->disconnectImageProducer(sender, signal);

  disconnect(sender, signal,
             this, SLOT(imageAcquired(const cv::Mat&)));
}



void AssemblyAutoFocus::imageAcquired(const cv::Mat& newImage)
{
  NQLog("AssemblyAutoFocus") << ":imageAcquired";

  newImage.copyTo(image_);
}

void AssemblyAutoFocus::keyReleaseEvent(QKeyEvent * event)
{
  if (!(event->modifiers() & Qt::ShiftModifier)) {
    switch (event->key()) {
    case Qt::Key_0:
      imageView_1->setZoomFactor(0.25);
      event->accept();
      break;
    case Qt::Key_1:
      imageView_1->setZoomFactor(1.00);
      event->accept();
      break;
    case Qt::Key_Plus:
      imageView_1->increaseZoomFactor();
      event->accept();
      break;
    case Qt::Key_Minus:
      imageView_1->decreaseZoomFactor();
      event->accept();
      break;
    default:
      break;
    }
  }
}




