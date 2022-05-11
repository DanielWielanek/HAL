/*
 * HalChiSqMap.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCHISQMAP2D_H_
#define HALCHISQMAP2D_H_

#include "Object.h"

#include <TString.h>


class TH2D;
class TLine;
namespace Hal {
  class ChiSqMap2D : public Object {
    TH2D* fHist;
    TLine* fLineX;
    TLine* fLineY;
    TLine* fLineXlow;
    TLine* fLineXhigh;
    TLine* fLineYlow;
    TLine* fLineYhigh;
    TString fLabel;
    void DrawLineX(TLine* l);
    void DrawLineY(TLine* l);
    Bool_t CheckX(Double_t x) const;
    Bool_t CheckY(Double_t y) const;
    TLine* MakeXLine(Double_t x, Style_t style) const;
    TLine* MakeYLine(Double_t x, Style_t style) const;

  protected:
    ChiSqMap2D();

  public:
    /**
     *
     * @param name name of ma
     * @param xbins number of x-bins
     * @param xlow low edge of first x-bin
     * @param xhigh high edge of last x-bin
     * @param ybins y bins
     * @param ylow low edge of first y-bin
     * @param yhigh high edge of last y-bin
     */
    ChiSqMap2D(TString name, Int_t xbins, Double_t xlow, Double_t xhigh, Int_t ybins, Double_t ylow, Double_t yhigh);
    /**
     * @param xpar name of x parameter
     * @param ypar name of y parameter
     */
    void SetParNames(TString xpar, TString ypar);
    /**
     * set label it should describe value of minimized value
     * @param label
     */
    void SetLabel(TString label) { fLabel = label; };
    /**
     *
     * @return label
     */
    TString GetLabel() const { return fLabel; };
    /**
     * @return label for chi-square test
     */
    static TString ChiLabel() { return "#chi^{2}"; };
    /**
     *
     * @return label for chi-square test value normalized by NDF
     */
    static TString ChiNDFLabel() { return "#chi^{2}/NDF"; };
    /**
     *
     * @return value of X parameter
     */
    Double_t GetFitX() const;
    /**
     *
     * @return value of Y parameter
     */
    Double_t GetFitY() const;
    /**
     *
     * @return low X-paremeter error
     */
    Double_t GetFitXErrorUp() const;
    /**
     *
     * @return high  X-paremeter error
     */
    Double_t GetFitXErrorLow() const;
    /**
     *
     * @return high Y-paremeter error
     */
    Double_t GetFitYErrorUp() const;
    /**
     *
     * @return low  Y-paremeter error
     */
    Double_t GetFitYErrorLow() const;
    /**
     * calculate uncertainty estimated as an thres% change
     * @param thres threshold of uncertainty, if negative uses "hessian like method"
     * @param around_fit if true estimate around fit, otherwise estimate around
     * global minimum
     * @return 0 if calculation failed
     */
    Double_t GetEstErrorX(Double_t thres = 0.1, Bool_t around_fit = kTRUE) const;
    /**
     * calculate uncertainty estimated as an thres% change
     * @param thres threshold of uncertainty, if negative uses "hessian like method"
     * @param around_fit if true estimate around fit, otherwise estimate around
     * global minimum
     * @return 0 if calculation failed
     */
    Double_t GetEstErrorY(Double_t thres = 0.1, Bool_t around_fit = kTRUE) const;
    /**
     *
     * @return minimum on chi2 with parabola interpolation
     */
    Double_t GetEstX() const;
    /**
     *
     * @return minimum on chi2 with parabola interpolation
     */
    Double_t GetEstY() const;

    /**
     * histogram with chi-square map
     */
    TH2D* GetHist() const { return fHist; };
    /**
     * set fitted parameter on X-axis
     * @param Value parameter value
     * @param errLow lower error
     * @param errHigh higher error if negative then high error = low error
     */
    void SetXFit(Double_t Value, Double_t errLow = 0, Double_t errHigh = -1);
    /**
     * set fitted parameter on Y-axis
     * @param Value parameter value
     * @param errLow lower error
     * @param errHigh higher error if negative then high error = low error
     */
    void SetYFit(Double_t Value, Double_t errLow = 0, Double_t errHigh = -1);
    /**
     * return minimal values
     * @param x
     * @param y
     */
    void GetMin(Double_t& x, Double_t& y) const;
    /**
     *
     * @param opt min - draw min value on chi2 map as a square
     * fit - draw min fitted by parabola around found minimum
     * nolin - draw without fitted parameters
     */
    void Draw(Option_t* opt = "");
    /**
     * find min max and calculate statistic uncertainty by revering hessian
     * @param x
     * @param eX
     * @param y
     * @param eY
     */
    void GetXYE(Double_t& x, Double_t& eX, Double_t& y, Double_t& eY) const;
    virtual ~ChiSqMap2D();
    ClassDef(ChiSqMap2D, 1)
  };
}  // namespace Hal
#endif /* HALCHISQMAP2D_H_ */
