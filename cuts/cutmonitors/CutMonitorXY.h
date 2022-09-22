/*
 * CutMonitorXY.h
 *
 *  Created on: 10-07-2013
 *      Author: Daniel Wielanek
 */

#ifndef HALCUTMONITORXY_H_
#define HALCUTMONITORXY_H_

#include "CutMonitor.h"
/**
 * class for monitoring two cuts/parameters
 */

namespace Hal {
  class CutMonitorXY : public CutMonitor {
#ifdef MPPCUTFULL
    Bool_t fIdenticalCuts;
#endif
  protected:
    void TrueUpdate(Bool_t passed);
    void CreateHistograms();

  public:
    CutMonitorXY();
    /**
     *
     * @param cut_A name of cut monitored on X axis
     * @param cut_B name of cut monitored on Y axis
     */
    CutMonitorXY(TString cut_A, TString cut_B);
    /**
     *
     * @param cut_A name of cut on X-axis
     * @param parameter_A parameter number on X axis
     * @param xAxis x axis conf {bins,min,max}
     * @param cut_B name of cut on Y-axis
     * @param parameter_B parameter number on Y axis
     * @param yAxis configuration of Y-axis (bins,min,max}
     */
    CutMonitorXY(TString cut_A,
                 Int_t parameter_A,
                 std::initializer_list<Double_t> xAxis,
                 TString cut_B,
                 Int_t parameter_B,
                 std::initializer_list<Double_t> yAxis);
    /**
     *
     * @param cut_A name of cut monitored on X axis
     * @param parameter_A number of parameter monitored on X axis
     * @param cut_B name of cut monitored on Y axis
     * @param parameter_B number of parameter monitored on Y axis
     */
    CutMonitorXY(TString cut_A, Int_t parameter_A, TString cut_B, Int_t parameter_B);
    /**
     * copy c-tor
     * @param other object to copy
     */
    CutMonitorXY(const CutMonitorXY& other);
    /**
     * Assignment operator
     * @param other
     * @return
     */
    CutMonitorXY& operator=(const CutMonitorXY& other);
    virtual Bool_t Init(Int_t task_id);
    virtual void Update(Bool_t passed, TObject* obj);
    virtual CutMonitor* MakeCopy() const;
    virtual ~CutMonitorXY();
    ClassDef(CutMonitorXY, 1)
  };
}  // namespace Hal
#endif /* HALCUTMONITORXY_H_ */
