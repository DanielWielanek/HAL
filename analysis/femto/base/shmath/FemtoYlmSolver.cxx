/*
 * FemtoYlmSolver.cxx
 *
 *  Created on: 24 sie 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoYlmSolver.h"

#include "Cout.h"
#include "FemtoSHCF.h"
#include "FemtoSHSlice.h"
#include <TH1.h>
#include <TMath.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_cblas.h>
#include <gsl/gsl_matrix_double.h>
#include <gsl/gsl_vector_double.h>
#include <iostream>

//#define _FINISH_DEBUG_
namespace Hal {

  FemtoYlmSolver::FemtoYlmSolver() {}

  FemtoYlmSolver::~FemtoYlmSolver() {}

  void FemtoYlmSolver::GetMtilde(std::complex<double>* aMat, double* aMTilde) {
    // Create the Mtilde for a given q bin
    double lzero, mzero;
    double lprim, mprim;
    double lbis, mbis;

    int lzeroi, mzeroi;
    int lprimi, mprimi;
    int lbisi, mbisi;

    for (int iz = 0; iz < GetMaxJM() * 2; iz++)
      for (int ip = 0; ip < GetMaxJM() * 2; ip++)
        aMTilde[iz * GetMaxJM() * 2 + ip] = 0.0;

    for (int izero = 0; izero < GetMaxJM(); izero++) {
      GetElEmForIndex(izero, lzero, mzero);
      GetElEmForIndex(izero, lzeroi, mzeroi);
      //     if (mzero < 0)
      //       continue;
      for (int ibis = 0; ibis < GetMaxJM(); ibis++) {
        GetElEmForIndex(ibis, lbis, mbis);
        GetElEmForIndex(ibis, lbisi, mbisi);

        //       if (mbis<0) continue;

        std::complex<double> val = std::complex<double>(0.0, 0.0);
        std::unique_ptr<std::complex<double>[]> mcomp(new std::complex<double>[fMaxJM]());
        for (int iprim = 0; iprim < GetMaxJM(); iprim++) {
          GetElEmForIndex(iprim, lprim, mprim);
          GetElEmForIndex(iprim, lprimi, mprimi);

          //    if (mprim < 0 ) continue;

          if (abs(mzeroi) % 2)
            mcomp[iprim] = std::complex<double>(-1.0, 0.0);  // (-1)^m
          else
            mcomp[iprim] = std::complex<double>(1.0, 0.0);

          mcomp[iprim] *= sqrt((2 * lzero + 1) * (2 * lprim + 1) * (2 * lbis + 1));  // P1
          mcomp[iprim] *= fLmMath.WignerSymbol(lzero, 0, lprim, 0, lbis, 0);         // W1
          mcomp[iprim] *= fLmMath.WignerSymbol(lzero, -mzero, lprim, mprim, lbis,
                                               mbis);  // W2
          mcomp[iprim] *= aMat[iprim];
          //    if (
          val += mcomp[iprim];
        }

        aMTilde[(izero * 2) * (2 * GetMaxJM()) + (ibis * 2)]     = real(val);
        aMTilde[(izero * 2 + 1) * (2 * GetMaxJM()) + (ibis * 2)] = imag(val);
        if (imag(val) != 0.0)
          aMTilde[(izero * 2) * (2 * GetMaxJM()) + (ibis * 2 + 1)] = -imag(val);
        else
          aMTilde[(izero * 2) * (2 * GetMaxJM()) + (ibis * 2 + 1)] = 0.0;
        aMTilde[(izero * 2 + 1) * (2 * GetMaxJM()) + (ibis * 2 + 1)] = real(val);
      }
    }
  }

  void FemtoYlmSolver::InvertYlmIndependentMatrix(double* inmat, double* outmat) const {
    // Invert the Ylm matrix by inverting only the matrix
    // with independent elements and filling in the rest
    // according to sign rules

    std::unique_ptr<double[]> mU(new double[GetMaxJM() * GetMaxJM() * 4]);
    int isize = PackYlmMatrixIndependentOnly(inmat, mU.get());
    //  cout << "Independent count " << isize << std::endl;
#ifndef DISABLE_GSL
    gsl_matrix_view matU = gsl_matrix_view_array(mU.get(), isize, isize);
#endif
    // Identity matrix helper for inversion
    std::unique_ptr<double[]> mI(new double[GetMaxJM() * GetMaxJM() * 4]);
    for (int iterm = 0; iterm < isize; iterm++)
      for (int iterp = 0; iterp < isize; iterp++)
        if (iterm == iterp)
          mI[iterm * isize + iterp] = 1.0;
        else
          mI[iterm * isize + iterp] = 0.0;
#ifndef DISABLE_GSL
    gsl_matrix_view matI = gsl_matrix_view_array(mI.get(), isize, isize);
    // Invert the matrix
    gsl_blas_dtrsm(CblasLeft, CblasUpper, CblasNoTrans, CblasNonUnit, 1.0, &matU.matrix, &matI.matrix);
#else
    std::cout << "GLS NOT ENABLED!" << std::endl;
#endif
    UnPackYlmMatrixIndependentOnly(mI.get(), outmat, isize);
  }

  void FemtoYlmSolver::UnPackYlmMatrixIndependentOnly(double* inmat, double* outmat, int insize) const {
    int lmax = sqrt(insize) - 1;
    //  cout << "lmax is  " << lmax << std::endl;
    if (0) { lmax *= 2; }
    int tmax = (lmax + 1) * (lmax + 1) * 2;
    std::unique_ptr<int[]> indexfrom(new int[tmax]);
    std::unique_ptr<int[]> multfrom(new int[tmax]);

    int el, em;
    for (int iter = 0; iter < tmax; iter++) {
      int im = iter % 2;
      GetElEmForIndex(iter / 2, el, em);
      if (em == 0) {
        if (im == 1) {
          indexfrom[iter] = 0;
          multfrom[iter]  = 0;
        } else {
          indexfrom[iter] = el * el;
          multfrom[iter]  = 1;
        }
      } else if (em < 0) {
        indexfrom[iter] = (el * el) + (-em) * 2 - 1;
        if (im) indexfrom[iter]++;
        if ((-em) % 2)
          if (im)
            multfrom[iter] = 1;
          else
            multfrom[iter] = -1;
        else if (im)
          multfrom[iter] = -1;
        else
          multfrom[iter] = 1;
      } else if (em > 0) {
        indexfrom[iter] = (el * el) + (em) *2 - 1;
        if (im) indexfrom[iter]++;
        multfrom[iter] = 1;
      }
    }

    //   cout << "From Mult " << std::endl;
    //   for (int iter=0; iter<tmax; iter++)
    //     cout << indexfrom[iter] << " ";
    //   cout << std::endl;
    //   for (int iter=0; iter<tmax; iter++)
    //     cout << multfrom[iter] << " ";
    //   cout << std::endl;

    for (int ilmz = 0; ilmz < GetMaxJM() * 2; ilmz++)
      for (int ilmp = 0; ilmp < GetMaxJM() * 2; ilmp++)
        outmat[ilmz * GetMaxJM() * 2 + ilmp] =
          inmat[(indexfrom[ilmz] * insize) + indexfrom[ilmp]] * multfrom[ilmz] * multfrom[ilmp];
  }

  int FemtoYlmSolver::PackYlmVectorIndependentOnly(double* invec, double* outvec) const {
    int ioutcount = 0;
    int em, el;
    for (int ilm = 0; ilm < GetMaxJM(); ilm++) {
      GetElEmForIndex(ilm, el, em);
      if (em < 0) continue;  // KURWA
      outvec[ioutcount++] = invec[ilm * 2];
      if (em == 0) continue;
      outvec[ioutcount++] = invec[ilm * 2 + 1];
    }
    return ioutcount;
  }

  int FemtoYlmSolver::PackYlmMatrixIndependentOnly(double* inmat, double* outmat) const {
    int ioutcountz = 0;
    int ioutcountp = 0;
    int emz, elz;
    int emp, elp;
    int finalsize = 0;

    for (int ilm = 0; ilm < GetMaxJM(); ilm++) {
      GetElEmForIndex(ilm, elz, emz);
      if (emz < 0) continue;
      finalsize++;
      if (emz == 0) continue;
      finalsize++;
    }

    //  cout << "Final size " << finalsize << std::endl;
    auto GetBin = [](int maxJm, int ilmzero, int zeroimag, int ilmprim, int primimag) -> int {
      return ((ilmprim * 2 + primimag) * maxJm * 2 + ilmzero * 2 + zeroimag);
    };

    for (int ilmz = 0; ilmz < GetMaxJM(); ilmz++) {
      GetElEmForIndex(ilmz, elz, emz);
      ioutcountp = 0;

      if (emz < 0) continue;
      for (int ilmp = 0; ilmp < GetMaxJM(); ilmp++) {
        GetElEmForIndex(ilmp, elp, emp);
        if (emp < 0) continue;
        outmat[ioutcountz * finalsize + ioutcountp] = inmat[GetBin(fMaxJM, ilmz, 0, ilmp, 0)];
        ioutcountp++;
        if (emp == 0) continue;
        outmat[ioutcountz * finalsize + ioutcountp] = inmat[GetBin(fMaxJM, ilmz, 0, ilmp, 1)];
        ioutcountp++;
      }
      ioutcountz++;

      if (emz == 0) continue;
      ioutcountp = 0;
      for (int ilmp = 0; ilmp < GetMaxJM(); ilmp++) {
        GetElEmForIndex(ilmp, elp, emp);
        if (emp < 0) continue;
        outmat[ioutcountz * finalsize + ioutcountp] = inmat[GetBin(fMaxJM, ilmz, 1, ilmp, 0)];
        ioutcountp++;
        if (emp == 0) continue;
        outmat[ioutcountz * finalsize + ioutcountp] = inmat[GetBin(fMaxJM, ilmz, 1, ilmp, 1)];
        ioutcountp++;
      }
      ioutcountz++;
    }

    return ioutcountz;
  }

  FemtoYlmSolver::FemtoYlmSolver(Int_t maxL, FemtoSHCF* cf, Bool_t sumw) :
    fMaxJM((maxL + 1) * (maxL + 1)),
    fFactorialsSize((maxL + 1) * 4),
    fMaxL(maxL),
    fSumw(sumw),
    fSlice(FemtoSHSlice(maxL)),
    fCF(cf) {
    fLmVals.Resize(maxL);
    fFactorials.resize(fFactorialsSize);
    fMaxJM2_4 = 4.0 * fMaxJM * fMaxJM;

    if (fSumw) {
      fNumEnt = cf->GetNumRe(0, 0)->GetEffectiveEntries();  // KURWA
      fDenEnt = cf->GetDenRe(0, 0)->GetEffectiveEntries();  // KURWA
    } else {
      fNumEnt = cf->GetNumRe(0, 0)->GetEntries();
      fDenEnt = cf->GetDenRe(0, 0)->GetEntries();
    }


    Double_t fac   = 1;
    fFactorials[0] = 1;
    for (int iter = 1; iter < fFactorialsSize; iter++) {
      fac *= iter;
      fFactorials[iter] = fac;
    }
  }

  void FemtoYlmSolver::DoMath(Bool_t recalc) {

    std::unique_ptr<std::complex<double>[]> tMq0(new std::complex<double>[fMaxJM]);
    std::unique_ptr<std::complex<double>[]> tTq0(new std::complex<double>[fMaxJM]);
    std::unique_ptr<double[]> tMTilde(new double[fMaxJM * fMaxJM * 4]);

    for (int i = 0; i < fMaxJM; i++) {
      fSlice.fCFImag[i]  = 0;
      fSlice.fCFReal[i]  = 0;
      fSlice.fCFImagE[i] = 0;
      fSlice.fCFRealE[i] = 0;
    }

    for (int i = 0; i < fMaxJM * 2; i++)
      for (int j = 0; j < fMaxJM * 2; j++)
        fSlice.fCovCF[i][j] = 0;
    if (fSlice.fCovNum[0][0] == 0) MakeFakeCovMatrix();
    if (fDebug) {
      std::cout << "NORM " << fNormFactor << std::endl;
      std::cout << "RECALC " << recalc << std::endl;

      std::cout << "Nu ent " << fCF->fNumReal[0]->GetEntries() << " " << fCF->fNumReal[0]->GetBinContent(1) << std::endl;
    }
    for (int ilm = 0; ilm < fMaxJM; ilm++) {
      //      cout << fNumImag[ilm]->GetBinContent(ibin) << std::endl;
      if (recalc) {
        // fNumEnt = 5.76589e+08;  // KURWA
        tMq0[ilm] =
          std::complex<double>(fSlice.fDenReal[ilm] / (fDenEnt / fNormFactor), fSlice.fDenImag[ilm] / (fDenEnt / fNormFactor));
        // std::cout << "tMQ" << ilm << "\t" << fSlice.fDenReal[ilm] << " " << fDenEnt << " " << fSlice.fDenImag[ilm] << " "
        //          << fDenEnt << std::endl;
        tTq0[ilm] = std::complex<double>(fSlice.fNumReal[ilm] / fNumEnt, fSlice.fNumImag[ilm] / fNumEnt);
      } else {
        tMq0[ilm] = std::complex<double>(fSlice.fDenReal[ilm] / fNormFactor, fSlice.fDenImag[ilm] / fNormFactor);
        tTq0[ilm] = std::complex<double>(fSlice.fNumReal[ilm], fSlice.fNumImag[ilm]);
      }
      //      cout << imag(tTq0[ilm]) << std::endl;
    }

    if (fDebug) {
      std::cout << "TT ";
      for (int ilm = 0; ilm < fMaxJM; ilm++) {
        std::cout << tTq0[ilm] << " " << std::endl;
      }
      std::cout << std::endl;
      std::cout << "NN ";
      for (int ilm = 0; ilm < fMaxJM; ilm++) {
        std::cout << tMq0[ilm] << " " << std::endl;
      }
      std::cout << std::endl;
    }
    // Calculate the proper error matrix for T
    // from the temporary covariance matrices
    //    int tabshift = (ibin-1)*GetMaxJM()*GetMaxJM()*4;
    if (recalc) {
      if (fDebug) { /*
         std::cout << "COVNUM" << std::endl;
         for (int ilmzero = 0; ilmzero < GetMaxJM() * 2; ilmzero++) {
           for (int ilmprim = 0; ilmprim < GetMaxJM() * 2; ilmprim++) {
             std::cout << fSlice.fCovNum[ilmzero][ilmprim] << " ";
           }
           std::cout << std::endl;
         }
         std::cout << std::endl;*/
      }

      for (int ilmzero = 0; ilmzero < GetMaxJM(); ilmzero++) {
        const int ilmzero2 = ilmzero * 2;
        for (int ilmprim = 0; ilmprim < GetMaxJM(); ilmprim++) {
          const int ilmprim2 = ilmprim * 2;
          if (std::isnan(fSlice.fCovNum[ilmzero2][ilmprim2])) {}
          if (std::isnan(fSlice.fCovNum[ilmzero2][ilmprim2 + 1])) {}
          if (std::isnan(fSlice.fCovNum[ilmzero2 + 1][ilmprim2])) {}
          if (std::isnan(fSlice.fCovNum[ilmzero2 + 1][ilmprim2 + 1])) {}
          double a, b, c, d;
          a = fSlice.fCovNum[ilmzero2][ilmprim2];
          b = fSlice.fCovNum[ilmzero2][ilmprim2 + 1];
          c = fSlice.fCovNum[ilmzero2 + 1][ilmprim2];
          d = fSlice.fCovNum[ilmzero2 + 1][ilmprim2 + 1];
          if (fDebug) std::cout << ">" << a << " " << b << " " << c << " " << d << " " << fNumEnt << std::endl;

          fSlice.fCovNum[ilmzero2][ilmprim2] /= fNumEnt;
          fSlice.fCovNum[ilmzero2][ilmprim2 + 1] /= fNumEnt;
          fSlice.fCovNum[ilmzero2 + 1][ilmprim2] /= fNumEnt;
          fSlice.fCovNum[ilmzero2 + 1][ilmprim2 + 1] /= fNumEnt;

          a = fSlice.fCovNum[ilmzero2][ilmprim2];
          b = fSlice.fCovNum[ilmzero2][ilmprim2 + 1];
          c = fSlice.fCovNum[ilmzero2 + 1][ilmprim2];
          d = fSlice.fCovNum[ilmzero2 + 1][ilmprim2 + 1];
          if (fDebug) std::cout << "->" << a << " " << b << " " << c << " " << d << std::endl;


          fSlice.fCovNum[ilmzero2][ilmprim2] -= real(tTq0[ilmzero]) * real(tTq0[ilmprim]);
          fSlice.fCovNum[ilmzero2][ilmprim2 + 1] -= real(tTq0[ilmzero]) * imag(tTq0[ilmprim]);
          fSlice.fCovNum[ilmzero2 + 1][ilmprim2] -= imag(tTq0[ilmzero]) * real(tTq0[ilmprim]);
          fSlice.fCovNum[ilmzero2 + 1][ilmprim2 + 1] -= imag(tTq0[ilmzero]) * imag(tTq0[ilmprim]);
          a = fSlice.fCovNum[ilmzero2][ilmprim2];
          b = fSlice.fCovNum[ilmzero2][ilmprim2 + 1];
          c = fSlice.fCovNum[ilmzero2 + 1][ilmprim2];
          d = fSlice.fCovNum[ilmzero2 + 1][ilmprim2 + 1];
          if (fDebug) std::cout << "-->" << a << " " << b << " " << c << " " << d << std::endl;
          fSlice.fCovNum[ilmzero2][ilmprim2] /= ((fNumEnt) -1);
          fSlice.fCovNum[ilmzero2][ilmprim2 + 1] /= ((fNumEnt) -1);
          fSlice.fCovNum[ilmzero2 + 1][ilmprim2] /= ((fNumEnt) -1);
          fSlice.fCovNum[ilmzero2 + 1][ilmprim2 + 1] /= ((fNumEnt) -1);
          a = fSlice.fCovNum[ilmzero2][ilmprim2];
          b = fSlice.fCovNum[ilmzero2][ilmprim2 + 1];
          c = fSlice.fCovNum[ilmzero2 + 1][ilmprim2];
          d = fSlice.fCovNum[ilmzero2 + 1][ilmprim2 + 1];
          if (fDebug) std::cout << "--->" << a << " " << b << " " << c << " " << d << std::endl;
        }
      }
    }

    if (fDebug) {
      std::cout << "COVNUM" << std::endl;
      for (int ilmzero = 0; ilmzero < GetMaxJM(); ilmzero++) {
        const int ilmzero2 = ilmzero * 2;
        for (int ilmprim = 0; ilmprim < GetMaxJM(); ilmprim++) {
          const int ilmprim2 = ilmprim * 2;
          std::cout << fSlice.fCovNum[ilmzero2 + 0][ilmprim2 + 0] << " ";
          std::cout << fSlice.fCovNum[ilmzero2 + 0][ilmprim2 + 1] << " ";
          std::cout << fSlice.fCovNum[ilmzero2 + 1][ilmprim2 + 0] << " ";
          std::cout << fSlice.fCovNum[ilmzero2 + 1][ilmprim2 + 1] << " ";
          std::cout << std::endl;
        }
        std::cout << std::endl;
      }
    }


    GetMtilde(tMq0.get(), tMTilde.get());

    // Perform the solution for the correlation function itself and the errors
    //     cout << "=============================" << std::endl;
    //     cout << "C calculation for bin " << (ibin-1) << std::endl;
    //     cout << std::endl;
    //     cout << "Input: " << std::endl;
    //     cout << "T vector " << std::endl;
    //     for (int ilm=0; ilm<GetMaxJM(); ilm++)
    //       cout << real(tTq0[ilm]) << " " << imag(tTq0[ilm]) << "   ";
    //     cout << std::endl << "M vector " << std::endl;
    //     for (int ilm=0; ilm<GetMaxJM(); ilm++)
    //       cout << real(tMq0[ilm]) << " " << imag(tMq0[ilm]) << "   ";
    //     cout << std::endl;

    if (fSlice.fNumReal[0] <= 0) {
      for (int ilm = 0; ilm < GetMaxJM(); ilm++) {
        fSlice.fCFReal[ilm]  = 0;
        fSlice.fCFRealE[ilm] = 0;
        fSlice.fCFImag[ilm]  = 0;
        fSlice.fCFImagE[ilm] = 0;
      }

      for (int ilmz = 0; ilmz < GetMaxJM(); ilmz++) {    // bylo *2
        for (int ilmp = 0; ilmp < GetMaxJM(); ilmp++) {  // bylo *2
          fSlice.fCovNum[2 * ilmz][2 * ilmp]     = 0;
          fSlice.fCovNum[2 * ilmz][2 * ilmp + 1] = 0;
        }
      }
      return;
    }

    // Rewrite the new way to use the solving wherever there is inversion

    std::unique_ptr<double[]> mDeltaT(new double[fMaxJM * fMaxJM * 4]);
    for (int ilmzero = 0; ilmzero < GetMaxJM() * 2; ilmzero++)
      for (int ilmprim = 0; ilmprim < GetMaxJM() * 2; ilmprim++)
        mDeltaT[(ilmzero * fMaxJM * 2) + ilmprim] = fSlice.fCovNum[ilmzero][ilmprim];

    if (fDebug) {
      std::cout << "Delta T matrix " << std::endl;
      for (int ilmz = 0; ilmz < GetMaxJM() * 2; ilmz++) {
        for (int ilmp = 0; ilmp < GetMaxJM() * 2; ilmp++) {
          std::cout.precision(3);
          std::cout.width(10);
          std::cout << mDeltaT[ilmz * GetMaxJM() * 2 + ilmp];
        }
        std::cout << std::endl;
      }
    }

    std::unique_ptr<double[]> mDeltaTPacked(new double[fMaxJM * fMaxJM * 4]);
    int msize = PackYlmMatrixIndependentOnly(mDeltaT.get(), mDeltaTPacked.get());

    if (fDebug) {
      std::cout << "Delta T matrix packed " << std::endl;
      for (int ilmz = 0; ilmz < msize; ilmz++) {
        for (int ilmp = 0; ilmp < msize; ilmp++) {
          std::cout.precision(3);
          std::cout.width(10);
          std::cout << mDeltaTPacked[ilmz * msize + ilmp];
        }
        std::cout << std::endl;
      }
    }

    // (1) Solve (DeltaT)^1 Mtilde = Q

    // Prepare halper matrices

    std::unique_ptr<double[]> mM(new double[fMaxJM * fMaxJM * 4]);
    std::unique_ptr<double[]> mMPacked(new double[fMaxJM * fMaxJM * 4]);
    for (int iter = 0; iter < fMaxJM * fMaxJM * 4; iter++)
      mM[iter] = tMTilde[iter];
    PackYlmMatrixIndependentOnly(mM.get(), mMPacked.get());

    gsl_matrix_view matM = gsl_matrix_view_array(mMPacked.get(), msize, msize);
    if (fDebug) {
      std::cout << "Mtilde matrix " << std::endl;
      for (int ilmz = 0; ilmz < GetMaxJM() * 2; ilmz++) {
        for (int ilmp = 0; ilmp < GetMaxJM() * 2; ilmp++) {
          std::cout.precision(3);
          std::cout.width(10);
          std::cout << mM[ilmz * GetMaxJM() * 2 + ilmp];
        }
        std::cout << std::endl;
      }

      std::cout << "Mtilde matrix packed " << std::endl;
      for (int ilmz = 0; ilmz < msize; ilmz++) {
        for (int ilmp = 0; ilmp < msize; ilmp++) {
          std::cout.precision(3);
          std::cout.width(10);
          std::cout << mMPacked[ilmz * msize + ilmp];
        }
        std::cout << std::endl;
      }
    }

    // Inverting matrix DeltaT.

    std::unique_ptr<double[]> mU(new double[fMaxJM * fMaxJM * 4]);
    InvertYlmIndependentMatrix(mDeltaT.get(), mU.get());

    std::unique_ptr<double[]> mDTInvertedPacked(new double[fMaxJM * fMaxJM * 4]);
    PackYlmMatrixIndependentOnly(mU.get(), mDTInvertedPacked.get());

    gsl_matrix_view matDTI = gsl_matrix_view_array(mDTInvertedPacked.get(), msize, msize);

    if (fDebug) {
      std::cout << "Delta T matrix inverted packed " << std::endl;
      for (int ilmz = 0; ilmz < msize; ilmz++) {
        for (int ilmp = 0; ilmp < msize; ilmp++) {
          std::cout.precision(3);
          std::cout.width(10);
          std::cout << mDTInvertedPacked[ilmz * msize + ilmp];
        }
        std::cout << std::endl;
      }
    }

    // (2) Multiply DeltaT^1 M = Q
    std::unique_ptr<double[]> mQ(new double[fMaxJM * fMaxJM * 4]);
    for (int iter = 0; iter < msize * msize; iter++)
      mQ[iter] = 0.0;
    gsl_matrix_view matQ = gsl_matrix_view_array(mQ.get(), msize, msize);

    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, &matDTI.matrix, &matM.matrix, 0.0, &matQ.matrix);


    std::unique_ptr<double[]> mTest(new double[fMaxJM * fMaxJM * 4]);
    gsl_matrix_view matTest = gsl_matrix_view_array(mTest.get(), msize, msize);

    std::unique_ptr<double[]> mF(new double[fMaxJM * fMaxJM * 4]);
    for (int iter = 0; iter < fMaxJM * fMaxJM * 4; iter++)
      mF[iter] = mDeltaTPacked[iter];
    gsl_matrix_view matF = gsl_matrix_view_array(mF.get(), msize, msize);

    gsl_blas_dgemm(CblasTrans, CblasNoTrans, 1.0, &matF.matrix, &matQ.matrix, 0.0, &matTest.matrix);

    if (fDebug) {
      std::cout << "Test matrix packed - compare to Mtilde" << std::endl;
      for (int ilmz = 0; ilmz < msize; ilmz++) {
        for (int ilmp = 0; ilmp < msize; ilmp++) {
          std::cout.precision(3);
          std::cout.width(10);
          std::cout << mTest[ilmz * msize + ilmp];
        }
        std::cout << std::endl;
      }
    }

    // (2) Multiply Mtilde^T Q = P

    std::unique_ptr<double[]> mP(new double[fMaxJM * fMaxJM * 4]);
    for (int iter = 0; iter < fMaxJM * fMaxJM * 4; iter++)
      mP[iter] = 0;

    gsl_matrix_view matP = gsl_matrix_view_array(mP.get(), msize, msize);

    gsl_blas_dgemm(CblasTrans, CblasNoTrans, 1.0, &matM.matrix, &matQ.matrix, 0.0, &matP.matrix);

    if (fDebug) {
      std::cout << "P matrix packed " << std::endl;
      for (int ilmz = 0; ilmz < msize; ilmz++) {
        for (int ilmp = 0; ilmp < msize; ilmp++) {
          std::cout.precision(3);
          std::cout.width(10);
          std::cout << mP[ilmz * msize + ilmp];
        }
        std::cout << std::endl;
      }
    }

    // (3) Solve P^-1 Mtilde^T = R
    std::unique_ptr<double[]> mPUnpacked(new double[fMaxJM * fMaxJM * 4]);
    UnPackYlmMatrixIndependentOnly(mP.get(), mPUnpacked.get(), msize);

    if (fDebug) {
      std::cout << "P matrix unpacked " << std::endl;
      for (int ilmz = 0; ilmz < GetMaxJM() * 2; ilmz++) {
        for (int ilmp = 0; ilmp < GetMaxJM() * 2; ilmp++) {
          std::cout.precision(3);
          std::cout.width(10);
          std::cout << mPUnpacked[ilmz * GetMaxJM() * 2 + ilmp];
        }
        std::cout << std::endl;
      }
    }

    // Invert the P matrix

    std::unique_ptr<double[]> mPInverted(new double[fMaxJM * fMaxJM * 4]);
    InvertYlmIndependentMatrix(mPUnpacked.get(), mPInverted.get());

    std::unique_ptr<double[]> mPInvertedPacked(new double[fMaxJM * fMaxJM * 4]);
    PackYlmMatrixIndependentOnly(mPInverted.get(), mPInvertedPacked.get());

    gsl_matrix_view matPI = gsl_matrix_view_array(mPInvertedPacked.get(), msize, msize);

    if (fDebug) {
      std::cout << "P matrix inverted packed " << std::endl;
      for (int ilmz = 0; ilmz < msize; ilmz++) {
        for (int ilmp = 0; ilmp < msize; ilmp++) {
          std::cout.precision(3);
          std::cout.width(10);
          std::cout << mPInvertedPacked[ilmz * msize + ilmp];
        }
        std::cout << std::endl;
      }
    }

    //       //      gsl_matrix_view matR = gsl_matrix_view_array(mR, msize,
    //       msize);

    //       double mG[fMaxJM*fMaxJM*4];
    //       for (int iter=0; iter<fMaxJM*fMaxJM*4; iter++)
    //  mG[iter] = mP[iter];
    //       gsl_matrix_view matG = gsl_matrix_view_array(mG, msize, msize);

    //       // Decomposing the M matrix
    //       gsl_linalg_SV_decomp(&matG.matrix, &matS.matrix, &vecST.vector,
    //       &vecWT.vector);

    //       for (int itert=0; itert<msize; itert++) {
    //  for (int iterm=0; iterm<msize; iterm++)
    //    vCT[iterm] = mMPacked[iterm*msize + itert];
    //    // Transvere !!!      ^^^^^         ^^^^^

    //  // Solving the problem
    //  gsl_linalg_SV_solve(&matG.matrix, &matS.matrix, &vecST.vector,
    // &vecCT.vector, &vecXT.vector);

    //  for (int iterm=0; iterm<msize; iterm++)
    //    mR[itert*msize + iterm] = vXT[iterm];
    //       }


    std::unique_ptr<double[]> mR(new double[fMaxJM * fMaxJM * 4]);
    for (int ir = 0; ir < fMaxJM * fMaxJM * 4; ir++)
      mR[ir] = 0.0;
    gsl_matrix_view matR = gsl_matrix_view_array(mR.get(), msize, msize);

    // (2) Multiply P^-1 M (Trans) = R

    if (fDebug) {
      std::cout << "Matrix M Packed " << std::endl;
      for (int ilmz = 0; ilmz < msize; ilmz++) {
        for (int ilmp = 0; ilmp < msize; ilmp++) {
          std::cout.precision(3);
          std::cout.width(10);
          std::cout << mMPacked[ilmz * msize + ilmp];
        }
        std::cout << std::endl;
      }
    }

    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, &matPI.matrix, &matM.matrix, 1.0, &matR.matrix);

    if (fDebug) {
      std::cout << "R matrix packed " << std::endl;

      for (int ilmz = 0; ilmz < msize; ilmz++) {
        for (int ilmp = 0; ilmp < msize; ilmp++) {
          std::cout.precision(3);
          std::cout.width(10);
          std::cout << mR[ilmz * msize + ilmp];
        }
        std::cout << std::endl;
      }
    }

    // (4) Solve DeltaT^-1 T = L
    std::unique_ptr<double[]> vL(new double[fMaxJM * 2]);
    for (int i = 0; i < fMaxJM * 2; i++)
      vL[i] = 0;
    gsl_vector_view vecL = gsl_vector_view_array(vL.get(), msize);

    //       // Decomposing the M matrix
    //       gsl_linalg_SV_decomp(&matF.matrix, &matS.matrix, &vecST.vector,
    //       &vecWT.vector);

    std::unique_ptr<double[]> vB(new double[fMaxJM * 2]);
    for (int iter = 0; iter < GetMaxJM(); iter++) {
      vB[iter * 2]     = real(tTq0[iter]);
      vB[iter * 2 + 1] = imag(tTq0[iter]);
    }

    std::unique_ptr<double[]> vBPacked(new double[fMaxJM * 2]);
    PackYlmVectorIndependentOnly(vB.get(), vBPacked.get());

    gsl_vector_view vecB = gsl_vector_view_array(vBPacked.get(), msize);

    //       // Solving the problem
    //       gsl_linalg_SV_solve(&matF.matrix, &matS.matrix, &vecST.vector,
    //       &vecB.vector, &vecL.vector);

    if (fDebug) {
      std::cout << "L vector packed " << std::endl;
      for (int ilmp = 0; ilmp < msize; ilmp++) {
        std::cout.precision(3);
        std::cout.width(10);
        std::cout << vL[ilmp];
      }
      std::cout << std::endl;
    }

    // Multiply DeltaT^-1 T = L

    gsl_blas_dgemv(CblasNoTrans, 1.0, &matDTI.matrix, &vecB.vector, 0.0, &vecL.vector);

    // (5) Multiply R L = C

    std::unique_ptr<double[]> vY(new double[fMaxJM * 2]);
    for (int iter = 0; iter < GetMaxJM() * 2; iter++) {
      vY[iter] = 0.0;
    }

    // Prepare inputs for solving the problem
    gsl_vector_view vecY = gsl_vector_view_array(vY.get(), msize);

    gsl_blas_dgemv(CblasNoTrans, 1.0, &matR.matrix, &vecL.vector, 0.0, &vecY.vector);

    if (fDebug) {
      std::cout << "C vector packed " << std::endl;
      for (int ilmp = 0; ilmp < msize; ilmp++) {
        std::cout.precision(3);
        std::cout.width(10);
        std::cout << vY[ilmp];
      }
      std::cout << std::endl;
    }
    int mpack = 0;
    int el, em;
    for (int ilm = 0; ilm < fMaxJM; ilm++) {
      //    fCFReal[ilm]->SetBinContent(ibin, vC[mpack++]);
      GetElEmForIndex(ilm, el, em);
      if ((el % 2) == 1) {
        fSlice.fCFReal[ilm] = 0.0;
        fSlice.fCFImag[ilm] = 0.0;
      }

#ifdef FULL_CALC
      fCFReal[ilm]->SetBinContent(ibin, fNumReal[ilm]->GetBinContent(ibin) / fDenReal[ilm]->GetBinContent(ibin));
      fCFImag[ilm]->SetBinContent(ibin, fNumImag[ilm]->GetBinContent(ibin) / fDenImag[ilm]->GetBinContent(ibin));

#else
      if (em < 0) {
        fSlice.fCFReal[ilm] = 0.0;
        fSlice.fCFImag[ilm] = 0.0;
      } else {
        fSlice.fCFReal[ilm] = vY[mpack++];
        if (em == 0) {
          fSlice.fCFImag[ilm] = 0;
        } else {
          //   fSlice.fCFImag[ilm] = vC[mpack++];
          fSlice.fCFImag[ilm] = vY[mpack++];
        }
      }

#endif
    }

    // invert the P matrix to get C errors
    //      double mS[fMaxJM*fMaxJM*4];

    //       for (int iterz=0; iterz<msize; iterz++)
    //  for (int iterp=0; iterp<msize; iterp++)
    //    if (iterp == iterz)
    //      mS[iterz*msize + iterp] = 1.0;
    //    else
    //      mS[iterz*msize + iterp] = 0.0;

    //      gsl_matrix_view matS = gsl_matrix_view_array(mS, msize, msize);

    // Invert V

    //       gsl_blas_dtrsm(CblasLeft, CblasUpper, CblasNoTrans,
    //       CblasNonUnit, 1.0, &matP.matrix, &matS.matrix);

    mpack = 0;
    for (int ilm = 0; ilm < fMaxJM; ilm++) {
      GetElEmForIndex(ilm, el, em);
      if (em < 0) {
        fSlice.fCFRealE[ilm] = 0;
        fSlice.fCFImagE[ilm] = 0;
      } else {
        fSlice.fCFRealE[ilm] = sqrt(fabs(mPInvertedPacked[mpack * msize + mpack]));
        mpack++;
        if (em == 0)
          fSlice.fCFImag[ilm] = 0;
        else {
          fSlice.fCFImagE[ilm] = sqrt(fabs(mPInvertedPacked[mpack * msize + mpack]));
          mpack++;
        }
      }
    }

    for (int ilmz = 0; ilmz < GetMaxJM() * 2; ilmz++) {
      for (int ilmp = 0; ilmp < GetMaxJM() * 2; ilmp++) {
        if (ilmp > ilmz)
          fSlice.fCovCF[ilmz][ilmp] = mPInverted[ilmz * GetMaxJM() * 2 + ilmp];
        else
          fSlice.fCovCF[ilmz][ilmp] = mPInverted[ilmp * GetMaxJM() * 2 + ilmz];
      }
    }
  }

  void FemtoYlmSolver::Solve(Bool_t recalc) {
    int nbins = fCF->GetNum()->GetNbinsX();
    for (int i = 1; i <= nbins; i++) {
      fSlice.BuildSlice(*fCF, i);
      if (fDebugBin == i) {
        fDebug = kTRUE;
      } else {
        fDebug = kFALSE;
      }
      DoMath(recalc);
      FixCF();
      UpdateCF(i);
    }
  }

  void FemtoYlmSolver::UpdateCF(Int_t bin) {
    for (int j = 0; j < fMaxJM; j++) {
      fCF->fCFReal[j]->SetBinContent(bin, fSlice.fCFReal[j]);
      fCF->fCFImag[j]->SetBinContent(bin, fSlice.fCFImag[j]);
      fCF->fCFReal[j]->SetBinError(bin, fSlice.fCFRealE[j]);
      fCF->fCFImag[j]->SetBinError(bin, fSlice.fCFImagE[j]);
    }
    for (int a = 0; a < 2 * fMaxJM; a++) {
      for (int b = 0; b < 2 * fMaxJM; b++) {
        fCF->fCovCf[bin - 1][a][b] = fSlice.fCovCF[a][b];
        // fCF->fCovNum[bin - 1][a][b] = fSlice.fCovNum[a][b]; do not update cov matrix in num
      }
    }
  }

  void FemtoYlmSolver::FixCF() {
    for (int l = 0; l <= fMaxL; l++) {
      for (int m = -l; m < 0; m++) {
        Int_t from          = fLmVals.GetIndex(l, -m);
        Int_t to            = fLmVals.GetIndex(l, m);
        fSlice.fCFReal[to]  = fSlice.fCFReal[from];
        fSlice.fCFImag[to]  = -fSlice.fCFImag[from];
        fSlice.fCFRealE[to] = fSlice.fCFRealE[from];
        fSlice.fCFImagE[to] = fSlice.fCFImagE[from];
      }
    }
  }

  void FemtoYlmSolver::SetNormalizationArea(Double_t min, Double_t max) {
    TH1* num0   = fCF->fNumReal[0];
    TH1* den0   = fCF->fDenReal[0];
    fNormFactor = 1.0;
    // TODO Fix/improvenormalization
    double normbinmax = min;  // fDenReal[0]->FindBin(fNormMax);
    double normbinmin = max;  // fDenReal[0]->FindBin(fNormMin);

    if (normbinmax > 0) {
      double sksum = 0.0;
      double wksum = 0.0;

      double sk, wk, ks;
      if (normbinmin < 1) normbinmin = 1;
      if (normbinmax > den0->GetNbinsX()) normbinmax = den0->GetNbinsX();
      normbinmin = 1;                  // KURWA
      normbinmax = den0->GetNbinsX();  // KURWA
      for (int ib = normbinmin; ib <= normbinmax; ib++) {
        ks = den0->GetXaxis()->GetBinCenter(ib);
        sk = num0->GetBinContent(ib) / (den0->GetBinContent(ib) * (1.0 - fNormPurity / (fNormRadius * fNormBohr * ks * ks)));
        wk = num0->GetBinContent(ib);
        sksum += sk * wk;
        wksum += wk;
      }
      fNormFactor *= sksum / wksum;
      if (fSumw) {
        fNormFactor /= num0->GetEffectiveEntries() / den0->GetEffectiveEntries();
      } else
        fNormFactor /= num0->GetEntries() / den0->GetEntries();
    }
    //    fNumEnt = num0->GetEntries();
    //    fDenEnt = den0->GetEntries();
  }

  void FemtoYlmSolver::MakeFakeCovMatrix() {
    double nent  = fNumEnt;
    double nentd = fDenEnt;
    for (int ilmx = 0; ilmx < GetMaxJM(); ilmx++) {
      const int ilmx2 = ilmx * 2;
      for (int ilmy = 0; ilmy < GetMaxJM(); ilmy++) {
        const int ilmy2 = ilmy * 2;
        double t1t2rr   = fSlice.fNumReal[ilmx] * fSlice.fNumReal[ilmy] / nent / nent;
        double t1t2ri   = fSlice.fNumReal[ilmx] * fSlice.fNumImag[ilmy] / nent / nent;
        double t1t2ir   = fSlice.fNumImag[ilmx] * fSlice.fNumReal[ilmy] / nent / nent;
        double t1t2ii   = fSlice.fNumImag[ilmx] * fSlice.fNumImag[ilmy] / nent / nent;
        if (ilmx == ilmy) {
          fSlice.fCovNum[ilmx2][ilmy2]         = nent * (TMath::Power(fSlice.fNumRealE[ilmx] / nent, 2) * (nent - 1) + t1t2rr);
          fSlice.fCovNum[ilmx2][ilmy2 + 1]     = nent * t1t2ri;
          fSlice.fCovNum[ilmx2 + 1][ilmy2]     = nent * t1t2ir;
          fSlice.fCovNum[ilmx2 + 1][ilmy2 + 1] = nent * (TMath::Power(fSlice.fNumImagE[ilmx] / nent, 2) * (nent - 1) + t1t2rr);
        } else {
          fSlice.fCovNum[ilmx2][ilmy2]         = nent * t1t2rr;
          fSlice.fCovNum[ilmx2][ilmy2 + 1]     = nent * t1t2ri;
          fSlice.fCovNum[ilmx2 + 1][ilmy2]     = nent * t1t2ir;
          fSlice.fCovNum[ilmx2 + 1][ilmy2 + 1] = nent * t1t2ii;
        }
        t1t2rr = fSlice.fDenReal[ilmx] * fSlice.fDenReal[ilmy] / nentd / nentd;
        t1t2ri = fSlice.fDenReal[ilmx] * fSlice.fDenImag[ilmy] / nentd / nentd;
        t1t2ir = fSlice.fDenImag[ilmx] * fSlice.fDenReal[ilmy] / nentd / nentd;
        t1t2ii = fSlice.fDenImag[ilmx] * fSlice.fDenImag[ilmy] / nentd / nentd;


        fSlice.fCovDen[ilmx2 + 0][ilmy2 + 0] = nentd * t1t2rr;
        fSlice.fCovDen[ilmx2 + 0][ilmy2 + 1] = nentd * t1t2ri;
        fSlice.fCovDen[ilmx2 + 1][ilmy2 + 0] = nentd * t1t2ir;
        fSlice.fCovDen[ilmx2 + 1][ilmy2 + 1] = nentd * t1t2ii;
      }
    }
  }

} /* namespace Hal */
