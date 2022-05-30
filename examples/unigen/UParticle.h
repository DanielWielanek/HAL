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

#ifndef UPARTICLE_H
#define UPARTICLE_H

#include "TLorentzVector.h"
#include "TMath.h"
#include "TObject.h"

class TParticle;


class UParticle : public TObject {

private:
  Int_t fIndex;        // index of this particle
  Int_t fPdg;          // PDG code
  Int_t fStatus;       // Status
  Int_t fParent;       // Index of parent
  Int_t fParentDecay;  // Parent decay index
  Int_t fMate;         // index of last collision partner
  Int_t fDecay;        // decay index (-1 if not decayed)
  Int_t fChild[2];     // index of first and last child
  Double32_t fPx;      // px (GeV)
  Double32_t fPy;      // py (GeV)
  Double32_t fPz;      // pz (GeV)
  Double32_t fE;       // Energy (GeV)
  Double32_t fX;       // x (fm)
  Double32_t fY;       // y (fm)
  Double32_t fZ;       // z (fm)
  Double32_t fT;       // t (fm)
  Double32_t fWeight;  // weight

public:
  UParticle();
  UParticle(Int_t index,
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
            Double_t weight);
  UParticle(Int_t index,
            Int_t pdg,
            Int_t status,
            Int_t parent,
            Int_t parentDecay,
            Int_t mate,
            Int_t decay,
            Int_t child[2],
            TLorentzVector mom,
            TLorentzVector pos,
            Double_t weight);
  UParticle(const UParticle& right);
  UParticle(const TParticle& right);
  virtual ~UParticle();
  const UParticle& operator=(const UParticle& right);
  const UParticle& operator=(const TParticle& right);
  const Bool_t operator==(const UParticle& right) const;
  void Print(Option_t* option = "");
  inline Int_t GetIndex() const { return fIndex; }
  inline Int_t GetPdg() const { return fPdg; }
  inline Int_t GetStatus() const { return fStatus; }
  inline Int_t GetParent() const { return fParent; }
  inline Int_t GetParentDecay() const { return fParentDecay; }
  inline Int_t GetMate() const { return fMate; }
  inline Int_t GetDecay() const { return fDecay; }
  inline Int_t GetFirstChild() const { return fChild[0]; }
  inline Int_t GetLastChild() const { return fChild[1]; }
  inline Double_t Px() const { return fPx; }
  inline Double_t Py() const { return fPy; }
  inline Double_t Pz() const { return fPz; }
  inline Double_t E() const { return fE; }
  inline TLorentzVector GetMomentum() const { return TLorentzVector(fPx, fPy, fPz, fE); }
  inline void Momentum(TLorentzVector& mom) const { mom.SetPxPyPzE(fPx, fPy, fPz, fE); }
  inline Double_t X() const { return fX; }
  inline Double_t Y() const { return fY; }
  inline Double_t Z() const { return fZ; }
  inline Double_t T() const { return fT; }
  inline TLorentzVector GetPosition() const { return TLorentzVector(fX, fY, fZ, fT); }
  inline void Position(TLorentzVector& pos) const { pos.SetXYZT(fX, fY, fZ, fT); }
  inline Double_t GetWeight() const { return fWeight; }
  inline void SetIndex(Int_t index) { fIndex = index; }
  inline void SetPdg(Int_t pdg) { fPdg = pdg; }
  inline void SetStatus(Int_t status) { fStatus = status; }
  inline void SetParent(Int_t parent) { fParent = parent; }
  inline void SetParentDecay(Int_t parentDecay) { fParentDecay = parentDecay; }
  inline void SetMate(Int_t mate) { fMate = mate; }
  inline void SetDecay(Int_t decay) { fDecay = decay; }
  inline void SetChild(Int_t child[2]) {
    fChild[0] = child[0];
    fChild[1] = child[1];
  }
  inline void SetFirstChild(Int_t child) { fChild[0] = child; }
  inline void SetLastChild(Int_t child) { fChild[1] = child; }
  inline void SetPx(Double_t px) { fPx = px; }
  inline void SetPy(Double_t py) { fPy = py; }
  inline void SetPz(Double_t pz) { fPz = pz; }
  inline void SetE(Double_t e) { fE = e; }
  inline void SetMomentum(Double_t px, Double_t py, Double_t pz, Double_t e) {
    fPx = px;
    fPy = py;
    fPz = pz;
    fE  = e;
  }
  inline void SetMomentum(TLorentzVector mom) {
    fPx = mom.Px();
    fPy = mom.Py();
    fPz = mom.Pz();
    fE  = mom.E();
  }
  inline void SetX(Double_t x) { fX = x; }
  inline void SetY(Double_t y) { fY = y; }
  inline void SetZ(Double_t z) { fZ = z; }
  inline void SetT(Double_t t) { fT = t; }
  inline void SetPosition(Double_t x, Double_t y, Double_t z, Double_t t) {
    fX = x;
    fY = y;
    fZ = z;
    fT = t;
  }
  inline void SetPosition(TLorentzVector pos) {
    fX = pos.X();
    fY = pos.Y();
    fZ = pos.Z();
    fT = pos.T();
  }
  inline void SetWeight(Double_t weight) { fWeight = weight; }

  ClassDef(UParticle, 1);
};


#endif
