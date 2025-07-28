/*
 * CutMonitorComplex.cxx
 *
 *  Created on: 28 lip 2025
 *      Author: daniel
 */

#include "CutMonitorComplex.h"

#include <RtypesCore.h>

#include "Package.h"

namespace Hal {


  CutMonitorReal::CutMonitorReal(const CutMonitorReal& other) {
    if (other.fMonitor) { fMonitor = other.fMonitor->MakeCopy(); }
  }

  CutMonitorReal::CutMonitorReal(Hal::CutMonitor& mon) { fMonitor = mon.MakeCopy(); }

  CutMonitorReal::~CutMonitorReal() {
    if (fMonitor) delete fMonitor;
  }

  CutMonitorImag::CutMonitorImag(const CutMonitorReal& other) {
    if (other.fMonitor) { fMonitor = other.fMonitor->MakeCopy(); }
  }

  CutMonitorImag::CutMonitorImag(Hal::CutMonitor& mon) { fMonitor = mon.MakeCopy(); }

  CutMonitorImag::~CutMonitorImag() {
    if (fMonitor) delete fMonitor;
  }

  Bool_t CutMonitorReal::ObjMonitor() const { return kTRUE; }

  Bool_t CutMonitorReal::AreSimilar(const Hal::CutMonitor& other) const {
    auto cast = dynamic_cast<const Hal::CutMonitorReal*>(&other);
    if (!cast) return kFALSE;
    return fMonitor->AreSimilar(*cast->fMonitor);
  }

  Hal::Package* CutMonitorReal::Report() const {
    auto copyReport = fMonitor->Report();
    copyReport->SetName(ClassName());
    return copyReport;
  }

  void CutMonitorReal::Update(Bool_t bool1, TObject*) {}

  Bool_t CutMonitorReal::Init(Int_t int1) {
    Bool_t stat  = fMonitor->Init(int1);
    fUpdateRatio = fMonitor->GetUpdateRatio();
    return stat;
  }

  Hal::CutMonitor* CutMonitorReal::MakeCopy() const { return new CutMonitorReal(*fMonitor); }

  Bool_t CutMonitorImag::ObjMonitor() const { return kTRUE; }

  Bool_t CutMonitorImag::AreSimilar(const Hal::CutMonitor& other) const {
    auto cast = dynamic_cast<const Hal::CutMonitorImag*>(&other);
    if (!cast) return kFALSE;
    return fMonitor->AreSimilar(*cast->fMonitor);
  }

  Hal::Package* CutMonitorImag::Report() const {
    auto copyReport = fMonitor->Report();
    copyReport->SetName(ClassName());
    return copyReport;
  }

  void CutMonitorImag::Update(Bool_t bool1, TObject*) {
	  switch(fUpdateRatio){
	  case ECutUpdate::kEvent:{

	  }break;
	  case ECutUpdate::kTrack:{

	  }break;
	  case ECutUpdate::kTwoTrack:{
		  Update(bool)
	  }break;
	  }
  }

  Bool_t CutMonitorImag::Init(Int_t int1) {
    Bool_t stat = fMonitor->Init(int1);
    fUpdateRatio = fMonitor->GetUpdateRatio();
    return stat;
  }

  Hal::CutMonitor* CutMonitorImag::MakeCopy() const { return new CutMonitorImag(*fMonitor); }

} /* namespace Hal */
