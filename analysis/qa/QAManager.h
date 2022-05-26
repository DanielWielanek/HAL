/*
 * QACoreManager.h
 *
 *  Created on: 4 maj 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_ANALYSIS_QA_HALQACOREMANAGER_H_
#define HALFEMTO_ANALYSIS_QA_HALQACOREMANAGER_H_

#include <Rtypes.h>
#include <TObject.h>
#include <TString.h>

namespace Hal {
  class Event;
  class TrackAna;
  class TwoTrackAna;
  class AnalysisManager;

  /**
   * basic class that might be used by nica macros, provides predefined format for given framework, predefined values of default
   * cuts etc.
   *
   */

  class QAManager : public TObject {
  protected:
    Double_t fEta[2]   = {-1, 1};
    Double_t fPt[2]    = {0, 10};
    Double_t fPMax     = {10};
    Double_t fDCAxy    = {10};
    Double_t fDCAz[2]  = {-10, 10};
    Bool_t fUsePrimSec = {kTRUE};
    Bool_t fUsetPid    = {kTRUE};

  public:
    enum class ePidCut { kPionPlus, kPionMinus, kKaonPlus, kKaonMinus, kProton, kAntiProton, kUnkown };
    enum class eParticleType { kPrimaryOnly, kSecondaryOnly, kAll };
    enum class eFormatType { kSim, kReco, kComplex };
    enum class eAnaType { kDefault, kHbt };
    QAManager() {};
    ePidCut ToPid(Int_t i);
    void UsePrimSecCut(Bool_t use) { fUsePrimSec = use; };
    void UsePidCut(Bool_t use) { fUsetPid = use; };
    Double_t GetEtaMin() const { return fEta[0]; };
    Double_t GetEtaMax() const { return fEta[1]; };
    Double_t GetPtMin() const { return fPt[0]; };
    Double_t GetPtMax() const { return fPt[1]; };
    Double_t GetDcaXY() const { return fDCAxy; };
    Double_t GetDcaZMin() const { return fDCAz[0]; };
    Double_t GetDcaZMax() const { return fDCAz[1]; };
    Double_t GetPMax() const { return fPMax; };
    virtual AnalysisManager* GetAna(TString outFile, TString simFile, TString recoFile = "");
    virtual Event* GetFormat(eFormatType type, eAnaType ana = eAnaType::kDefault);
    /**
     * set default cuts for MC tracks if no ovewriten the PDG cuts are used
     * @param ana analysis
     * @param cut type of selection cryteria
     * @param primary type of selected particles
     * @param flag
     */
    virtual void SetMcTrackCut(TrackAna* ana, ePidCut cut, eParticleType primary, TString flag = "");
    /**
     * set default cuts for reconstructed tracks
     * @param ana
     * @param cut
     * @param col
     * @param flag
     */
    virtual void SetRecoTrackCut(TrackAna* ana, ePidCut cut, eParticleType primary, TString flag = "");
    virtual void SetEventCut(TrackAna* ana, Int_t col, TString flag = "");
    virtual void SetPairCut(TwoTrackAna* ana, ePidCut pid1, ePidCut pid2);
    virtual ~QAManager() {};
    ClassDef(QAManager, 1)
  };
}  // namespace Hal


#endif /* HALFEMTO_ANALYSIS_QA_HALQACOREMANAGER_H_ */
