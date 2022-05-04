#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link off all namespaces;

#pragma link C++ namespace HalStd;
#ifdef __MAKECINT__
#endif

//#pragma link C++ class HalCout+
#pragma link C++ class HalPackage + ;
#pragma link C++ class HalAnaFile + ;
#pragma link C++ class HalParameterDouble + ;
#pragma link C++ class HalParameterFloat + ;
#pragma link C++ class HalParameterInt + ;
#pragma link C++ class HalParameterString + ;
#pragma link C++ class HalParameter + ;
#pragma link C++ class HalParameterUInt + ;
#pragma link C++ class HalParameterULong + ;
#pragma link C++ class HalParameterULong64 + ;
#pragma link C++ class HalParameterBool + ;
#pragma link C++ class HalPackage2HTML + ;
#pragma link C++ class HalDataManager + ;
#pragma link C++ class HalFairRootManager + ;
#pragma link C++ class HalRootManager + ;
#pragma link C++ enum Hal::EInfo;
#pragma link C++ enum Hal::ECutUpdate;
#pragma link C++ enum Hal::EFormatDepth;
#pragma link C++ enum Hal::EFormatType;
#pragma link C++ enum HalHtmlTableRowStyles;
#pragma link C++ class HalOptionArray + ;
#pragma link C++ class HalOptionConverter + ;
#pragma link C++ class HalPackageSql + ;
#pragma link C++ class HalObject + ;
#pragma link C++ class HalMultiGraph + ;
#pragma link C++ class HalCout + ;

#pragma link C++ class HalLink + ;
#pragma link C++ class HalConst + ;
#pragma link C++ class HalJobs + ;

#pragma link C++ class HalHtmlCore + ;
#pragma link C++ class HalHtmlDiv + ;
#pragma link C++ class HalHtmlFile + ;
#pragma link C++ class HalHtmlObject + ;
#pragma link C++ class HalHtmlTable + ;
#pragma link C++ class HalHtmlRow + ;
#pragma link C++ class HalHtmlCell + ;
#pragma link C++ class HalHtmlCellCol + ;
#pragma link C++ class HalHtmlCellRow + ;

#pragma link C++ class HalDividedHisto1D + ;
#pragma link C++ class HalDividedHisto2D + ;
#pragma link C++ class HalDividedHisto3D + ;

#pragma link C++ class HalChiSqMap2D + ;

// define Histogram Managers templates
#pragma link C++ class HalHistogramAxisConf + ;

#pragma link C++ class HalHistogramManager_1_1D < TH1D> + ;
#pragma link C++ class HalHistogramManager_1_2D < TH2D> + ;
#pragma link C++ class HalHistogramManager_1_3D < TH3D> + ;
#pragma link C++ class HalHistogramManager_1_1D < TH1F> + ;
#pragma link C++ class HalHistogramManager_1_2D < TH2F> + ;
#pragma link C++ class HalHistogramManager_1_3D < TH3F> + ;
#pragma link C++ class HalHistogramManager_1_1D < TH1I> + ;
#pragma link C++ class HalHistogramManager_1_2D < TH2I> + ;
#pragma link C++ class HalHistogramManager_1_3D < TH3I> + ;

#pragma link C++ class HalHistogramManager_2_1D < TH1D> + ;
#pragma link C++ class HalHistogramManager_2_2D < TH2D> + ;
#pragma link C++ class HalHistogramManager_2_3D < TH3D> + ;
#pragma link C++ class HalHistogramManager_2_1D < TH1F> + ;
#pragma link C++ class HalHistogramManager_2_2D < TH2F> + ;
#pragma link C++ class HalHistogramManager_2_3D < TH3F> + ;
#pragma link C++ class HalHistogramManager_2_1D < TH1I> + ;
#pragma link C++ class HalHistogramManager_2_2D < TH2I> + ;
#pragma link C++ class HalHistogramManager_2_3D < TH3I> + ;

