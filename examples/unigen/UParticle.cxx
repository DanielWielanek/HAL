/**
 * UniGen - Common Format for Event Generators in High-energy and Nuclear Physics
 * Copyright (C) 2006 - 2019 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <iostream>
using namespace std;

#include "TObject.h"
#include "TParticle.h"

#include "UParticle.h"


//____________________________________________________________________
//
// UParticle
//
// Class for particle description.
//


//--------------------------------------------------------------------
UParticle::UParticle() {
  // Default constructor
  fIndex = fPdg = fStatus = fParent = fParentDecay = fMate = fDecay = 0;
  fChild[0] = fChild[1] = 0;
  fPx = fPy = fPz = fE = 0.;
  fX = fY = fZ = fT = 0.;
  fWeight           = 0.;
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
UParticle::UParticle(Int_t index,
                     Int_t pdg,
                     Int_t status,
                     Int_t parent,
                     Int_t parentDecay,
                     Int_t mate,
                     Int_t decay,
                     Int_t child[2],
                     Double_t px,
                     Double_t py,
                     Double_t pz,
                     Double_t e,
                     Double_t x,
                     Double_t y,
                     Double_t z,
                     Double_t t,
                     Double_t weight) {
  // Standard constructor
  fIndex       = index;
  fPdg         = pdg;
  fStatus      = status;
  fParent      = parent;
  fParentDecay = parentDecay;
  fMate        = mate;
  fDecay       = decay;
  fChild[0]    = child[0];
  fChild[1]    = child[1];
  fPx          = px;
  fPy          = py;
  fPz          = pz;
  fE           = e;
  fX           = x;
  fY           = y;
  fZ           = z;
  fT           = t;
  fWeight      = weight;
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
UParticle::UParticle(Int_t index,
                     Int_t pdg,
                     Int_t status,
                     Int_t parent,
                     Int_t parentDecay,
                     Int_t mate,
                     Int_t decay,
                     Int_t child[2],
                     TLorentzVector mom,
                     TLorentzVector pos,
                     Double_t weight) {
  // Standard constructor
  fIndex       = index;
  fPdg         = pdg;
  fStatus      = status;
  fParent      = parent;
  fParentDecay = parentDecay;
  fMate        = mate;
  fDecay       = decay;
  fChild[0]    = child[0];
  fChild[1]    = child[1];
  fPx          = mom.Px();
  fPy          = mom.Py();
  fPz          = mom.Pz();
  fE           = mom.E();
  fX           = pos.X();
  fY           = pos.Y();
  fZ           = pos.Z();
  fT           = pos.T();
  fWeight      = weight;
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
UParticle::UParticle(const UParticle& right) {
  // Copy constructor
  *this = right;
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
UParticle::UParticle(const TParticle& right) {
  // Copy constructor from the TParticle
  *this = right;
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
UParticle::~UParticle() {
  // Destructor
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
const UParticle& UParticle::operator=(const UParticle& right) {
  // Assignment operator
  fIndex       = right.fIndex;
  fPdg         = right.fPdg;
  fStatus      = right.fStatus;
  fParent      = right.fParent;
  fParentDecay = right.fParentDecay;
  fMate        = right.fMate;
  fDecay       = right.fDecay;
  fChild[0]    = right.fChild[0];
  fChild[1]    = right.fChild[1];
  fPx          = right.fPx;
  fPy          = right.fPy;
  fPz          = right.fPz;
  fE           = right.fE;
  fX           = right.fX;
  fY           = right.fY;
  fZ           = right.fZ;
  fT           = right.fT;
  fWeight      = right.fWeight;
  return (*this);
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
const UParticle& UParticle::operator=(const TParticle& right) {
  // Assignment operator from the TParticle
  fIndex       = 0;
  fPdg         = right.GetPdgCode();
  fStatus      = right.GetStatusCode();
  fParent      = right.GetFirstMother();
  fParentDecay = 0;
  fMate        = 0;
  fDecay       = 0;
  fChild[0]    = right.GetFirstDaughter();
  fChild[1]    = right.GetLastDaughter();
  fPx          = right.Px();
  fPy          = right.Py();
  fPz          = right.Pz();
  fE           = right.Energy();
  fX           = right.Vx();
  fY           = right.Vy();
  fZ           = right.Vz();
  fT           = right.T();
  fWeight      = right.GetWeight();
  return (*this);
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
const Bool_t UParticle::operator==(const UParticle& right) const {
  // If equal operator
  return (fIndex == right.fIndex && fPdg == right.fPdg && fStatus == right.fStatus && fParent == right.fParent
          && fParentDecay == right.fParentDecay && fMate == right.fMate && fDecay == right.fDecay && fChild[0] == right.fChild[0]
          && fChild[1] == right.fChild[1]
          && ((TMath::Abs((fPx - right.fPx) / fPx) < 0.0001) || (TMath::Abs(fPx) < 1e-16 && TMath::Abs(right.fPx) < 1e-16))
          && ((TMath::Abs((fPy - right.fPy) / fPy) < 0.0001) || (TMath::Abs(fPy) < 1e-16 && TMath::Abs(right.fPy) < 1e-16))
          && ((TMath::Abs((fPz - right.fPz) / fPz) < 0.0001) || (TMath::Abs(fPz) < 1e-16 && TMath::Abs(right.fPz) < 1e-16))
          && ((TMath::Abs((fE - right.fE) / fE) < 0.0001) || (TMath::Abs(fE) < 1e-16 && TMath::Abs(right.fE) < 1e-16))
          && ((TMath::Abs((fX - right.fX) / fX) < 0.0001) || (TMath::Abs(fX) < 1e-16 && TMath::Abs(right.fX) < 1e-16))
          && ((TMath::Abs((fY - right.fY) / fY) < 0.0001) || (TMath::Abs(fY) < 1e-16 && TMath::Abs(right.fY) < 1e-16))
          && ((TMath::Abs((fZ - right.fZ) / fZ) < 0.0001) || (TMath::Abs(fZ) < 1e-16 && TMath::Abs(right.fZ) < 1e-16))
          && ((TMath::Abs((fT - right.fT) / fT) < 0.0001) || (TMath::Abs(fT) < 1e-16 && TMath::Abs(right.fT) < 1e-16))
          && ((TMath::Abs((fWeight - right.fWeight) / fWeight) < 0.0001)
              || (TMath::Abs(fWeight) < 1e-16 && TMath::Abs(right.fWeight) < 1e-16)));
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void UParticle::Print(Option_t* option) {
  // Print the data members to the standard output
  cout << "------------------------------------------------" << endl
       << "-I-                 Particle                 -I-" << endl
       << "Index                       : " << fIndex << endl
       << "PDG code                    : " << fPdg << endl
       << "Status code                 : " << fStatus << endl
       << "Parent index                : " << fParent << endl
       << "Parent decay index          : " << fParentDecay << endl
       << "Last collision partner      : " << fMate << endl
       << "Decay index                 : " << fDecay << endl
       << "First child index           : " << fChild[0] << endl
       << "Last child index            : " << fChild[1] << endl
       << "Momentum (px, py, pz) (GeV) : (" << fPx << ", " << fPy << ", " << fPz << ")" << endl
       << "Energy (GeV)                : " << fE << endl
       << "Position (x, y, z) (fm)     : (" << fX << ", " << fY << ", " << fZ << ")" << endl
       << "Creation time (fm)          : " << fT << endl
       << "Weight                      : " << fWeight << endl
       << "------------------------------------------------" << endl;
}
//--------------------------------------------------------------------
