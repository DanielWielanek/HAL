/*
 * HalConst.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCONST_H_
#define HALCONST_H_
/**
 * const list it should be namespace but root don't use namespaces "out of box"
 *
 */
#include <TObject.h>
namespace Hal {
  class Const {
  public:
    Const();
    static Int_t PionPlusPID() { return 211; };
    static Int_t PionZeroPID() { return 111; }
    static Int_t ElectronPID() { return 11; };
    static Int_t ElectronNeutrinoPID() { return 12; }
    static Int_t PhotonPID() { return 22; }
    static Int_t MuonPID() { return 13; };
    static Int_t KaonPlusPID() { return 321; };
    static Int_t KaonZeroPID() { return 311; }
    static Int_t KaonZeroLongPID() { return 130; }
    static Int_t KaonZeroShortPID() { return 310; }
    static Int_t ProtonPID() { return 2212; };
    static Int_t LambdaPID() { return 3122; };
    static Int_t NeutronPID() { return 2112; };
    static Int_t DeltaPlusPID() { return 2214; }
    static Int_t DeltaPlusPlusPID() { return 2224; }
    static Int_t DeltaZeroPID() { return 2114; }
    static Int_t DeltaMinusPID() { return 1114; }
    static Int_t SigmaZeroPID() { return 3212; };
    static Int_t SigmaPlusPID() { return 3222; };
    static Int_t SigmaMinusPID() { return 3112; };
    static Int_t XiZeroPID() { return 3322; };
    static Int_t XiMinusPID() { return 3312; };
    static Double_t ElectronMass() { return 0.000510998; };
    static Double_t MuonMass() { return 0.105658; };
    static Double_t PionPlusMass() { return 0.13957; };
    static Double_t KaonPlusMass() { return 0.49367; };
    static Double_t ProtonMass() { return 0.938272029; };
    static Double_t LambdaMass() { return 1.115638; };
    static Double_t NeutronMass() { return 0.939565; }
    static Double_t KaonZeroMass() { return 0.497611; };
    inline static Double_t PiTo32() { return 5.56832799683170787119479427929036; };
    inline static Double_t PiTo32Over() { return 0.17958712212516655948313371027325; };
    inline static Double_t Sqrt3() { return 1.73205080756887719317660412343685; };
    inline static Double_t Sqrt3Over() { return 0.57735026918962584208117050366127; };
    inline static Double_t SqrtPi() { return 1.77245385090551588191942755656783; };
    inline static Double_t DummyVal() { return -9999999; }
    virtual ~Const();
    ClassDef(Const, 1)
  };
}  // namespace Hal

#endif /* HALCONST_H_ */
