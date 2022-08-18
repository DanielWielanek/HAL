/*
 * QAPlotAxis.h
 *
 *  Created on: 17 sie 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_QA_QAPLOTAXIS_H_
#define HAL_ANALYSIS_QA_QAPLOTAXIS_H_

#include <RtypesCore.h>
#include <TNamed.h>

namespace Hal {

  class QAPlotAxis : public TObject {
    Int_t fFlag;
    Int_t fNBins;
    Double_t fMin;
    Double_t fMax;

  public:
    QAPlotAxis();
    QAPlotAxis(Int_t flag, Int_t bins, Double_t min, Double_t max);
    Int_t GetNBins() const { return fNBins; }
    Int_t GetFlag() const { return fFlag; }
    Double_t GetMax() const { return fMax; }
    Double_t GetMin() const { return fMin; }
    void SetBins(Int_t bins) { fNBins = bins; }
    void SetFlag(Int_t flag) { fFlag = flag; }
    void SetMax(Double_t max) { fMax = max; }
    void SetMin(Double_t min) { fMin = min; }
    void SetRange(Double_t min, Double_t max) {
      fMin = min;
      fMax = max;
    }
    QAPlotAxis(const QAPlotAxis& other) = default;
    QAPlotAxis& operator=(const QAPlotAxis& other) = default;
    virtual ~QAPlotAxis();
    ClassDef(QAPlotAxis, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_QA_QAPLOTAXIS_H_ */
