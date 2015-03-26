#include "MultiPeakFitter.hh"

MultiPeakFitter::MultiPeakFitter( TH1D &fithisto, int numPeaks, int* fitranges ) :
	detectorHist(&fithisto), numberPeaksToFit(numPeaks), arrayRanges(fitranges) {}

void MultiPeakFitter::chooseFit( std::string fitChoice){
  fitKind=fitChoice;
  FormOfFunction fOF(fitKind,"fit");
  strcpy(function,fOF.ffunction); 
  }	 
  
void MultiPeakFitter::previousFitResults( ROOT::Fit::FitResult pResult ){
	previousResult = pResult;
	}
	
PrintResults MultiPeakFitter::doFit(int finalFit, int sigmaContours){

  TH1D** histo = new TH1D*[numberPeaksToFit];
  TF1** ff = new TF1*[numberPeaksToFit];
  std::vector<ROOT::Math::WrappedMultiTF1> wfs;
  std::vector<ROOT::Fit::DataRange> ranges(numberPeaksToFit);
  ROOT::Fit::DataOptions opt;
  std::vector<ROOT::Fit::BinData> dataSets;

  ROOT::Fit::PoissonLLFunction** lls = new ROOT::Fit::PoissonLLFunction*[numberPeaksToFit];
  ROOT::Fit::Chi2Function** cqs = new ROOT::Fit::Chi2Function*[numberPeaksToFit];
  
  SingleMinFunc *smf_ll = new SingleMinFunc[numberPeaksToFit]; 
  SingleMinFunc *smf_cq = new SingleMinFunc[numberPeaksToFit]; 
  
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
  for(int pkN=0; pkN<numberPeaksToFit; pkN++){
  	lls[pkN] = (ROOT::Fit::PoissonLLFunction*)new ROOT::Fit::PoissonLLFunction(dataSets[pkN],wfs[pkN]);
  	cqs[pkN] = (ROOT::Fit::Chi2Function*)new ROOT::Fit::Chi2Function(dataSets[pkN],wfs[pkN]);
  	}
   
  //ROOT::Math::IMultiGenFunction * fMF_i cannot be put into an array, so the class
  //SingleMinFunc defines fMF_i for each peak and we create an array of SingleMinFunc.
  for(int pkN=0; pkN<numberPeaksToFit; pkN++){
	smf_ll[pkN] = SingleMinFunc(*lls[pkN], pkN);
	smf_cq[pkN] = SingleMinFunc(*cqs[pkN], pkN);
	}
  
  //GlobalMinFunc is a friend of SingleMinFunc so it will be able to access each peak's fMF_i
  //to compute the minimization function.
  GlobalMinFunc globalLL(smf_ll, numberPeaksToFit);
  GlobalMinFunc globalCQ(smf_cq, numberPeaksToFit);
  
  DeclareFitter fFitter(*detectorHist, numberPeaksToFit, arrayRanges);  
  if(fitKind.compare(0,4,"gaus")==0) fFitter.setupGaus(previousResult, fitKind);
  if(fitKind.compare(0,15,"full_fixedHtail")==0) fFitter.setupTail(previousResult, fitKind); 
  else if(fitKind.compare(0,4,"full")==0) fFitter.setupAll(previousResult, fitKind);
  const int Npar = fFitter.fitter.Config().NPar();
  fFitter.fitter.Config().MinimizerOptions().SetPrintLevel(1);
  if(finalFit==1) fFitter.fitter.Config().SetMinimizer("Minuit","Minos"); 
  else fFitter.fitter.Config().SetMinimizer("Minuit","Migrad"); 
  fFitter.fitter.FitFCN(Npar,globalLL,0,dataSize,false); // fit FCN function directly 
  
  minimizer = fFitter.fitter.GetMinimizer();
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
    
  if(sigmaContours==1){
    minimizer->SetPrintLevel(1);
    contourResults = new TFile("mu_contours.root","recreate");
    const int nPairs = 13;
    std::pair<int,int> parIJ[nPairs];
    int aCounter = 0; 
    for(int i=0; i<7; i++){
      parIJ[aCounter] = std::make_pair(13,i);
      aCounter++;
      }
    for(int i=14; i<20; i++){
      parIJ[aCounter] = std::make_pair(13,i);
      aCounter++;
      }
    unsigned int np = 80;
    double *xI = new double[np];
    double *xJ = new double[np];
    TGraph *grIJ, *bestFit;
    bool boolContour[2];
    for(int i=0; i<nPairs; i++){ 
      printf("\nDrawing sigma contours for par %d and %d\n", parIJ[i].first, parIJ[i].second);
      for(int sigma=2; sigma>0; sigma--){
        printf("%d-sigma\n",sigma);
		minimizer->SetErrorDef(pow(sigma,2.));
    	boolContour[sigma] = minimizer->Contour(parIJ[i].first,parIJ[i].second,np,xI,xJ);
        if(boolContour[sigma]==1) {
      	  grIJ = new TGraph(np,xI,xJ);
          grIJ->SetFillColor(38-2*(sigma-1)); //36=dark blue, 38=light blue
          grIJ->Write(Form("par%d_par%d_sigma%d",parIJ[i].first,parIJ[i].second,sigma));
          }
        if(minimizer->Status()!=0) printf("\n\n\n\n MINIMIZER STATUS = %d \n\n\n\n",minimizer->Status());
	    }
      bestFit = new TGraph(1);
      bestFit->SetPoint(0,results.Parameter(parIJ[i].first),results.Parameter(parIJ[i].second));
      bestFit->SetMarkerStyle(3);
      bestFit->SetMarkerColor(46); //salmon
      bestFit->Write(Form("par%d_par%d_bestFit",parIJ[i].first,parIJ[i].second));
      }
    contourResults->Close();
    }   
  
  PrintResults pfr(fitKind, results, histo, arrayRanges);
  return pfr;
  }
  