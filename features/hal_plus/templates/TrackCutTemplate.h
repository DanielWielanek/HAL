/*
 *Generated by hal-cmake
 */
#ifndef HAL_CMAKE_TEMPLATES_TRACKCUTTEMPLATE_H_
#define HAL_CMAKE_TEMPLATES_TRACKCUTTEMPLATE_H_

#include <Hal/TrackCut.h>

namespace MyHal {
  class Hal::Package;
  class TrackCutTemplate : public Hal::TrackCut {
  public:
    TrackCutTemplate();
    Int_t Init(Int_t taskID);
    Bool_t Pass(Hal::Track* pair);
    Hal::Package* Report() const;
    virtual ~TrackCutTemplate();
    ClassDef(TrackCutTemplate, 1)
  };

}  // namespace MyHal

#endif /* HAL_CMAKE_TEMPLATES_TRACKCUTTEMPLATE_H_ */