#pragma link C++ class HalHistogramManager_3_1D < TH1D> + ;
#pragma link C++ class HalHistogramManager_3_2D < TH2D> + ;
#pragma link C++ class HalHistogramManager_3_3D < TH3D> + ;
#pragma link C++ class HalHistogramManager_3_1D < TH1F> + ;
#pragma link C++ class HalHistogramManager_3_2D < TH2F> + ;
#pragma link C++ class HalHistogramManager_3_3D < TH3F> + ;
#pragma link C++ class HalHistogramManager_3_1D < TH1I> + ;
#pragma link C++ class HalHistogramManager_3_2D < TH2I> + ;
#pragma link C++ class HalHistogramManager_3_3D < TH3I> + ;

#pragma link C++ class HalHistogramManager_4_1D < TH1D> + ;
#pragma link C++ class HalHistogramManager_4_2D < TH2D> + ;
#pragma link C++ class HalHistogramManager_4_3D < TH3D> + ;
#pragma link C++ class HalHistogramManager_4_1D < TH1F> + ;
#pragma link C++ class HalHistogramManager_4_2D < TH2F> + ;
#pragma link C++ class HalHistogramManager_4_3D < TH3F> + ;
#pragma link C++ class HalHistogramManager_4_1D < TH1I> + ;
#pragma link C++ class HalHistogramManager_4_2D < TH2I> + ;
#pragma link C++ class HalHistogramManager_4_3D < TH3I> + ;

//=============================
#pragma link C++ class HalObjectMatrix_1 + ;
#pragma link C++ class HalObjectMatrix_2 + ;
#pragma link C++ class HalObjectMatrix_3 + ;
#pragma link C++ class HalObjectMatrix_4 + ;

#pragma link C++ class HalArray_1 < Bool_t> + ;
#pragma link C++ class HalArray_1 < Int_t> + ;
#pragma link C++ class HalArray_1 < Short_t> + ;
#pragma link C++ class HalArray_1 < Float_t> + ;
#pragma link C++ class HalArray_1 < Double_t> + ;
#pragma link C++ class HalArray_2 < Bool_t> + ;
#pragma link C++ class HalArray_2 < Int_t> + ;
#pragma link C++ class HalArray_2 < Short_t> + ;
#pragma link C++ class HalArray_2 < Float_t> + ;
#pragma link C++ class HalArray_2 < Double_t> + ;
#pragma link C++ class HalArray_3 < Bool_t> + ;
#pragma link C++ class HalArray_3 < Int_t> + ;
#pragma link C++ class HalArray_3 < Short_t> + ;
#pragma link C++ class HalArray_3 < Float_t> + ;
#pragma link C++ class HalArray_3 < Double_t> + ;
#pragma link C++ class HalArray_4 < Bool_t> + ;
#pragma link C++ class HalArray_4 < Int_t> + ;
#pragma link C++ class HalArray_4 < Short_t> + ;
#pragma link C++ class HalArray_4 < Float_t> + ;
#pragma link C++ class HalArray_4 < Double_t> + ;

#pragma link C++ class HalSpline1D + ;
#pragma link C++ class HalSpline2D + ;
#pragma link C++ class HalSpline3D + ;
#pragma link C++ class HalHelix + ;
#pragma link C++ class HalHelixBase + ;
#pragma link C++ class HalHelixX + ;
#pragma link C++ class HalHelixY + ;
#pragma link C++ class HalHelixZ + ;

#pragma link C++ class HalXMLAttrib + ;
#pragma link C++ class HalXMLNode + ;
#pragma link C++ class HalXMLFile + ;

#pragma link C++ class std::vector < TString> + ;

#pragma link C++ class HalMinimizer + ;
#pragma link C++ class HalFitParam + ;
#pragma link C++ class std::vector < HalFitParam> + ;


//#pragma link C++ class HalParameter+;

#endif
