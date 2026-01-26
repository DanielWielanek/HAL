/*
 * StdPhys.h
 *
 *  Created on: 11 gru 2025
 *      Author: daniel
 */

#ifndef HAL_FEATURES_STD_STDPHYS_H_
#define HAL_FEATURES_STD_STDPHYS_H_

#include <RtypesCore.h>

class TLorentzVector;

namespace Hal {
  namespace Std {
    namespace LorentzGenerator {
      /**
       * generate TLorentzVector with given parameters
       * @param pt - transversal momentum
       * @param y - rapidity
       * @param pid - PDG code
       * @param phi - azimuthal angle if -100 generate random <-phi,phi>
       * @return
       */
      TLorentzVector PtY(Double_t pt, Double_t y, Int_t pid, Double_t phi = -100);
      /**
       * generate TLorentzVector with given parameters
       * @param pt
       * @param y
       * @param m mass
       * @param phi
       * @return
       */
      TLorentzVector PtYM(Double_t pt, Double_t y, Double_t m, Double_t phi = -100);
      /**
       * generate TLorentzVector with given parameters
       * @param pt - transversal momentum
       * @param eta - pseudoapidity
       * @param pid - PDG code
       * @param phi - azimuthal angle if -100 generate random <-phi,phi>
       * @return
       */
      TLorentzVector PtEta(Double_t pt, Double_t eta, Int_t pid, Double_t phi = -100);
      /**
       * generate TLorentzVector with given parameters
       * @param pt
       * @param eta
       * @param m
       * @param phi
       * @return
       */
      TLorentzVector PtEtaM(Double_t pt, Double_t eta, Double_t m, Double_t phi = -100);
    }  // namespace LorentzGenerator
  }    // namespace Std
}  // namespace Hal

#endif /* HAL_FEATURES_STD_STDPHYS_H_ */
