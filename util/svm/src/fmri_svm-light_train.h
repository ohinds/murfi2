// train an svm model on fmri data using fsl for preprocessing and
// svm-light for model training
//
// Oliver Hinds <ohinds@mit.edu> 2009-07-15

#ifndef FMRI_SVMLIGHT_TRAIN
#define FMRI_SVMLIGHT_TRAIN

#include "fmri_svm-light_common.h"

// read files, perform preprocessing
bool createTrainingData(Parms &parms);

// command line parm manipulation
void printUsage(string executename);
Parms parseArgs(int argc, char **argv);
bool validateParams(Parms &p);

#endif
