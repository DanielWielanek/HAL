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
 * const list it shuold be namespace but root don't use namespaces "out of box"
 *
 */
#include "TObject.h"
namespace Hal {
  class Const {
  public:
    Const();
    Int_t static PionPlusPID() { return 211; };
    Int_t static PionZeroPID() { return 111; }
    Int_t static ElectronPID() { return 11; };
    Int_t static MuonPID() { return 13; };
    Int_t static KaonPlusPID() { return 321; };
    Int_t static ProtonPID() { return 2212; };
    Int_t static LambdaPID() { return 3122; };
    Int_t static NeutronPID() { return 2112; };
    Int_t static SigmaZeroPID() { return 3212; };
    Int_t static SigmaPlusPID() { return 3222; };
    Int_t static SigmaMinusPID() { return 3112; };
    Int_t static XiZeroPID() { return 3322; };
    Int_t static XiMinusPID() { return 3312; };
    Double_t static ElectronMass() { return 0.000510998; };
    Double_t static MuonMass() { return 0.105658; };
    Double_t static PionPlusMass() { return 0.13957; };
    Double_t static KaonPlusMass() { return 0.49367; };
    Double_t static ProtonMass() { return 0.938272029; };
    Double_t static LambdaMass() { return 1.115638; };
    Double_t static NeutronMass() { return 0.939565; }
    Double_t static KaonZeroMass() { return 0.497611; };
    inline Double_t static PiTo32() { return 5.56832799683170787119479427929036; };
    inline Double_t static PiTo32Over() { return 0.17958712212516655948313371027325; };
    inline Double_t static Sqrt3() { return 1.73205080756887719317660412343685; };
    inline Double_t static Sqrt3Over() { return 0.57735026918962584208117050366127; };
    inline Double_t static SqrtPi() { return 1.77245385090551588191942755656783; };
    virtual ~Const();
    ClassDef(Const, 1)
  };
}  // namespace Hal

#endif /* HALCONST_H_ */
