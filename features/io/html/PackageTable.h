/*
 * PackageTable.h
 *
 *  Created on: 22 cze 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FEATURES_IO_PACKAGETABLE_H_
#define HAL_FEATURES_IO_PACKAGETABLE_H_

#include "Object.h"
/**
 * class that represents simple table of strings
 */
namespace Hal {
  class PackageTable : public Object {
    std::vector<std::vector<TString>> fArray;

  public:
    PackageTable() {};
    PackageTable(std::initializer_list<TString> list);
    void AddRow(std::initializer_list<TString> list);
    /**
     * used to merge similar objects
     * @param collection
     */
    Long64_t Merge(TCollection* collection);
    virtual TString GetExtractType() const;
    virtual TString HTMLExtract(Int_t no, TString path) const;
    virtual ~PackageTable() {};
    ClassDef(PackageTable, 1)
  };
}  // namespace Hal
#endif /* HAL_FEATURES_IO_PACKAGETABLE_H_ */
