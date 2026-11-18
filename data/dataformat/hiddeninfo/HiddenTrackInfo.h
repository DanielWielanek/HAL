/*
 * HiddenInfo.h
 *
 *  Created on: 24 lis 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_DATA_DATAFORMAT_HIDDENTRACKINFO_HIDDENINFO_H_
#define HAL_DATA_DATAFORMAT_HIDDENTRACKINFO_HIDDENINFO_H_

#include "TObject.h"
namespace Hal {
  /**
   * class for storing "hidden info"
   */
  class CompressionMap;
  class HiddenTrackInfo : public TObject {
  protected:
    std::vector<int> fData;

  public:
    HiddenTrackInfo(Int_t size = 1) : fData(size, -1) {};
    virtual void CopyData(HiddenTrackInfo* object);
    virtual void Reset();
    /**
     * recalculate links - note default option start from 1 cause tracks variables are automatically recalulcated by event
     * @param map
     * @param start
     */
    void TranslateLinks(const CompressionMap& map, Int_t start = 1);
    Int_t GetId() const { return fData[0]; }
    void SetId(Int_t i) { fData[0] = i; };
    const std::vector<int>& GetLinks() const { return fData; }
    virtual ~HiddenTrackInfo() {};
    ClassDef(HiddenTrackInfo, 1)
  };
} /* namespace Hal */

#endif /* HAL_DATA_DATAFORMAT_HIDDENINFO_HIDDENINFO_H_ */
