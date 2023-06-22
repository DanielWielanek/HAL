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
      TString style = HtmlTableRowStyles::LightBlue();
      if (i == 0) { style = HtmlTableRowStyles::DarkBlue(); }
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

}  // namespace Hal
