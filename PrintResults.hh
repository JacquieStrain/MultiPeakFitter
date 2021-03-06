#ifndef PRINTRESULTS_HH
#define PRINTRESULTS_HH

#include <stdio.h>
#include <typeinfo>
#include <cstddef>
#include "TF1.h"
#include "Fit/FitResult.h"
#include "TMath.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TList.h"
#include "TGraphErrors.h"
#include "TFile.h"
#include "TMatrixTSym.h"

class PrintResults 
{
  int numberPeaksToFit;
  int* rArray;
 
  double bH, bT, bTerm, canvasWidth;
  double cov_bT_bH, cov_bH_mH, cov_bH_mT, cov_bT_mH, cov_bT_mT, cov_mT_mH;
  double cov_s0_bH, cov_s0_bT, cov_s0_mH, cov_s0_mT, cov_s0_s1, cov_s0_s2;
  double cov_s1_bH, cov_s1_bT, cov_s1_mH, cov_s1_mT, cov_s1_s2;
  double cov_s2_bH, cov_s2_bT, cov_s2_mH, cov_s2_mT;
  double dbH, dbT, dCdbH, dCdbT, dCdmH, dCdmT, dCdMu;
  double dHdbH, dHdbT, dHdmH, dHdmT, dHdMu;
  double dmH, dmT, dNdmH, dNdmT;
  double ds0, ds1, ds2, dSdbH, dSdbT, dSdC, dSdmH, dSdmT, dSdMu, dSds0, dSds1, dSds2;
  double dSVdbH, dSVdbT, dSVdmH, dSVdmT, dSVdMu, dSVds0, dSVds1, dSVds2;
  double dTdbH, dTdbT, dTdmH, dTdmT, dTdMu;
  double htailComp, mH, mT, Numerator, s0, s1, s2, sqrtTerm, tauComp;
  double HalfMaxXFit, highFWHM, lowFWHM, MaxXFit;
 
  char histTitle[10], outputFileName[100]; 
  std::string fitOpt;
  
  ROOT::Fit::FitResult resultToPrint;
  TCanvas *c1;
  TF1 **bkgdQ, **bkgdS, **fArray, *fFWHM, *fHtail, *fSigma, *fTau, **signal, **signalG, **signalT;
  TFile *rootResults;
  TGraphErrors *gFWHM, *gHstep, *gHtail, *gSigma, *gSqrtVar, *gTau;
  TH1D **hArray, *hPar, *hParErr;
  
  public:
	PrintResults( std::string ffitOpt, ROOT::Fit::FitResult r2p, TH1D* histArray[], int* rangeArray );
	PrintResults ( );
	void PrintFitResults();
};
#endif