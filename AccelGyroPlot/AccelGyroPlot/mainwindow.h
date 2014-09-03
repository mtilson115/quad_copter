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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "/home/mitch/dataDrive/qcustomplot/qcustomplot.h" // the header file of QCustomPlot. Don't forget to add it to your project, if you use an IDE, so it gets compiled.

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void setupRealtimeDataDemo(QCustomPlot *customPlot);

private slots:
  void realtimeDataSlot();

private:
  Ui::MainWindow *ui;
  QString demoName;
  QTimer dataTimer;
};

#endif // MAINWINDOW_H

