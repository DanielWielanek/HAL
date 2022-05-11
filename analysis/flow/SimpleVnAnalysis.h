/*
 * SimpleVnAnalysis.h
 *
 *  Created on: 19-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALSIMPLEVNANALYSIS_H_
#define HALSIMPLEVNANALYSIS_H_

#include "FlowVariable.h"

#include "HistogramManager.h"
#include "TrackAna.h"

#include <FairTask.h>
#include <Rtypes.h>
#include <RtypesCore.h>
#include <TH2.h>


/**
 * class for making simple flow RP analysis, for this analysis user can add
 * FlowVariable, flow will be drawn as a function of this variable
 */

namespace Hal {
  class SimpleVnAnalysis : public TrackAna {
    Int_t fBinsX, fBinsY;
    FlowVariable *fVarX, *fVarY;
    Double_t fMinX, fMinY, fMaxX, fMaxY, fN, fPhi;
    HistogramManager_1_2D<TH2D>* fNum;
    HistogramManager_1_2D<TH2D>* fDen;
    Double_t GetPhi();

  protected:
    virtual void CheckCutContainerCollections();
    virtual void ProcessTrack();
    virtual void LinkCollections();
    /**
     * not used now
     */
    virtual void FinishAllEvents();
    Package* Report() const;
    virtual Task::EInitFlag Init();

  public:
    /**
     * default constructor
     * @param n coefficient used for calculating flow default 2 - for eliptic flow
     */
    SimpleVnAnalysis(Double_t n = 2);
    /**
     * copy c-tor
     * @param ana
     */
    SimpleVnAnalysis(const SimpleVnAnalysis& ana);
    /**
     * set axis for flow plots
     * @param nbins number of bins
     * @param min lower edge of histogram
     * @param max upper edge of histogram
     * @param axis axis id  - 0 for X axis, 1 for Y axis
     */
    void SetAxis(Int_t nbins, Double_t min, Double_t max, Int_t axis);
    void SetFlowVariableX(const FlowVariable& var);
    void SetFlowVariableY(const FlowVariable& var);
    virtual void FinishTask();
    virtual ~SimpleVnAnalysis();
    ClassDef(SimpleVnAnalysis, 1)
  };
}  // namespace Hal

#endif /* HALSIMPLEN2ANALYSIS_H_ */
