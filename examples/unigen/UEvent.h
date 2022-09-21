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

#ifndef UEVENT_H
#define UEVENT_H

#include "TLorentzVector.h"
#include "TObject.h"
#include "TString.h"

class TClonesArray;
class UParticle;


class UEvent : public TObject {

private:
  Int_t fEventNr;            // Event number
  Double_t fB;               // Impact parameter (fm)
  Double_t fPhi;             // Reaction plane angle
  Int_t fNes;                // Number of event steps
  Int_t fStepNr;             // Event step number
  Double_t fStepT;           // Event step time
  Int_t fNpa;                // Number of particles
  TString fComment;          // Generator-specific information
  TClonesArray* fParticles;  // Array of particles

public:
  UEvent();
  UEvent(const UEvent& right);
  UEvent& operator=(const UEvent& right);
  virtual ~UEvent();
  void Print(Option_t* option = "");
  inline Int_t GetEventNr() const { return fEventNr; }
  inline Double_t GetB() const { return fB; }
  inline Double_t GetPhi() const { return fPhi; }
  inline Int_t GetNes() const { return fNes; }
  inline Int_t GetStepNr() const { return fStepNr; }
  inline Double_t GetStepT() const { return fStepT; }
  inline Int_t GetNpa() const { return fNpa; }
  inline TClonesArray* GetParticleList() const { return fParticles; }
  inline void GetComment(TString& comment) const { comment = fComment; }
  UParticle* GetParticle(Int_t index) const;
  void SetParameters(Int_t eventNr, Double_t b, Double_t phi, Int_t nes, Int_t stepNr, Double_t stepT, const char* comment = "");
  inline void SetEventNr(Int_t eventNr) { fEventNr = eventNr; }
  inline void SetB(Double_t b) { fB = b; }
  inline void SetPhi(Double_t phi) { fPhi = phi; }
  inline void SetNes(Int_t nes) { fNes = nes; }
  inline void SetStepNr(Int_t stepNr) { fStepNr = stepNr; }
  inline void SetStepT(Double_t stepT) { fStepT = stepT; }
  inline void SetComment(const char* comment) { fComment = comment; }
  void AddParticle(Int_t index,
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
  void AddParticle(Int_t index,
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
  void AddParticle(const UParticle& particle);
  void Clear();
  void RemoveAt(Int_t i);

  ClassDef(UEvent, 2);
};


#endif
