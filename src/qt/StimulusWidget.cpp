#include "StimulusWidget.h"

#include <QHBoxLayout>
#include <QMargins>

#include "qcustomplot.h"

#include "RtActivation.h"
#include "RtDataIDs.h"
#include "RtDesignMatrix.h"
#include "RtExperiment.h"

using std::map;
using std::string;

StimulusWidget::StimulusWidget(QWidget *parent)
  : plot(new QCustomPlot(this))
  , therm(new QCPItemRect(plot))
  , zero(new QCPItemLine(plot))
  , instruction(new QCPItemText(plot))
  , current_tr(0)
  , total_feedback(0.0)
  , num_rois_this_tr(0)
  , design(NULL) {

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

  instruction->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
  instruction->position->setCoords(0, -3.5);
  instruction->setText("");
  instruction->setFont(QFont(font().family(), 20));

  plot->addItem(therm);
  plot->addItem(zero);
  plot->addItem(instruction);

  QHBoxLayout *layout = new QHBoxLayout;
  layout->addWidget(plot);

  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);
}

StimulusWidget::~StimulusWidget() {}

void StimulusWidget::setupFromConfig(RtConfig &config) {
  if (!config.isSet("stimulus:disabled")) {
    cerr << "No stimulus configuration found" << endl;
    return;
  }

  if (config.get("stimulus:disabled")) {
    cerr << "Stimulus disabled" << endl;
    return;
  }

  if (string("thermometer") != config.get("stimulus:type")) {
    cerr << "Error parsing stimulus config: unknown stimulus type "
         << config.get("stimulus:type") << endl;
    return;
  }

  TiXmlElement *stim = static_cast<TiXmlElement*>(config.getNode("stimulus"));
  TiXmlNode *child = 0;
  while((child = stim->IterateChildren(child))) {
    if (child->ValueStr() == "roi") {
      string roi = config.get("name", child);
      double weight = config.get("weight", child);
      cout << "adding roi " << roi << " with weight " << weight << endl;
      addRoi(roi, weight);
    }
    else if (child->ValueStr() == "instruction") {
      string text = config.get("text", child);
      if (config.isSet("condition", child)) {
        string condition = config.get("condition", child);
        addConditionInstruction(condition, text);
      }
      else {
        size_t tr = config.get("tr", child);
        size_t length = config.get("length", child);
        addInstruction(tr, length, text);
      }
    }
  }

  cout << "Stimulus setup completed" << endl;
}

void StimulusWidget::addRoi(const string &name, double weight) {
  if (roi_weights.find(name) != roi_weights.end()) {
    cerr << "Error adding ROI to the stimulus. ROI " << name
         << " already exists" << endl;
    return;
  }

  roi_weights[name] = weight;
}

void StimulusWidget::addConditionInstruction(const string &condition,
                                             const string &instruction) {
  condition_instructions[condition] = instruction;
}

void StimulusWidget::addInstruction(size_t tr, size_t len, const string &text) {
  if (instructions.size() < tr + len + 1) {
    instructions.resize(tr + len + 1);
  }

  for (size_t cur = tr; cur < tr + len; cur++) {
    if (!instructions[cur].empty()) {
      cerr << "Error: can't process overlapping instructions! Ignoring "
           << text << " at " << tr << " because it cofilcts with "
           << instructions[cur];
      continue;
    }

    instructions[cur] = text;
  }

  if (tr == 0) {
    instruction->setText(QString(text.c_str()));
    plot->replot();
  }
}

void StimulusWidget::handleData(QString qid) {
  RtDataID id;
  id.setFromString(qid.toStdString());

  if (id.getDataName() == NAME_DESIGN) {
    design = static_cast<RtDesignMatrix*>(getDataStore().getData(id));
    buildConditionInstructions();
  }

  if (id.getTimePoint() < DATAID_NUM_WILDCARD_VALUE &&
      id.getTimePoint() > current_tr) {
    current_tr = id.getTimePoint();

    if (current_tr < instructions.size()) {
      instruction->setText(QString(instructions[current_tr].c_str()));
    }
    else {
      instruction->setText("");
    }

    plot->replot();
  }

  if (id.getModuleID() == ID_ROICOMBINE) {
    map<string, double>::iterator it = roi_weights.find(id.getRoiID());

    if (it != roi_weights.end()) {

      double val = static_cast<RtActivation*>(
        getDataStore().getData(id))->getPixel(0);

      double weight = it->second;

      // add this contribution
      total_feedback += weight * val;
      num_rois_this_tr++;

      // if it's the last roi, show the feedback
      if (num_rois_this_tr == roi_weights.size()) {

        if (total_feedback > 0) {
          therm->setPen(QPen(Qt::red));
          therm->setBrush(Qt::red);
          therm->topLeft->setCoords(-0.1, total_feedback);
          therm->bottomRight->setCoords(0.1, 0);
        }
        else {
          therm->setPen(QPen(Qt::blue));
          therm->setBrush(Qt::blue);
          therm->topLeft->setCoords(-0.1, 0);
          therm->bottomRight->setCoords(0.1, total_feedback);
        }

        therm->setVisible(true);
        plot->replot();
        total_feedback = 0;
        num_rois_this_tr = 0;
      }
    }
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

void StimulusWidget::buildConditionInstructions() {

  // Add instructions from conditions.
  for (map<string, string>::iterator it = condition_instructions.begin();
       it != condition_instructions.end(); ++it) {
    string condition = it->first;
    string text = it->second;

    size_t condition_column = design->getColumnIndex(condition);
    if (condition_column == std::numeric_limits<unsigned int>::max()) {
      cerr << "Error adding instruction for condition " << condition
           << " not found in the design matrix" << endl;
      continue;
    }

    for (size_t tr = 0; tr < design->getNumMeas(); tr++) {
      if (design->getConditionValueAtTR(tr, condition_column) > 0) {
        if (tr < instructions.size() && !instructions[tr].empty()) {
          cerr << "Error: can't process overlapping condtion instructions! "
               << "Ignoring " << text << " at " << tr
               << " because it cofilcts with " << instructions[tr];
          continue;
        }

        if (tr >= instructions.size()) {
          instructions.resize(tr + 1);
        }

        instructions[tr] = text;
      }
    }
  }
}
