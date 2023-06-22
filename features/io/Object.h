/*
 * HalPack.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALPACK_H_
#define HALPACK_H_

#include <TNamed.h>
#include <TString.h>

class HtmlTable;


/**
 * Class for objects that can be extracted by HTML extractor. In contrast to
 * HalPackage HalPack usually store object that should be treated as single
 * object e.g. parameter. In some rare cases it migth contain more objects if
 * they are connecte. Example of such object are HalDIvidedHisto-based classes
 * * that store two histograms (numerator and denominator).
 */

namespace Hal {

  class Object : public TNamed {
  public:
    /**
     * default constructor
     */
    Object();
    /**
     * not used
     * @return extraction type options:
     * "String" - extract to string
     * "Page" - extract to separate HTML page
     */
    virtual TString GetExtractType() const;
    /**
     * exttact this object into HTML
     * @param no (used to numbering directories to avoid name conflicts)
     * @param dir directory where object should be extracted
     * @return value or Url to html with report
     */
    virtual TString HTMLExtract(Int_t /*no*/, TString /*dir*/ = "") const;
    /**
     * extract this object into html table table
     * @param no object number
     * @param table table to extract this object
     * @param dir directory to extract the strucuture of object
     * @param rel_dir relative path from this to dir
     */
    virtual void
    HTMLExtractIntoTable(Int_t /* no*/, HtmlTable& /*table*/, TString /*dir*/ = "", TString /*rel_dir */ = "") const {};
    /**
     * add object of the same class to this
     * @param pack added object
     */
    virtual void Add(const Object* pack);
    /**
     * used to merge similar objects
     * @param collection
     */
    virtual Long64_t Merge(TCollection* collection);
    /**
     *
     * @return true if HTMLExtractIntoTable can be used
     */
    virtual Bool_t CanBeTableElement() const { return kFALSE; };
    virtual ~Object();
    ClassDef(Object, 1)
  };
}  // namespace Hal
#endif /* HALPACK_H_ */
