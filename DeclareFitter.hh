#ifndef DECLAREFITTER_HH
#define DECLAREFITTER_HH

#include <iostream>
#include <vector>
#include <cstddef>
#include <string.h>
#include <stdio.h>
//#include "Fit/FitResult.h"
#include "TFitResult.h"
#include "TH1.h"
#include "TGraph.h"
#include "TF1.h"
#include "TFitResultPtr.h"
#include "Fit/Fitter.h"
#include "TMath.h"
#include "TFile.h"
#include "HFitInterface.h"

class DeclareFitter
{ 
  const int Npar;
  int binWithMax, k, m, numberPeaksToFit;
  int* setOfRanges;

  double lowerFWHM, maxR, minR, sigmaParameter[3], upperFWHM;
  
  std::vector<int> fixedPars, fixedParsA, fixedParsH;

  TF1 *sqrtQuad;
  TFitResultPtr rSigma;
  TGraph *gSigma;
  TH1D* tempHist;  	  
  
  public:
	DeclareFitter( TH1D &histo, int numPeaks, int* ranges );
	DeclareFitter( );
	void setupGaus( ROOT::Fit::FitResult result, std::string whichFit );
	void setupTail( ROOT::Fit::FitResult resultH, std::string whichFitH );
	void setupAll( ROOT::Fit::FitResult resultA, std::string whichFitA );
	ROOT::Fit::Fitter fitter;
};
#endif
