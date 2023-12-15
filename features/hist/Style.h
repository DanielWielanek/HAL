/*
 * Style.h
 *
 *  Created on: 27 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FEATURES_HIST_STYLE_H_
#define HAL_FEATURES_HIST_STYLE_H_

#include <map>
#include <utility>
#include <vector>

#include <RtypesCore.h>
#include <TH1.h>
#include <TString.h>

class TGraph;

namespace Hal {
  class MultiGraph;
  /**
   * abstract class for holding styles, useful for plot many histograms
   */
  class Style {
    std::map<TString, Double_t> fDoubles;
    std::map<TString, Int_t> fInts;

  protected:
    /**
     * finds given integer value
     * @param key
     * @param val
     * @return false if key not found
     */
    Bool_t Find(TString key, Int_t& val) const;
    /**
     * finds given double value
     * @param key
     * @param val
     * @return false if key not found
     */
    Bool_t Find(TString key, Double_t& val) const;
    /**
     * register int parameter
     * @param key
     * @param val
     */
    void Register(TString key, Int_t val);
    /**
     * register double parameter
     * @param key
     * @param val
     */
    void Register(TString key, Double_t val);
    /**
     * register many parameters
     * @param keys
     * @param vals
     */
    void Register(std::initializer_list<TString> keys, std::initializer_list<Int_t> vals);
    /**
     * register many parameters
     * @param keys
     * @param vals
     */
    void Register(std::initializer_list<TString> keys, std::initializer_list<Double_t> vals);

  public:
    Style() {};
    /**
     *
     * @param key
     * @return true if key is found
     */
    Bool_t FindKey(TString key) const;
    Style& operator=(const Style& style) = default;
    virtual ~Style() {};
    ClassDef(Style, 1);
  };

  class PadStyle : public Style {
  public:
    PadStyle();
    PadStyle(Double_t x1, Double_t y1, Double_t x2, Double_t y2);
    void SetBottomMargin(Double_t x) { Register("bottom", x); };
    void SetTopMargin(Double_t x) { Register("top", x); };
    void SetLeftMargin(Double_t x) { Register("left", x); };
    void SetRightMargin(Double_t x) { Register("right", x); };
    void SetMargins(Double_t x1, Double_t y1, Double_t x2, Double_t y2);
    void SetLogx(Bool_t logz = kTRUE) { Register("logx", Int_t(logz)); };
    void SetLogy(Bool_t logz = kTRUE) { Register("logy", Int_t(logz)); };
    void SetLogz(Bool_t logz = kTRUE) { Register("logz", Int_t(logz)); };
    void SetLog(Bool_t x, Bool_t y, Bool_t z);
    void SetGridx(Bool_t grid = kTRUE) { Register("gridx", Int_t(grid)); };
    void SetGridy(Bool_t grid = kTRUE) { Register("gridy", Int_t(grid)); };
    void SetGrid(Bool_t x, Bool_t y) {
      SetGridx(x);
      SetGridy(y);
    };
    /**
     * apply setting to current gpad
     */
    void Apply();
    virtual ~PadStyle() {}
    ClassDef(PadStyle, 1);
  };

  class HistoStyle : public Style {
    TString fTitle;
    TString fAxTitle[3];
    void ApplyTH(TH1& h) const;
    void ApplyTGraph(TGraph& gr) const;
    void ApplyMGraph(MultiGraph& gr, Int_t no) const;

  public:
    HistoStyle();
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
     * set marker color
     * @param col
     */
    void SetMarkerColor(Color_t col);
    /**
     * set marker style
     * @param marker
     */
    void SetMarkerStyle(Int_t marker);
    /**
     * set marker size
     * @param size
     */
    void SetMarkerSize(Size_t size);
    /**
     * set marker properties
     * @param c
     * @param marker
     * @param size
     */
    void SetMarkerProperties(Color_t c, Int_t marker, Size_t size);
    /**
     * set line width
     * @param width
     */
    void SetLineWidth(Double_t width);
    /**
     * sets line color
     * @param c
     */
    void SetLineColor(Color_t c);
    /**
     * sets line style
     * @param s
     */
    void SetLineStyle(Style_t s);
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
    virtual ~HistoStyle() {};
    ClassDef(HistoStyle, 1);
  };


} /* namespace Hal */


#endif /* HAL_FEATURES_HIST_STYLE_H_ */
