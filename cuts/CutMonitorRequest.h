
/*
 * Cuts.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *      E-mail: daniel.wielanek@gmail.com
 *      Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCUTSANDMONITORSREQUEST_H_
#define HALCUTSANDMONITORSREQUEST_H_

#include "HistogramManager.h"

#include <TObject.h>
#include <TString.h>


/**
 * represents configuration of axis in cut monitor
 */
namespace Hal {
  class CutMonAxisConf : public HistogramAxisConf {
    Int_t fCutNo;
    Int_t fParameterNo;
    TString fCutName;

  public:
    CutMonAxisConf() : HistogramAxisConf(), fCutNo(-1), fParameterNo(0) {};
    /**
     *
     * @param cut number of cut
     * @param par number of monitored parameter in cut
     * @param bin number of bins
     * @param min lower range of axis
     * @param max upper range of axis
     */
    CutMonAxisConf(Int_t cut, Int_t par = 0, Int_t bin = 100, Double_t min = 0, Double_t max = 1) :
      HistogramAxisConf(bin, min, max), fCutNo(cut), fParameterNo(par) {}
    /**
     *
     * @param name name of the cut class
     * @param par number of monitored parameter in cut
     * @param bin number of bins
     * @param min lower range of axis
     * @param max upper range of axis
     */
    CutMonAxisConf(TString name, Int_t par = 0, Int_t bin = 100, Double_t min = 0, Double_t max = 1) :
      HistogramAxisConf(bin, min, max), fCutNo(-1), fParameterNo(par), fCutName(name) {}
    /***
     *
     * @return number of cut
     */
    Int_t GetCutNo() const { return fCutNo; }
    /**
     * set axis parameters
     * @param bins number of bins
     * @param min lower range of axis
     * @param max upper range of axis
     */
    void SetAxis(Int_t bins, Double_t min, Double_t max) { SetConf(bins, min, max); };
    /**
     * set cut number only if it equal to -1
     * @param no
     */
    void FixCutNo(Int_t no);
    /**
     *
     * @return number of monitor paramter in given cut
     */
    Int_t GetParameterNo() const { return fParameterNo; }
    /**
     *
     * @return class name of cut
     */
    TString GetCutClassName() const { return fCutName; };
    virtual ~CutMonAxisConf() {};
    ClassDef(CutMonAxisConf, 1)
  };

  /**
   * represents request of cut monitor for CutsAndMonitors
   */

  class CutMonitorRequest : public TObject {
    CutMonAxisConf fXConf;
    CutMonAxisConf fYConf;
    CutMonAxisConf fZConf;
    const Int_t fDim;

  public:
    CutMonitorRequest(Int_t dim = -1) : fDim(dim) {};
    CutMonitorRequest(const CutMonAxisConf& x);
    CutMonitorRequest(const CutMonAxisConf& x, const CutMonAxisConf& y);
    CutMonitorRequest(const CutMonAxisConf& x, const CutMonAxisConf& y, const CutMonAxisConf& z);
    /**
     *
     * @param axis 'x' 'y' or 'z'
     * @return configuration for given axis
     */
    CutMonAxisConf GetConf(Char_t axis) const;
    /**
     *
     * @return number of dimensions in histogram
     */
    Int_t GetNDim() const { return fDim; };
    virtual ~CutMonitorRequest() {};
    ClassDef(CutMonitorRequest, 1)
  };
}  // namespace Hal
#endif
