/*
 * Hal::StdHist.cxx
 *
 *  Created on: 24 lut 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "StdHist.h"
#include "StdString.h"


#include <TArray.h>
#include <TAxis.h>
#include <TCollection.h>
#include <TColor.h>
#include <TDirectory.h>
#include <TGaxis.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TList.h>
#include <TMath.h>
#include <TMatrixDfwd.h>
#include <TMatrixT.h>
#include <TNamed.h>
#include <TObjArray.h>
#include <TPaletteAxis.h>
#include <TROOT.h>
#include <TRandom.h>
#include <TStyle.h>
#include <TVirtualPad.h>
#include <iostream>
#include <utility>

#include "Cout.h"
#include "Splines.h"
#include "Std.h"

NamespaceImp(Hal::Std);
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
        if (option.Contains("autoname")) name = Form("%i_pz", gDirectory->GetList()->GetEntries() + 1);
        if (option.Contains("randname")) name = Form("pz_%i", (int) (gRandom->Rndm() * 1E+6));
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
        if (option.Contains("autoname")) name = Form("%i_py", gDirectory->GetList()->GetEntries() + 1);
        if (option.Contains("randname")) name = Form("py_%i", (int) (gRandom->Rndm() * 1E+6));
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
        if (option.Contains("autoname")) name = Form("%i_px", gDirectory->GetList()->GetEntries() + 1);
        if (option.Contains("randname")) name = Form("px_%i", (int) (gRandom->Rndm() * 1E+6));
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
    };

    TH1D* GetProjection1D(const TH2* histo, Double_t min, Double_t max, Option_t* opt) {
      TString option = opt;
      TH1D* projection;
      Bool_t sumw    = Hal::Std::FindParam(option, "sumw", kTRUE);
      Double_t nbins = 0;
      if (option.Contains("y")) {
        TString name = "_py";
        if (option.Contains("autoname")) name = Form("%i_py", gDirectory->GetList()->GetEntries() + 1);
        if (option.Contains("randname")) name = Form("py_%i", (int) (gRandom->Rndm() * 1E+6));
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
        if (option.Contains("autoname")) name = Form("%i_px", gDirectory->GetList()->GetEntries() + 1);
        if (option.Contains("randname")) name = Form("px_%i", (int) (gRandom->Rndm() * 1E+6));
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
    };

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
        axis->SetRangeUser(min, max);
        nbins = 1 + axis->FindBin(max) - axis->FindBin(min);
      }
      projection = (TH2D*) histo_cloned->Project3D(projection_option);
      delete histo_cloned;
      axis->SetRange(0, 0);
      if (sumw) projection->Sumw2();
      if (option.Contains("scale")) { projection->Scale(1.0 / nbins); }
      projection->SetDirectory(0);
      return projection;
    };

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
    };

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
    };

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

    TH1D* GetDiagonalProjection1D(TH3* h, TString dir, Double_t start, Double_t start2) {
      if (h->GetNbinsX() != h->GetNbinsY() || h->GetNbinsX() != h->GetNbinsZ()) {
        Hal::Cout::PrintInfo("Cannto make projection in nonsymetric histo", Hal::EInfo::kLowWarning);
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

    void MakeBeautiful() {
      gStyle->SetPalette(kRainBow);
      TGaxis::SetMaxDigits(3);
      gStyle->SetCanvasPreferGL(kTRUE);
    }

    void SetRainbow(TH2& h, Double_t x1, Double_t y1, Double_t x2, Double_t y2) {
      gPad->Update();
      TPaletteAxis* palette = (TPaletteAxis*) h.GetListOfFunctions()->FindObject("palette");
      palette->SetX1NDC(x1);
      palette->SetX2NDC(x2);
      palette->SetY1NDC(y1);
      palette->SetY2NDC(y2);
      gPad->Modified();
      gPad->Update();
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
  }  // namespace Std
}  // namespace Hal
