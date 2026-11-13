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

#include <TGraph.h>

#include <TDecompLU.h>
#include <TDecompSVD.h>
#include <TH1.h>
#include <TH2.h>
#include <TMatrixD.h>
#include <TRandom.h>
#include <TVectorD.h>
#include <iostream>
#include <stdexcept>

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
      const Double_t dx1 = x2 - x1;
      const Double_t dx2 = x3 - x1;

      const Double_t slope12 = (y2 - y1) / dx1;
      const Double_t slope13 = (y3 - y1) / dx2;

      a = (slope13 - slope12) / (x3 - x2);
      b = slope12 - a * (x1 + x2);
      c = y1 - slope12 * x1 + a * x1 * x2;
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
    namespace Math {
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

      std::vector<Double_t>
      ChebyshevInterpolation(const std::vector<std::pair<Double_t, Double_t>>& x, Int_t n, Double_t low, Double_t high) {
        if (low == high) {
          low  = x[0].first;
          high = x[x.size() - 1].second;
        }
        TGraph* gr = new TGraph();
        for (int i = 0; i < (int) x.size(); i++) {
          double X = x[i].first;
          double Y = x[i].second;
          gr->SetPoint(i, X, Y);
        }
        std::vector<double> nodes(n);
        for (int k = 0; k < n; ++k) {
          double X = TMath::Cos(TMath::Pi() * (2.0 * k + 1) / (2.0 * n));  // [-1,1]
          nodes[k] = 0.5 * ((high - low) * X + (high + low));              // przeskaluj do [a,b]
        }
        TMatrixD A(n, n);
        TVectorD b(n);

        for (int i = 0; i < n; ++i) {
          b[i]         = gr->Eval(nodes[i]);
          double pow_x = 1.0;
          for (int j = 0; j < n; ++j) {
            A(i, j) = pow_x;
            pow_x *= nodes[i];
          }
        }

        TDecompLU lu(A);
        Bool_t ok;
        TVectorD coeffs = lu.Solve(b, ok);
        std::vector<Double_t> res;
        if (!ok) {
          std::cerr << "Could't find solution in ChebyshevInterpolation!" << std::endl;
          return res;
        }
        delete gr;
        for (int i = 0; i < n; i++) {
          res.push_back(coeffs[i]);
        }
        return res;
      }

      TMatrixD TikhonovUnfold(const TMatrixD& smearedM, const TMatrixD& response, double lambda) {
        const int n = response.GetNcols();
        TVectorD smeared(smearedM.GetNrows());
        for (int i = 0; i < smearedM.GetNrows(); i++) {
          smeared[i] = smearedM[i][0];
        }
        // A^T A
        TMatrixD At  = TMatrixD(TMatrixD::kTransposed, response);
        TMatrixD AtA = At * response;

        // A^T y
        TVectorD Aty = At * smeared;

        // A^T A + lambda^2 I
        TMatrixD regularized = AtA;

        const double lambda2 = lambda * lambda;

        for (int i = 0; i < n; ++i)
          regularized(i, i) += lambda2;

        // Solve:
        // (A^T A + lambda^2 I) x = A^T y
        TDecompSVD svd(regularized);

        Bool_t ok;
        TVectorD result = svd.Solve(Aty, ok);
        TMatrixD mResult(result.GetNrows(), 0);
        for (int i = 0; i < result.GetNrows(); i++) {
          mResult[i][0] = result[i];
        }
        if (!ok) { throw std::runtime_error("Tikhonov unfolding: failed to solve system"); }

        return mResult;
      }

      TMatrixD TikhonovUnfold2(const TMatrixD& smeared, const TMatrixD& response, double lambda) {
        const int n = response.GetNcols();

        if (response.GetNrows() != n) throw std::runtime_error("Response matrix must be square");

        if (smeared.GetNrows() != n) throw std::runtime_error("Smeared vector size does not match response matrix");

        if (lambda < 0.0) throw std::runtime_error("Lambda must be >= 0");

        // ------------------------------------------------------------
        // Second-difference regularization matrix:
        //
        // (L f)_i = f_i - 2 f_{i+1} + f_{i+2}
        //
        // L has dimensions (n-2) x n
        // ------------------------------------------------------------

        const int nReg = n - 2;

        if (nReg <= 0) throw std::runtime_error("Response matrix is too small");

        TMatrixD L(nReg, n);

        for (int i = 0; i < nReg; ++i) {
          L(i, i)     = 1.0;
          L(i, i + 1) = -2.0;
          L(i, i + 2) = 1.0;
        }

        // ------------------------------------------------------------
        // Tikhonov problem:
        //
        //   minimize || A f - y ||^2 + lambda^2 || L f ||^2
        //
        // Instead of solving
        //
        //   (A^T A + lambda^2 L^T L) f = A^T y
        //
        // solve the equivalent augmented system
        //
        //   [ A ]       [ y ]
        //   [ λL ] f  = [ 0 ]
        //
        // using SVD.
        //
        // This avoids explicitly constructing A^T A, whose
        // condition number is approximately cond(A)^2.
        // ------------------------------------------------------------

        TMatrixD B(n + nReg, n);
        TVectorD rhs(n + nReg);

        // First block: A f = y
        for (int i = 0; i < n; ++i) {

          rhs(i) = smeared(i, 0);

          for (int j = 0; j < n; ++j) {
            B(i, j) = response(i, j);
          }
        }

        // Second block: lambda L f = 0
        for (int i = 0; i < nReg; ++i) {

          rhs(n + i) = 0.0;

          for (int j = 0; j < n; ++j) {
            B(n + i, j) = lambda * L(i, j);
          }
        }

        // ------------------------------------------------------------
        // Solve augmented least-squares problem with SVD
        // ------------------------------------------------------------

        TDecompSVD svd(B);

        Bool_t ok = false;

        TVectorD unfolded = svd.Solve(rhs, ok);

        if (!ok) throw std::runtime_error("Tikhonov unfolding failed");
        TMatrixD mResult(unfolded.GetNrows(), 0);
        for (int i = 0; i < unfolded.GetNrows(); i++) {
          mResult[i][0] = unfolded[i];
        }
        return mResult;
      }

      void NormalizeMatrix(TMatrixD& matrix, ENorm opt) {
        switch (opt) {
          case ENorm::kTotal: {
            Double_t sum = 0;
            for (int iCol = 0; iCol < matrix.GetNcols(); iCol++) {
              for (int iRow = 0; iRow < matrix.GetNrows(); iRow++) {
                sum += matrix[iRow][iCol];
              }
            }
            if (sum == 0) sum = 1;
            sum    = 1.0 / sum;
            matrix = matrix * sum;
          } break;
          case ENorm::kByRow: {
            for (int iRow = 0; iRow < matrix.GetNrows(); iRow++) {
              Double_t sum = 0;
              for (int iCol = 0; iCol < matrix.GetNcols(); iCol++) {
                sum += matrix[iRow][iCol];
              }
              sum = 1.0 / sum;
              for (int iCol = 0; iCol < matrix.GetNcols(); iCol++) {
                matrix[iRow][iCol] = matrix[iRow][iCol] * sum;
              }
            }
          } break;
          case ENorm::kByColum: {
            for (int iCol = 0; iCol < matrix.GetNcols(); iCol++) {
              Double_t sum = 0;
              for (int iRow = 0; iRow < matrix.GetNrows(); iRow++) {
                sum += matrix[iRow][iCol];
              }
              if (sum == 0) sum = 1;
              sum = 1.0 / sum;
              for (int iRow = 0; iRow < matrix.GetNrows(); iRow++) {
                matrix[iRow][iCol] = matrix[iRow][iCol] * sum;
              }
            }
          } break;
        }
      }

      TMatrixD GetMatrix(const TH1& h, EOrient opt, Bool_t underflow) {
        Int_t size  = h.GetNbinsX();
        Int_t shift = 1;
        if (underflow) {
          size += 2;
          shift = 0;
        }

        switch (opt) {
          case EOrient::kVertical: {
            TMatrixD matrix(size, 1);
            for (int i = 0; i < size; i++) {
              matrix[i][0] = h.GetBinContent(shift + i);
            }
            return matrix;
          } break;
          case EOrient::kHorizontal: {
            TMatrix matrix(1, size);
            for (int i = 0; i < size; i++) {
              matrix[0][i] = h.GetBinContent(shift + i);
            }
            return matrix;
          } break;
        }
        return TMatrixD();
      }

      TMatrixD GetMatrix(const TH2& h, Bool_t swap, Bool_t underflow) {
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

        Int_t binPlus = underflow ? 2 : 0;
        Int_t shift   = underflow ? 0 : 1;

        TMatrixD vect(binX + binPlus, binX + binPlus);
        for (int i = 0; i < binX + binPlus; i++) {
          for (int j = 0; j < binX + binPlus; j++) {
            if (swap) {
              vect[binX + binPlus - j][i] = h.GetBinContent(i + shift, j + shift);
            } else {
              vect[j][i] = h.GetBinContent(i + shift, j + shift);
            }
          }
        }
        return vect;
      }

      void DiagonalOnEmpty(TMatrixD& matrix, ENorm opt, Double_t epsilon) {
        if (opt == ENorm::kByRow) {
          for (int iRow = 0; iRow < matrix.GetNrows(); iRow++) {
            double rowSum = 0;
            for (int iCol = 0; iCol < matrix.GetNcols(); iCol++) {
              rowSum += matrix[iCol][iRow];
            }
            if (TMath::Abs(rowSum) <= epsilon) { matrix[iRow][iRow] = 1; }
          }
        } else if (opt == ENorm::kByColum) {
          for (int iCol = 0; iCol < matrix.GetNcols(); iCol++) {
            double colSum = 0;
            for (int iRow = 0; iRow < matrix.GetNrows(); iRow++) {
              colSum += matrix[iCol][iRow];
            }
            if (TMath::Abs(colSum) <= epsilon) { matrix[iCol][iCol] = 1; }
          }
        } else {
          for (int iCol = 0; iCol < matrix.GetNcols(); iCol++) {
            if (matrix[iCol][iCol] < epsilon) matrix[iCol][iCol] = 1;
          }
        }
      }

      void RemoveNan(TMatrixD& m) {
        for (int i = 0; i < m.GetNrows(); i++) {
          for (int j = 0; j < m.GetNcols(); j++) {
            if (std::isnan(m[i][j])) m[i][j] = 0;
          }
        }
      }

      TH1D* GetHist1D(TMatrixD& matrix, Double_t min, Double_t max, Bool_t overflow) {
        Int_t nMatrixBins = 0;
        std::vector<double> vec;
        if (matrix.GetNcols() == 1) {
          nMatrixBins = matrix.GetNrows();
          for (int i = 0; i < nMatrixBins; i++)
            vec.push_back(matrix[i][0]);
        } else {
          nMatrixBins = matrix.GetNcols();
          for (int i = 0; i < nMatrixBins; i++)
            vec.push_back(matrix[0][i]);
        }
        TH1D* h = new TH1D("mhist", "mhist", overflow ? nMatrixBins - 2 : nMatrixBins, min, max);
        h->SetDirectory(nullptr);
        const Int_t shift = overflow ? 0 : 1;
        for (int i = 0; i < nMatrixBins; i++) {
          h->SetBinContent(i + shift, vec[i]);
        }
        return h;
      }
    }  // namespace Math
  }    // namespace Std
}  // namespace Hal
