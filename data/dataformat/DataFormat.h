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
      kVertexX   = 10000001,
      kVertexY   = 10000002,
      kVertexZ   = 10000003,
      kVertexXY  = 10000004,
      kVertexT   = 10000005,
      kEventPhi  = 10000006,
      kPhiError  = 10000007,
      kTracksNo  = 10000008,
      kEventId   = 10000009,
      kEventZero = 10000010

    };
    enum EMcEvent { kB = 10000011 };
    enum EExpEvent { kTofTracksNo = 10000012 };
    enum EComplexEvent {
      kDeltaRPhi     = 10000050,
      kDeltaVertexXY = 10000051,
      kDeltaVertexX  = 10000052,
      kDeltaVertexY  = 10000053,
      kDeltaVertexZ  = 10000054,

      kDeltaVertexT  = 10000055,
      kDeltaVertex   = 10000056,
      kDeltaTracksNo = 10000057
    };
    const Int_t ReStep = 100000;
    const Int_t ImStep = 200000;
    namespace Internal {  // do not use in macro
      const Int_t ReStepEvent = 10100000;
      const Int_t ImStepEvent = 10200000;
      const Int_t EventStart  = 10000000;
    }  // namespace Internal

    enum ETrack {
      kPx        = 1,
      kPy        = 2,
      kPz        = 3,
      kE         = 4,
      kPhi       = 5,
      kPt        = 6,
      kTheta     = 7,
      kP         = 8,
      kEta       = 9,
      kRapidity  = 10,
      kCharge    = 11,
      kStatus    = 12,
      kPq        = 13,
      kTrackZero = 14
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
