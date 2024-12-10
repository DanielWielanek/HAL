#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link off all namespaces;

#pragma link C++ namespace Hal;
#pragma link C++ namespace Hal::Std;
#ifdef __MAKECINT__
#endif

//#pragma link C++ class HalCout+
#pragma link C++ class Hal::Package + ;
#pragma link C++ class Hal::AnaFile + ;
#pragma link C++ class Hal::ParameterDouble + ;
#pragma link C++ class Hal::ParameterFloat + ;
#pragma link C++ class Hal::ParameterInt + ;
#pragma link C++ class Hal::ParameterString + ;
#pragma link C++ class Hal::Parameter + ;
#pragma link C++ class Hal::ParameterUInt + ;
#pragma link C++ class Hal::ParameterULong + ;
#pragma link C++ class Hal::ParameterULong64 + ;
#pragma link C++ class Hal::ParameterBool + ;
#pragma link C++ class Hal::Package2HTML + ;
#pragma link C++ class Hal::PicTree + ;
#pragma link C++ enum Hal::EInfo;
#pragma link C++ enum Hal::ECutUpdate;
#pragma link C++ enum Hal::EFormatDepth;
#pragma link C++ enum Hal::EFormatType;
#pragma link C++ enum Hal::HtmlTableRowClass;
#pragma link C++ class Hal::OptionArray + ;
#pragma link C++ class Hal::UniqueOptions + ;
#pragma link C++ class Hal::OptionConverter + ;
#pragma link C++ class Hal::PackageSql + ;
#pragma link C++ class Hal::Object + ;
#pragma link C++ class Hal::MultiGraph + ;
#pragma link C++ class Hal::Cout + ;
#pragma link C++ class Hal::Pointer < TObject> + ;
#pragma link C++ class Hal::DoublePointer < TObject> + ;

#pragma link C++ class Hal::Link + ;
#pragma link C++ class Hal::Const + ;
#pragma link C++ class Hal::Jobs + ;

#pragma link C++ class Hal::HtmlCore + ;
#pragma link C++ class Hal::HtmlDiv + ;
#pragma link C++ class Hal::HtmlFile + ;
#pragma link C++ class Hal::HtmlObject + ;
#pragma link C++ class Hal::HtmlTable + ;
#pragma link C++ class Hal::HtmlRow + ;
#pragma link C++ class Hal::HtmlCell + ;
#pragma link C++ class Hal::HtmlCellCol + ;
#pragma link C++ class Hal::HtmlCellRow + ;

#pragma link C++ class Hal::DividedHisto1D + ;
#pragma link C++ class Hal::DividedHisto2D + ;
#pragma link C++ class Hal::DividedHisto3D + ;

#pragma link C++ class Hal::ChiSqMap2D + ;

// define Histogram Managers templates
#pragma link C++ class Hal::HistogramAxisConf + ;

#pragma link C++ class Hal::HistogramManager_1_1D < TH1D> + ;
#pragma link C++ class Hal::HistogramManager_1_2D < TH2D> + ;
#pragma link C++ class Hal::HistogramManager_1_3D < TH3D> + ;
#pragma link C++ class Hal::HistogramManager_1_1D < TH1F> + ;
#pragma link C++ class Hal::HistogramManager_1_2D < TH2F> + ;
#pragma link C++ class Hal::HistogramManager_1_3D < TH3F> + ;
#pragma link C++ class Hal::HistogramManager_1_1D < TH1I> + ;
#pragma link C++ class Hal::HistogramManager_1_2D < TH2I> + ;
#pragma link C++ class Hal::HistogramManager_1_3D < TH3I> + ;

#pragma link C++ class Hal::HistogramManager_2_1D < TH1D> + ;
#pragma link C++ class Hal::HistogramManager_2_2D < TH2D> + ;
#pragma link C++ class Hal::HistogramManager_2_3D < TH3D> + ;
#pragma link C++ class Hal::HistogramManager_2_1D < TH1F> + ;
#pragma link C++ class Hal::HistogramManager_2_2D < TH2F> + ;
#pragma link C++ class Hal::HistogramManager_2_3D < TH3F> + ;
#pragma link C++ class Hal::HistogramManager_2_1D < TH1I> + ;
#pragma link C++ class Hal::HistogramManager_2_2D < TH2I> + ;
#pragma link C++ class Hal::HistogramManager_2_3D < TH3I> + ;

#pragma link C++ class Hal::HistogramManager_3_1D < TH1D> + ;
#pragma link C++ class Hal::HistogramManager_3_2D < TH2D> + ;
#pragma link C++ class Hal::HistogramManager_3_3D < TH3D> + ;
#pragma link C++ class Hal::HistogramManager_3_1D < TH1F> + ;
#pragma link C++ class Hal::HistogramManager_3_2D < TH2F> + ;
#pragma link C++ class Hal::HistogramManager_3_3D < TH3F> + ;
#pragma link C++ class Hal::HistogramManager_3_1D < TH1I> + ;
#pragma link C++ class Hal::HistogramManager_3_2D < TH2I> + ;
#pragma link C++ class Hal::HistogramManager_3_3D < TH3I> + ;

