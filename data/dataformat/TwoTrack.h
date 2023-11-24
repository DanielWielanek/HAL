/*
 * TwoTrack.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALTWOTRACK_H_
#define HALTWOTRACK_H_

#include <TObject.h>
namespace Hal {
  class Track;
  class HiddenInfo;

  /**
   * class for storing pair of tracks
   */
  class TwoTrack : public TObject {
  public:
    enum PairType { kSignal, kBackground, kMixed, kRotated, kHemishpere, kOther };

  private:
    Track* fTrack1          = {nullptr};
    Track* fTrack2          = {nullptr};
    PairType fType          = {PairType::kOther};
    Bool_t fSame            = {kTRUE};
    HiddenInfo* fHiddenInfo = {nullptr};

  public:
    TwoTrack();
    /**
     * build pair from tracks
     * @param track1 first track
     * @param track2 second track
     */
    inline void FastBuild(Track* track1, Track* track2) {
      fTrack1 = track1;
      fTrack2 = track2;
    };
    /**
     * swap tracks in pair
     */
    inline void SwapTracks() {
      Track* temp = fTrack1;
      fTrack1     = fTrack2;
      fTrack2     = temp;
    }
    /**
     * add first track
     * @param track first track
     */
    inline void AddFirstTrack(Track* track) { fTrack1 = track; };
    /**
     * add second track
     * @param track second  track
     */
    inline void AddSecondTrack(Track* track) { fTrack2 = track; };
    /**
     * set pair type
     * @param type
     */
    inline void SetPairType(PairType type) { fType = type; }
    /**
     *
     * @return pair type
     */
    inline PairType GetPairType() const { return fType; };
    /**
     *
     * @return pointer to first track (in "fake" format)
     */
    inline Track* GetTrack1() const { return fTrack1; };
    /**
     *
     * @return pointer to second track (in "fake" format)
     */
    inline Track* GetTrack2() const { return fTrack2; };
    /**
     *
     * @return hidden info for pair (if available)
     */
    inline HiddenInfo* GetHiddenInfo() const { return fHiddenInfo; }
    /**
     * same if particles have the same type
     * @param same
     */
    inline void SetSame(Bool_t same) { fSame = same; };
    /**
     * set pair hidden info
     * @param info
     */
    inline void SetHiddenInfo(HiddenInfo* info) { fHiddenInfo = info; };
    /**
     *
     * @return true if both particles are the same type
     */
    inline Bool_t AreSame() const { return fSame; };
    virtual ~TwoTrack();
    ClassDef(TwoTrack, 1)
  };
}  // namespace Hal
#endif /* HALTWOTRACK_H_ */
