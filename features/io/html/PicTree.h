/*
 * PicTree.h
 *
 *  Created on: 4 lip 2024
 *      Author: daniel
 */

#ifndef HAL_FEATURES_IO_HTML_PICTREE_H_
#define HAL_FEATURES_IO_HTML_PICTREE_H_

#include "Object.h"

/**
 * generate picture with flow of data in task
 */
namespace Hal {

  class PicTree : public Object {
    struct circle {
      Int_t sDepth;
      Int_t sNumber;
      Int_t sStyles;
      Double_t sRatio;
      circle(Int_t depth, Int_t number, Int_t style, Double_t ratio) :
        sDepth(depth), sNumber(number), sStyles(style), sRatio(ratio) {}
      void Draw(const PicTree& tree);
    };
    struct link {
      Int_t sFromNumber;
      Int_t sToNumber;
      Int_t sFromDepth;
      Int_t sToDepth;
      Int_t sStyles;
      link(std::initializer_list<Int_t> depths, std::initializer_list<Int_t> ids, Int_t style);
      void Draw(const PicTree& tree);
    };
    std::vector<std::vector<std::pair<double, double>>> fPoints;

    std::vector<circle> fCircles;
    std::vector<link> fLinks;
    const Int_t fMaxExpDepth = {10};
    Int_t fDepthMax          = {1};
    Double_t fBasicRadius = {20}, fWindowWidth = {800}, fWindowHeight = {800};

    Double_t GetDepth() const { return fDepthMax; }

  public:
    PicTree();
    /**
     * add point drawn as circle
     * @param depth depth of circle (x)
     * @param number position of circle (y)
     * @param style 0 for normal data, 1 for background
     * @param ratio ratio of passed objects
     */
    void AddPoint(Int_t depth, Int_t number, Int_t style, Double_t ratio);
    /**
     * adds link
     * @param depths depth of data {from, to}
     * @param ids position of circles {from, to}
     * @param style 0 for normal link 1 for background
     */
    void AddLink(std::initializer_list<Int_t> depths, std::initializer_list<Int_t> ids, Int_t style);
    /**
     * save picture
     * @param path
     */
    void SaveAs(TString path);
    /**
     *
     * @return width of picture
     */
    Double_t GetWidth() const { return fWindowWidth; }
    /**
     *
     * @return height of picture
     */
    Double_t GetHeight() const { return fWindowHeight; }
    /**
     *
     * @return array of coordinates (calcualted during calling SaveAs [tier level][collection] first -> x second ->y coordinate
     */
    std::vector<std::vector<std::pair<double, double>>> GetCoords() const { return fPoints; };
    virtual ~PicTree() {};
    ClassDef(PicTree, 0)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_IO_HTML_PICTREE_H_ */
