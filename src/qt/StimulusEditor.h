#pragma once

#include <sstream>

#include <QWizard>

class RtConfigFmriRun;

class StimulusEditor : public QWizard {
  Q_OBJECT

 public:

  StimulusEditor(QWidget *parent, RtConfigFmriRun *run_config);
  virtual ~StimulusEditor();

  bool hasFinished() const {
    return finished;
  }

 public slots:

  void handleNextButton();

  void finish();

 private:

  RtConfigFmriRun *run_config;

  bool finished;
};
