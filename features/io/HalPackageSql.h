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

class HalAnaFile;
class HalPackage;
class TSQLServer;

/**
 * class for put reports in database
 */
class HalPackageSql : public TObject {
  TSQLServer* fSQL;
  Int_t fID;
  TString fFilename;
  HalPackage* fRunInfo;
  TString RemoveDir(TString path);
  TString RemoveDoubleDot(TString path);
  Bool_t HasDir(TString path);
  TString GetPath(TString filename);
  void ExportTask(HalAnaFile* extr, Int_t file_id, Int_t task_id);

public:
  /**
   *
   * @param filename file to export
   */
  HalPackageSql(TString filename);
  virtual ~HalPackageSql();
  ClassDef(HalPackageSql, 1)
};

#endif /* HALPACKAGESQL_H_ */
