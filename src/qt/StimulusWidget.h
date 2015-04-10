#pragma once

#include <QtWidgets>

#include "RtDataID.h"

class QCPItemRect;
class QCustomPlot;

class StimulusWidget : public QWidget{
  Q_OBJECT

 public:
  explicit StimulusWidget(QWidget *parent=0);
  ~StimulusWidget();

  QSize minimumSizeHint() const {
    return QSize(600, 600);
  }

  QSize sizeHint() const {
    return QSize(800, 800);
  }

 public slots:

  void handleData(QString qid);

 private:

  QCustomPlot *plot;
  QCPItemRect *therm;

};
