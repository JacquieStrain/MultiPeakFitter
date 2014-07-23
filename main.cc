//~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
//~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
// March 2014 -		Created by J. MacMullin
//			  		Modified fit functions for MJD use.
//			  		Modified so it can accomodate a dynamic array of peaks
//			  		(Framework provided by L. Moneta.)
//////////////////////////////////////////////////////////////////////////////////////////
// ROOT Forum that the original script was posted to: 
// http://root.cern.ch/phpBB3/viewtopic.php?t=11422&
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

int long long printBranchAddress(std::vector<int> fBranch){
  int long long branchAddress = 0;
  branchAddress += 8*pow(10, fBranch.size()); //start with 8 so leading zeros show
  for(int i=0; i<fBranch.size(); i++) branchAddress += fBranch[i]*pow(10, fBranch.size()-1-i);
  printf("Branch Address: %lld\n", branchAddress);
  return branchAddress;
}
  
int main(){
  
  /*TFile* fileData = TFile::Open("~/30000074_30000093_LinearCalib.root");
  TH1D* PonamaI = (TH1D*)fileData->Get("Ch0"); 
  const int numberPeaksToFit = 4;
  int setOfRanges[] = { 348, 357, 604, 617, 1112, 1130, 1450, 1475 };*/
  /*TFile* fileData = TFile::Open("~/40001929_40002098.root");
  TH1D* PonamaII = (TH1D*)fileData->Get("Ch146"); 
  const int numberPeaksToFit = 5;
  //int setOfRanges[] = { 533, 551, 927, 951, 1710, 1743, 2232, 2270, 4002, 4054 };
  int setOfRanges[] = { 527, 557, 916, 962, 1685, 1767, 2197, 2304, 3931, 4122 }; 
  //int setOfRanges[] = { 440, 470, 527, 557, 916, 962, 1685, 1767, 2197, 2304, 3931, 4122 };  */
  /*TFile* fileData = TFile::Open("~/40001929_40002098.root");
  TH1D* PonamaI = (TH1D*)fileData->Get("Ch148");
  const int numberPeaksToFit = 6; //5;
  //int setOfRanges[] = { 534, 551, 926, 952, 1708, 1746, 2222, 2281, 3985, 4073 };
  int setOfRanges[] = { 446, 463, 533, 551, 927, 951, 1710, 1743, 2232, 2270, 4002, 4054 };*/
  /*TFile* fileData = TFile::Open("~/40001929_40002098.root");
  TH1D* B8717 = (TH1D*)fileData->Get("Ch150"); 
  const int numberPeaksToFit = 6;
  int setOfRanges[] = { 446, 463, 533, 551, 927, 951, 1710, 1743, 2232, 2270, 4002, 4054 };*/
  /*TFile* fileData = TFile::Open("~/source.root");
  TH1D* B8717 = (TH1D*)fileData->Get("hcal"); 
  const int numberPeaksToFit = 3;
  int setOfRanges[] = { 4000, 5500, 5505, 6000, 7800, 9300 };*/
  
  TFile* fileData = TFile::Open("~/MultiPeakFitter/3500_3537/Runs_40003500_40003537.root");
  TH1D* detector = (TH1D*)fileData->Get("Ch112"); 
  const int numberPeaksToFit = 6;
  int setOfRanges[] = { 272, 283, 295, 305, 577, 590, 721, 734, 853, 868, 2600, 2628 };

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

  //int long long stopCommand = 81; 
  int long long stopCommand = 1;
  int long long bAddress;
  std::vector<int> branch, statuses;
  std::vector<double> minFuncVal;
  int status;
  MultiPeakFitter mpf(*detector, numberPeaksToFit, setOfRanges); 
  
  mpf.chooseFit("gaus");
  PrintResults recentResults = mpf.doFit();
  ROOT::Fit::FitResult resultsForTail = mpf.results;
	printf("\t Minimum function value = %.2f\n",resultsForTail.MinFcnValue());
  //status = (resultsForTail.IsValid()) ? ((mpf.minimizerFlag==0) ? 1 : 0) : 0;
  status = (mpf.minimizerFlag==1) ? 0 : resultsForTail.IsValid();
  branch.push_back(status);  
  bAddress = printBranchAddress(branch);
//   if(bAddress == stopCommand){
// 	printf("Fit complete\n");
// 	recentResults.PrintFitResults();
// 	return 1; 
// 	}
  if(branch[0] == 0){
    //printf("The MultiPeakFitter is not able to fit the peaks with a simple gaussian + const bkgd. Exiting.\n");
  	return 0;
  	}
  	
  mpf.previousFitResults(resultsForTail);
  mpf.chooseFit("full_fixedHtail_00");
  recentResults = mpf.doFit();
  ROOT::Fit::FitResult result_hTail00 = mpf.results;
  status = (mpf.minimizerFlag==1) ? 0 : result_hTail00.IsValid();
  statuses.push_back(status);
  minFuncVal.push_back(result_hTail00.MinFcnValue());
    printf("\t Minimum function value = %.2f\n",minFuncVal.back());
    recentResults.PrintFitResults();
  
  mpf.previousFitResults(resultsForTail);
  mpf.chooseFit("full_fixedHtail_02");
  recentResults = mpf.doFit();
  ROOT::Fit::FitResult result_hTail02 = mpf.results;
  status = (mpf.minimizerFlag==1) ? 0 : result_hTail02.IsValid();
  statuses.push_back(status);
  minFuncVal.push_back(result_hTail02.MinFcnValue());
    printf("\t Minimum function value = %.2f\n",minFuncVal.back());
    recentResults.PrintFitResults();

  mpf.previousFitResults(resultsForTail);
  mpf.chooseFit("full_fixedHtail_04");
  recentResults = mpf.doFit();
  ROOT::Fit::FitResult result_hTail04 = mpf.results;
  status = (mpf.minimizerFlag==1) ? 0 : result_hTail04.IsValid();
  statuses.push_back(status);
  minFuncVal.push_back(result_hTail04.MinFcnValue());
    printf("\t Minimum function value = %.2f\n",minFuncVal.back());
    recentResults.PrintFitResults();
    
  mpf.previousFitResults(resultsForTail);
  mpf.chooseFit("full_fixedHtail_06");
  recentResults = mpf.doFit();
  ROOT::Fit::FitResult result_hTail06 = mpf.results;
  status = (mpf.minimizerFlag==1) ? 0 : result_hTail06.IsValid();
  statuses.push_back(status);
  minFuncVal.push_back(result_hTail06.MinFcnValue());
    printf("\t Minimum function value = %.2f\n",minFuncVal.back());
    recentResults.PrintFitResults();
    
  mpf.previousFitResults(resultsForTail);
  mpf.chooseFit("full_fixedHtail_08");
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
  return 1;
  
 /* if(branch[0] == true){
  	mpf.previousFitResults(result0);
  	mpf.chooseFit("full_fixedHtail");
  	}
  else mpf.chooseFit("gaus_fixedQuad");
  recentResults = mpf.doFit();
  ROOT::Fit::FitResult result1 = mpf.results;
    printf("\t Minimum function value = %.2f\n",result1.MinFcnValue());
  status = ((mpf.minimizerFlag==1) || (mpf.hTailFlag==1)) ? 0 : result1.IsValid();
  branch.push_back(status);  
  bAddress = printBranchAddress(branch);
  if(bAddress == stopCommand){
	printf("Fit complete\n");
	recentResults.PrintFitResults();
	return 1; 
	}
	  
  if(branch[1] == true){
  	mpf.previousFitResults(result1);
  	if(branch[0] == true) mpf.chooseFit("full");
	else mpf.chooseFit("full_fixedHtail_fixedQuad_00");
	}
  if((branch[1] == false) && (branch[0] == false)){ 
  	printf("The MultiPeakFitter is not able to fit the peaks with a simple gaussian + const bkgd. Exiting.\n");
  	return 0;
  	}
  if((branch[1] == false) && (branch[0] == true)) mpf.chooseFit("full_fixedHtail_fixedQuad_00");
  recentResults = mpf.doFit();
  ROOT::Fit::FitResult result2 = mpf.results;
    printf("\t Minimum function value = %.2f\n",result2.MinFcnValue());
  status = ((mpf.minimizerFlag==1) || (mpf.hTailFlag==1)) ? 0 : result2.IsValid();
  branch.push_back(status);  
  bAddress = printBranchAddress(branch);
  if(bAddress == stopCommand){
	printf("Fit complete\n");
	recentResults.PrintFitResults();
	return 1; 
	}

  if(branch[2] == true){
  	if((branch[1] == true) && (branch[0] == true)){
  		printf("Fit complete\n");
  		recentResults.PrintFitResults();
  		return 1;
  		}
	else{
		mpf.previousFitResults(result2);
		mpf.chooseFit("full_fixedQuad");
		}
  	}
  if((branch[2] == false) && (branch[1] == true) && (branch[0] == true)) mpf.chooseFit("full_fixedQuad");
  else if(branch[2] == false) mpf.chooseFit("full_fixedHtail_fixedQuad_01");
  recentResults = mpf.doFit();
  ROOT::Fit::FitResult result3 = mpf.results;
    printf("\t Minimum function value = %.2f\n",result3.MinFcnValue());
  status = ((mpf.minimizerFlag==1) || (mpf.hTailFlag==1)) ? 0 : result3.IsValid();
  branch.push_back(status);  
  bAddress = printBranchAddress(branch);
  if(bAddress == stopCommand){
	printf("Fit complete\n");
	recentResults.PrintFitResults();
	return 1; 
	}
	  
  if(branch[3] == true){
  	mpf.previousFitResults(result3);
  	if( (branch[2] == true) ||
  		((branch[2] == false) && (branch[1] == true) && (branch[0] == true)) ){
  		mpf.chooseFit("full");
  		}
	else mpf.chooseFit("full_fixedQuad");
	}
  if( ((branch[3] == false) && (branch[2] == true)) ||
  	  ((branch[3] == false) && (branch[2] == false) && (branch[1] == true) && (branch[0] == true)) ){
  	mpf.chooseFit("full_fixedQuadQuad");
  	}
  else if(branch[3] == false) mpf.chooseFit("full_fixedHtail_fixedQuad_02");
  recentResults = mpf.doFit();
  ROOT::Fit::FitResult result4 = mpf.results;
    printf("\t Minimum function value = %.2f\n",result4.MinFcnValue());
  status = ((mpf.minimizerFlag==1) || (mpf.hTailFlag==1)) ? 0 : result4.IsValid();
  branch.push_back(status);  
  bAddress = printBranchAddress(branch);
  if(bAddress == stopCommand){
	printf("Fit complete\n");
	recentResults.PrintFitResults();
	return 1; 
	}

  if(branch[4] == true){
  	if( ((branch[3] == true) && (branch[2] == true)) || 
  		((branch[3] == true) && (branch[2] == false) && (branch[1] == true) && (branch[0] == true)) ){
  		printf("Fit complete\n");
  		recentResults.PrintFitResults();
  		return 0;
  		}
  	mpf.previousFitResults(result4);
  	if((branch[3] == false) && (branch[2] == false)){
  		if((branch[1] == true) && (branch[0] == true)) mpf.chooseFit("full");
  		else mpf.chooseFit("full_fixedQuad");
  		}
  	else mpf.chooseFit("full");
  	}	
  if(branch[4] == false){
  	if(branch[3] == true) mpf.chooseFit("full_fixedQuadQuad");
  	else mpf.chooseFit("full_fixedHtail_fixedQuad_03");
  	}
  recentResults = mpf.doFit();
  ROOT::Fit::FitResult result5 = mpf.results;
    printf("\t Minimum function value = %.2f\n",result5.MinFcnValue());
  status = ((mpf.minimizerFlag==1) || (mpf.hTailFlag==1)) ? 0 : result5.IsValid();
  branch.push_back(status);  
  bAddress = printBranchAddress(branch);
  if(bAddress == stopCommand){
	printf("Fit complete\n");
	recentResults.PrintFitResults();
	return 1; 
	}
  		
  if(branch[5] == true){
  	if( ((branch[4] == true) && (branch[3] == true)) ||
  		((branch[4] == true) && (branch[3] == false) && (branch[2] == true) && (branch[1] == true) && (branch[0] = false)) ||
  		((branch[4] == true) && (branch[3] == false) && (branch[2] == true) && (branch[1] == false) && (branch[0] = true)) ||
  		((branch[4] == true) && (branch[3] == false) && (branch[2] == false) && (branch[1] == true) && (branch[0] = true)) ){
  		printf("Fit complete\n");
  		recentResults.PrintFitResults();
  		return 0;  
  		}
  	mpf.previousFitResults(result5);
  	if((branch[4] == false) && (branch[3] == false)) mpf.chooseFit("full_fixedQuad");
  	else mpf.chooseFit("full");
  	}
  if(branch[5] == false){
  	if( ((branch[4] == true) && (branch[3] == true)) ||
  		((branch[4] == true) && (branch[3] == false) && (branch[2] == false) && (branch[1] == true) && (branch[0] == false)) ||
  		((branch[4] == true) && (branch[3] == false) && (branch[2] == false) && (branch[1] == false) && (branch[0] == true)) ){
  		mpf.chooseFit("full_fixedQuadQuad");
  		}
  	else{
		printf("Cannot get the fit to converge. Exiting \n");
		return 0; 
		} 	
	}
  recentResults = mpf.doFit();
  ROOT::Fit::FitResult result6 = mpf.results;
    printf("\t Minimum function value = %.2f\n",result6.MinFcnValue());
  status = ((mpf.minimizerFlag==1) || (mpf.hTailFlag==1)) ? 0 : result6.IsValid();
  branch.push_back(status);  
  bAddress = printBranchAddress(branch);
  if(bAddress == stopCommand){
	printf("Fit complete\n");
	recentResults.PrintFitResults();
	return 1; 
	}
	
  if(branch[6] == true){
  	if( ((branch[5] == true) && (branch[4] == false) && (branch[3] == true)) ||
  		((branch[5] == true) && (branch[4] == true)) ){
  		printf("Fit complete\n");
  		recentResults.PrintFitResults();
  		return 0;  
  		}
  	else{
  		mpf.previousFitResults(result6);
  		mpf.chooseFit("full");
  		}
  	}
  if(branch[6] == false){
  	if( ((branch[5] == true) && (branch[4] == true)) ||
  		((branch[5] == true) && (branch[3] == false)) ){
  		mpf.chooseFit("full_fixedQuadQuad");
  		}
  	else{
  		printf("Cannot get the fit to converge. Exiting \n");
		return 0;
		}
	}
  recentResults = mpf.doFit();
  ROOT::Fit::FitResult result7 = mpf.results;
    printf("\t Minimum function value = %.2f\n",result7.MinFcnValue());
  status = ((mpf.minimizerFlag==1) || (mpf.hTailFlag==1)) ? 0 : result7.IsValid();
  branch.push_back(status);  
  bAddress = printBranchAddress(branch); 
  if(bAddress == stopCommand){
	printf("Fit complete\n");
	recentResults.PrintFitResults();
	return 1; 
	} 
	
  if(branch[7] == true){
	if(branch[6] == true){
		printf("Fit complete\n");
		recentResults.PrintFitResults();
		return 0;  
		}  	
	else mpf.chooseFit("full");
	}
  if(branch[7] == false){
  	if((branch[6] == true) && (branch[5] == true)) mpf.chooseFit("full_fixedQuadQuad");
  	else{
		printf("Cannot get the fit to converge. Exiting \n");
		return 0;
		}  
	}
  recentResults = mpf.doFit();
  ROOT::Fit::FitResult result8 = mpf.results;
    printf("\t Minimum function value = %.2f\n",result8.MinFcnValue());
  status = ((mpf.minimizerFlag==1) || (mpf.hTailFlag==1)) ? 0 : result8.IsValid();
  branch.push_back(status);  
  bAddress = printBranchAddress(branch);  	
  if(bAddress == stopCommand){
	printf("Fit complete\n");
	recentResults.PrintFitResults();
	return 1; 
	}
	  
  if(branch[8] == true){
  	if(branch[7] == true){
  		printf("Fit complete\n");
  		recentResults.PrintFitResults();
  		return 0;
  		}
  	else mpf.chooseFit("full");
  	}
  else{
	printf("Cannot get the fit to converge. Exiting \n");
	return 0;
	}  	
  recentResults = mpf.doFit();
  ROOT::Fit::FitResult result9 = mpf.results;
    printf("\t Minimum function value = %.2f\n",result9.MinFcnValue());
  status = ((mpf.minimizerFlag==1) || (mpf.hTailFlag==1)) ? 0 : result9.IsValid();
  branch.push_back(status);  
  bAddress = printBranchAddress(branch);  
  if(bAddress == stopCommand){
	printf("Fit complete\n");
	recentResults.PrintFitResults();
	return 1; 
	}
	  
  if(branch[9] == true){
	printf("Fit complete\n");
	recentResults.PrintFitResults();
	return 0;
	}
  else{
	printf("Cannot get the fit to converge. Exiting \n");
	return 0;
	}  	  */	
  
}
