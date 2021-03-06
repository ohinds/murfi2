/*=========================================================================
 *  RtPasser.cpp implements a class that passes data from an
 *  analysis stream to output
 *
 *  Copyright 2007-2013, the MURFI dev team.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include"RtPasser.h"
#include"RtData.h"

string RtPasser::moduleString("passer");

// default constructor
RtPasser::RtPasser() : RtStreamComponent() {
  componentID = moduleString;
}

// id constructor
//  in
//   idTemplate is a an instance of RtDataID where the fields are used as a
//   template to match incomming data to decide if it should be send to
//   the outputs or not. Empty fields are not matched and filled fields are
RtPasser::RtPasser(const RtDataID &idTemplate) : RtStreamComponent() {
  componentID = moduleString;

  // setup id template
  passDataID = idTemplate;
}

// destructor
RtPasser::~RtPasser() {}

//*** initialization routines  ***//
void RtPasser::addOutput(RtOutput *out) {
  outputs.push_back(out);
}

// process a single acquisition
void RtPasser::sendToOutputs(RtData *d) {
  ACE_TRACE(("RtPasser::sendToOutputs"));

  // check if this data is our type
  if(d == NULL || d->getDataID() != passDataID) {
    return;
  }

  // set to all outputs
  for(vector<RtOutput*>::iterator j = outputs.begin();
      j != outputs.end(); j++) {
    (*j)->setData(d);
  }

  return;
}

// processes as a stream component
int RtPasser::process(ACE_Message_Block* mb) {
  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();
  RtData *img = msg->getCurrentData();
  sendToOutputs(img);
  return 0;
}
