/*
 * HalLink.cxx
 *
 *  Created on: 22 lut 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "Link.h"

namespace Hal {

  Link::Link(Int_t i) : TObject(), fActiveSize(i), fSize(i) {
    fLinks    = new Int_t[i];
    fLinks[0] = 0;
  }

  Link::Link(const Link& link) : TObject(link), fActiveSize(link.fActiveSize), fSize(link.fSize) {
    fLinks = new Int_t[fSize];
    for (int i = 0; i < fSize; i++)
      fLinks[i] = link.fLinks[i];
  }

  Link Link::operator=(const Link& link) {
    if (this != &link) {
      if (fSize < link.fActiveSize) {
        delete[] fLinks;
        fSize  = link.fSize;
        fLinks = new Int_t[fSize];
      }
      fActiveSize = link.fActiveSize;
      for (int i = 0; i < fActiveSize; i++) {
        fLinks[i] = link.fLinks[i];
      }
    }
    return *this;
  }

  Link::~Link() { delete[] fLinks; }

  void Link::SetLinkAndExpand(Int_t index, Int_t val) {
    if (fSize <= index) {
      Int_t old_size = fActiveSize;
      Int_t* temp    = fLinks;
      fLinks         = new Int_t[index + 1];
      fSize          = index + 1;
      fActiveSize    = index + 1;
      for (int i = 0; i < old_size; i++)
        fLinks[i] = temp[i];
      delete[] temp;
      fLinks[index] = val;
    } else {
      if (fActiveSize < index) fActiveSize = index;
      fLinks[index] = val;
    }
  }

  void Link::SetNextLink(Int_t i) {
    if (fActiveSize < fSize) {
      fLinks[fActiveSize++] = i;
    } else {
      SetLinkAndExpand(fActiveSize, i);
    }
  }

  void Link::Clear(Option_t* /*opt*/) {
    fActiveSize = 1;
    fLinks[0]   = 0;
  }

  void Link::ExpandFast(Int_t size) {
    if (size < fSize) {
      fActiveSize = size;
    } else {
      Int_t old_size = fActiveSize;
      Int_t* temp    = fLinks;
      fLinks         = new Int_t[size];
      fSize          = size;
      fActiveSize    = size;
      for (int i = 0; i < old_size; i++)
        fLinks[i] = temp[i];
      delete[] temp;
    }
  }

  void Link::ClearLinks() {
    for (int i = 0; i < fActiveSize; i++)
      fLinks[i] = -1;
  }
}  // namespace Hal
