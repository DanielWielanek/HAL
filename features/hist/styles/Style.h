/*
 * Style.h
 *
 *  Created on: 27 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FEATURES_HIST_STYLE2_H_
#define HAL_FEATURES_HIST_STYLE2_H_

#include <array>
#include <map>
#include <utility>
#include <vector>

#include <RtypesCore.h>
#include <TH1.h>
#include <TString.h>

class TGraph;
class TVirtualPad;
namespace Hal {
  class MultiGraph;
  class XMLNode;
  /**
   * basic class for holding styles, useful for plot many histograms
   * this class however do not store all possible configuration of histogram
   * in principle it contains values (int or float) and flags, wherever user call
   * set the flag bit is changed from 0 to 1, such values are later exported.
   * It mean that it contains only information about values that were changed.
   */
  class Style {
  private:
    union value {
      Float_t sFval;
      Int_t sIval;
    };
    std::array<Hal::Style::value, 64> fValues;
    ULong64_t fFlags = {0};

  protected:
    void SetF(Int_t bit, Float_t val);
    void SetI(Int_t bit, Int_t val);
    Float_t GetF(Int_t bit) const { return fValues[bit].sFval; }
    Int_t GetI(Int_t bit) const { return fValues[bit].sIval; }

  public:
    Style() {};
    /**
     * return true if value was set
     * @param bit
     * @return
     */
    Bool_t Find(Int_t bit) const { return TESTBIT(fFlags, bit); }
    Style& operator=(const Style& style) = default;
    /**
     * this method should export configuration to xml - note only changed
     * values are exported
     * @param node
     */
    void ExportToXML(XMLNode* node) const;
    /**
     * this should import configuration from xml
     * @param node
     */
    void ImportFromXML(XMLNode* node);
    virtual ~Style() {};
    ClassDef(Style, 1);
  };


}  // namespace Hal


#endif /* HAL_FEATURES_HIST_STYLE_H_ */
