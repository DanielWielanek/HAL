/*
 * HalLink.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALLINK_H_
#define HALLINK_H_
#include "Array.h"

#include <TObject.h>

/**
 * class for linking objects, contains id that point to other object
 *used especially in @see HalTrack to provide information about track
 *dependencies
 */

namespace Hal {
  class Link : public TObject {
    Int_t fActiveSize;
    Int_t fSize;
    Int_t* fLinks;  // [fActiveSize]
  public:
    /**
     * i - link from
     * @param i link size
     */
    Link(Int_t i = 1);
    Link(const Link& link);
    /**
     * operator
     * @param link
     * @return
     */
    Link operator=(const Link& link);
    /**
     * set id of linked object and clean "to" array, if you don't want to clean
     * "to" array @see OverwriteFrom
     * @param i id of linked object
     */
    void SetNextLink(Int_t i);
    /**
     * set link to object at position val
     * @param index object number
     * @param val link value
     */
    void SetLink(Int_t index, Int_t val) { fLinks[index] = val; }
    void ExpandFast(Int_t size);
    void SetLinkAndExpand(Int_t index, Int_t val);
    /**
     * get link to index-th object connected with this object, this
     * method doesn't check link size!
     * @param index
     * @return
     */
    inline Int_t GetLink(Int_t index) const { return fLinks[index]; };
    /**
     *
     * @return number of objects conneted with this object
     */
    inline Int_t GetLinkSize() const { return fActiveSize; };
    virtual void Clear(Option_t* opt = "");
    void ClearLinks();
    virtual ~Link();
    ClassDef(Link, 1)
  };
}  // namespace Hal

#endif /* HALLINK_H_ */
