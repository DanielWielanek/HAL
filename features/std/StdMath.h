/*
 * Hal::StdMath.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALSTDMATH_H_
#define HALSTDMATH_H_

#include <TMath.h>
#include <TMatrixD.h>
#include <TVector3.h>
#include <vector>

class TH1;
class TH2;
namespace Hal {
  namespace Std {
    /**
     * calculate the parameters of fuction f(x) = ax^2 + bx +c
     * @param x1
     * @param x2
     * @param x3
     * @param y1
     * @param y2
     * @param y3
     * @param a
     * @param b
     * @param c
     */
    void FitParabola(Double_t x1,
                     Double_t x2,
                     Double_t x3,
                     Double_t y1,
                     Double_t y2,
                     Double_t y3,
                     Double_t& a,
                     Double_t& b,
                     Double_t& c);
    /**
     * calculate the solution of equation f(x) 0= ax^2 + bx + c
     *
     * @param a
     * @param b
     * @param c
     * @param y
     * @param x1 first solution
     * @param y1 second solution
     * @return 0 if no solution 1 if one solution exists, 2 if two solution exists
     */
    Int_t SolveParabola(Double_t a, Double_t b, Double_t c, Double_t& x1, Double_t& x2);
    /**
     * calculate the position of bin when convert from 3D to 1D array
     * @param nbinsX
     * @param nBinsY
     * @param binX
     * @param binY
     * @param binZ
     * @param root_number - set true to convert from TH3D (first bin with number 1)
     * @return
     */
    Int_t Bin3dToBin1d(Int_t nbinsX, Int_t nBinsY, Int_t binX, Int_t binY, Int_t binZ, Bool_t root_number = kTRUE);
    /**
     * convert histogram into math vector
     * @param h
     * @param horizontal - return horizontal vector if true
     * @return
     */
    TMatrixD GetVec(const TH1& h, Bool_t horizontal = kFALSE);
    /**
     * convert histogram into matrix
     * @param h
     * @param swaprow swap vertical positions if true
     * @return
     */
    TMatrixD GetMatrix(const TH2& h, Bool_t swaprow = kFALSE);
    /**
     * calculate the position of bin in 3D histogram when convert from 1D array
     * @param nbinsX
     * @param nBinsY
     * @param binX
     * @param binY
     * @param binZ
     * @param root_number set true to convert in TH3D cooridiantes (first bin with number 1)
     * @return
     */
    TVector3 Bin1dToBin3d(Int_t nbinsX, Int_t nBinsY, Int_t bin, Bool_t root_number = kTRUE);
    /**
     * round given value to the closest step
     * @param areas number of areas
     * @param min lower edge of values
     * @param max higher edge of values
     * @param val value to round
     * @param type '=' round to the nearest, '+' round to the upper edge , '-' round to the lower value in range
     * @return rounded value
     */
    Double_t Discretize(Int_t areas, Double_t min, Double_t max, Double_t val, Char_t type);
    /**
     * return statistical uncertaindy of x operator y as
     * @param operator + - / or *
     * @param x
     * @param y
     * @param dx statisical error of x - take sqrt(x) if negative
     * @param dy statistical error of y - take sqrt(y) if negative
     * @return
     */
    Double_t StatError2Var(Char_t ope, Double_t x, Double_t y, Double_t dx = -1, Double_t dy = -1);
    /**
     * convert position in multidimensional array into position in vertex, to convert from one dim to multidim position use
     * @OneToMultiDimIndex
     * @param size size of array in each dimension
     * @param position position inside of array in each dimension
     * @return position in vertex
     */
    Int_t MultiToOneDimIndex(const std::vector<int>& size, const std::vector<int>& position);
    /**
     * oposite to MultiToOneDimIndex
     * @param size size of array in each dimension
     * @param n position in vector
     * @return position in multidimensional space
     */
    std::vector<int> OneToMultiDimIndex(const std::vector<int>& size, Int_t n);
    /**
     *
     * @param num numerator (dividend)
     * @param den denominator (divisor)
     * @return pair - quotient + reminder
     */
    std::pair<Int_t, Int_t> Division(Int_t num, Int_t div);
  }  // namespace Std
}  // namespace Hal
#endif /* HAL_FEATURES_STD_HALSTDMATH_H_ */
