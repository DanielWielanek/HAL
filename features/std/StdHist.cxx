/*
 * Hal::StdHist.cxx
 *
 *  Created on: 24 lut 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "StdHist.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TArray.h>
#include <TArrayD.h>
#include <TAttAxis.h>
#include <TAttFill.h>
#include <TAttLine.h>
#include <TAttMarker.h>
#include <TAttPad.h>
#include <TAxis.h>
#include <TCollection.h>
#include <TColor.h>
#include <TGaxis.h>
#include <TGraph.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TList.h>
#include <TMath.h>
#include <TMathBase.h>
#include <TMatrixDfwd.h>
#include <TMatrixT.h>
#include <TNamed.h>
#include <TObjArray.h>
#include <TPaletteAxis.h>
#include <TPave.h>
#include <TROOT.h>
#include <TRandom.h>
#include <TStyle.h>
#include <TVirtualPad.h>
#include <algorithm>
#include <functional>
#include <iostream>
#include <stddef.h>
#include <utility>
#include <vector>

#include "Cout.h"
#include "Splines.h"
#include "StdMath.h"
#include "StdString.h"

NamespaceImp(Hal::Std)


  namespace Hal {
  namespace Std {
    void RemoveNan(TH1* h, Double_t fill, Double_t fill_e) {
      if (h->InheritsFrom("TH3")) {
        for (int i = 0; i <= h->GetNbinsX() + 1; i++) {
          for (int j = 0; j <= h->GetNbinsY() + 1; j++) {
            for (int k = 0; k <= h->GetNbinsZ() + 1; k++) {
              if (TMath::IsNaN(h->GetBinContent(i, j, k))) {
                h->SetBinContent(i, j, k, fill);
                h->SetBinError(i, j, k, fill_e);
              }
            }
          }
        }
      } else if (h->InheritsFrom("TH2")) {
        for (int i = 0; i <= h->GetNbinsX() + 1; i++) {
          for (int j = 0; j <= h->GetNbinsY() + 1; j++) {
            if (TMath::IsNaN(h->GetBinContent(i, j))) {
              h->SetBinContent(i, j, fill);
              h->SetBinError(i, j, fill_e);
            }
          }
        }
      } else {
        for (int i = 0; i <= h->GetNbinsX() + 1; i++) {
          if (TMath::IsNaN(h->GetBinContent(i))) {
            h->SetBinContent(i, fill);
            h->SetBinError(i, fill_e);
          }
        }
      }
    }

    TH1D* GetProjection1D(const TH3* histo, Double_t min1, Double_t max1, Double_t min2, Double_t max2, Option_t* opt) {
      TString option = opt;
      Bool_t sumw    = Hal::Std::FindParam(option, "sumw", kTRUE);
      const TAxis *axisA, *axisB, *axisC;
      TH1D* projection;
      Int_t nbins = 1;
      if (option.Contains("z")) {
        axisA        = histo->GetXaxis();
        axisB        = histo->GetYaxis();
        axisC        = histo->GetZaxis();
        TString name = "_pz";
        if (!option.Contains("noautoname")) { name = Form("%i_pz", anonymCounter++); }
        if (option.Contains("bins")) {
          projection = histo->ProjectionZ(name, min1, max1, min2, max2, "e");
        } else {
          projection = histo->ProjectionZ(
            name, axisA->FindFixBin(min1), axisA->FindFixBin(max1), axisB->FindFixBin(min2), axisB->FindFixBin(max2), "o");
        }
      } else if (option.Contains("y")) {
        axisA        = histo->GetXaxis();
        axisB        = histo->GetZaxis();
        axisC        = histo->GetYaxis();
        TString name = "_py";
        if (!option.Contains("noautoname")) { name = Form("%i_py", anonymCounter++); }
        if (option.Contains("bins")) {
          projection = histo->ProjectionY(name, min1, max1, min2, max2, "e");
        } else {
          projection = histo->ProjectionY(
            name, axisA->FindFixBin(min1), axisA->FindFixBin(max1), axisB->FindFixBin(min2), axisB->FindFixBin(max2), "o");
        }
      } else {
        axisA        = histo->GetYaxis();
        axisB        = histo->GetZaxis();
        axisC        = histo->GetXaxis();
        TString name = "_px";
        if (!option.Contains("noautoname")) { name = Form("%i_px", anonymCounter++); }
        if (option.Contains("bins")) {
          projection = histo->ProjectionX(name, min1, max1, min2, max2, "e");
        } else {
          projection = histo->ProjectionX(
            name, axisA->FindFixBin(min1), axisA->FindFixBin(max1), axisB->FindFixBin(min2), axisB->FindFixBin(max2), "o");
        }
      }
      if (option.Contains("bins")) {
        nbins = (max1 - min1 + 1) * (max2 - min2 + 1);
      } else {
        nbins = axisA->FindFixBin(max1) - axisA->FindFixBin(min1) + 1;
        nbins = nbins * (axisB->FindFixBin(max2) - axisB->FindFixBin(min2) + 1);
      }
      if (sumw) projection->Sumw2();
      if (option.Contains("scale")) {
        if (nbins != 0) projection->Scale(1.0 / nbins);
      }
      projection->GetXaxis()->SetTitle(axisC->GetTitle());
      // projection->SetDirectory(0);
      return projection;
    }

    TH1D* GetProjection1D(const TH2* histo, Double_t min, Double_t max, Option_t* opt) {
      TString option = opt;
      TH1D* projection;
      Bool_t sumw    = Hal::Std::FindParam(option, "sumw", kTRUE);
      Double_t nbins = 0;
      if (option.Contains("y")) {
        TString name = "_py";
        if (!option.Contains("noautoname")) { name = Form("%i_py", anonymCounter++); }
        if (option.Contains("bins")) {
          projection = histo->ProjectionY(name, min, max);
          nbins      = max - min;
        } else {
          projection = histo->ProjectionY(name, histo->GetXaxis()->FindBin(min), histo->GetXaxis()->FindBin(max));
          nbins      = histo->GetXaxis()->FindBin(max) - histo->GetXaxis()->FindBin(min);
        }
        projection->GetXaxis()->SetTitle(histo->GetYaxis()->GetTitle());
      } else {
        TString name = "_px";
        if (!option.Contains("noautoname")) { name = Form("%i_px", anonymCounter++); }
        if (option.Contains("bins")) {
          projection = histo->ProjectionX(name, min, max);
          nbins      = max - min;
        } else {
          projection = histo->ProjectionX(name, histo->GetYaxis()->FindBin(min), histo->GetYaxis()->FindBin(max));
          nbins      = histo->GetYaxis()->FindBin(max) - histo->GetYaxis()->FindBin(min);
        }
        projection->GetXaxis()->SetTitle(histo->GetXaxis()->GetTitle());
      }
      if (sumw) projection->Sumw2();
      if (option.Contains("scale")) {
        if (nbins != 0) projection->Scale(1.0 / (nbins + 1));
      }
      projection->SetDirectory(0);
      return projection;
    }

    TH2D* GetProjection2D(const TH3* histo, Double_t min, Double_t max, Option_t* opt) {
      TString option    = opt;
      TString title     = histo->GetTitle();
      TH3* histo_cloned = (TH3*) histo->Clone();
      TH2D* projection;
      TAxis* axis;
      Bool_t sumw = Hal::Std::FindParam(option, "sumw", kTRUE);
      // TAxis *axisA, *axisB;
      Int_t nbins = 0;
      TString projection_option;
      if (Hal::Std::FindParam(option, "yz")) {
        axis              = histo_cloned->GetXaxis();
        projection_option = "yze";
      } else if (Hal::Std::FindParam(option, "zy")) {
        axis              = histo_cloned->GetXaxis();
        projection_option = "zye";
      } else if (Hal::Std::FindParam(option, "xz")) {
        axis              = histo_cloned->GetYaxis();
        projection_option = "xze";
        // axisA = histo->GetXaxis();
        // axisB = histo->GetZaxis();
      } else if (Hal::Std::FindParam(option, "zx")) {
        axis              = histo_cloned->GetYaxis();
        projection_option = "zxe";
      } else if (Hal::Std::FindParam(option, "yx")) {
        axis              = histo_cloned->GetZaxis();
        projection_option = "yxe";
      } else {
        axis              = histo_cloned->GetZaxis();
        projection_option = "xye";
        // axisA = histo->GetXaxis();
        // axisB = histo->GetYaxis();
      }
      if (Hal::Std::FindParam(option, "bins")) {
        axis->SetRange(min, max);
        nbins = 1 + max - min;
      } else {
        if (min == max) {  // same values, this makes problem
          max = min + axis->GetBinWidth(1) * 0.1;
        }
        axis->SetRangeUser(min, max);
        nbins = 1 + axis->FindBin(max) - axis->FindBin(min);
      }
      projection = (TH2D*) histo_cloned->Project3D(projection_option);
      delete histo_cloned;
      //   axis->SetRange(0, 0);
      if (sumw) projection->Sumw2();
      if (option.Contains("scale")) { projection->Scale(1.0 / nbins); }
      if (!option.Contains("noautoname")) {
        TString name = Form("%i_2dproj", anonymCounter++);
        projection->SetName(name);
      }
      projection->SetDirectory(0);
      return projection;
    }

    TH1D* SmearHistogram(TH1D* input_histogram, TH2D* smear_matrix, Option_t* opt) {
      TString option = opt;
      Bool_t bad_map = kFALSE;
      if (input_histogram->GetNbinsX() != smear_matrix->GetNbinsX()) {
        if (!option.Contains("forced")) Hal::Cout::PrintInfo("Incompatible histograms for smearing", Hal::EInfo::kWarning);
        bad_map = kTRUE;
      }
      if (input_histogram->GetNbinsX() != smear_matrix->GetNbinsY()) {
        if (!option.Contains("forced")) Hal::Cout::PrintInfo("Incompatible histograms for smearing", Hal::EInfo::kWarning);
        bad_map = kTRUE;
      }
      if (option.Contains("forced") && bad_map) {
        Hal::Spline2D* m_map = new Hal::Spline2D(smear_matrix);
        m_map->Refit();
        TH2D* smear_new = new TH2D("smear_temp",
                                   "smear_temp",
                                   input_histogram->GetNbinsX(),
                                   input_histogram->GetXaxis()->GetXmin(),
                                   input_histogram->GetXaxis()->GetXmax(),
                                   input_histogram->GetNbinsX(),
                                   input_histogram->GetXaxis()->GetXmin(),
                                   input_histogram->GetXaxis()->GetXmax());
        for (int i = 0; i <= input_histogram->GetNbinsX() + 1; i++) {
          Double_t x = smear_new->GetXaxis()->GetBinCenter(i);
          for (int j = 0; j <= input_histogram->GetNbinsX() + 1; j++) {
            Double_t y = smear_new->GetYaxis()->GetBinCenter(j);
            if (m_map->Eval(x, y) >= 0) {
              smear_new->SetBinContent(i, j, m_map->Eval(x, y));
              smear_new->SetBinError(i, j, m_map->GetError(x, y));
            }
          }
        }
        smear_matrix = smear_new;
        delete m_map;
      } else {
        if (bad_map) return NULL;
      }

      TH1D* cloned = (TH1D*) input_histogram->Clone(Form("%s_smeared", input_histogram->GetName()));
      cloned->Reset();
      Int_t N = smear_matrix->GetNbinsX() + 2;

      TH2D* norm_matrix = smear_matrix;  //(TH2D*)smear_matrix->Clone("temp");
      for (int i = 0; i <= N - 1; i++) {
        Double_t val = 0;
        for (int j = 0; j < N; j++) {
          val += smear_matrix->GetBinContent(i, j);
        }
        for (int j = 0; j < N; j++) {
          if (val != 0) {
            norm_matrix->SetBinContent(i, j, smear_matrix->GetBinContent(i, j) / val);
            norm_matrix->SetBinError(i, j, smear_matrix->GetBinError(i, j) / val);
          } else {
            Double_t diag = 0.0;
            if (j == i) diag = 1.0;
            norm_matrix->SetBinContent(i, j, diag);
            norm_matrix->SetBinError(i, j, 0);
          }
        }
      }

      TMatrixD AM(N, N);
      TMatrixD AME(N, N);
      TMatrixD XM(N, 1);
      TMatrixD XME(N, 1);
      TMatrixD YM(N, 1), YME(N, 1);

      for (int i = 0; i < N; i++) {
        XM[i][0]  = input_histogram->GetBinContent(i);
        XME[i][0] = input_histogram->GetBinError(i);
        for (int j = 0; j < N; j++) {
          AM[i][j]  = norm_matrix->GetBinContent(j, i);
          AME[i][j] = norm_matrix->GetBinError(j, i);
        }
      }

      if (option.Contains("REV") || option.Contains("rev")) {
        YME = AM * XME;  // TODO approximation !
        AM.Invert();
        YM = AM * XM;
        for (int i = 0; i < N; i++) {
          cloned->SetBinContent(i, YM[i][0]);
          cloned->SetBinError(i, YME[i][0]);
        }
      } else {
        YM            = AM * XM;
        YME           = AM * XME;
        TMatrixD YME2 = AME * XM;
        for (int i = 0; i < N; i++) {
          cloned->SetBinContent(i, YM[i][0]);
          cloned->SetBinError(i, YME[i][0]);
        }
      }
      if (bad_map) delete smear_matrix;
      //		delete norm_matrix;
      return cloned;
    }

    TH1* MakeHisto1D(TString name, TString title, TVector3 Xaxis, Char_t type) {
      switch (type) {
        case 'D': return new TH1D(name, title, (int) Xaxis.X(), Xaxis.Y(), Xaxis.Z()); break;
        case 'F': return new TH1F(name, title, (int) Xaxis.X(), Xaxis.Y(), Xaxis.Z()); break;
        case 'I': return new TH1I(name, title, (int) Xaxis.X(), Xaxis.Y(), Xaxis.Z()); break;
      }
      return nullptr;
    }

    TH2* MakeHisto2D(TString name, TString title, TVector3 Xaxis, TVector3 Yaxis, Char_t type) {
      switch (type) {
        case 'D':
          return new TH2D(name, title, (int) Xaxis.X(), Xaxis.Y(), Xaxis.Z(), (int) Yaxis.X(), Yaxis.Y(), Yaxis.Z());
          break;
        case 'F':
          return new TH2F(name, title, (int) Xaxis.X(), Xaxis.Y(), Xaxis.Z(), (int) Yaxis.X(), Yaxis.Y(), Yaxis.Z());
          break;
        case 'I':
          return new TH2I(name, title, (int) Xaxis.X(), Xaxis.Y(), Xaxis.Z(), (int) Yaxis.X(), Yaxis.Y(), Yaxis.Z());
          break;
      }
      return nullptr;
    }

    TH3* MakeHisto3D(TString name, TString title, TVector3 Xaxis, TVector3 Yaxis, TVector3 Zaxis, Char_t type) {
      switch (type) {
        case 'D':
          return new TH3D(name,
                          title,
                          (int) Xaxis.X(),
                          Xaxis.Y(),
                          Xaxis.Z(),
                          (int) Yaxis.X(),
                          Yaxis.Y(),
                          Yaxis.Z(),
                          (int) Zaxis.X(),
                          Zaxis.Y(),
                          Zaxis.Z());
          break;
        case 'F':
          return new TH3F(name,
                          title,
                          (int) Xaxis.X(),
                          Xaxis.Y(),
                          Xaxis.Z(),
                          (int) Yaxis.X(),
                          Yaxis.Y(),
                          Yaxis.Z(),
                          (int) Zaxis.X(),
                          Zaxis.Y(),
                          Zaxis.Z());
          break;
        case 'I':
          return new TH3I(name,
                          title,
                          (int) Xaxis.X(),
                          Xaxis.Y(),
                          Xaxis.Z(),
                          (int) Yaxis.X(),
                          Yaxis.Y(),
                          Yaxis.Z(),
                          (int) Zaxis.X(),
                          Zaxis.Y(),
                          Zaxis.Z());
          break;
      }
      return nullptr;
    }

    void HistogramEdges(TH1* h, TString option, Double_t value) {
      Char_t axis = 'x';
      enum eFill { kByVal, kByNeighbour };
      enum eBin { kFirst, kLast };
      if (option.Contains("y")) { axis = 'y'; }
      if (option.Contains("z")) { axis = 'z'; }
      eBin bin = kFirst;
      if (option.Contains("ov")) { bin = kLast; }
      eFill fill = kByNeighbour;
      if (option.Contains("val")) {
        fill = kByVal;
      } else if (h->InheritsFrom("TH3")) {
        if (axis == 'z') {
          for (int i = 0; i <= h->GetNbinsX(); i++) {
            for (int j = 0; j < h->GetNbinsY(); j++) {
              switch (bin) {
                case kFirst: {
                  switch (fill) {
                    case kByNeighbour: {
                      h->SetBinContent(i, j, 0, h->GetBinContent(i, j, 1));
                    } break;
                    case kByVal: {
                      h->SetBinContent(i, j, 0, value);
                    } break;
                  }
                } break;
                case kLast: {
                  switch (fill) {
                    case kByNeighbour: {
                      h->SetBinContent(i, j, h->GetNbinsX() + 1, h->GetBinContent(i, j, 1));
                    } break;
                    case kByVal: {
                      h->SetBinContent(i, j, h->GetNbinsX() + 1, value);
                    } break;
                  }
                } break;
              }
            }
          }
        } else if (axis == 'y') {
          for (int i = 0; i <= h->GetNbinsX(); i++) {
            for (int j = 0; j < h->GetNbinsZ(); j++) {
              switch (bin) {
                case kFirst: {
                  switch (fill) {
                    case kByNeighbour: {
                      h->SetBinContent(i, 0, j, h->GetBinContent(i, 1, j));
                    } break;
                    case kByVal: {
                      h->SetBinContent(i, 0, j, value);
                    } break;
                  }
                } break;
                case kLast: {
                  switch (fill) {
                    case kByNeighbour: {
                      h->SetBinContent(i, h->GetNbinsX() + 1, j, h->GetBinContent(i, 1, j));
                    } break;
                    case kByVal: {
                      h->SetBinContent(i, h->GetNbinsX() + 1, j, value);
                    } break;
                  }
                } break;
              }
            }
          }
        } else {  // x
          for (int i = 0; i <= h->GetNbinsY(); i++) {
            for (int j = 0; j < h->GetNbinsZ(); j++) {
              switch (bin) {
                case kFirst: {
                  switch (fill) {
                    case kByNeighbour: {
                      h->SetBinContent(0, i, j, h->GetBinContent(1, i, j));
                    } break;
                    case kByVal: {
                      h->SetBinContent(0, i, j, value);
                    } break;
                  }
                } break;
                case kLast: {
                  switch (fill) {
                    case kByNeighbour: {
                      h->SetBinContent(h->GetNbinsX() + 1, i, j, h->GetBinContent(1, i, j));
                    } break;
                    case kByVal: {
                      h->SetBinContent(h->GetNbinsX() + 1, i, j, value);
                    } break;
                  }
                } break;
              }
            }
          }
        }
      } else if (h->InheritsFrom("TH2")) {
        if (axis == 'y') {
          for (int i = 0; i <= h->GetNbinsX() + 1; i++) {
            switch (bin) {
              case kFirst: {
                switch (fill) {
                  case kByNeighbour: {
                    h->SetBinContent(i, 0, h->GetBinContent(i, 1));
                  } break;
                  case kByVal: {
                    h->SetBinContent(i, 0, value);
                  } break;
                }
              } break;
              case kLast: {
                switch (fill) {
                  case kByNeighbour: {
                    h->SetBinContent(i, h->GetNbinsY() + 1, h->GetBinContent(i, h->GetNbinsY()));
                  } break;
                  case kByVal: {
                    h->SetBinContent(i, h->GetNbinsY() + 1, value);
                  } break;
                }
              } break;
            }
          }
        } else {  // axis x
          for (int i = 0; i <= h->GetNbinsY() + 1; i++) {
            switch (bin) {
              case kFirst: {
                switch (fill) {
                  case kByNeighbour: {
                    h->SetBinContent(0, i, h->GetBinContent(1, i));
                  } break;
                  case kByVal: {
                    h->SetBinContent(0, i, value);
                  } break;
                }
              } break;
              case kLast: {
                switch (fill) {
                  case kByNeighbour: {
                    h->SetBinContent(h->GetNbinsX() + 1, i, h->GetBinContent(h->GetNbinsX(), i));
                  } break;
                  case kByVal: {
                    h->SetBinContent(h->GetNbinsX() + 1, i, value);
                  } break;
                }
              } break;
            }
          }
        }

      } else {  // TH1
        if (bin == kFirst) {
          if (fill == kByVal) { h->SetBinContent(0, value); }
          if (fill == kByNeighbour) { h->SetBinContent(0, h->GetBinContent(1)); }
        }
        if (bin == kLast) {
          if (fill == kByVal) { h->SetBinContent(h->GetNbinsX() + 1, value); }
          if (fill == kByNeighbour) { h->SetBinContent(h->GetNbinsX() + 1, h->GetBinContent(h->GetNbinsX())); }
        }
      }
    }

    TH1* ExtendToUnderFlowOverFlow(const TH1& h) {
      TH1* res = nullptr;
      Int_t binsX, binsY, binsZ;
      Double_t minX, minY, minZ, maxX, maxY, maxZ;
      Hal::Std::GetAxisPar(h, binsX, minX, maxX, "x");
      Hal::Std::GetAxisPar(h, binsY, minY, maxY, "y");
      Hal::Std::GetAxisPar(h, binsZ, minZ, maxZ, "z");
      Double_t dx   = h.GetXaxis()->GetBinWidth(1);
      Double_t dy   = h.GetYaxis()->GetBinWidth(1);
      Double_t dz   = h.GetZaxis()->GetBinWidth(1);
      TString title = h.GetTitle();
      TString name  = h.GetName();

      if (h.InheritsFrom("TH3")) {  // 3D histo
        res = new TH3D(
          name, title, binsX + 2, minX - dx, maxX + dx, binsY + 2, minY - dy, maxY + dy, binsZ + 2, minZ - dz, maxZ + dz);
        for (int i = 0; i <= binsX + 1; i++) {
          for (int j = 0; j <= binsY + 1; j++) {
            for (int k = 0; k <= binsZ + 1; k++) {
              res->SetBinContent(i + 1, j + 1, k + 1, h.GetBinContent(i, j, k));
              res->SetBinError(i + 1, j + 1, k + 1, h.GetBinError(i, j, k));
            }
          }
        }
      } else if (h.InheritsFrom("TH2")) {
        res = new TH2D(name, title, binsX + 2, minX - dx, maxX + dx, binsY + 2, minY - dy, maxY + dy);
        for (int i = 0; i <= binsX + 1; i++) {
          for (int j = 0; j <= binsY + 1; j++) {
            res->SetBinContent(i + 1, j + 1, h.GetBinContent(i, j));
            res->SetBinError(i + 1, j + 1, h.GetBinError(i, j));
          }
        }
      } else {
        res = new TH1D(name, title, binsX + 2, minX - dx, maxX + dx);
        for (int i = 0; i <= binsX + 1; i++) {
          res->SetBinContent(i + 1, h.GetBinContent(i));
          res->SetBinError(i + 1, h.GetBinError(i));
        }
      }
      return res;
    }

    void HistogramExtend(TH1* h, Char_t axis, Double_t factor) {
      TAxis* x = NULL;
      switch (axis) {
        case 'y': {
          x = h->GetYaxis();
        } break;
        case 'z': {
          x = h->GetZaxis();
        } break;
        default: {
          x = h->GetXaxis();
        } break;
      }
      if (x->IsVariableBinSize()) {
        const TArrayD* ar = x->GetXbins();
        Int_t size        = ar->GetSize();
        Double_t* edges   = new Double_t[size];
        for (int i = 0; i < ar->GetSize(); i++) {
          edges[i] = ar->At(i) * factor;
        }
        x->Set(size - 1, edges);
        delete[] edges;
      } else {
        x->Set(x->GetNbins(), x->GetXmin() * factor, x->GetXmax() * factor);
      }
      h->ResetStats();
    }

    void CopyAxisProp(const TAxis* from, TAxis* to, TString option) {
      TAxis default_axis = TAxis();
      default_axis.SetAxisColor(kBlack);
      default_axis.SetTitleColor(kBlack);
      default_axis.SetLabelColor(kBlack);
      if (default_axis.GetCenterTitle() != from->GetCenterTitle()) to->CenterTitle(from->GetCenterTitle());
      if (default_axis.GetNdivisions() != from->GetNdivisions()) to->SetNdivisions(from->GetNdivisions());
      if (default_axis.GetDecimals() != from->GetDecimals()) to->SetDecimals(from->GetDecimals());
      if (default_axis.GetCenterTitle() != from->GetCenterTitle()) to->CenterTitle(from->GetCenterTitle());
      if (default_axis.GetTickLength() != from->GetTickLength()) to->SetTickLength(from->GetTickLength());
      if (default_axis.GetAxisColor() != from->GetAxisColor()) to->SetAxisColor(from->GetAxisColor());
      if (default_axis.GetTitleColor() != from->GetTitleColor()) to->SetTitleColor(from->GetTitleColor());
      if (default_axis.GetTitleFont() != from->GetTitleFont()) to->SetTitleFont(from->GetTitleFont());
      if (default_axis.GetTitleOffset() != from->GetTitleOffset()) to->SetTitleOffset(from->GetTitleOffset());
      if (Float_t(0.035) != from->GetTitleSize()) to->SetTitleSize(from->GetTitleSize());  // why 0.35 is not default? don't know
      if (default_axis.GetLabelColor() != from->GetLabelColor()) to->SetLabelColor(from->GetLabelColor());
      if (default_axis.GetLabelFont() != from->GetLabelFont()) to->SetLabelFont(from->GetLabelFont());
      if (default_axis.GetLabelOffset() != from->GetLabelOffset()) to->SetLabelOffset(from->GetLabelOffset());
      if (default_axis.GetLabelSize() != from->GetLabelSize()) to->SetLabelSize(from->GetLabelSize());
      if (!Hal::Std::FindParam(option, "!tit", kTRUE)) { to->SetTitle(from->GetTitle()); }
    }

    TH1D* GetDiagonalProjection1D(TH3* h, TString dir, Double_t start, Double_t start2) {
      if (h->GetNbinsX() != h->GetNbinsY() || h->GetNbinsX() != h->GetNbinsZ()) {
        Hal::Cout::PrintInfo("Cannot make projection in nonsymetric histo", Hal::EInfo::kLowWarning);
        return nullptr;
      }
      TString temp     = dir;
      Double_t xmin    = h->GetXaxis()->GetBinLowEdge(1);
      Double_t xmax    = h->GetXaxis()->GetBinUpEdge(h->GetNbinsX());
      Int_t binsTot    = h->GetXaxis()->GetNbins();
      TString titleX   = h->GetXaxis()->GetTitle();
      TString titleY   = h->GetYaxis()->GetTitle();
      TString titleZ   = h->GetZaxis()->GetTitle();
      TString randname = Form("%i", (int) gRandom->Uniform(1E+9));
      TH1D* res        = new TH1D(randname, "res", binsTot, xmin, xmax);
      res->SetMarkerStyle(h->GetMarkerStyle());
      res->SetMarkerColor(h->GetMarkerColor());
      res->SetMarkerSize(h->GetMarkerSize());
      res->SetLineColor(h->GetLineColor());
      res->SetLineStyle(h->GetLineStyle());
      res->SetLineWidth(h->GetLineWidth());
      TString newTitle = "";
      std::vector<int> binsVecX(binsTot), binsVecY(binsTot), binsVecZ(binsTot);
      if (temp.Length() == 6 && temp.BeginsWith("xyz")) {
        for (int i = 1; i <= binsTot; i++) {
          binsVecX[i - 1] = i;
          binsVecY[i - 1] = i;
          binsVecZ[i - 1] = i;
        }
        if (temp[3] == '-') std::sort(binsVecX.begin(), binsVecX.end(), std::greater<int>());
        if (temp[4] == '-') std::sort(binsVecY.begin(), binsVecY.end(), std::greater<int>());
        if (temp[5] == '-') std::sort(binsVecZ.begin(), binsVecZ.end(), std::greater<int>());
        newTitle = titleX + temp[3] + titleY + temp[4] + titleZ + temp[5];
      } else if (temp.Length() == 4) {
        if (temp.BeginsWith("yz")) {
          Int_t binMid = h->GetXaxis()->FindBin(start);
          for (int i = 1; i <= binsTot; i++) {
            binsVecX[i - 1] = binMid;
            binsVecY[i - 1] = i;
            binsVecZ[i - 1] = i;
          }
          if (temp[2] == '-') std::sort(binsVecY.begin(), binsVecY.end(), std::greater<int>());
          if (temp[3] == '-') std::sort(binsVecZ.begin(), binsVecZ.end(), std::greater<int>());
          newTitle = titleY + temp[2] + titleZ + temp[3];
        } else if (temp.BeginsWith("xz")) {
          Int_t binMid = h->GetYaxis()->FindBin(start);
          for (int i = 1; i <= binsTot; i++) {
            binsVecX[i - 1] = i;
            binsVecY[i - 1] = binMid;
            binsVecZ[i - 1] = i;
          }
          if (temp[2] == '-') std::sort(binsVecX.begin(), binsVecX.end(), std::greater<int>());
          if (temp[3] == '-') std::sort(binsVecZ.begin(), binsVecZ.end(), std::greater<int>());
          newTitle = titleX + temp[2] + titleZ + temp[3];
        } else {  // xy
          Int_t binMid = h->GetZaxis()->FindBin(start);
          for (int i = 1; i <= binsTot; i++) {
            binsVecX[i - 1] = i;
            binsVecY[i - 1] = i;
            binsVecZ[i - 1] = binMid;
          }
          if (temp[2] == '-') std::sort(binsVecX.begin(), binsVecX.end(), std::greater<int>());
          if (temp[3] == '-') std::sort(binsVecY.begin(), binsVecY.end(), std::greater<int>());
          newTitle = titleX + temp[2] + titleY + temp[3];
        }
      } else if (temp.Length() == 2 || temp.Length() == 1) {
        TString signFlag = "";
        if (temp.Length() == 2) {
          if (temp[1] == '-') signFlag = "-";
          if (temp[1] == '+') signFlag = "+";
        }
        Bool_t rev = kFALSE;
        if (signFlag.EqualTo("-")) rev = kTRUE;
        if (temp[0] == 'y') {
          Int_t bin1 = h->GetXaxis()->FindBin(start);
          Int_t bin2 = h->GetZaxis()->FindBin(start2);
          for (int i = 1; i <= binsTot; i++) {
            binsVecX[i - 1] = bin1;
            binsVecY[i - 1] = i;
            binsVecZ[i - 1] = bin2;
          }
          if (rev) std::sort(binsVecY.begin(), binsVecY.end(), std::greater<int>());
          newTitle = titleY + signFlag;
        } else if (temp[0] == 'z') {
          Int_t bin1 = h->GetXaxis()->FindBin(start);
          Int_t bin2 = h->GetYaxis()->FindBin(start2);
          for (int i = 1; i <= binsTot; i++) {
            binsVecX[i - 1] = bin1;
            binsVecY[i - 1] = bin2;
            binsVecZ[i - 1] = i;
          }
          if (rev) std::sort(binsVecZ.begin(), binsVecZ.end(), std::greater<int>());
          newTitle = titleZ + signFlag;
        } else {
          Int_t bin1 = h->GetYaxis()->FindBin(start);
          Int_t bin2 = h->GetZaxis()->FindBin(start2);
          for (int i = 1; i <= binsTot; i++) {
            binsVecX[i - 1] = i;
            binsVecY[i - 1] = bin1;
            binsVecZ[i - 1] = bin2;
          }
          if (rev) std::sort(binsVecX.begin(), binsVecX.end(), std::greater<int>());
          newTitle = titleX + signFlag;
        }
      } else {
        Hal::Cout::PrintInfo("Std::GetDiagonalProjection1D unknown option", Hal::EInfo::kLowWarning);
        delete res;
        return nullptr;
      }

      for (int i = 1; i <= binsTot; i++) {
        res->SetBinContent(i, h->GetBinContent(binsVecX[i - 1], binsVecY[i - 1], binsVecZ[i - 1]));
        res->SetBinError(i, h->GetBinError(binsVecX[i - 1], binsVecY[i - 1], binsVecZ[i - 1]));
      }
      res->GetXaxis()->SetTitle(newTitle);
      return res;
    }  // namespace Hal::Std

    Bool_t AreSimilar(const TH1* x, const TH1* y, Bool_t classes) {
      if (classes) {
        TString classname1 = x->ClassName();
        TString classname2 = y->ClassName();
        if (!classname1.EqualTo(classname2)) return kFALSE;
      }
      if (x->InheritsFrom("TH3") && y->InheritsFrom("TH3")) {
        const TH3* h1 = static_cast<const TH3*>(x);
        const TH3* h2 = static_cast<const TH3*>(y);
        if (h1->GetNbinsX() != h2->GetNbinsX()) return kFALSE;
        if (h1->GetNbinsY() != h2->GetNbinsY()) return kFALSE;
        if (h1->GetNbinsZ() != h2->GetNbinsZ()) return kFALSE;
        if (h1->GetXaxis()->GetXmin() != h2->GetXaxis()->GetXmin()) return kFALSE;
        if (h1->GetXaxis()->GetXmax() != h2->GetXaxis()->GetXmax()) return kFALSE;
        if (h1->GetYaxis()->GetXmin() != h2->GetYaxis()->GetXmin()) return kFALSE;
        if (h1->GetYaxis()->GetXmax() != h2->GetYaxis()->GetXmax()) return kFALSE;
        if (h1->GetZaxis()->GetXmin() != h2->GetZaxis()->GetXmin()) return kFALSE;
        if (h1->GetZaxis()->GetXmax() != h2->GetZaxis()->GetXmax()) return kFALSE;
      } else if (x->InheritsFrom("TH2") && y->InheritsFrom("TH2")) {
        const TH2* h1 = static_cast<const TH2*>(x);
        const TH2* h2 = static_cast<const TH2*>(y);
        if (h1->GetNbinsX() != h2->GetNbinsX()) return kFALSE;
        if (h1->GetNbinsY() != h2->GetNbinsY()) return kFALSE;
        if (h1->GetXaxis()->GetXmin() != h2->GetXaxis()->GetXmin()) return kFALSE;
        if (h1->GetXaxis()->GetXmax() != h2->GetXaxis()->GetXmax()) return kFALSE;
        if (h1->GetYaxis()->GetXmin() != h2->GetYaxis()->GetXmin()) return kFALSE;
        if (h1->GetYaxis()->GetXmax() != h2->GetYaxis()->GetXmax()) return kFALSE;
      } else {
        const TH1* h1 = static_cast<const TH1*>(x);
        const TH1* h2 = static_cast<const TH1*>(y);
        if (h1->GetNbinsX() != h2->GetNbinsX()) return kFALSE;
        if (h1->GetXaxis()->GetXmin() != h2->GetXaxis()->GetXmin()) return kFALSE;
        if (h1->GetXaxis()->GetXmax() != h2->GetXaxis()->GetXmax()) return kFALSE;
      }
      return kTRUE;
    }

    void GetAxisPar(const TH1& obj, Int_t& nbins, Double_t& min, Double_t& max, Option_t* opt) {
      TString option = opt;
      if (option == "x" || option == "X") {
        const TAxis* x = obj.GetXaxis();
        nbins          = x->GetNbins();
        min            = x->GetBinLowEdge(1);
        max            = x->GetBinUpEdge(nbins);
      } else if (option == "y" || option == "Y") {
        const TAxis* x = obj.GetYaxis();
        nbins          = x->GetNbins();
        min            = x->GetBinLowEdge(1);
        max            = x->GetBinUpEdge(nbins);
      } else if (option == "z" || option == "Z") {
        const TAxis* x = obj.GetZaxis();
        nbins          = x->GetNbins();
        min            = x->GetBinLowEdge(1);
        max            = x->GetBinUpEdge(nbins);
      }
    }

    std::vector<double> GetAxisCenters(const TH1& obj, Option_t* opt) {
      TString option = opt;
      auto getAxis   = [](TString& opti, const TH1& hst) {
        if (Hal::Std::FindParam(opti, "x", kTRUE)) { return hst.GetXaxis(); }
        if (Hal::Std::FindParam(opti, "y", kTRUE)) { return hst.GetYaxis(); }
        if (Hal::Std::FindParam(opti, "z", kTRUE)) { return hst.GetZaxis(); }
        return (const TAxis*) nullptr;
      };
      auto axis = (const TAxis*) getAxis(option, obj);
      if (axis == nullptr) return std::vector<double>();

      int bins = axis->GetNbins();
      std::vector<double> vec;
      int startbin = 1;
      if (Hal::Std::FindParam(option, "o", kTRUE)) {
        startbin = 0;
        bins++;
      }
      for (int i = startbin; i <= bins; i++) {
        vec.push_back(axis->GetBinCenter(i));
      }
      return vec;
    }

    namespace {  // anonymous namespace this is for computing axes of histogram

      std::vector<int> FoldAxis(const TAxis& x, Double_t val) {
        const Int_t nBins = x.GetNbins();
        Int_t binFold     = x.FindBin(val);
        Double_t center   = x.GetBinCenter(binFold);
        //  Double_t low      = x.GetBinLowEdge(binFold);
        std::vector<int> foldBinMap(nBins + 1);
        Int_t shift = 0;
        if (val != center) shift = 1;
        for (int i = 1; i <= nBins; i++) {
          Int_t delta = binFold - i + shift;
          Int_t bin   = binFold + delta;
          if (bin < 1) bin = bin + nBins;
          if (bin > nBins) bin = bin - nBins;
          foldBinMap[i] = bin;
        }
        return foldBinMap;
      }

      void CropAxis(const TAxis* x,
                    Int_t& nbins,
                    std::pair<int, int>& binId,
                    std::pair<double, double>& val,
                    Double_t lmin,
                    Double_t lmax,
                    TString opt) {
        if (opt == "vals") {
          binId.first  = x->FindBin(lmin);
          binId.second = x->FindBin(lmax);
        } else {
          binId.first  = lmin;
          binId.second = lmax;
        }
        binId.first  = TMath::Max(1, binId.first);
        binId.second = TMath::Min(binId.second, x->GetNbins());
        val.first    = x->GetBinLowEdge(binId.first);
        val.second   = x->GetBinUpEdge(binId.second);
        nbins        = binId.second - binId.first + 1;
      };

    }  // namespace

    void Fold1D(Double_t val, TH1& h) {
      TH1* tempCopy               = (TH1*) h.Clone();
      const Int_t nBins           = h.GetXaxis()->GetNbins();
      std::vector<int> foldBinMap = FoldAxis(*h.GetXaxis(), val);
      h.Reset();
      for (int i = 1; i <= nBins; i++) {
        Double_t old = h.GetBinContent(i);
        h.SetBinContent(i, old + tempCopy->GetBinContent(foldBinMap[i]));
        Double_t sumv = tempCopy->GetSumw2()->GetAt(foldBinMap[i]);
        h.GetSumw2()->SetAt(h.GetSumw2()->At(i) + sumv, i);
      }
      delete tempCopy;
    }

    void Fold2D(Double_t val, TH2& h, TString opt) {
      TH2* tempCopy      = (TH2*) h.Clone();
      const Int_t nBinsX = h.GetXaxis()->GetNbins();
      const Int_t nBinsY = h.GetYaxis()->GetNbins();
      if (Hal::Std::FindParam(opt, "x", kTRUE)) {
        std::vector<int> foldBinMap = FoldAxis(*h.GetXaxis(), val);
        for (int i = 1; i <= nBinsX; i++) {
          for (int j = 1; j <= nBinsY; j++) {
            Double_t old = h.GetBinContent(i, j);
            Double_t add = tempCopy->GetBinContent(foldBinMap[i], j);
            h.SetBinContent(i, j, old + add);
          }
        }
      } else if (Hal::Std::FindParam(opt, "y", kTRUE)) {
        std::vector<int> foldBinMap = FoldAxis(*h.GetYaxis(), val);
        for (int i = 1; i <= nBinsX; i++) {
          for (int j = 1; j <= nBinsY; j++) {
            Double_t old = h.GetBinContent(i, j);
            Double_t add = tempCopy->GetBinContent(i, foldBinMap[j]);
            h.SetBinContent(i, j, old + add);
          }
        }
      }
      h.ResetStats();
      delete tempCopy;
    }

    void Fold3D(Double_t val, TH2& h, TString opt) {
      TH3* tempCopy      = (TH3*) h.Clone();
      const Int_t nBinsX = h.GetXaxis()->GetNbins();
      const Int_t nBinsY = h.GetYaxis()->GetNbins();
      const Int_t nBinsZ = h.GetZaxis()->GetNbins();
      if (Hal::Std::FindParam(opt, "x", kTRUE)) {
        std::vector<int> foldBinMap = FoldAxis(*h.GetXaxis(), val);
        for (int i = 1; i <= nBinsX; i++) {
          for (int j = 1; j <= nBinsY; j++) {
            for (int k = 1; k <= nBinsZ; k++) {
              Double_t old = h.GetBinContent(i, j, k);
              Double_t add = tempCopy->GetBinContent(foldBinMap[i], j, k);
              h.SetBinContent(i, j, k, old + add);
            }
          }
        }
      } else if (Hal::Std::FindParam(opt, "y", kTRUE)) {
        std::vector<int> foldBinMap = FoldAxis(*h.GetYaxis(), val);
        for (int i = 1; i <= nBinsX; i++) {
          for (int j = 1; j <= nBinsY; j++) {
            for (int k = 1; k <= nBinsZ; k++) {
              Double_t old = h.GetBinContent(i, j, k);
              Double_t add = tempCopy->GetBinContent(i, foldBinMap[j], k);
              h.SetBinContent(i, j, k, old + add);
            }
          }
        }
      } else if (Hal::Std::FindParam(opt, "z", kTRUE)) {
        std::vector<int> foldBinMap = FoldAxis(*h.GetZaxis(), val);
        for (int i = 1; i <= nBinsX; i++) {
          for (int j = 1; j <= nBinsY; j++) {
            for (int k = 1; k <= nBinsZ; k++) {
              Double_t old = h.GetBinContent(i, j, k);
              Double_t add = tempCopy->GetBinContent(i, j, foldBinMap[k]);
              h.SetBinContent(i, j, k, old + add);
            }
          }
        }
      }
      h.ResetStats();
      delete tempCopy;
    }

    void SetColor(TH1& h, Color_t color) {
      h.SetLineColor(color);
      h.SetMarkerColor(color);
    }

    void SetColorAndMarker(TH1& h, Color_t color, Marker_t m, Size_t s) {
      h.SetLineColor(color);
      h.SetMarkerColor(color);
      h.SetMarkerStyle(m);
      h.SetMarkerSize(s);
    }

    void MakeBeautiful(TString opt) {
      if (FindParam(opt, "apollo", kTRUE)) {
        gStyle->SetTitleFont(82, "");
        gStyle->SetStatFont(82);
        gStyle->SetLabelFont(82);
        gStyle->SetPalette(kGreyScale);
      }
      if (FindParam(opt, "bold", kTRUE)) {
        gStyle->SetLineWidth(2);
        gStyle->SetFrameLineWidth(2);
      }
      if (opt == "beauty") {
        gStyle->SetPalette(kRainBow);
        TGaxis::SetMaxDigits(3);
        gStyle->SetCanvasPreferGL(kTRUE);
      }
      if (opt == "") {
        gStyle->SetPalette(kRainBow);
        TGaxis::SetMaxDigits(3);
      }
    }

    void SetRainbow(TH2& h, Double_t x1, Double_t y1, Double_t x2, Double_t y2, TString opt) {
      if (!gPad) {
        Hal::Cout::PrintInfo("Hal::Std::SetRainbow, pad not found, did you forgot to draw histogram before? ", EInfo::kError);
        return;
      }
      gPad->Update();
      TPaletteAxis* palette = (TPaletteAxis*) h.GetListOfFunctions()->FindObject("palette");
      if (!palette) {
        Hal::Cout::PrintInfo("Hal::Std::SetRainbow, palette not found, did you forgot to draw histogram before?", EInfo::kError);
        return;
      }
      if (Hal::Std::FindParam(opt, "nc")) {
        palette->SetX1NDC(x1);
        palette->SetX2NDC(x2);
        palette->SetY1NDC(y1);
        palette->SetY2NDC(y2);
      } else {
        palette->SetX1(x1);
        palette->SetX2(x2);
        palette->SetY1(y1);
        palette->SetY2(y2);
      }
      gPad->Modified();
      gPad->Update();
    }

    void MakeEmptyPad(Color_t fill, Color_t line, TVirtualPad* pad) {
      if (!pad) pad = gPad;
      if (!pad) return;
      TH1D* h = new TH1D("dummy_empty", "", 1, 0, 1);
      h->SetMinimum(0);
      h->SetMaximum(0);
      gPad->SetFillColor(fill);
      gPad->SetFrameLineColor(fill);
      h->GetXaxis()->SetLabelColor(fill);
      h->GetXaxis()->SetTitleColor(fill);
      h->GetXaxis()->SetAxisColor(fill);
      h->GetYaxis()->SetLabelColor(fill);
      h->GetYaxis()->SetTitleColor(fill);
      h->GetYaxis()->SetAxisColor(fill);
      gPad->SetLineColor(line);
      h->Draw();
    }

    Int_t GetAntiColor(Int_t col) {
      if (col < 0) return -1;

      // Get list of all defined colors
      TObjArray* colors = (TObjArray*) ::ROOT::GetROOT()->GetListOfColors();
      Int_t ncolors     = colors->GetSize();
      // Get existing color at index n
      TColor* color = nullptr;
      if (col < ncolors) color = (TColor*) colors->At(col);
      if (!color) return -1;

      // Get the rgb of the new bright color corresponding to color n
      Float_t r, g, b;

      color->GetRGB(r, g, b);
      r = 1.0 - r;
      b = 1.0 - b;
      g = 1.0 - g;

      // Build the bright color (unless the slot nb is already used)
      Int_t nb       = col + 150;
      TColor* colorb = nullptr;
      if (nb < ncolors) colorb = (TColor*) colors->At(nb);
      if (colorb) return nb;
      colorb = new TColor(nb, r, g, b);
      colorb->SetName(Form("%s_bright", color->GetName()));
      colors->AddAtAndExpand(colorb, nb);
      return nb;
    }

    Int_t GetListOfSubPads(TVirtualPad* pad) {
      TList* l      = pad->GetListOfPrimitives();
      Int_t subpads = 0;
      for (int i = 0; i < l->GetEntries(); i++) {
        if (dynamic_cast<TVirtualPad*>(l->At(i))) subpads++;
      }
      return subpads;
    }

    TH1D* Crop1D(const TH1& h, Double_t min, Double_t max, TString option) {
      std::pair<int, int> binsX;
      std::pair<double, double> valsX;
      Int_t nBins = 0;
      CropAxis(h.GetXaxis(), nBins, binsX, valsX, min, max, option);

      TH1D* nh  = new TH1D(h.GetName(), h.GetTitle(), nBins, valsX.first, valsX.second);
      int count = 0;
      for (int i = binsX.first; i <= binsX.second; i++) {
        nh->SetBinContent(++count, h.GetBinContent(i));
        nh->SetBinError(count, h.GetBinError(i));
      }
      return nh;
    }

    TH2D* Crop2D(const TH2& h, Double_t minX, Double_t maxX, Double_t minY, Double_t maxY, TString option) {
      Int_t nBinsX, nBinsY;
      std::pair<int, int> binsX, binsY;
      std::pair<double, double> valsX, valsY;
      CropAxis(h.GetXaxis(), nBinsX, binsX, valsX, minX, maxX, option);
      CropAxis(h.GetYaxis(), nBinsY, binsY, valsY, minY, maxY, option);
      TH2D* nh   = new TH2D(h.GetName(), h.GetTitle(), nBinsX, valsX.first, valsX.second, nBinsY, valsY.first, valsY.second);
      int countX = 0;
      int countY = 0;
      for (int i = binsX.first; i <= binsX.second; i++) {
        ++countX;
        for (int j = binsY.first; j <= binsY.second; j++) {
          ++countY;
          nh->SetBinContent(countX, countY, h.GetBinContent(i, j));
          nh->SetBinError(countX, countY, h.GetBinError(i, j));
        }
      }
      return nh;
    }

    TH3D* Crop3D(const TH3& h,
                 Double_t minX,
                 Double_t maxX,
                 Double_t minY,
                 Double_t maxY,
                 Double_t minZ,
                 Double_t maxZ,
                 TString option) {
      std::pair<int, int> X, Y, Z;
      std::pair<double, double> valsX, valsY, valsZ;

      Int_t nBinsX, nBinsY, nBinsZ;
      CropAxis(h.GetXaxis(), nBinsX, X, valsX, minX, maxX, option);
      CropAxis(h.GetYaxis(), nBinsY, Y, valsY, minY, maxY, option);
      CropAxis(h.GetZaxis(), nBinsZ, Z, valsZ, minZ, maxZ, option);
      TH3D* nh   = new TH3D(h.GetName(),
                          h.GetTitle(),
                          nBinsX,
                          valsX.first,
                          valsX.second,
                          nBinsY,
                          valsY.first,
                          valsY.second,
                          nBinsZ,
                          valsZ.first,
                          valsZ.second);
      int countX = 0;
      int countY = 0;
      int countZ = 0;
      for (int i = X.first; i <= X.second; i++) {
        ++countX;
        countY = 0;
        for (int j = Y.first; j <= Y.second; j++) {
          ++countY;
          countZ = 0;
          for (int k = Z.first; k <= Z.second; k++) {
            ++countZ;
            nh->SetBinContent(countX, countY, countZ, h.GetBinContent(i, j, k));
            nh->SetBinError(countX, countY, countZ, h.GetBinError(i, j, k));
          }
        }
      }
      return nh;
    }

    std::vector<TObject*> GetPadChildren(TString objName, TString className, TVirtualPad* pad) {
      if (!pad) pad = gPad;
      std::vector<TObject*> list;
      if (!pad) return list;
      TList* l = gPad->GetListOfPrimitives();
      for (int i = 0; i < l->GetEntries(); i++) {
        auto obj             = l->At(i);
        TString classNameObj = obj->ClassName();
        TString objectName   = obj->GetName();
        Bool_t objNameSame   = ((objName == objectName) || objName.Length() == 0);
        Bool_t classNameSame = (className == classNameObj || className.Length() == 0);
        if (objNameSame && classNameSame) { list.push_back(obj); }
      }
      return list;
    }

    Bool_t CheckHistogramData(const TH1& h1, const TH1& h2, Double_t thres, Option_t* opt) {
      TString option  = opt;
      Bool_t debug    = Hal::Std::FindParam(option, "print", kTRUE);
      Bool_t skiperr  = Hal::Std::FindParam(option, "skiperr", kTRUE);
      Bool_t relative = Hal::Std::FindParam(option, "rel", kTRUE);
      if (!AreSimilar(&h1, &h2, kFALSE)) {
        if (debug) std::cout << __FILE__ << __LINE__ << " histograms are not similiar" << std::endl;
        return kFALSE;
      }
      auto compare = [&](double x1, double e1, double x2, double e2) {
        if (relative) {
          double relval = TMath::Abs((x1 - x2) / x1);
          if (relval > thres) return kFALSE;
        } else {
          if (TMath::Abs(x1 - x2) > thres) return kFALSE;
        }

        if (skiperr) return kTRUE;
        if (relative) {
          double relval = TMath::Abs((e1 - e2) / e1);
          if (relval > thres) return kFALSE;
        } else {
          if (TMath::Abs(e1 - e2) > thres) return kFALSE;
        }
        return kTRUE;
      };
      if (dynamic_cast<const TH3*>(&h1)) {
        auto H1 = static_cast<const TH3*>(&h1);
        auto H2 = static_cast<const TH3*>(&h2);
        for (int i = 1; i <= H1->GetNbinsX(); i++) {
          for (int j = 1; j <= H1->GetNbinsY(); j++) {
            for (int k = 1; k <= H1->GetNbinsZ(); k++) {
              bool compared = compare(
                H1->GetBinContent(i, j, k), H1->GetBinError(i, j, k), H2->GetBinContent(i, j, k), H2->GetBinError(i, j, k));
              if (!compared) {
                if (debug) { std::cout << "Detected difference at bin " << i << " " << j << " " << k << std::endl; }
                return kFALSE;
              }
            }
          }
        }
      } else if (dynamic_cast<const TH2*>(&h1)) {
        auto H1 = static_cast<const TH2*>(&h1);
        auto H2 = static_cast<const TH2*>(&h2);
        for (int i = 1; i <= H1->GetNbinsX(); i++) {
          for (int j = 1; j <= H1->GetNbinsY(); j++) {
            bool compared =
              compare(H1->GetBinContent(i, j), H1->GetBinError(i, j), H2->GetBinContent(i, j), H2->GetBinError(i, j));
            if (!compared) {
              if (debug) { std::cout << "Detected difference at bin " << i << " " << j << " " << std::endl; }
              return kFALSE;
            }
          }
        }
      } else {
        auto H1 = static_cast<const TH1*>(&h1);
        auto H2 = static_cast<const TH1*>(&h2);
        for (int i = 1; i <= H1->GetNbinsX(); i++) {
          bool compared = compare(H1->GetBinContent(i), H1->GetBinError(i), H2->GetBinContent(i), H2->GetBinError(i));
          if (!compared) {
            if (debug) { std::cout << "Detected difference at bin " << i << " " << std::endl; }
            return kFALSE;
          }
        }
      }
      return kTRUE;
    }

    void CopyHistProp(const TH1& from, TH1& to, TString opt) {
      auto d3 = static_cast<const TH3*>(&from);
      CopyAxisProp(from.GetXaxis(), to.GetXaxis(), opt);
      CopyAxisProp(from.GetYaxis(), to.GetYaxis(), opt);
      if (d3) CopyAxisProp(from.GetZaxis(), to.GetZaxis(), opt);
      to.SetMarkerSize(from.GetMarkerSize());
      to.SetMarkerStyle(from.GetMarkerStyle());
      to.SetMarkerColor(from.GetMarkerColor());
      to.SetLineStyle(from.GetLineStyle());
      to.SetLineWidth(from.GetLineWidth());
      to.SetLineColor(from.GetLineColor());
      to.SetFillColor(from.GetFillColor());
      to.SetFillStyle(from.GetFillStyle());
    }

    Double_t GetMinimum(const std::vector<TH1*> histos, Bool_t underflow, Bool_t overflow) {
      Double_t minig = 1E+10;
      auto findMin   = [](TH1* x, Bool_t underFlow, Bool_t overFlow) {
        Double_t mini = 1E+10;
        TH1* h1       = dynamic_cast<TH1*>(x);
        TH1* h2       = dynamic_cast<TH2*>(x);
        TH1* h3       = dynamic_cast<TH3*>(x);
        int start     = 1;
        if (underFlow) start = 0;
        int endx = x->GetNbinsX();
        int endy = x->GetNbinsY();
        int endz = x->GetNbinsZ();
        if (overFlow) {
          endx++;
          endy++;
          endz++;
        }
        if (h3) {
          for (int i = start; i <= endx; i++)
            for (int j = start; j <= endy; j++)
              for (int k = start; k <= endz; k++)
                mini = TMath::Min(h3->GetBinContent(i, j, k), mini);
        } else if (h2) {
          for (int i = start; i <= endx; i++)
            for (int j = start; j <= endy; j++)
              mini = TMath::Min(h2->GetBinContent(i, j), mini);
        } else {
          for (int i = start; i <= endx; i++)
            mini = TMath::Min(h1->GetBinContent(i), mini);
        }
        return mini;
      };

      for (auto x : histos) {
        minig = TMath::Min(minig, findMin(x, underflow, overflow));
      }
      return minig;
    }

    Double_t GetMaximum(const std::vector<TH1*> histos, Bool_t underflow, Bool_t overflow) {
      Double_t maxig = -1E+10;
      auto findMax   = [](TH1* x, Bool_t underFlow, Bool_t overFlow) {
        Double_t maxi = -1E+10;
        TH1* h1       = dynamic_cast<TH1*>(x);
        TH1* h2       = dynamic_cast<TH2*>(x);
        TH1* h3       = dynamic_cast<TH3*>(x);
        int start     = 1;
        if (underFlow) start = 0;
        int endx = x->GetNbinsX();
        int endy = x->GetNbinsY();
        int endz = x->GetNbinsZ();
        if (overFlow) {
          endx++;
          endy++;
          endz++;
        }
        if (h3) {
          for (int i = start; i <= endx; i++)
            for (int j = start; j <= endy; j++)
              for (int k = start; k <= endz; k++)
                maxi = TMath::Max(h3->GetBinContent(i, j, k), maxi);
        } else if (h2) {
          for (int i = start; i <= endx; i++)
            for (int j = start; j <= endy; j++)
              maxi = TMath::Max(h2->GetBinContent(i, j), maxi);
        } else {
          for (int i = start; i <= endx; i++)
            maxi = TMath::Max(h1->GetBinContent(i), maxi);
        }
        return maxi;
      };

      for (auto x : histos) {
        maxig = TMath::Max(maxig, findMax(x, underflow, overflow));
      }
      return maxig;
    }

    void HideAxisLabel(TObject* obj, TString opt) {
      opt.ToLower();
      opt      = opt.ReplaceAll(" ", "");
      auto vec = ExplodeString(opt, '+', kFALSE);
      if (vec.size() > 1) {
        for (auto str : vec) {
          HideAxisLabel(obj, str);
        }
        return;
      }

      Bool_t xAx = opt.Contains("x");
      Bool_t yAx = opt.Contains("y");
      Bool_t zAx = opt.Contains("z");
      std::vector<TAxis*> ax;
      auto hist  = dynamic_cast<TH1*>(obj);
      auto graph = dynamic_cast<TGraph*>(obj);
      auto axis  = dynamic_cast<TAxis*>(obj);
      if (!hist && !graph && !axis) {
        Cout::PrintInfo("Hal::Std::HideAxisLabel - object is not a histo or a graph", EInfo::kWarning);
        return;
      }
      if (hist) {
        if (xAx) ax.push_back(hist->GetXaxis());
        if (yAx) ax.push_back(hist->GetYaxis());
        if (zAx && hist->GetZaxis()) ax.push_back(hist->GetZaxis());
      } else if (graph) {
        if (xAx) ax.push_back(graph->GetXaxis());
        if (yAx) ax.push_back(graph->GetYaxis());
      } else if (axis) {
        ax.push_back(axis);
      }
      Bool_t low  = opt.Contains("l");
      Bool_t high = opt.Contains("h");
      for (auto iAxis : ax) {
        if (low) { iAxis->ChangeLabel(1, -1, -1, -1, kWhite, 0, " "); }
        if (high) { iAxis->ChangeLabel(-1, -1, -1, -1, kWhite, 0, " "); }
      }
    }

    std::vector<Double_t> ChebyshevInterpolation(const TH1D& histo, Int_t n, Double_t low, Double_t high) {
      if (low == high) {
        low  = histo.GetXaxis()->GetBinLowEdge(0);
        high = histo.GetXaxis()->GetBinUpEdge(histo.GetNbinsX());
      }

      auto vector = Hal::Std::TH1ToPair(histo);
      return ChebyshevInterpolation(vector, n, low, high);
    }

    std::vector<Hal::Std::Triplet<Double_t>> TH1ToTriplets(const TH1& histo) {
      std::vector<Triplet<Double_t>> res;
      for (int i = 1; i <= histo.GetNbinsX(); i++) {
        Triplet<Double_t> triplet;
        triplet.x = histo.GetXaxis()->GetBinCenter(i);
        triplet.y = histo.GetBinContent(i);
        triplet.z = histo.GetBinError(i);
        res.push_back(triplet);
      }
      return res;
    }

    std::vector<std::pair<Double_t, Double_t>> TH1ToPair(const TH1& histo, TString opt) {
      std::vector<std::pair<Double_t, Double_t>> res;
      std::pair<Double_t, Double_t> el;
      int binLo = 1;
      int binHi = histo.GetNbinsX();
      if (Hal::Std::FindParam(opt, "u")) { binLo = 0; }
      if (Hal::Std::FindParam(opt, "o")) { binHi++; }
      if (Hal::Std::FindParam(opt, "xy")) {
        for (int i = binLo; i <= binHi; i++) {
          el.first  = histo.GetXaxis()->GetBinCenter(i);
          el.second = histo.GetBinContent(i);
          res.push_back(el);
        }
      } else if (Hal::Std::FindParam(opt, "ye")) {
        for (int i = binLo; i <= binHi; i++) {
          el.first  = histo.GetBinContent(i);
          el.second = histo.GetBinError(i);
          res.push_back(el);
        }
      } else {
        Hal::Cout::PrintInfo(Form("TH1ToPair cannot recognize option %s", opt.Data()), EInfo::kError);
      }
      return res;
    }

    void DrawDiagonalBins(const TH2& sample, Double_t x, Double_t y, TString opt, Color_t color, Int_t width) {
      Double_t x1 = sample.GetXaxis()->GetBinLowEdge(sample.GetXaxis()->FindBin(x));
      Double_t y1 = sample.GetYaxis()->GetBinLowEdge(sample.GetYaxis()->FindBin(y));
      Double_t x2 = sample.GetXaxis()->GetBinUpEdge(sample.GetXaxis()->FindBin(x));
      Double_t y2 = sample.GetYaxis()->GetBinUpEdge(sample.GetYaxis()->FindBin(y));
      std::vector<TLine*> lines;
      lines.push_back(new TLine(x1, y1, x1, y2));
      lines.push_back(new TLine(x2, y1, x2, y2));
      lines.push_back(new TLine(x1, y1, x2, y1));
      lines.push_back(new TLine(x1, y2, x2, y2));
      if (Hal::Std::FindParam(opt, "x")) {
        lines.push_back(new TLine(x1, y1, x2, y2));
        lines.push_back(new TLine(x1, y2, x2, y1));
      } else if (Hal::Std::FindParam(opt, "l")) {
        lines.push_back(new TLine(x1, y1, x2, y2));
      } else if (Hal::Std::FindParam(opt, "r")) {
        lines.push_back(new TLine(x1, y2, x2, y1));
      }
      for (auto line : lines) {
        line->SetLineColor(color);
        line->SetLineWidth(width);
        line->Draw("SAME");
      }
    }

    void NormalizeHistogram(TH1& h, TString option) {
      enum class eCalcErrorMode { kSqrt, kNoRecalc, kScale };
      eCalcErrorMode mode = eCalcErrorMode::kScale;
      if (Hal::Std::FindParam(option, "keepe", kTRUE)) mode = eCalcErrorMode::kNoRecalc;
      if (Hal::Std::FindParam(option, "sqrt", kTRUE)) mode = eCalcErrorMode::kSqrt;
      const Int_t ndim          = h.InheritsFrom("TH3") ? 3 : h.InheritsFrom("TH2") ? 2 : 1;
      const Bool_t useUnderFlow = Hal::Std::FindParam(option, "u", kTRUE);
      const Bool_t useOverFlow  = Hal::Std::FindParam(option, "o", kTRUE);

      auto CalcParams = [&](std::pair<Double_t, Double_t>& res, Double_t invsum) {
        res.first = res.first * invsum;
        switch (mode) {
          case eCalcErrorMode::kNoRecalc: {  // do nothing
            return;
          } break;
          case eCalcErrorMode::kSqrt: {
            res.second = TMath::Sqrt(res.first);
          } break;
          case eCalcErrorMode::kScale: {
            res.second = res.second * invsum;
          } break;
        }
      };

      auto GetSetBinContent1d = [&](Double_t invsum, int x) {
        std::pair<Double_t, Double_t> res;
        res.first  = h.GetBinContent(x);
        res.second = h.GetBinError(x);
        CalcParams(res, invsum);
        h.SetBinContent(x, res.first);
        h.SetBinError(x, res.second);
      };
      auto GetSetBinContent2d = [&](Double_t invsum, int x, int y) {
        std::pair<Double_t, Double_t> res;
        res.first  = h.GetBinContent(x, y);
        res.second = h.GetBinError(x, y);
        CalcParams(res, invsum);
        h.SetBinContent(x, y, res.first);
        h.SetBinError(x, y, res.second);
      };

      auto GetSetBinContent3d = [&](Double_t invsum, int x, int y, int z) {
        std::pair<Double_t, Double_t> res;
        res.first  = h.GetBinContent(x, y, z);
        res.second = h.GetBinError(x, y, z);
        CalcParams(res, invsum);
        h.SetBinContent(x, y, z, res.first);
        h.SetBinError(x, y, z, res.second);
      };


      const Int_t start[] = {useUnderFlow ? 0 : 1, useUnderFlow ? 0 : 1, useUnderFlow ? 0 : 1};
      const Int_t end[]   = {useOverFlow ? h.GetXaxis()->GetNbins() + 1 : h.GetXaxis()->GetNbins(),
                           useOverFlow ? h.GetYaxis()->GetNbins() + 1 : h.GetYaxis()->GetNbins(),
                           useOverFlow ? h.GetZaxis()->GetNbins() + 1 : h.GetZaxis()->GetNbins()};
      switch (ndim) {
        case 1: {
          Double_t sum = GetSumByBin(h, start[0], end[0]);
          if (sum == 0) return;
          const Double_t invSum = 1.0 / sum;
          for (int x = start[0]; x <= end[0]; x++)
            GetSetBinContent1d(invSum, x);
        } break;
        case 2: {
          if (Hal::Std::FindParam(option, "x", true)) {
            for (int y = 0; y <= h.GetNbinsY() + 1; y++) {
              Double_t sum = GetSumByBin(h, start[0], end[0], y, y);
              if (sum == 0) continue;  // do not normalize
              const Double_t invSum = 1.0 / sum;
              for (int x = start[0]; x <= end[0]; x++) {
                GetSetBinContent2d(invSum, x, y);
              }
            }

          } else if (Hal::Std::FindParam(option, "y", true)) {
            for (int x = 0; x <= h.GetNbinsX() + 1; x++) {
              Double_t sum = GetSumByBin(h, x, x, start[1], end[1]);
              if (sum == 0) continue;  // do not normalize
              const Double_t invSum = 1.0 / sum;
              for (int y = start[1]; y <= end[1]; y++) {
                GetSetBinContent2d(invSum, x, y);
              }
            }
          } else if (Hal::Std::FindParam(option, "xy", true)) {
            Double_t sum = GetSumByBin(h, start[0], end[0], start[1], end[1]);
            if (sum == 0) return;  // do not normalize
            const Double_t invSum = 1.0 / sum;
            for (int x = start[0]; x <= start[0]; x++) {
              for (int y = start[1]; y <= start[1]; y++) {
                GetSetBinContent2d(invSum, x, y);
              }
            }
          }
        } break;
        case 3: {
          if (Hal::Std::FindParam(option, "xyz", true)) {
            Double_t sum = GetSumByBin(h, start[0], end[0], start[1], end[1], start[2], end[2]);
            if (sum == 0) return;
            const Double_t invSum = 1.0 / sum;
            for (int x = start[0]; x <= end[0]; x++) {
              for (int y = start[1]; y <= end[1]; y++) {
                for (int z = start[2]; z <= end[2]; z++) {
                  GetSetBinContent3d(invSum, x, y, z);
                }
              }
            }

          } else if (Hal::Std::FindParam(option, "xy", true) || Hal::Std::FindParam(option, "yx", true)) {
            for (int z = 0; z <= h.GetNbinsZ(); z++) {
              Double_t sum = GetSumByBin(h, start[0], end[0], start[1], end[1], z, z);
              if (sum == 0) continue;
              const Double_t invSum = 1.0 / sum;
              for (int x = start[0]; x <= end[0]; x++) {
                for (int y = start[1]; y <= end[1]; y++) {
                  GetSetBinContent3d(invSum, x, y, z);
                }
              }
            }
          } else if (Hal::Std::FindParam(option, "xz", true) || Hal::Std::FindParam(option, "zx", true)) {
            for (int y = 0; y <= h.GetNbinsY(); y++) {
              Double_t sum = GetSumByBin(h, start[0], end[0], y, y, start[2], end[2]);
              if (sum == 0) continue;
              const Double_t invSum = 1.0 / sum;
              for (int x = start[0]; x <= end[0]; x++) {
                for (int z = start[2]; z <= end[2]; z++) {
                  GetSetBinContent3d(invSum, x, y, z);
                }
              }
            }

          } else if (Hal::Std::FindParam(option, "yz", true) || Hal::Std::FindParam(option, "yz", true)) {
            for (int x = 0; x <= h.GetNbinsX(); x++) {
              Double_t sum = GetSumByBin(h, x, x, start[1], end[1], start[2], end[2]);
              if (sum == 0) continue;
              const Double_t invSum = 1.0 / sum;
              for (int y = start[1]; y <= end[1]; y++) {
                for (int z = start[2]; z < end[2]; z++) {
                  GetSetBinContent3d(invSum, x, y, z);
                }
              }
            }
          } else if (Hal::Std::FindParam(option, "x", true)) {
            for (int y = 0; y <= h.GetNbinsY(); y++) {
              for (int z = 0; z <= h.GetNbinsZ(); z++) {
                Double_t sum = GetSumByBin(h, start[0], end[0], y, y, z, z);
                if (sum == 0) continue;
                const Double_t invSum = 1.0 / sum;
                for (int x = start[0]; x <= end[0]; x++) {
                  GetSetBinContent3d(invSum, x, y, z);
                }
              }
            }
          } else if (Hal::Std::FindParam(option, "y", true)) {
            for (int x = 0; x <= h.GetNbinsX(); x++) {
              for (int z = 0; z <= h.GetNbinsZ(); z++) {
                Double_t sum = GetSumByBin(h, x, x, start[1], end[1], z, z);
                if (sum == 0) continue;
                const Double_t invSum = 1.0 / sum;
                for (int y = start[1]; y <= end[1]; y++) {
                  GetSetBinContent3d(invSum, x, y, z);
                }
              }
            }

          } else if (Hal::Std::FindParam(option, "z", true)) {
            for (int x = 0; x <= h.GetNbinsX(); x++) {
              for (int y = 0; y <= h.GetNbinsY(); y++) {
                Double_t sum = GetSumByBin(h, x, y, start[2], end[2]);
                if (sum == 0) continue;
                const Double_t invSum = 1.0 / sum;
                for (int z = start[2]; z <= end[2]; z++) {
                  GetSetBinContent3d(invSum, x, y, z);
                }
              }
            }
          }
        } break;
      }
    }

    Double_t GetSumByBin(const TH1& x, Int_t start_x, Int_t end_x, Int_t start_y, Int_t end_y, Int_t start_z, Int_t end_z) {
      const Int_t ndim = dynamic_cast<const TH3*>(&x) ? 3 : dynamic_cast<const TH2*>(&x) ? 2 : 1;
      Double_t sum     = 0;
      switch (ndim) {
        case 1: {
          for (int ix = start_x; ix <= end_x; ix++) {
            sum += x.GetBinContent(ix);
          }
        } break;
        case 2: {
          for (int ix = start_x; ix <= end_x; ix++) {
            for (int iy = start_y; iy <= end_y; iy++) {
              sum += x.GetBinContent(ix, iy);
            }
          }
        } break;
        case 3: {
          for (int ix = start_x; ix <= end_x; ix++) {
            for (int iy = start_y; iy <= end_y; iy++) {
              for (int iz = start_z; iz <= end_z; iz++) {
                sum += x.GetBinContent(ix, iy, iz);
              }
            }
          }
        } break;
        default: sum = -1; break;
      }
      return sum;
    }

    Double_t GetSumByVals(const TH1& x, Double_t sx, Double_t ex, Double_t sy, Double_t ey, Double_t sz, Double_t ez) {
      const Int_t ndim = dynamic_cast<const TH3*>(&x) ? 3 : dynamic_cast<const TH2*>(&x) ? 2 : 1;
      const TAxis* X   = x.GetXaxis();
      const TAxis* Y   = x.GetYaxis();
      const TAxis* Z   = x.GetZaxis();
      switch (ndim) {
        case 1: return GetSumByBin(x, X->FindBin(sx), X->FindBin(ex)); break;
        case 2: return GetSumByBin(x, X->FindBin(sz), X->FindBin(ex), Y->FindBin(sy), Y->FindBin(ey)); break;
        case 3:
          return GetSumByBin(x, X->FindBin(sx), X->FindBin(ex), Y->FindBin(sy), Y->FindBin(ey), Z->FindBin(sz), Z->FindBin(ez));
          break;
      }
      return -1;
    }

    Double_t GetTrimmedMean(const TH1& h, Double_t threshold, Bool_t underover) {
      std::vector<double> values;
      int startBin = 1;
      int endBinX  = h.GetNbinsX();
      int endBinY  = h.GetNbinsY();
      int endBinZ  = h.GetNbinsZ();
      if (underover) {
        startBin = 0;
        ++endBinX;
        ++endBinY;
        ++endBinZ;
      }
      if (h.GetDimension() == 3) {
        for (int i = startBin; i <= endBinX; i++) {
          for (int j = startBin; j <= endBinY; j++) {
            for (int k = startBin; k <= endBinZ; k++) {
              values.push_back(h.GetBinContent(i, j, k));
            }
          }
        }
      } else if (h.GetDimension() == 2) {
        for (int i = startBin; i <= endBinX; i++) {
          for (int j = startBin; j <= endBinY; j++) {
            values.push_back(h.GetBinContent(i, j));
          }
        }
      } else {
        for (int i = startBin; i <= endBinX; i++) {
          values.push_back(h.GetBinContent(i));
        }
      }
      std::sort(values.begin(), values.end());

      int n        = values.size();
      int cut      = static_cast<int>(threshold * n);
      double count = 0, sum = 0;
      for (int i = cut; i < n - cut * 2; i++) {
        count++;
        sum += values[i];
      }
      return sum / count;
    }


    Bool_t TestHistogram(const TH1& h1,
                         const TH1& h2,
                         Bool_t axes,
                         Bool_t names,
                         Bool_t values,
                         Bool_t errors,
                         Bool_t overflow,
                         Double_t thres,
                         Bool_t print) {
      if (h1.IsA() != h2.IsA()) return kFALSE;

      if (h1.GetDimension() != h2.GetDimension()) return kFALSE;

      if (axes || names) {
        auto testAxis = [&](const TAxis* a1, const TAxis* a2) -> Bool_t {
          if (axes) {
            if (a1->GetNbins() != a2->GetNbins()) return kFALSE;
            if (a1->GetXmin() != a2->GetXmin()) return kFALSE;
            if (a1->GetXmax() != a2->GetXmax()) return kFALSE;
          }
          if (names) {
            if (strcmp(a1->GetTitle(), a2->GetTitle()) != 0) return kFALSE;
          }
          return kTRUE;
        };

        if (!testAxis(h1.GetXaxis(), h2.GetXaxis())) return kFALSE;
        if (h1.GetDimension() >= 2)
          if (!testAxis(h1.GetYaxis(), h2.GetYaxis())) return kFALSE;
        if (h1.GetDimension() >= 3)
          if (!testAxis(h1.GetZaxis(), h2.GetZaxis())) return kFALSE;
      }

      int startBin = 1;
      int endBinX  = h1.GetNbinsX();
      int endBinY  = h1.GetNbinsY();
      int endBinZ  = h1.GetNbinsZ();
      if (overflow) {
        startBin = 0;
        ++endBinX;
        ++endBinY;
        ++endBinZ;
      }
      auto check = [&](double a, double ae, double b, double be, std::vector<int> pos) {
        if (values && (TMath::Abs(a - b) > thres)) {
          if (print) {
            std::cout << "Different values [";
            for (auto i : pos) {
              std::cout << i << " ";
            }
            std::cout << "] " << a << " vs " << b << std::endl;
          }
          return false;
        }
        if (errors && (TMath::Abs(ae - be) > thres)) {
          if (print) {
            std::cout << "Different errors [";
            for (auto i : pos) {
              std::cout << i << " ";
            }
            std::cout << "] " << ae << " vs " << be << std::endl;
          }
          return false;
        }
        return true;
      };


      if (h1.GetDimension() == 3) {
        for (int i = startBin; i <= endBinX; i++) {
          for (int j = startBin; j <= endBinY; j++) {
            for (int k = startBin; k <= endBinZ; k++) {
              double a  = h1.GetBinContent(i, j, k);
              double b  = h2.GetBinContent(i, j, k);
              double ae = h1.GetBinError(i, j, k);
              double be = h2.GetBinError(i, j, k);
              if (!check(a, ae, b, be, {i, j, k})) return kFALSE;
            }
          }
        }
      } else if (h1.GetDimension() == 2) {
        for (int i = startBin; i <= endBinX; i++) {
          for (int j = startBin; j <= endBinY; j++) {
            double a  = h1.GetBinContent(i, j);
            double b  = h2.GetBinContent(i, j);
            double ae = h1.GetBinError(i, j);
            double be = h2.GetBinError(i, j);
            if (!check(a, ae, b, be, {i, j})) return kFALSE;
          }
        }
      } else {
        for (int i = startBin; i <= endBinX; i++) {
          double a  = h1.GetBinContent(i);
          double b  = h2.GetBinContent(i);
          double ae = h1.GetBinError(i);
          double be = h2.GetBinError(i);
          if (!check(a, ae, b, be, {i})) return kFALSE;
        }
      }
      return kTRUE;
    }
  }  // namespace Std
}  // namespace Hal
