/*
 * CorrFitMask.cxx
 *
 *  Created on: 11 lut 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFitMask.h"

#include "Cout.h"
#include "Femto3DCF.h"
#include "FemtoSHCF.h"
#include "StdHist.h"


namespace Hal {

  CorrFitMask::CorrFitMask() {}

  void CorrFitMask::ApplyThreshold(const TH1& /*h*/, Double_t /*threshold*/, ELogic /*logic*/) {}

  void CorrFitMask::Mask(Array_1<Short_t>& map, const Array_1<Short_t>& mask, ELogic logic) const {
    if (map.GetSize() != mask.GetSize()) return;
    switch (logic) {
      case ELogic::kAnd: {
        for (int i = 0; i < map.GetSize(); i++) {
          if (map[i] == 1 && mask[i] == 1) {
            map[i] = 1;
          } else {
            map[i] = 0;
          }
        }
      } break;
      case ELogic::kOr: {
        for (int i = 0; i < map.GetSize(); i++) {
          if (mask[i] == 1) { map[i] = 1; }
        }
      } break;
      case ELogic::kNot: {
        for (int i = 0; i < map.GetSize(); i++) {
          if (mask[i] == 1) { map[i] = 0; }
        }
      } break;
    }
  }


  void CorrFitMask::Mask(Array_2<Short_t>& map, const Array_2<Short_t>& mask, ELogic logic) const {
    if (map.GetSize() != mask.GetSize()) return;
    if (map[0].GetSize() != mask[0].GetSize()) return;
    switch (logic) {
      case ELogic::kAnd: {
        for (int i = 0; i < map.GetSize(); i++) {
          for (int j = 0; j < map[i].GetSize(); j++) {
            if (map[i][j] == 1 && mask[i][j] == 1) {
              map[i][j] = 1;
            } else {
              map[i][j] = 0;
            }
          }
        }
      } break;
      case ELogic::kOr: {
        for (int i = 0; i < map.GetSize(); i++) {
          for (int j = 0; j < map[i].GetSize(); j++) {
            if (mask[i][j] == 1) { map[i][j] = 1; }
          }
        }
      } break;
      case ELogic::kNot: {
        for (int i = 0; i < map.GetSize(); i++) {
          for (int j = 0; j < map[i].GetSize(); j++) {
            if (mask[i][j] == 1) { map[i][j] = 0; }
          }
        }
      } break;
    }
  }


  void CorrFitMask::Mask(Array_3<Short_t>& map, const Array_3<Short_t>& mask, ELogic logic) const {
    if (map.GetSize() != mask.GetSize()) return;
    if (map[0].GetSize() != mask[0].GetSize()) return;
    if (map[0][0].GetSize() != mask[0][0].GetSize()) return;
    switch (logic) {
      case ELogic::kAnd: {
        for (int i = 0; i < map.GetSize(); i++) {
          for (int j = 0; j < map[i].GetSize(); j++) {
            for (int k = 0; k < map[i][j].GetSize(); k++) {
              if (map[i][j][k] == 1 && mask[i][j][k] == 1) {
                map[i][j][k] = 1;
              } else {
                map[i][j][k] = 0;
              }
            }
          }
        }
      } break;
      case ELogic::kOr: {
        for (int i = 0; i < map.GetSize(); i++) {
          for (int j = 0; j < map[i].GetSize(); j++) {
            for (int k = 0; k < map[i][j].GetSize(); k++) {
              if (mask[i][j][k] == 1) { map[i][j][k] = 1; }
            }
          }
        }
      } break;
      case ELogic::kNot: {
        for (int i = 0; i < map.GetSize(); i++) {
          for (int j = 0; j < map[i].GetSize(); j++) {
            for (int k = 0; k < map[i][j].GetSize(); k++) {
              if (mask[i][j][k] == 1) { map[i][j][k] = 0; }
            }
          }
        }
      } break;
    }
  }

  void CorrFitMask::SetGlobalStatus(Array_1<Short_t>& map, Int_t status) const {
    for (int i = 0; i < map.GetSize(); i++) {
      map[i] = status;
    }
  }

  void CorrFitMask::SetGlobalStatus(Array_2<Short_t>& map, Int_t status) const {
    for (int i = 0; i < map.GetSize(); i++) {
      for (int j = 0; j < map[i].GetSize(); j++) {
        map[i][j] = status;
      }
    }
  }

  void CorrFitMask::SetGlobalStatus(Array_3<Short_t>& map, Int_t status) const {
    for (int i = 0; i < map.GetSize(); i++) {
      for (int j = 0; j < map[i].GetSize(); j++) {
        for (int k = 0; k < map[i][j].GetSize(); k++) {
          map[i][j][k] = status;
        }
      }
    }
  }

}  // namespace Hal
