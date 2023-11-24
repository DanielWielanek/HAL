/*
 * TwoTrackAna.h
 *
 *  Created on: 27-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALTWOTRACKANA_H_
#define HALTWOTRACKANA_H_

#include "MultiTrackAna.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>

#include "TwoTrack.h"

//#define DISABLE_TWO_TRACK_SWAPPING
namespace Hal {
  class TwoTrackAnaChain;
  /**
   * basic class for making two track analysis
   */
  class TwoTrackAna : public MultiTrackAna {
    friend class TwoTrackAnaChain;

  public:
    /**
     * enum for describing of backgrounds
     */
    enum EAnaMode {
      kPerfectPairs,        //!< kPerfectPairs
      kPerfectPairsID,      //!< kPerfectPairsID
      kPerfectPairsNID,     //!< kPerfectPairsNID
      kMixedPairs,          //!< kMixedPairs
      kMixedPairsID,        //!< kMixedPairsID
      kMixedPairsNID,       //!< kMixedPairsNID
      kRotatedPairs,        //!< kRotatedPairs
      kRotatedPairsID,      //!< kRotatedPairsID
      kRotatedPairsNID,     //!< kRotatedPairsNID
      kHemispherePairs,     //!< kHemispherePairs
      kHemispherePairsID,   //!< kHemispherePairsID
      kHemispherePairsNID,  //!< kHemispherePairsNID
      kNoBackgroundID,      //!< kNoBackgroundID
      kNoBackgroundNID,     //!< kNoBackgroundNID
      kNoBackground,        //!< kNoBackground
      kCharged,             //!< kCharged
      kChargedID2,          //!< kChargedID2
      kChargedID3,          //!< kChargedID3
      kChargedNID2,         //!< kChargedNID2
      kChargedNID3          //!< kChargedNID3
    };

  private:
    Bool_t fNonIdIsSet;
    Bool_t fSignedBoth;
    Bool_t fIdentical;
    Bool_t fSwapPair;  // variable to swap identical particles
    Bool_t fSkipEmpty;
    Int_t fCurrentTrack2CollectionNo;
    Int_t fCurrentTrack1CollectionNo;
    Track *fCurrentTrack1, *fCurrentTrack2;
    /**
     * build swapped pair of tracks
     */
    void BuildPairSwapped(TwoTrack* pair);
    /**
     * build non-swapped pair of tracks
     */
    void BuildPair(TwoTrack* pair);
    /**
     * makes mixed  pairs of identical particles
     */
    void MakePairs_Mixed();
    /**
     * makes mixed pairs of non-identical particles
     */
    void MakePairs2_Mixed();
    /**
     * makes  pairs of identical particles in signal
     */
    void MakePairs();
    /**
     * makes non-identical pairs of particles in background
     */
    void MakePairs2();
    /**
     * makes pairs of identical particles in "perfect" signal
     */
    void MakePairs_Perfect();
    /**
     * makes pairs of non-identical particles  in "perfect" signal
     */
    void MakePairs2_Perfect();
    /**
     * makes pairs of identical particles in "rotation" mode
     */
    void MakePairs_Rotated();
    /**
     * makes pairs of non-identical particles in "rotation" mode
     */
    void MakePairs2_Rotated();
    /**
     * makes pairs of identical particles in "hemisphere" mode
     */
    void MakePairs_Hemisphere();
    /**
     * makes pairs of non-identical particles in "hemisphere" mode
     */
    void MakePairs2_Hemisphere();
    /**
     * process pairs of non-identical particles in "charged" mode (signal is from
     * identical particles)
     */
    void MakePairs_Charged2();
    /**
     * process pairs of identical particles in "charged" mode (signal is from
     * non-identical particles)
     */
    void MakePairs2_Charged2();
    /**
     * process pairs of non-identical particles in "charged" mode (signal is made
     * from identical particles)
     */
    void MakePairs_Charged3();
    /**
     * process pair of identical particles in "charged" mode  (signal is from
     * identical particles)
     */
    void MakePairs2_Charged3();

  protected:
    /**
     * background mode used in analysis
     */
    EAnaMode fBackgroundMode;
    /**
     * points to collection number of actually processed pair
     */
    Int_t fCurrentPairCollectionID;
    /**
     * total number of two track collections
     */
    Int_t fTwoTrackCollectionsNo;
    /**
     * total number of two track collections in background
     */
    Int_t fTwoTrackCollectionsNoBackground;
    /**
     * currently processed pair
     */
    TwoTrack* fCurrentSignalPair;
    /**
     * currently processed background pair
     */
    TwoTrack* fCurrentBackgroundPair;
    /**
     * set some tags connected with used background
     */
    virtual void SetTags();
    /**
     *
     * @return true if identical particles are processed in signal
     */
    Bool_t IdenticalParticles() const { return (fIdentical); };
    virtual Package* Report() const;
    /**
     * set hidden pair info if necessary
     */
    virtual void SetPairHiddenInfo() {};
    /**
     * delete hidden pair info, by default it's done automatically
     */
    virtual void DeleteHiddenPairInfo();
    /**
     * process pair of identical particles in signal
     */
    virtual void ProcessPair() {};
    /**
     * process pair of non-identical particles in signal
     */
    virtual void ProcessPair2() { ProcessPair(); };
    /**
     * process pair of identical particles in mixed background
     */
    virtual void ProcessPair_Mixed() {};
    /**
     * process pair of non-identical particles in mixed background
     */
    virtual void ProcessPair2_Mixed() { ProcessPair_Mixed(); };
    /**
     * process pair of identical particles in perfect background
     */
    virtual void ProcessPair_Perfect() {};
    /**
     * process pair of identical non-particles in perfect background
     */
    virtual void ProcessPair2_Perfect() { ProcessPair_Perfect(); };
    /**
     * process pair of identical particles in rotated background
     */
    virtual void ProcessPair_Rotated() {};
    /**
     * process pair of non-identical particles in rotated background
     */
    virtual void ProcessPair2_Rotated() { ProcessPair_Rotated(); };
    /**
     * process pair of identical particles in hemisphere background
     */
    virtual void ProcessPair_Hemisphere() {};
    /**
     * process pair of non-identical particles in hemisphere background
     */
    virtual void ProcessPair2_Hemisphere() { ProcessPair_Hemisphere(); };
    /**
     * process pair of identical particles in charged background
     */
    virtual void ProcessPair_ChargedId() {};
    /**
     * process pair of non-identical particles in charged background
     */
    virtual void ProcessPair_ChargedNId() { ProcessPair_ChargedId(); };
    /**
     * process pair of non-identical particles in charged background (where
     * singlal is made from identical particles)
     */
    virtual void ProcessPair_Charged2() { ProcessPair_ChargedNId(); };
    /**
     * process pair of identical particles in charged background (where singlal is
     * made from non-identical particles)
     */
    virtual void ProcessPair2_Charged2() { ProcessPair_ChargedId(); };
    /**
     * process pair of non-identical particles in charged background (where
     * singlal is made from identical particles)
     */
    virtual void ProcessPair_Charged3() { ProcessPair_ChargedNId(); };
    /**
     * process pair of identical particles in charged background (where singlal is
     * made from non-identical particles)
     */
    virtual void ProcessPair2_Charged3() { ProcessPair_ChargedId(); };
    virtual void ProcessEvent();
    virtual void InitNewCutContainer();
    // check collections prepare them to analysis
    virtual void CheckCutContainerCollections();
    // linking collections
    virtual void LinkCollections();
    /**
     * finish event where pairs in signal are made from identical particles
     */
    virtual void FinishEventIdentical();
    /**
     * finish event where pairs in signal are made from identical particles
     */
    virtual void FinishEventNonIdentical();
    virtual Task::EInitFlag Init();

  public:
    /**
     * default ct-or
     * @param use_background - if true -then background should be used
     */
    TwoTrackAna(Bool_t use_background = kFALSE);
    /**
     * copy constructor
     * @param ana
     */
    TwoTrackAna(const TwoTrackAna& ana);
    /**
     * assignement operator
     * @param other
     * @return
     */
    TwoTrackAna& operator=(const TwoTrackAna& other);
    /**
     * set mix side
     * @param mixsize size of buffer to mixing
     */
    void SetMixSize(Int_t mixsize);
    /**
     * if set to true events without pairs are processed to mixing buffer
     * @param proceed
     */
    void ProceedWithEmtyEvents(Bool_t proceed) { fSkipEmpty = !proceed; }
    virtual void FinishTask();
    /**
     * like for TrackAna +
     * @param option possible options : "background:perfect" - for perfect
     * background. "background:no" - for no background, "background:charge" - for
     * background made by opposite charges method, "background:rotation" - for
     * made background where second particle is rotated around Z-axis,
     * "background:hemispere" - for making analysis with full rotated second
     * particle, "signs_sum" - for disabling merging resutls from two groups
     * backgrounds made from pairs of identical particles,"id" - for processing
     * pairs of identical tracks, "nonid"
     */
    virtual void SetOption(Option_t* option);
    /**
     * set this analysis as non-identical analysis
     */
    void EnableNonIdentical() { SetOption("nonid"); };
    /**
     *
     * @return option to set mixed background
     */
    static TString BackgroundOptionMixed() { return "background:mixing"; };
    /**
     *
     * @return option to set perfect background
     */
    static TString BackgroundOptionPerfect() { return "background:perfect"; };
    /**
     *
     * @return option to set rotated background
     */
    static TString BackgroundOptionRotated() { return "background:rotation"; };
    /**
     *
     * @return option to set hemispehre background
     */
    static TString BackgroundOptionHemisphere() { return "background:hemisphere"; };
    /**
     *
     * @return option to set charged background
     */
    static TString BackgroundOptionCharge() { return "background:charge"; };
    /**
     *
     * @return option to set no background
     */
    static TString BackgroundOptionNoBackground() { return "background:no"; };
    /**
     * convert analysis pair type to pair type
     * @return
     */
    static TwoTrack::PairType GetPairType(EAnaMode mode);
    virtual ~TwoTrackAna();
    ClassDef(TwoTrackAna, 1)
  };
}  // namespace Hal

#endif /* HALTWOTRACKANA_H_ */
