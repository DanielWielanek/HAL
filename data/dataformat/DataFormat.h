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
    const Int_t ReStep = 1000000;          // 1M
    const Int_t ImStep = 2000000;          // 2M
    namespace Internal {                   // do not use in macro
      const Int_t EventStart  = 10000000;  // 10 M
      const Int_t ReStepEvent = EventStart + ReStep;
      const Int_t ImStepEvent = EventStart + ImStep;
    }  // namespace Internal
    namespace Event {
      enum EBasic {
        kVertexX      = Hal::DataFieldID::Internal::EventStart + 1,
        kVertexY      = Hal::DataFieldID::Internal::EventStart + 2,
        kVertexZ      = Hal::DataFieldID::Internal::EventStart + 3,
        kVertexXY     = Hal::DataFieldID::Internal::EventStart + 4,
        kVertexT      = Hal::DataFieldID::Internal::EventStart + 5,
        kEventPhi     = Hal::DataFieldID::Internal::EventStart + 6,
        kPhiError     = Hal::DataFieldID::Internal::EventStart + 7,
        kTracksNo     = Hal::DataFieldID::Internal::EventStart + 8,
        kEventId      = Hal::DataFieldID::Internal::EventStart + 9,
        kEventZero    = Hal::DataFieldID::Internal::EventStart + 10,
        kMultiplicity = Hal::DataFieldID::Internal::EventStart + 11
      };
      enum EMc { kB = Hal::DataFieldID::Internal::EventStart + 101 };
      enum EExp { kTofTracksNo = Hal::DataFieldID::Internal::EventStart + 201 };
      enum EComplex {
        kDeltaRPhi     = Hal::DataFieldID::Internal::EventStart + 301,
        kDeltaVertexXY = Hal::DataFieldID::Internal::EventStart + 302,
        kDeltaVertexX  = Hal::DataFieldID::Internal::EventStart + 303,
        kDeltaVertexY  = Hal::DataFieldID::Internal::EventStart + 304,
        kDeltaVertexZ  = Hal::DataFieldID::Internal::EventStart + 305,

        kDeltaVertexT  = Hal::DataFieldID::Internal::EventStart + 306,
        kDeltaVertex   = Hal::DataFieldID::Internal::EventStart + 307,
        kDeltaTracksNo = Hal::DataFieldID::Internal::EventStart + 308
      };
    }  // namespace Event

    namespace Track {
      enum EBasic {
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

      enum EMc {
        kXfr   = 101,
        kYfr   = 102,
        kZfr   = 103,
        kTfr   = 104,
        kXstar = 105,

        kYstart  = 106,
        kZstart  = 107,
        kTstart  = 108,
        kPdg     = 109,
        kXYstart = 110,

        kXYfr = 111
      };
      enum EExp {
        kChi2        = 201,
        kNHits       = 202,
        kTrackLenght = 203,
        kDcaX        = 204,
        kDcaY        = 205,
        kDcaZ        = 206,
        kDcaXY       = 207,
        kDca         = 208,
        // only for detectors
        kTofM2     = 301,
        kToFBeta   = 302,
        kToFFlag   = 303,
        kToFTime   = 304,
        kToFEnergy = 305,

        kTpcDedx     = 310,
        kTpcNHits    = 311,
        kTpcNsigmaPi = 312,
        kTpcNsigmaKa = 313,
        kTpcNsigmaPr = 314,
        kTpcNsigmaEl = 315

      };
      enum EComplex {
        kDeltaPt    = 401,
        kDeltaPz    = 402,
        kDeltaP     = 403,
        kDeltaPx    = 404,
        kDeltaPy    = 405,
        kDeltaPhi   = 406,
        kDeltaTheta = 407,
        kDeltaEta   = 408
      };

      enum EV0 {
        kAssumedPdg     = 501,
        kAssumedPdgPos  = 502,
        kAssumedPdgNeg  = 503,
        kS1             = 504,
        kS2             = 505,
        kAlphaArm       = 506,
        kPtArm          = 507,
        kDauDist        = 508,
        kDecLenght      = 509,
        kCosAngle       = 510,
        kLambdaMass     = 511,
        kAntiLambdaMass = 512,
        kK0Mass         = 516
      };
    }  // namespace Track

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
