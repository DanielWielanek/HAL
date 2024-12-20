/*
 * HistoStyle.h
 *
 *  Created on: 30 lip 2024
 *      Author: daniel
 */

#ifndef HAL_FEATURES_HIST_STYLES_HISTOSTYLE_H_
#define HAL_FEATURES_HIST_STYLES_HISTOSTYLE_H_

#include <RtypesCore.h>
#include <TGraph.h>
#include <TString.h>

#include "AxisStyle.h"
#include "DividedHisto.h"
#include "FillStyle.h"
#include "LineStyle.h"
#include "MarkerStyle.h"

namespace Hal {
  /**
   * contains information about histogram/graph styles
   */
  class HistoStyle : public Style {
    TString fTitle;
    AxisStyle fXAxis;
    AxisStyle fYAxis;
    AxisStyle fZAxis;
    MarkerStyle fMarker;
    LineStyle fLine;
    FillStyle fFill;

    template<class T>
    void ApplyInternal(T& object) const;

  public:
    static const unsigned short int kTitle;
    static const unsigned short int kMin;
    static const unsigned short int kMax;
    HistoStyle() {};
    /**
     * return predefined style for 1d histogram
     * @param style options:
     * 05 - set margins sizes to 0.05 (default optimal)
     * 05 like above but sets margin sizes to 0.06
     * color - set line color/marker color to red
     * circle - set marker to full circle
     * center center axes
     * apollo - use tt fods
     * @return
     */
    static HistoStyle GetStyle(TString style);
    /**
     * set title size, title offset, label size and label offset simultaneously
     * @param val
     * @param x
     */
    void SetAxisValues(Double_t val, Char_t x = 'x') { SetAxis(val, val, val, val, x); };
    /**
     * set minimal value
     * @param val
     */
    void SetMin(Double_t val);
    /**
     * set maximal value
     */
    void SetMax(Double_t val);
    /**
     * set min and max
     * @param minVal
     * @param maxVal
     */
    void SetMinMax(Double_t minVal, Double_t maxVal);
    /**
     * set marker properties
     * @param c
     * @param marker
     * @param size
     */
    void SetMarkerProperties(Color_t c, Int_t marker, Size_t size);
    /**
     * set tile properties, negative values are ignored
     * @param c
     * @param style
     * @param widgth
     */
    void SetLineProperties(Color_t c, Style_t style, Double_t widgth);
    /**
     * set marker and line color
     * @param c
     */
    void SetColor(Color_t c);
    /**
     * sets range on given axis
     * @param min
     * @param max
     * @param ax axis label x,y or z
     */
    void SetRangeUser(Double_t min, Double_t max, Char_t ax);
    /**
     * set titles of histogram and axes - NOTE: if lenght of string is zero - this attribute will not be set
     * @param histo
     * @param xAxis
     * @param yAxis
     * @param zAxis
     */
    void SetTitles(TString histo, TString xAxis = "", TString yAxis = "", TString zAxis = "");
    /**
     * set axis labels and title attributes, negative values are ignored
     * @param titleSize
     * @param titleOffset
     * @param labelSize
     * @param labelOffset
     * @param x axis type x/y or z
     */
    void SetAxis(Double_t titleSize, Double_t titleOffset, Double_t labelSize, Double_t labelOffset, Char_t x = 'x');
    /**
     * center axis title
     * @param center
     * @param
     */
    void CenterTitle(Bool_t center = kTRUE, Char_t = 'x');
    /*
     * apply setting to histogram
     * @param h
     * @param no - for multigraph of hall
     */
    void Apply(TObject& h, Int_t no = 0) const;
    /**
     * sets anti-color if color is set
     * @param safe - check if color is not to bright (currenty not used)
     */
    void SetAntiColor(Bool_t safe = kFALSE);
    void SetTitle(TString title);
    AxisStyle& GetAxisStyle(Char_t x);
    FillStyle& GetFillStyle() { return fFill; };
    LineStyle& GetLineStyle() { return fLine; }
    MarkerStyle& GetMarkerStyle() { return fMarker; }
    TString GetTitle() const { return fTitle; };
    void ExportToXML(XMLNode* node) const;
    void ImportFromXML(XMLNode* node);
    void Import(const TObject& obj);
    virtual ~HistoStyle() {};
    ClassDef(HistoStyle, 1);
  };

} /* namespace Hal */


#endif /* HAL_FEATURES_HIST_STYLES_HISTOSTYLE_H_ */
