#include "MultiPeakFitter.hh"

MultiPeakFitter::MultiPeakFitter( TH1D &fithisto, int numPeaks, int* fitranges ) :
	detectorHist(&fithisto), numberPeaksToFit(numPeaks), arrayRanges(fitranges) {}

void MultiPeakFitter::chooseFit( char* fitChoice ){
  int length = strlen(fitChoice);
  for(int i=0; i<(length+1); i++) fitKind[i] = fitChoice[i];
  FormOfFunction fOF(fitKind,"fit");
  strcpy(function,fOF.ffunction); 
  }	 
  
void MultiPeakFitter::previousFitResults( ROOT::Fit::FitResult pResult ){
	previousResult = pResult;
	}
	
PrintResults MultiPeakFitter::doFit(){

  TH1D** histo = new TH1D*[numberPeaksToFit];
  TF1** ff = new TF1*[numberPeaksToFit];
  std::vector<ROOT::Math::WrappedMultiTF1> wfs;
  std::vector<ROOT::Fit::DataRange> ranges(numberPeaksToFit);
  ROOT::Fit::DataOptions opt;
  std::vector<ROOT::Fit::BinData> dataSets;

  ROOT::Fit::PoissonLLFunction** lls = new ROOT::Fit::PoissonLLFunction*[numberPeaksToFit];
  ROOT::Fit::Chi2Function** cqs = new ROOT::Fit::Chi2Function*[numberPeaksToFit];
  
  SingleMinFunc smf_ll[numberPeaksToFit];
  SingleMinFunc smf_cq[numberPeaksToFit];
  
  for(int pkN=0; pkN<numberPeaksToFit; pkN++){
  	//create a histogram for each pk that is to be fit
  	histo[pkN] = (TH1D*)detectorHist->Clone();
  	histo[pkN]->SetDirectory(0);
  	histo[pkN]->GetXaxis()->SetRangeUser(*(arrayRanges+2*pkN),*(arrayRanges+2*pkN+1));

  	//create a function for each pk that is to be fit
	ff[pkN] = new TF1("",function,*(arrayRanges+2*pkN),*(arrayRanges+2*pkN+1));

  	//create a WrappedMultiTF1 for each pk that is to be fit
  	//inputs: (pointer to the TF1 for the peak, 1 is because it is 1D (x only))
  	wfs.push_back( ROOT::Math::WrappedMultiTF1(*ff[pkN],1) );

  	//create "BinData" for each pk with a range equal to the pk region
  	ranges[pkN].SetRange(*(arrayRanges+2*pkN),*(arrayRanges+2*pkN+1));
  	dataSets.push_back( ROOT::Fit::BinData(opt,ranges[pkN]) );
  	}
  
  //fill the "BinData" with the histogram values and find total data size
  int dataSize = 0;		
  for(int pkN=0; pkN<numberPeaksToFit; pkN++){
  	ROOT::Fit::FillData(dataSets[pkN],histo[pkN]);
  	dataSize += dataSets[pkN].Size();
  	}
  
  //PoisonLLFunction and Chi2Function for each peak that is to be fit
  for(int i=0; i<numberPeaksToFit; i++){
  	lls[i] = (ROOT::Fit::PoissonLLFunction*)new ROOT::Fit::PoissonLLFunction(dataSets[i],wfs[i]);
  	cqs[i] = (ROOT::Fit::Chi2Function*)new ROOT::Fit::Chi2Function(dataSets[i],wfs[i]);
  	}
   
  //ROOT::Math::IMultiGenFunction * fMF_i cannot be put into an array, so the class
  //SingleMinFunc defines fMF_i for each peak and we create an array of SingleMinFunc.
  for(int i=0; i<numberPeaksToFit; i++){
	smf_ll[i] = SingleMinFunc(*lls[i], i);
	smf_cq[i] = SingleMinFunc(*cqs[i], i);
	}
  
  //GlobalMinFunc is a friend of SingleMinFunc so it will be able to access each peak's fMF_i
  //to compute the minimization function.
  GlobalMinFunc globalLL(smf_ll, numberPeaksToFit);
  GlobalMinFunc globalCQ(smf_cq, numberPeaksToFit);
  
  DeclareFitter fFitter(*detectorHist, numberPeaksToFit, arrayRanges);  
  if(strncmp(fitKind,"gaus",4)==0) fFitter.setupGaus(previousResult, fitKind);
  if(strncmp(fitKind,"full_fixedHtail",15)==0) fFitter.setupTail(previousResult, fitKind); 
  else if(strncmp(fitKind,"full",4)==0) fFitter.setupAll(previousResult, fitKind);
  const int Npar = fFitter.fitter.Config().NPar();
  fFitter.fitter.Config().MinimizerOptions().SetPrintLevel(1);
  //fitter.Config().SetMinimizer("Minuit2","Migrad"); //original but getting error on it - look into this 
  fFitter.fitter.Config().SetMinimizer("Minuit","Migrad"); 
  //fFitter.fitter.Config().SetMinimizer("Minuit","Simplex"); 
  fFitter.fitter.FitFCN(Npar,globalLL,0,dataSize,false); // fit FCN function directly 
  
  ROOT::Math::Minimizer* minimizer = fFitter.fitter.GetMinimizer();
  if(minimizer->CovMatrixStatus() == 3) minimizerFlag = 0;
  else{
  	printf("Problem with second-derivative/covariance matrix: ");
  	if(minimizer->CovMatrixStatus() == 0) printf("It was not calculated. ");
  	else if(minimizer->CovMatrixStatus() == 1) printf("It was approximated. ");
  	else if(minimizer->CovMatrixStatus() == 2) printf("It was forced positive. ");
  	printf("Rejecting Fit.\n");  
	minimizerFlag = 1;
	}
	
  results = fFitter.fitter.Result();
  results.Print(std::cout);
  minHtail = results.Parameter(5);
  maxHtail = results.Parameter(5)+results.Parameter(6)*5000.;
  hTailFlag = 0;
  if( (minHtail<0.) || (minHtail>1.) ){
    hTailFlag = 1;
    printf("min hTail value = %.2f. Rejecting Fit.\n", minHtail);
    }
 if( (maxHtail<0.) || (maxHtail>1.) ){
    hTailFlag = 1;
    printf("max hTail value = %.2f. Rejecting Fit.\n", maxHtail);
    }
    	
  PrintResults pfr(fitKind, results, histo, ff, arrayRanges);
  return pfr;
  }
  