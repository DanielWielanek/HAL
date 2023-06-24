/*
 * PackageTable.cxx
 *
 *  Created on: 22 cze 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "PackageTable.h"
#include "HtmlCore.h"
#include "HtmlFile.h"
#include "HtmlObject.h"
#include "HtmlTable.h"
#include "Parameter.h"

#include "Cout.h"
#include "Std.h"

#include <initializer_list>
#include <iostream>
#include <vector>

#include <RtypesCore.h>
#include <TString.h>
#include <TSystem.h>

namespace Hal {

  PackageTable::PackageTable(std::initializer_list<TString> list) {
    auto vec = Std::GetVector(list);
    fArray.push_back(vec);
  }

  void PackageTable::AddRow(std::initializer_list<TString> list) {
    auto vec = Std::GetVector(list);
    if (fArray.size() > 0) {
      if (fArray[0].size() != vec.size()) {
        std::cout << "Cannot add row" << __FILE__ << " " << __LINE__ << std::endl;
        return;
      }
    }
    fArray.push_back(vec);
  }

  TString PackageTable::GetExtractType() const { return "Page"; }

  TString PackageTable::HTMLExtract(Int_t no, TString path) const {
    path = Form("%s/ptable_%i", path.Data(), no);
    gSystem->mkdir(path);
    HtmlFile F(Form("%s/ptable.html", path.Data()), kFALSE);
    HtmlTable table;
    table.SetDefaultClass();

    for (unsigned int i = 0; i < fArray.size(); i++) {
      auto vec = fArray[i];
      HtmlRow row;
      TString style = HtmlTableRowClass::LightBlue();
      if (i == 0) { style = HtmlTableRowClass::DarkBlue(); }
      for (unsigned int j = 0; j < vec.size(); j++) {
        auto rec = vec[j];
        HtmlCell cell;
        cell.SetStringContent(rec);
        row.AddContent(cell);
      }
      row.SetClass(style);
      table.AddContent(row);
    }
    F.AddContent(table);
    F.Save();
    return HtmlCore::GetUrl(Form("ptable_%i/ptable.html", no), ClassName());
  }

  Long64_t PackageTable::Merge(TCollection* collection) {
    if (collection) {
      PackageTable* pack = NULL;
      TIter iterator(collection);
      while ((pack = (PackageTable*) iterator())) {
        if (pack->fArray.size() != this->fArray.size()) {
          Cout::PrintInfo("Different Y-size of PackageTable!", EInfo::kError);
          return 1;
        }
        for (unsigned int i = 0; i < fArray.size(); i++) {
          if (fArray[i].size() != pack->fArray[i].size()) {
            Cout::PrintInfo("Different X-size of PackageTable!", EInfo::kError);
            return 1;
          }
          for (unsigned int j = 0; j < fArray[i].size(); j++) {
            auto str  = fArray[i][j];
            auto str2 = pack->fArray[i][j];
            if (!str.EqualTo(str2)) { Cout::PrintInfo(Form("Different content of PackageTable [%i,%i]!", i, j), EInfo::kError); }
          }
        }
      }
    }

    return 1;
  }

}  // namespace Hal
