#include "StimulusWidget.h"

#include <QHBoxLayout>
#include <QMargins>

#include "qcustomplot.h"

#include "RtActivation.h"
#include "RtDataIDs.h"
#include "RtExperiment.h"

StimulusWidget::StimulusWidget(QWidget *parent)
  : plot(new QCustomPlot(this))
  , therm(new QCPItemRect(plot)) {

  plot->setBackground(Qt::gray);

  plot->axisRect()->setAutoMargins(QCP::msNone);
  plot->axisRect()->setMargins(QMargins(0,0,0,0));

  plot->xAxis->grid()->setVisible(false);
  plot->yAxis->grid()->setVisible(false);

  plot->xAxis->setTicks(false);
  plot->yAxis->setTicks(false);

  plot->xAxis->setRange(-1, 1);
  plot->yAxis->setRange(-10, 10);

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

    plot->replot();
  }

}
