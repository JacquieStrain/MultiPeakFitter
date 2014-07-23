#ifndef MULTIPEAKFITTER_HH
#define MULTIPEAKFITTER_HH

#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cstddef>
#include "TH1.h"
#include "TF1.h"
#include "Math/WrappedMultiTF1.h"
#include "Fit/DataRange.h"
#include "Fit/DataOptions.h"
#include "Fit/BinData.h"
#include "Fit/Chi2FCN.h"
#include "Fit/PoissonLikelihoodFCN.h"
#include "Fit/Fitter.h"
#include "Fit/FitResult.h"
#include "Fit/FitConfig.h"
#include "HFitInterface.h"
#include "Math/Minimizer.h"

#include "SingleMinFunc.hh"
#include "FormOfFunction.hh"
#include "GlobalMinFunc.hh"
#include "DeclareFitter.hh"
#include "PrintResults.hh"

class MultiPeakFitter
{ 
  int numberPeaksToFit;
  int* arrayRanges;
  double minHtail, maxHtail;
  TH1D* detectorHist;
  char fitKind[50];
  char function[10000]; 
  DeclareFitter fFitter;
  ROOT::Fit::FitResult previousResult;
  
  public:
  	MultiPeakFitter( TH1D &fitHisto, int numPeaks, int* fitRanges );
  	void chooseFit(char* fitChoice);
  	void previousFitResults( ROOT::Fit::FitResult pResult );
  	PrintResults doFit();
  	ROOT::Fit::FitResult results;
  	int minimizerFlag;
  	int hTailFlag;
};
#endif