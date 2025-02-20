/*
 * OTFIOManager.cxx
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "OTFIOManager.h"

#include "Cout.h"
#include "InputDataInfo.h"
#include "VirtualSource.h"

#include "OTFSource.h"

#include <TBranch.h>
#include <TFile.h>
#include <TList.h>
#include <TObjString.h>
#include <TSystem.h>
#include <TTree.h>

#include <iostream>

namespace HalOTF {

  IOManager::IOManager(HalOTF::Source* source, Int_t entries) : Hal::VirtualIOManager(source, entries) {}

  IOManager::~IOManager() {}

}  // namespace HalOTF
