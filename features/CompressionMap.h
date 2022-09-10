/*
 * CompressionMap.h
 *
 *  Created on: 10 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FEATURES_COMPRESSIONMAP_H_
#define HAL_FEATURES_COMPRESSIONMAP_H_

#include <TObject.h>
/**
 * class that represent compression of an array
 */
namespace Hal {

  class CompressionMap : public TObject {
    Int_t fSize;
    Int_t fAllocatedSize;
    Int_t fCounter;
    Int_t* fOldToNewIndex;  //[fAllocatedSize]
    Int_t* fNewToOldIndex;  //[fAllocatedSize]
    void CheckSize(Int_t size);

  public:
    CompressionMap();
    CompressionMap(const CompressionMap& map);
    /**
     * should be called before marking objects
     * @param size size of array to compression
     */
    void Reset(Int_t size);
    /**
     * mark object at given index as good
     * @param old_index
     */
    inline void MarkAsGood(Int_t old_index) { fOldToNewIndex[old_index] = 0; }
    /**
     * properly calculates indexes, should be called after last MarkAsGood
     */
    void Recalculate();
    /**
     * convert old index to new one
     * @param old_index
     * @return
     */
    inline Int_t GetNewIndex(Int_t old_index) const { return fOldToNewIndex[old_index]; }
    /**
     * convert new index to new one
     * @param new_index
     * @return
     */
    inline Int_t GetOldIndex(Int_t new_index) const { return fNewToOldIndex[new_index]; }
    /**
     *
     * @return size of uncompressed array
     */
    inline Int_t GetSize() const { return fSize; }
    /**
     *
     * @return size of array after compression
     */
    inline Int_t GetNewSize() const { return fCounter; }
    virtual ~CompressionMap();
    ClassDef(CompressionMap, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_COMPRESSIONMAP_H_ */