#pragma link C++ class Hal::HistogramManager_4_1D < TH1D> + ;
#pragma link C++ class Hal::HistogramManager_4_2D < TH2D> + ;
#pragma link C++ class Hal::HistogramManager_4_3D < TH3D> + ;
#pragma link C++ class Hal::HistogramManager_4_1D < TH1F> + ;
#pragma link C++ class Hal::HistogramManager_4_2D < TH2F> + ;
#pragma link C++ class Hal::HistogramManager_4_3D < TH3F> + ;
#pragma link C++ class Hal::HistogramManager_4_1D < TH1I> + ;
#pragma link C++ class Hal::HistogramManager_4_2D < TH2I> + ;
#pragma link C++ class Hal::HistogramManager_4_3D < TH3I> + ;
#pragma link C++ class Hal::FastHist + ;
#pragma link C++ class Hal::FastHist1D + ;
#pragma link C++ class Hal::FastHist2D + ;
#pragma link C++ class Hal::FastHist3D + ;
#pragma link C++ class Hal::Style + ;
#pragma link C++ class Hal::PadStyle + ;
#pragma link C++ class Hal::MarkerStyle + ;
#pragma link C++ class Hal::LineStyle + ;
#pragma link C++ class Hal::FillStyle + ;
#pragma link C++ class Hal::AxisStyle + ;
#pragma link C++ class Hal::HistoStyle + ;
#pragma link C++ class Hal::CorrelationHisto + ;
#pragma link C++ class Hal::Painter + ;

#pragma link C++ class Hal::ErrorCalc + ;
#pragma link C++ class Hal::FastAxisCalc + ;
#pragma link C++ class Hal::ProfileAna2D + ;

//=============================
#pragma link C++ class Hal::ObjectMatrix_1 + ;
#pragma link C++ class Hal::ObjectMatrix_2 + ;
#pragma link C++ class Hal::ObjectMatrix_3 + ;
#pragma link C++ class Hal::ObjectMatrix_4 + ;

#pragma link C++ class Hal::Array_1 < Bool_t> + ;
#pragma link C++ class Hal::Array_1 < Int_t> + ;
#pragma link C++ class Hal::Array_1 < Short_t> + ;
#pragma link C++ class Hal::Array_1 < Float_t> + ;
#pragma link C++ class Hal::Array_1 < Double_t> + ;
#pragma link C++ class Hal::Array_2 < Bool_t> + ;
#pragma link C++ class Hal::Array_2 < Int_t> + ;
#pragma link C++ class Hal::Array_2 < Short_t> + ;
#pragma link C++ class Hal::Array_2 < Float_t> + ;
#pragma link C++ class Hal::Array_2 < Double_t> + ;
#pragma link C++ class Hal::Array_3 < Bool_t> + ;
#pragma link C++ class Hal::Array_3 < Int_t> + ;
#pragma link C++ class Hal::Array_3 < Short_t> + ;
#pragma link C++ class Hal::Array_3 < Float_t> + ;
#pragma link C++ class Hal::Array_3 < Double_t> + ;
#pragma link C++ class Hal::Array_4 < Bool_t> + ;
#pragma link C++ class Hal::Array_4 < Int_t> + ;
#pragma link C++ class Hal::Array_4 < Short_t> + ;
#pragma link C++ class Hal::Array_4 < Float_t> + ;
#pragma link C++ class Hal::Array_4 < Double_t> + ;

#pragma link C++ class Hal::Spline1D + ;
#pragma link C++ class Hal::Spline2D + ;
#pragma link C++ class Hal::Spline3D + ;
#pragma link C++ class Hal::Helix + ;
#pragma link C++ class Hal::HelixBase + ;
#pragma link C++ class Hal::HelixX + ;
#pragma link C++ class Hal::HelixY + ;
#pragma link C++ class Hal::HelixZ + ;

#pragma link C++ class Hal::XMLAttrib + ;
#pragma link C++ class Hal::XMLNode + ;
#pragma link C++ class Hal::XMLFile + ;


#pragma link C++ class Hal::Minimizer + ;
#pragma link C++ class Hal::FitParam + ;
#pragma link C++ class Hal::MinimizerStepConf + ;
#pragma link C++ class Hal::CompressionMap + ;

// STD STUFF
#pragma link C++ class std::vector < Hal::FitParam> + ;
#pragma link C++ class std::vector < TString> + ;
#pragma link C++ class std::vector < std::vector < TString>> + ;
#pragma link C++ class std::map < TString, Double_t> + ;
#pragma link C++ class std::map < TString, Int_t> + ;
#pragma link C++ class Hal::PackageTable + ;
//#pragma link C++ class HalParameter+;

#endif
