/*
 * HalPackageSql.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALPACKAGESQL_H_
#define HALPACKAGESQL_H_

#include <TObject.h>
#include <TString.h>


class TSQLServer;

/**
 * class for put reports in database
 */
namespace Hal {
  class AnaFile;
  class Package;
  class PackageSql : public TObject {
    TSQLServer* fSQL;
    Int_t fID;
    TString fFilename;
    Hal::Package* fRunInfo;
    Bool_t HasDir(TString path);
    TString RemoveDir(TString path);
    TString RemoveDoubleDot(TString path);
    TString GetPath(TString filename);
    void ExportTask(AnaFile* extr, Int_t file_id, Int_t task_id);

  public:
    /**
     *
     * @param filename file to export
     */
    PackageSql(TString filename);
    virtual ~PackageSql();
    ClassDef(PackageSql, 1)
  };
}  // namespace Hal
#endif /* HALPACKAGESQL_H_ */
