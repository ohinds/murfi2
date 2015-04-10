#include "StimulusWidget.h"

#include <QHBoxLayout>
#include <QMargins>

#include "qcustomplot.h"

#include "RtActivation.h"
#include "RtDataIDs.h"
#include "RtExperiment.h"

StimulusWidget::StimulusWidget(QWidget *parent)
  : plot(new QCustomPlot(this))
  , therm(new QCPItemRect(plot))
  , zero(new QCPItemLine(plot)) {

  plot->setBackground(Qt::gray);

  plot->axisRect()->setAutoMargins(QCP::msNone);
  plot->axisRect()->setMargins(QMargins(0,0,0,0));

  plot->xAxis->grid()->setVisible(false);
  plot->yAxis->grid()->setVisible(false);

  plot->xAxis->setTicks(false);
  plot->yAxis->setTicks(false);

  plot->xAxis->setRange(-1, 1);
  plot->yAxis->setRange(-4, 4);

  therm->setVisible(false);

  zero->setPen(QPen(Qt::black));
  zero->start->setCoords(-0.2, 0);
  zero->end->setCoords(0.2, 0);

  plot->addItem(therm);
  plot->addItem(zero);

  QHBoxLayout *layout = new QHBoxLayout;
  layout->addWidget(plot);

  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

}

StimulusWidget::~StimulusWidget() {
  delete plot;
}

void StimulusWidget::handleData(QString qid) {
  RtDataID id;
  id.setFromString(qid.toStdString());

  if (id.getModuleID() == ID_ROICOMBINE) {
    double val = static_cast<RtActivation*>(
      getDataStore().getData(id))->getPixel(0);

    if (val > 0) {
      therm->setPen(QPen(Qt::red));
      therm->setBrush(Qt::red);
      therm->topLeft->setCoords(-0.1, val);
      therm->bottomRight->setCoords(0.1, 0);
    }
    else {
      therm->setPen(QPen(Qt::blue));
      therm->setBrush(Qt::blue);
      therm->topLeft->setCoords(-0.1, 0);
      therm->bottomRight->setCoords(0.1, val);
    }

    therm->setVisible(true);
    plot->replot();
  }

}

void StimulusWidget::keyPressEvent(QKeyEvent *event) {
  switch(event->key()) {
    case Qt::Key_Escape:
      setWindowState(Qt::WindowNoState);
      break;
    case Qt::Key_F:
      setWindowState(Qt::WindowFullScreen);
      break;
  }

}
