/******************************************************************************
 * RtDataID.cpp defines a class for identifying data uniquely
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-08-13
 *
 *****************************************************************************/

#include"RtDataID.h"

#include"RtExperiment.h"
#include"RtStreamComponent.h"
#include"RtData.h"
#include"site_config.h" // for SITE_ID_NUMBER

#include<climits>

// default constructor
RtDataID::RtDataID() {
  siteIDNum = SITE_ID_NUMBER;
  studyNum =  DATAID_UNSET_VALUE;
  seriesNum = DATAID_UNSET_VALUE;
  timePoint = DATAID_UNSET_VALUE;
  history = "";
  moduleID = "";
  dataName = "";
  roiID = "";

  instantiationTime = RtExperiment::getExperimentElapsedTime();
}

// constructor with known fields
RtDataID::RtDataID(unsigned int _studyNum,
		   unsigned int _seriesNum,
		   unsigned int _timePoint,
		   const string &_history,
		   const string &_moduleID,
		   const string &_dataName,
		   const string &_roiID) {
  siteIDNum = SITE_ID_NUMBER;
  studyNum = _studyNum;
  seriesNum = _seriesNum;
  timePoint = _timePoint;
  history = _history;
  moduleID = _moduleID;
  dataName = _dataName;
  roiID = _roiID;

  instantiationTime = RtExperiment::getExperimentElapsedTime();
}

// constructor from a data id from a previous module
RtDataID::RtDataID(const RtDataID &prevModuleData,
		   const RtStreamComponent &thisModule) {
  setFromInputDataID(prevModuleData, thisModule);
}


// constructor from another data id
//RtDataID::RtDataID(const RtDataID &other) {
//
//}

RtDataID::~RtDataID() {}

//*** operators ***//

// comparison
// if either data id has a field blank ("") it is ignored in the comparison
bool RtDataID::operator==(const RtDataID &other) const {
  return
     (siteIDNum          == other.siteIDNum 
      || siteIDNum       == DATAID_UNSET_VALUE 
      || other.siteIDNum == DATAID_UNSET_VALUE)

  && (studyNum           == other.studyNum  
      || studyNum        == DATAID_UNSET_VALUE 
      || other.studyNum  == DATAID_UNSET_VALUE)

  && (seriesNum          == other.seriesNum 
      || seriesNum       == DATAID_UNSET_VALUE 
      || other.seriesNum == DATAID_UNSET_VALUE)

  && (timePoint          == other.timePoint 
      || timePoint       == DATAID_UNSET_VALUE 
      || other.timePoint == DATAID_UNSET_VALUE)

  && (history            == other.history   
      || history         == "" 
      || other.history   == "")

  && (moduleID           == other.moduleID  
      || moduleID        == "" 
      || other.moduleID  == "")

  && (dataName           == other.dataName  
      || dataName        == "" 
      || other.dataName  == "")

  && (roiID              == other.roiID     
      || roiID           == "" 
      || other.roiID     == "")
    ;
}

// less
bool RtDataID::operator<(const RtDataID &other) const {
  return
       siteIDNum < other.siteIDNum
    || studyNum  < other.studyNum
    || seriesNum < other.seriesNum
    || timePoint < other.timePoint
    || history   < other.history
    || moduleID  < other.moduleID
    || dataName  < other.dataName
    || roiID     < other.roiID
    ;

}

// output to a stream
// ostream &RtDataID::operator<<(ostream &out) const {
//   out
//     << ":" << siteIDNum
//     << ":" << studyNum
//     << ":" << seriesNum
//     << ":" << timePoint
//     << ":" << history
//     << ":" << moduleID
//     << ":" << dataName
//     << ":" << roiID
//     << ":";
//   return out;
// }

// get a string version of the ID
string RtDataID::toString() const {
  stringstream sstr("");

  sstr
    << ":" << siteIDNum
    << ":" << studyNum
    << ":" << seriesNum
    << ":" << timePoint
    << ":" << history
    << ":" << moduleID
    << ":" << dataName
    << ":" << roiID
    << ":";
  return sstr.str();
}

// set the ID from a string
string RtDataID::setFromString(const string &id) {
  return "not yet implemented";
}

// set from input data
void RtDataID::setFromInputData(const RtData &prevModuleData,
				const RtStreamComponent &thisModule) {
  setFromInputDataID(prevModuleData.getDataID(), thisModule);
}

// set from input data
void RtDataID::setFromInputDataID(const RtDataID &prevModuleDataID,
				  const RtStreamComponent &thisModule) {
  (*this) = prevModuleDataID;
  siteIDNum = SITE_ID_NUMBER;
  history = history + moduleID;
  moduleID = thisModule.getID();

  instantiationTime = RtExperiment::getExperimentElapsedTime();
}

