#pragma once

#include <map>
#include <string>
#include <vector>

#include <QtWidgets>

#include "RtConfig.h"
#include "RtDataID.h"

class QCPItemLine;
class QCPItemRect;
class QCPItemText;
class QCustomPlot;
class QKeyEvent;

class RtDesignMatrix;

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

  void setupFromConfig(RtConfig &config);

  // register an ROI for feedback display
  void addRoi(const string &name, double weight);

  // schedule an instruction to be shown at a particular time. TR is
  // one-based, specifying TR=0 will set instructions prior to the
  // start of the run.
  void addInstruction(size_t tr, size_t len, const string &text);

  // add an instruction to be shows only during a certain condition
  void addConditionInstruction(const string &condition, const string &text);

 public slots:

  void handleData(QString qid);

  void keyPressEvent(QKeyEvent *);

 private:

  void buildConditionInstructions();

  QCustomPlot *plot;
  QCPItemRect *therm;
  QCPItemLine *zero;
  QCPItemText *instruction;

  std::map<std::string, double> roi_weights;
  std::vector<std::string> instructions;
  std::map<std::string,std::string> condition_instructions;

  size_t current_tr;
  double total_feedback;
  size_t num_rois_this_tr;

  RtDesignMatrix *design;
};
