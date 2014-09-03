/***************************************************************************
 *
 * Plots the accel and gyroscope output
 *
 * Creater: Mitchell Tilson
 *
 * Revision: 1.0 4/13/2014
 *
 *
 ***************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serial_port.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>

int serial_fd = 0;

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setGeometry(400, 250, 542, 390);
  setupRealtimeDataDemo(ui->widget);
}

void MainWindow::setupRealtimeDataDemo(QCustomPlot *cPlot)
{
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
  QMessageBox::critical(this, "", "You're using Qt < 4.7, the realtime data demo needs functions that are available with Qt 4.7 to work properly");
#endif
  demoName = "Real Time Data Demo";

  // include this section to fully disable antialiasing for higher performance:
  /*
  customPlot->setNotAntialiasedElements(QCP::aeAll);
  QFont font;
  font.setStyleStrategy(QFont::NoAntialias);
  customPlot->xAxis->setTickLabelFont(font);
  customPlot->yAxis->setTickLabelFont(font);
  customPlot->legend->setFont(font);
  */
  cPlot->addGraph(); // blue line
  cPlot->graph(0)->setPen(QPen(Qt::blue));
  cPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
  cPlot->graph(0)->setAntialiasedFill(false);
  cPlot->addGraph(); // red line
  cPlot->graph(1)->setPen(QPen(Qt::red));
  cPlot->graph(0)->setChannelFillGraph(cPlot->graph(1));

  cPlot->addGraph(); // blue dot
  cPlot->graph(2)->setPen(QPen(Qt::blue));
  cPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
  cPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
  cPlot->addGraph(); // red dot
  cPlot->graph(3)->setPen(QPen(Qt::red));
  cPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
  cPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);

  cPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  cPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  cPlot->xAxis->setAutoTickStep(false);
  cPlot->xAxis->setTickStep(2);
  cPlot->axisRect()->setupFullAxesBox();

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(cPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), cPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(cPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), cPlot->yAxis2, SLOT(setRange(QCPRange)));

  // Serial port
  serial_fd = open_port("/dev/ttyUSB0");
  set_baud(serial_fd,"115200");

  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

void MainWindow::realtimeDataSlot()
{
  // calculate two new data points:
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
  double key = 0;
#else
  double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
#endif
  static double lastPointKey = 0;

  // Serial port data
  char buffer[1] = {0};
  QString data;
  int time_out = 3000;
  int i = 0;

  // lock up and read the serial port buffer
  while( buffer[0] != '\n' && buffer[0] != '\r' && i < 200 && (time_out-- != 0) ) {
      read(serial_fd,buffer,1);
      data.append(buffer[0]);
      i++;
  }
  QStringList data_list;
  data_list = data.split(",");


  if (key-lastPointKey > 0.01 && i > 10 ) // at most add point every 10 ms
  {
    double value0 = qSin(key); //sin(key*1.6+cos(key*1.7)*2)*10 + sin(key*1.2+0.56)*20 + 26;
    double value1 = qCos(key); //sin(key*1.3+cos(key*1.2)*1.2)*7 + sin(key*0.9+0.26)*24 + 26;
    // add data to lines:
    //ui->customPlot->graph(0)->addData(key, value0);
    ui->widget->graph(0)->addData(key, data_list[3].toInt());
    //ui->customPlot->graph(1)->addData(key, value1);
    ui->widget->graph(1)->addData(key, data_list[4].toInt());
    // set data of dots:
    ui->widget->graph(2)->clearData();
    // ui->customPlot->graph(2)->addData(key, value0);
    ui->widget->graph(2)->addData(key, data_list[3].toInt());
    ui->widget->graph(3)->clearData();
    // ui->customPlot->graph(3)->addData(key, value1);
    ui->widget->graph(3)->addData(key, data_list[4].toInt());
    // remove data of lines that's outside visible range:
    ui->widget->graph(0)->removeDataBefore(key-8);
    ui->widget->graph(1)->removeDataBefore(key-8);
    // rescale value (vertical) axis to fit the current data:
    ui->widget->graph(0)->rescaleValueAxis();
    ui->widget->graph(1)->rescaleValueAxis(true);
    lastPointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->widget->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
  ui->widget->replot();

  // calculate frames per second:
  static double lastFpsKey;
  static int frameCount;
  ++frameCount;
  if (key-lastFpsKey > 2) // average fps over 2 seconds
  {
    ui->statusBar->showMessage(
          QString("%1 FPS, Total Data points: %2")
          .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
          .arg(ui->widget->graph(0)->data()->count()+ui->widget->graph(1)->data()->count())
          , 0);
    lastFpsKey = key;
    frameCount = 0;
  }
}

MainWindow::~MainWindow()
{
  delete ui;
}
