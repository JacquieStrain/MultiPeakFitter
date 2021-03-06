//~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
//~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
// March 2014 -		Created by J. MacMullin
//			  		Modified fit functions for MJD use.
//			  		Modified so it can accomodate a dynamic array of peaks
//			  		(Framework provided by L. Moneta.)
//////////////////////////////////////////////////////////////////////////////////////////
// ROOT Forum that the original script was posted to: 
// http://root.cern.ch/phpBB3/viewtopic.php?t=11422&
// Tutorial: http://root.cern.ch/root/html/tutorials/fit/combinedFit.C.html
//
// Original Documentation:
//+ Combined (simultaneous) fit of two histogram with separate functions 
//  and some common parameters
// See http://root.cern.ch/phpBB3//viewtopic.php?f=3&t=11740#p50908
// for a modified version working with Fumili or GSLMultiFit 
// N.B. this macro must be compiled with ACliC 
// Author: L. Moneta - Dec 2010 
//~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
//~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*

#include "MultiPeakFitter.hh"
#include "DeclareFitter.hh"
#include <getopt.h>
#include "TKey.h"

int long long printBranchAddress(std::vector<int> fBranch){
  int long long branchAddress = 0;
  branchAddress += 8*pow(10, fBranch.size()); //start with 8 so leading zeros show
  for(int i=0; i<fBranch.size(); i++) branchAddress += fBranch[i]*pow(10, fBranch.size()-1-i);
  printf("Branch Address: %lld\n", branchAddress);
  return branchAddress;
}

