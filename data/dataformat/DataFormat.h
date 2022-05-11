/*
 * DataFormat.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_DATAFORMAT_HALDATAFORMAT_H_
#define HALFEMTO_DATAFORMAT_HALDATAFORMAT_H_

#include <TObject.h>
namespace Hal {
  namespace DataFieldID {
    enum EEvent {
      kVertexX  = 1,
      kVertexY  = 2,
      kVertexZ  = 3,
      kVertexXY = 4,
      kVertexT  = 5,
      kEventPhi = 6,
      kPhiError = 7,
      kTracksNo = 8,
      kEventId  = 9
    };
    enum EMcEvent { kB = 10 };
    enum EExpEvent { kTofTracksNo = 11 };
    enum EComplexEvent {
      kDeltaRPhi     = 50,
      kDeltaVertexXY = 51,
      kDeltaVertexX  = 52,
      kDeltaVertexY  = 53,
      kDeltaVertexZ  = 54,

      kDeltaVertexT  = 55,
      kDeltaVertex   = 56,
      kDeltaTracksNo = 57
    };
    const Int_t ReStep = 10000;
    const Int_t ImStep = 20000;

    enum ETrack {
      kPx       = 1,
      kPy       = 2,
      kPz       = 3,
      kE        = 4,
      kPhi      = 5,
      kPt       = 6,
      kTheta    = 7,
      kP        = 8,
      kEta      = 9,
      kRapidity = 10,
      kCharge   = 11,
      kStatus   = 12,
      kPq       = 13,
    };

    enum EMcTrack {
      kXfr   = 24,
      kYfr   = 25,
      kZfr   = 26,
      kTfr   = 27,
      kXstar = 28,

      kYstart  = 29,
      kZstart  = 30,
      kTstart  = 31,
      kPdg     = 32,
      kXYstart = 33,

      kXYfr = 41
    };
    enum EExpTrack {
      kChi2        = 51,
      kNHits       = 52,
      kTrackLenght = 53,
      kDcaX        = 54,
      kDcaY        = 55,
      kDcaZ        = 56,
      kDcaXY       = 57,
      kDca         = 58,
      // only for detectors
      kTofM2     = 101,
      kToFBeta   = 102,
      kToFFlag   = 103,
      kToFTime   = 104,
      kToFEnergy = 105,

      kTpcDedx     = 110,
      kTpcNHits    = 111,
      kTpcNsigmaPi = 112,
      kTpcNsigmaKa = 113,
      kTpcNsigmaPr = 114,
      kTpcNsigmaEl = 115

    };
    enum EComplexTrack {
      kDeltaPt    = 151,
      kDeltaPz    = 152,
      kDeltaP     = 153,
      kDeltaPx    = 154,
      kDeltaPy    = 155,
      kDeltaPhi   = 156,
      kDeltaTheta = 157,
      kDeltaEta   = 158
    };

  }  // namespace DataFieldID

  namespace DetectorID {
    /**
     * get detector ID by converting up to 6 letters/digits into unige long int
     * @param pattern
     * @return
     */
    UInt_t GetDetectorID(TString pattern);
    /**
     * detector IDs calculated by GetDetectorID
     */
    const UInt_t kTPC = 38496;
    const UInt_t kTOF = 38463;
    const UInt_t kSTS = 37360;
  };  // namespace DetectorID
}  // namespace Hal

#endif /* HALFEMTO_DATAFORMAT_HALDATAFORMAT_H_ */
