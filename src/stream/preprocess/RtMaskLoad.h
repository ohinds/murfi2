/*=========================================================================
 *  RtMaskLoad.h is the header for a stream module that loads a mask
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

#ifndef RTMASKLOAD_H
#define RTMASKLOAD_H

#include"RtStreamComponent.h"

class RtActivation;
class RtMaskImage;

// class declaration
class RtMaskLoad : public RtStreamComponent {

 public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtMaskLoad();

  // destructor
  virtual ~RtMaskLoad();

 protected:

  // process an option
  //  in
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text,
                             const map<string,string> &attr);

  // make sure we are configured properly
  virtual bool validateComponentConfig();

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  // data members
  string filename;
  string roiID;
  bool align;
  bool mosaic;
  bool unmosaic;
  bool flipLR;
  bool dynamic;
  bool isWeights;
  bool save;

  // data
  RtMaskImage *maskLoad;
  RtActivation *weightMask;
};

#endif