int main(int argc, char *argv[]){

  int oVal, index;
  int channel, chooseHtail, printAll=0;
  while( (oVal=getopt(argc, argv,"ac:h:")) != -1 )
    switch (oVal){
      case 'c':
        channel = atoi(optarg);
        break;
      case 'h':
        chooseHtail = atoi(optarg);
        break;
      case 'a':
        printAll=1;
        break;
      case '?':
        if((optopt == 'c') || (optopt == 'h')){
          printf("Option -%c requires an argument. Usage:\n", optopt);
          printf(" -c [channel]\n -h [hTail value * 10] ('-h 999' = test for Htail value)\n");
          }
        else if(isprint(optopt))
          printf("Unknown option '-%c'\n",optopt);
        else
          printf("Unknown option character '\\x%x'\n",optopt);
        return 1;
      default:
        abort ();
      }
  for(index=optind; index<argc; index++)
	printf("Non-option argument %s\n",argv[index]);

  printf("channel = %d\n",channel);
  printf("hTail = %d\n",chooseHtail);
  printf("Printing sigma contours? : %s\n",(printAll==1)?"yes":"no"); 
  if(chooseHtail==999) printf("Testing for Htail values\n");
  else printf("Initializing Htail = %.1f\n",chooseHtail/10.);
  
  TFile* fileData = TFile::Open("~/SURF_Data/surfprot/Runs_4126_4155_trapEcuts.root");
  TH1D* detector = (TH1D*)fileData->Get( Form("Ch%d",channel) ); 
  const int numberPeaksToFit = 5;
  //int setOfRanges[] = { 270, 283, 293, 306, 572, 593, 852, 869, 2598, 2632 };
  int pkApproxAt[numberPeaksToFit] = { 277, 300, 583, 860, 2614 };
  int setOfRanges[numberPeaksToFit*2];
  
  TH1D *hRange = (TH1D*)detector->Clone("h_Range");
  hRange->SetDirectory(0);
  int delta = 0;
  int maxBin[numberPeaksToFit];
  double upperHM[numberPeaksToFit], lowerHM[numberPeaksToFit], sigmaApprox[numberPeaksToFit];
  for(int i=0; i<numberPeaksToFit; i++){
    hRange->GetXaxis()->SetRangeUser( pkApproxAt[i]*0.99, pkApproxAt[i]*1.01 );
    maxBin[i] = hRange->GetMaximumBin();
    while( hRange->GetBinContent(maxBin[i]+delta) > (0.5*hRange->GetBinContent(maxBin[i])) ) delta++;
    upperHM[i] = ( hRange->GetXaxis()->GetBinCenter(maxBin[i]+delta) + hRange->GetXaxis()->GetBinCenter(maxBin[i]+delta-1) )/2.;
    delta = 0;
    while( hRange->GetBinContent(maxBin[i]-delta) > (0.5*hRange->GetBinContent(maxBin[i])) ) delta++;
    lowerHM[i] = ( hRange->GetXaxis()->GetBinCenter(maxBin[i]-delta) + hRange->GetXaxis()->GetBinCenter(maxBin[i]-delta+1) )/2.;    
    sigmaApprox[i] = (upperHM[i]-lowerHM[i])/(2.*sqrt(2.*log(2)));
    setOfRanges[2*i] = pkApproxAt[i]-10.*sigmaApprox[i];
    setOfRanges[2*i+1] = pkApproxAt[i]+10.*sigmaApprox[i];
    }
    
  if((sizeof(setOfRanges)/sizeof(*setOfRanges))/2. != numberPeaksToFit){ 
  	printf("setOfRanges does not match number of peaks. Exiting \n");
  	return 0;
  	}
  printf("File: %s\n", fileData->GetName());
  printf("Histo: %s\n", detector->GetName()); 
  printf("Ranges: ");
  for(int i=0; i<numberPeaksToFit; i++){
  	if(i==(numberPeaksToFit-1)) printf("%d-%d\n", setOfRanges[2*i], setOfRanges[2*i+1]);
  	else printf("%d-%d, ", setOfRanges[2*i], setOfRanges[2*i+1]);
  	}

  int long long bAddress;
  std::vector<int> branch, statuses;
  std::vector<double> minFuncVal;
  int status;
  MultiPeakFitter mpf(*detector, numberPeaksToFit, setOfRanges); 

  char hTailChoice[100]; 
  sprintf(hTailChoice,"full_fixedHtail_linear_%02d",chooseHtail);
  
  mpf.chooseFit("gaus");
  PrintResults recentResults = mpf.doFit();
  ROOT::Fit::FitResult resultsForTail = mpf.results;
	printf("\t Minimum function value = %.2f\n",resultsForTail.MinFcnValue());
  status = (mpf.minimizerFlag==1) ? 0 : resultsForTail.IsValid();
  branch.push_back(status);  
  bAddress = printBranchAddress(branch);
  
  if(branch[0] == 0){
    mpf.chooseFit("gaus_const");
    recentResults = mpf.doFit();
    resultsForTail = mpf.results;
    status = (mpf.minimizerFlag==1) ? 0 : resultsForTail.IsValid();
    branch.push_back(status);  
    bAddress = printBranchAddress(branch);
    if(branch[1] == 0){
      recentResults.PrintFitResults();
      return 1;
      }
    }
    
  if(chooseHtail!=999){
    if(chooseHtail==0){
      printf("Accepting full_fixedHtail_00. Doing final fit with Minos\n");
      mpf.previousFitResults(resultsForTail);
      mpf.chooseFit("full_fixedHtail_00");	
	  recentResults = mpf.doFit(1,printAll);
	  ROOT::Fit::FitResult resultFinal = mpf.results;
        printf("\t Minimum function value = %.2f\n",resultFinal.MinFcnValue());
      status = (mpf.minimizerFlag==1) ? 0 : resultFinal.IsValid();
      if(status==1) recentResults.PrintFitResults();
      return 0;
	  }
    
    mpf.previousFitResults(resultsForTail);
    mpf.chooseFit(hTailChoice);
    recentResults = mpf.doFit();
    ROOT::Fit::FitResult result1 = mpf.results;
      printf("\t Minimum function value = %.2f\n",result1.MinFcnValue());
    status = (mpf.minimizerFlag==1) ? 0 : result1.IsValid();
    branch.push_back(status);
    bAddress = printBranchAddress(branch);
    if(branch[1]==0) return 1;

    mpf.previousFitResults(result1);
	mpf.chooseFit("full_linear");
	recentResults = mpf.doFit();
	ROOT::Fit::FitResult result2 = mpf.results;
	  printf("\t Minimum function value = %.2f\n",result2.MinFcnValue());
	status = (mpf.minimizerFlag==1) ? 0 : result2.IsValid();
	branch.push_back(status);
	bAddress = printBranchAddress(branch);
      
	if(branch[2]==0){
	  mpf.previousFitResults(result1);
	  mpf.chooseFit("full_const");        
	  }
	else{
	  mpf.previousFitResults(result2);
	  mpf.chooseFit("full");         
	  }
	recentResults = mpf.doFit();  
	ROOT::Fit::FitResult result3 = mpf.results;
	  printf("\t Minimum function value = %.2f\n",result3.MinFcnValue());
	status = (mpf.minimizerFlag==1) ? 0 : result3.IsValid();
	branch.push_back(status);
	bAddress = printBranchAddress(branch);
      
	if(branch[2]==1){ 
	  if(branch[3]==1){
		printf("Accepting full. Doing final fit with Minos\n");
		mpf.previousFitResults(result3);
		mpf.chooseFit("full");
		}
	  if(branch[3]==0){
		printf("Accepting full_linear. Doing final fit with Minos\n");
		mpf.previousFitResults(result2);
		mpf.chooseFit("full_linear");
        }
      recentResults = mpf.doFit(1,printAll);  
	  ROOT::Fit::FitResult resultFinal = mpf.results;
        printf("\t Minimum function value = %.2f\n",resultFinal.MinFcnValue());
      status = (mpf.minimizerFlag==1) ? 0 : resultFinal.IsValid();
      if(status==1) recentResults.PrintFitResults();
      return 0;
	  }
	if(branch[3]==0) return 1;
	mpf.previousFitResults(result3);
	mpf.chooseFit("full_linear");
	recentResults = mpf.doFit();
	ROOT::Fit::FitResult result4 = mpf.results;
	  printf("\t Minimum function value = %.2f\n",result4.MinFcnValue());
	status = (mpf.minimizerFlag==1) ? 0 : result4.IsValid();
	branch.push_back(status);
	bAddress = printBranchAddress(branch);	
	
	if(branch[4]==0){
	  printf("Accepting full_const. Doing final fit with Minos\n");
	  mpf.previousFitResults(result3);
	  mpf.chooseFit("full_const");
	  recentResults = mpf.doFit(1,printAll);
      ROOT::Fit::FitResult resultFinal = mpf.results;
        printf("\t Minimum function value = %.2f\n",resultFinal.MinFcnValue());
      status = (mpf.minimizerFlag==1) ? 0 : resultFinal.IsValid();
      if(status==1) recentResults.PrintFitResults();
	  return 0;
	  }
	mpf.previousFitResults(result4);
	mpf.chooseFit("full");
	recentResults = mpf.doFit();
	ROOT::Fit::FitResult result5 = mpf.results;
	  printf("\t Minimum function value = %.2f\n",result5.MinFcnValue());
	status = (mpf.minimizerFlag==1) ? 0 : result5.IsValid();
	branch.push_back(status);
	bAddress = printBranchAddress(branch);		
	
	if(branch[5]==0){
	  printf("Accepting full_linear. Doing final fit with Minos\n");
	  mpf.previousFitResults(result4);
	  mpf.chooseFit("full_linear");
	  }
	else{
	  printf("Accepting full. Doing final fit with Minos\n");
	  mpf.previousFitResults(result5);
	  mpf.chooseFit("full");
	  }
    recentResults = mpf.doFit(1,printAll);  
    ROOT::Fit::FitResult resultFinal = mpf.results;
	  printf("\t Minimum function value = %.2f\n",resultFinal.MinFcnValue());
	status = (mpf.minimizerFlag==1) ? 0 : resultFinal.IsValid();
    if(status==1) recentResults.PrintFitResults();
	return 0;  
    }
  
  recentResults.PrintFitResults();
  
  mpf.previousFitResults(resultsForTail);
  mpf.chooseFit("full_fixedHtail_linear_00");
  recentResults = mpf.doFit();
  ROOT::Fit::FitResult result_hTail00 = mpf.results;
  status = (mpf.minimizerFlag==1) ? 0 : result_hTail00.IsValid();
  statuses.push_back(status);
  minFuncVal.push_back(result_hTail00.MinFcnValue());
    printf("\t Minimum function value = %.2f\n",minFuncVal.back());
    recentResults.PrintFitResults();
  
  mpf.previousFitResults(resultsForTail);
  mpf.chooseFit("full_fixedHtail_linear_02");
  recentResults = mpf.doFit();
  ROOT::Fit::FitResult result_hTail02 = mpf.results;
  status = (mpf.minimizerFlag==1) ? 0 : result_hTail02.IsValid();
  statuses.push_back(status);
  minFuncVal.push_back(result_hTail02.MinFcnValue());
    printf("\t Minimum function value = %.2f\n",minFuncVal.back());
    recentResults.PrintFitResults();

  mpf.previousFitResults(resultsForTail);
  mpf.chooseFit("full_fixedHtail_linear_04");
  recentResults = mpf.doFit();
  ROOT::Fit::FitResult result_hTail04 = mpf.results;
  status = (mpf.minimizerFlag==1) ? 0 : result_hTail04.IsValid();
  statuses.push_back(status);
  minFuncVal.push_back(result_hTail04.MinFcnValue());
    printf("\t Minimum function value = %.2f\n",minFuncVal.back());
    recentResults.PrintFitResults();
    
  mpf.previousFitResults(resultsForTail);
  mpf.chooseFit("full_fixedHtail_linear_06");
  recentResults = mpf.doFit();
  ROOT::Fit::FitResult result_hTail06 = mpf.results;
  status = (mpf.minimizerFlag==1) ? 0 : result_hTail06.IsValid();
  statuses.push_back(status);
  minFuncVal.push_back(result_hTail06.MinFcnValue());
    printf("\t Minimum function value = %.2f\n",minFuncVal.back());
    recentResults.PrintFitResults();
    
  mpf.previousFitResults(resultsForTail);
  mpf.chooseFit("full_fixedHtail_linear_08");
  recentResults = mpf.doFit();
  ROOT::Fit::FitResult result_hTail08 = mpf.results;
  status = (mpf.minimizerFlag==1) ? 0 : result_hTail08.IsValid();
  statuses.push_back(status);
  minFuncVal.push_back(result_hTail08.MinFcnValue());
    printf("\t Minimum function value = %.2f\n",minFuncVal.back());
    recentResults.PrintFitResults();
  
  printf("~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*\n");
  for(int i=0; i<statuses.size(); i++)
    printf("hTail = %.1f \t status = %d \t minFuncVal = %.1f\n", 0.0+0.2*i, statuses[i], minFuncVal[i]);
  return 0;
  
}
