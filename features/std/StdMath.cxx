/*
 * Hal::StdMath.cxx
 *
 *  Created on: 16 lip 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "StdMath.h"
#include "Cout.h"

#include <TH1.h>
#include <TH2.h>
#include <TMatrixD.h>
#include <iostream>

NamespaceImp(Hal::Std)


  ;
namespace Hal {
  namespace Std {

    void FitParabola(Double_t x1,
                     Double_t x2,
                     Double_t x3,
                     Double_t y1,
                     Double_t y2,
                     Double_t y3,
                     Double_t& a,
                     Double_t& b,
                     Double_t& c) {
      TMatrixD AM(3, 3);
      TMatrixD BM(3, 1);
      AM[0][0] = 1;
      AM[0][1] = x1;
      AM[0][2] = x1 * x1;
      AM[1][0] = 1;
      AM[1][1] = x2;
      AM[1][2] = x2 * x2;
      AM[2][0] = 1;
      AM[2][1] = x3;
      AM[2][2] = x3 * x3;

      BM[0][0] = y1;
      BM[1][0] = y2;
      BM[2][0] = y3;

      TMatrixD SOL(3, 1);
      AM = AM.InvertFast();

      SOL = AM * BM;

      c = SOL[0][0];
      b = SOL[1][0];
      a = SOL[2][0];
    }

    Int_t SolveParabola(Double_t a, Double_t b, Double_t c, Double_t& x1, Double_t& x2) {
      Double_t Delta = b * b - 4.0 * a * c;
      if (Delta < 0) return 0;
      if (Delta == 0) {
        x1 = x2 = -b / (2.0 * a);
        return 1;
      }
      Delta = TMath::Sqrt(Delta);
      x1    = (-b - Delta) / (2.0 * a);
      x2    = (-b + Delta) / (2.0 * a);
      return 2;
    }

    Int_t Bin3dToBin1d(Int_t nbinsX, Int_t nBinsY, Int_t binX, Int_t binY, Int_t binZ, Bool_t root) {
      if (root) {
        return (binX - 1) * nbinsX * nBinsY + (binY - 1) * nBinsY + binZ - 1;
      } else {
        return binX * nbinsX * nBinsY + binY * nBinsY + binZ;
      }
    }

    TVector3 Bin1dToBin3d(Int_t nbinsX, Int_t nBinsY, Int_t bin, Bool_t root) {
      Int_t XY    = nBinsY * nbinsX;
      Int_t alpha = TMath::Floor(bin / XY);
      Int_t beta  = TMath::Floor((bin - alpha * XY) / nbinsX);
      Int_t gamma = bin - alpha * XY - beta * nbinsX;
      if (root) {
        return TVector3(alpha + 1, beta + 1, gamma + 1);
      } else {
        return TVector3(alpha, beta, gamma);
      }
    }

    TMatrixD GetVec(const TH1& h, Bool_t horizontal) {
      if (horizontal) {
        TMatrixD vect(1, h.GetNbinsX());
        for (int i = 1; i <= h.GetNbinsX(); i++) {
          vect[0][i - 1] = h.GetBinContent(i);
        }
        return vect;
      } else {
        TMatrixD vect(h.GetNbinsX(), 1);
        for (int i = 1; i <= h.GetNbinsX(); i++) {
          vect[i - 1][0] = h.GetBinContent(i);
        }
        return vect;
      }
    }

    TMatrixD GetMatrix(const TH2& h, Bool_t swap) {
      int binX, binY;
      double minX, minY, maxX, maxY;
      Hal::Std::GetAxisPar(h, binX, minX, maxX, "x");
      Hal::Std::GetAxisPar(h, binY, minY, maxY, "y");
      if (binX != binY) {
        Hal::Cout::PrintInfo("Cannot call Hal::Std::GetMatrix on non-square histogram", EInfo::kError);
        return TMatrixD(1, 1);
      }
      if (minY != minX || maxX != maxY) {
        Hal::Cout::PrintInfo("Wrong histogram Hal::Std::GetMatrix min/max are not equal", EInfo::kWarning);
      }
      TMatrixD vect(binX, binX);
      for (int i = 1; i <= binX; i++) {
        for (int j = 1; j <= binX; j++) {
          if (swap) {
            vect[binX - j][i - 1] = h.GetBinContent(i, j);
          } else {
            vect[j - 1][i - 1] = h.GetBinContent(i, j);
          }
        }
      }
      return vect;
    }

    Double_t Discretize(Int_t areas, Double_t min, Double_t max, Double_t val, Char_t type) {
      Double_t step_size = (max - min) / ((Double_t) areas);
      Double_t step      = (val - min) / step_size;
      if (areas <= 1) {
        step      = 0;
        step_size = 0;
      }
      Double_t epsilon = 1E-10;
      switch (type) {
        case '+': {
          step = TMath::Ceil(step - epsilon);
        } break;
        case '-': {
          step = TMath::Floor(step + epsilon);
        } break;
        case '=': {
          step = std::round(step);
        } break;
        default: return 0; break;
      }
      Double_t res = min + step * step_size;
      if (res < min) return min;
      if (res > max) return max;
      return res;
    }

    Double_t StatError2Var(Char_t ope, Double_t x, Double_t y, Double_t dx, Double_t dy) {
      if (dx < 0) dx = TMath::Sqrt(TMath::Abs(x));
      if (dy < 0) dy = TMath::Sqrt(TMath::Abs(y));
      Double_t dfx = 0, dfy = 0;
      switch (ope) {
        case '+': {
          dfx = 1;
          dfy = 1;
        } break;
        case '-': {
          dfx = 1;
          dfy = 1;
        } break;
        case '/': {
          dfx = 1.0 / y;
          dfy = x / y / y;
        } break;
        case '*': {
          dfx = y;
          dfy = x;
        } break;
        default: break;
      }
      return TMath::Sqrt(dfx * dfx * dx * dx + dfy * dfy * dy * dy);
    }

    Int_t MultiToOneDimIndex(const std::vector<int>& size, const std::vector<int>& position) {
      if (size.size() != position.size()) return -1;
      Int_t step = 1;
      Int_t pos  = 0;
      for (int i = size.size() - 1; i >= 0; i--) {
        pos  = pos + position[i] * step;
        step = step * size[i];
      }
      return pos;
    }

    std::vector<int> OneToMultiDimIndex(const std::vector<int>& size, Int_t n) {
      std::vector<int> res(size.size());
      for (int i = size.size() - 1; i >= 0; i--) {
        Int_t pos = n % size[i];
        n         = (n - pos) / size[i];
        res[i]    = pos;
      }
      return res;
    }

    std::pair<Int_t, Int_t> Division(Int_t num, Int_t div) {
      std::pair<Int_t, Int_t> res;
      Int_t reminder = num % div;
      res.second     = reminder;
      Int_t afdiv    = (num - reminder) / div;
      res.first      = afdiv;
      return res;
    }

    std::vector<Double_t> LagrangeInterpol(const std::vector<Double_t>& x, const std::vector<Double_t>& y) {
      std::vector<Double_t> res(x.size(), 0.0);
      if (x.size() != y.size()) { throw std::invalid_argument("LagrnageInterpol different size of x and y"); }
      const int n = x.size();

      TMatrixD X(n, n);
      TMatrixD Y(n, 1);

      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          X[i][j] = TMath::Power(x[i], n - j - 1);
        }
        Y[i][0] = y[i];
      }
      auto xP     = X;
      auto X1     = X.Invert();
      auto S      = X1 * Y;
      auto SOL    = xP * S;
      Bool_t fine = kTRUE;
      for (int i = 0; i < n; i++) {
        if (TMath::Abs(Y[i][0] - SOL[i][0]) > 0.01) fine = kFALSE;
      }
      if (!fine) {
        std::cout << "Hal::Std::Math::LagrangeInterpol" << std::endl;
        std::cout << "Probably there was a problem with inverion of matrix in LangrangeInterpol" << std::endl;
        for (int i = 0; i < n; i++) {
          std::cout << "\tExpected val" << Y[i][0] << " obtained " << SOL[i][0] << std::endl;
        }
      }
      for (int i = 0; i < n; i++) {
        res[i] = S[n - i - 1][0];
      }
      return res;
    }
  }  // namespace Std
}  // namespace Hal
