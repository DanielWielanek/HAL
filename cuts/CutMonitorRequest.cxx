#include "CutMonitorRequest.h"
namespace Hal {
  CutMonAxisConf CutMonitorRequest::GetConf(Char_t dim) const {
    switch (dim) {
      case 'y': return fYConf; break;
      case 'z': return fZConf; break;
      default: return fXConf; break;
    }
  }

  CutMonitorRequest::CutMonitorRequest(const CutMonAxisConf& x) : fDim(1) { fXConf = x; }

  CutMonitorRequest::CutMonitorRequest(const CutMonAxisConf& x, const CutMonAxisConf& y) : fDim(2) {
    fXConf = x;
    fYConf = y;
  }

  CutMonitorRequest::CutMonitorRequest(const CutMonAxisConf& x, const CutMonAxisConf& y, const CutMonAxisConf& z) : fDim(3) {
    fXConf = x;
    fYConf = y;
    fZConf = z;
  }

  void CutMonAxisConf::FixCutNo(Int_t no) {
    if (fCutNo == -1) fCutNo = no;
  }
}  // namespace Hal
