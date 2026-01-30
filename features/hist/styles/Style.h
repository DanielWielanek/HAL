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
      value() : sIval(0) {};
    };
    std::array<Hal::Style::value, 64> fValues;
    ULong64_t fFlags = {0};
    void SetFloatInner(Int_t bit, Float_t val);
    void SetIntInner(Int_t bit, Int_t val);
    Float_t GetFloatInner(Int_t bit) const { return fValues[bit].sFval; }
    Int_t GetIntInner(Int_t bit) const { return fValues[bit].sIval; }
    Bool_t FindInner(Int_t bit) const { return TESTBIT(fFlags, bit); }

  protected:
    /**
     * set float
     * @tparam TEnum
     * @tparam
     * @param bit
     * @param val
     */
    template<typename TEnum, typename = std::enable_if_t<std::is_enum_v<TEnum>>>
    void SetF(TEnum bit, Float_t val) {
      SetFloatInner(static_cast<Int_t>(bit), val);
    };
    /**
     * set integer
     * @tparam TEnum
     * @tparam
     * @param bit
     * @param val
     */
    template<typename TEnum, typename = std::enable_if_t<std::is_enum_v<TEnum>>>
    void SetI(TEnum bit, Int_t val) {
      SetIntInner(static_cast<Int_t>(bit), val);
    }
    /**
     * return float
     * @tparam TEnum
     * @tparam
     * @param bit
     * @return
     */
    template<typename TEnum, typename = std::enable_if_t<std::is_enum_v<TEnum>>>
    Float_t GetF(TEnum bit) const {
      return GetFloatInner(static_cast<Int_t>(bit));
    }
    /**
     * set integer
     * @tparam TEnum
     * @tparam
     * @param bit
     * @return
     */
    template<typename TEnum, typename = std::enable_if_t<std::is_enum_v<TEnum>>>
    Int_t GetI(TEnum bit) const {
      return GetIntInner(static_cast<Int_t>(bit));
    }

  public:
    Style() {};
    /**
     * return true if value was set
     * @param bit
     * @return
     */
    template<typename TEnum, typename = std::enable_if_t<std::is_enum_v<TEnum>>>
    Bool_t Find(TEnum bit) const {
      return FindInner(static_cast<Int_t>(bit));
    }
    Style& operator=(const Style& style) = default;
    /**
     * this method should export configuration to xml - note only changed
     * values are exported
     * @param node
     */
    virtual void ExportToXML(XMLNode& node) const = 0;
    /**
     * this should import configuration from xml
     * @param node
     */
    virtual void ImportFromXML(const XMLNode& node) = 0;
    /**
     * import directly from file
     * @param filename - name of the xml-file
     * @param nodeName - name of node with configuration (child of root node)
     */
    void LoadFromXML(TString filename, TString nodeName);
    virtual ~Style() {};
    ClassDef(Style, 1);
  };


}  // namespace Hal


#endif /* HAL_FEATURES_HIST_STYLE_H_ */
