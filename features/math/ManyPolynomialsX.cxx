/*
 * PolynomialX.cxx
 *
 *  Created on: 16 gru 2025
 *      Author: daniel
 */

#include "ManyPolynomialsX.h"

#include <RtypesCore.h>
#include <TF1.h>
#include <TString.h>
#include <vector>


#include "Cout.h"
#include "Std.h"

namespace Hal {

  ManyPolynomialsX::ManyPolynomialsX() {}

  Bool_t ManyPolynomialsX::IsInitialzed(TString str) const {
    if (fInit) {
      TString className = ClassName();
      str               = className + "::" + str + " already called";
      Hal::Cout::PrintInfo(str, EInfo::kWarning);
      return kTRUE;
    }
    return kFALSE;
  }
  Double_t ManyPolynomialsX::Eval(Double_t* x, Double_t* /*params*/) const {
    double sum  = 0;
    const int j = FindBin(x[0]);
    if (j < 0) return fRawValue;
    for (unsigned int i = 0; i < fTerms[j].size(); i++) {
      sum += TMath::Power(x[0], (int) i) * fTerms[j][i];
    }
    return sum;
  }

  void ManyPolynomialsX::SetRanges(const std::vector<double>& ranges) {
    if (IsInitialzed(__func__)) return;
    double prev = ranges[0] - 1;
    for (int i = 0; i < (int) ranges.size(); i++) {
      if (prev > ranges[i]) {
        Hal::Cout::PrintInfo("PolynomialX::SetRanges wrong ranges order !", EInfo::kError);
        return;
      }
      prev = ranges[i];
    }
    fRanges = ranges;
    fTerms.resize(fRanges.size() - 1);
  }

  void ManyPolynomialsX::SetTerms(const std::vector<double>& terms, int range) {
    if (IsInitialzed(__func__)) return;
    if (range < 0 || range >= (int) fTerms.size()) {
      Hal::Cout::PrintInfo("PolynomialX::SetTerms to large range !", EInfo::kError);
      return;
    }
    fTerms[range] = terms;
  }

  void ManyPolynomialsX::SetTermsByValue(const std::vector<double>& terms, double value) {
    if (IsInitialzed(__func__)) return;
    int i = FindBin(value);
    if (i >= 0) SetTerms(terms, i);
  }

  Double_t ManyPolynomialsX::Integral(Double_t low, Double_t hi) { return fFunc->Integral(low, hi); }

  ManyPolynomialsX* ManyPolynomialsX::GetDeriverative() const {
    ManyPolynomialsX* poly = new ManyPolynomialsX();
    poly->SetRanges(fRanges);
    for (int i = 0; i < (int) fTerms.size(); i++) {
      std::vector<double> terms = GetDeriverativeTerms(fTerms[i]);
      poly->SetTerms(terms, i);
    }
    poly->Init();
    return poly;
  }

  ManyPolynomialsX* ManyPolynomialsX::GetDeriverative2() const {
    ManyPolynomialsX* poly = new ManyPolynomialsX();
    poly->SetRanges(fRanges);
    for (int i = 0; i < (int) fTerms.size(); i++) {
      std::vector<double> terms = GetDeriverativeTerms(GetDeriverativeTerms(fTerms[i]));
      poly->SetTerms(terms, i);
    }
    poly->Init();
    return poly;
  }
  /*
    PolynomialX* PolynomialX::GetIntegral() const {
      PolynomialX* poly = new PolynomialX();
      poly->SetRanges(fRanges);
      auto GetIntegralTerms = [](const std::vector<double>& a, double C = 0.0) {
        std::vector<double> integ(a.size() + 1);
        integ[0] = C;
        for (size_t i = 0; i < a.size(); ++i)
          integ[i + 1] = a[i] / (i + 1);
        return integ;
      };
      double constant = 0;
      for (size_t i = 0; i < fTerms.size(); i++) {
        std::vector<double> term = GetTerms(i);
        if (i > 0) {
          auto term_prev = poly->GetTerms(i - 1);
          double low = 0, high = 0;
          double xLo = fRanges[i - 1];
          double xHi = fRanges[i];
          for (int p = 0; p < term_prev.size(); p++) {
            low += term_prev[p] * TMath::Power(xLo, p);
            high += term_prev[p] * TMath::Power(xHi, p);
          }
          double at_edge = 0;
          for (int p = 0; p < term.size(); p++) {
            at_edge += term[p] * TMath::Power(xHi, p);
          }
          double dif = high - low;  // prev integral
          constant   = dif - at_edge;
          std::cout << constant << std::endl;
        }

        std::vector<double> terms = GetIntegralTerms(term, constant);
        poly->SetTerms(terms, i);
      }


      poly->Init();
      return poly;
    }*/

  void ManyPolynomialsX::Init() {
    if (IsInitialzed(__func__)) return;
    fInit     = kTRUE;
    int parNo = 0;
    for (auto i : fTerms)
      parNo += i.size();
    fFunc = new TF1(
      Hal::Std::GetUniqueName("func_draw"), this, &ManyPolynomialsX::Eval, fRanges[0], fRanges[fRanges.size() - 1], parNo);
    parNo     = 0;
    int count = 0;
    for (auto i : fTerms) {
      for (int j = 0; j < (int) i.size(); j++) {
        fFunc->SetParName(parNo, Form("p_%i_%i", count, j));
        fFunc->FixParameter(parNo, fTerms[count][j]);
        parNo++;
      }
      count++;
    }
  }

  Int_t ManyPolynomialsX::FindBin(Double_t x) const {
    if (fRanges.size() < 2) return -1;
    auto it = std::upper_bound(fRanges.begin(), fRanges.end(), x);
    int idx = std::distance(fRanges.begin(), it) - 1;
    if (idx < 0 || idx >= static_cast<int>(fRanges.size()) - 1) return -1;
    return idx;
  }

  std::vector<double> ManyPolynomialsX::GetDeriverativeTerms(const std::vector<double>& x) const {
    if (x.size() <= 1) return {};  // pochodna stałej = 0

    std::vector<double> d(x.size() - 1);
    for (size_t i = 1; i < x.size(); ++i) {
      d[i - 1] = i * x[i];
    }
    return d;
  }

  ManyPolynomialsX::~ManyPolynomialsX() {
    if (fFunc) delete fFunc;
  }


} /* namespace Hal */
