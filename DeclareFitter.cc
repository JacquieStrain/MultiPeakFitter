#include "DeclareFitter.hh"

char section[500] = "~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~";

DeclareFitter::DeclareFitter( TH1D &histo, int numPeaks, int* ranges ) :
  tempHist(&histo), numberPeaksToFit(numPeaks), setOfRanges(ranges), Npar(7+6*numPeaks) {}
	
DeclareFitter::DeclareFitter( ) :
	tempHist(NULL), numberPeaksToFit(0), setOfRanges(NULL), Npar(0) {}
  
void DeclareFitter::setupGaus( ROOT::Fit::FitResult result, std::string whichFit ){ 
  double fpar[Npar];
  
  if(result.IsEmpty()){
    double yintApprox[numberPeaksToFit];
    double peakApprox[numberPeaksToFit];
    double sigmaApprox[numberPeaksToFit];
    double areaApprox[numberPeaksToFit];
  
    minR = setOfRanges[0];
    maxR = setOfRanges[0];
    
    for(int j=0; j<numberPeaksToFit; j++){
      minR = std::min(minR, 1.*setOfRanges[2*j]);
      maxR = std::max(maxR, 1.*setOfRanges[2*j+1]);
      tempHist->GetXaxis()->SetRangeUser(*(setOfRanges+2*j),*(setOfRanges+2*j+1));
      for(int i=0; i<20; i++) yintApprox[j] += tempHist->GetBinContent( tempHist->FindBin(*(setOfRanges+2*j)+i*0.1) );
   	  yintApprox[j] /= 20.;
   	  binWithMax = tempHist->GetMaximumBin();
   	  peakApprox[j] = tempHist->GetXaxis()->GetBinCenter(binWithMax);
   	  upperFWHM = tempHist->GetBinContent(binWithMax); 
   	  lowerFWHM = tempHist->GetBinContent(binWithMax);
   	  k=1; m=1;
   	  while(upperFWHM > (tempHist->GetBinContent(binWithMax)/2.)){
   	    upperFWHM = tempHist->GetBinContent(binWithMax+k);
   		k++;
   		}
   	  while(lowerFWHM > (tempHist->GetBinContent(binWithMax)/2.)){
   		lowerFWHM = tempHist->GetBinContent(binWithMax-m);
   		m++;
   		}
   	  sigmaApprox[j] = ( tempHist->GetXaxis()->GetBinCenter(binWithMax+k) - 
   					     tempHist->GetXaxis()->GetBinCenter(binWithMax-m) )/2.355;
   	  areaApprox[j] = tempHist->GetBinContent(binWithMax)*sqrt(2.*TMath::Pi())*sigmaApprox[j];
   	  printf("Peak %d, peakApprox = %.2f, sigmaApprox = %.2f\n", j, peakApprox[j], sigmaApprox[j]);
  	  }  		
    
      gSigma = new TGraph(numberPeaksToFit, peakApprox, sigmaApprox);
      sqrtQuad = new TF1("sqrtQuad","sqrt(pow([0],2.)+x*pow([1],2.)+pow(x*[2],2.))",minR*0.9,maxR*1.1);
  	    sqrtQuad->SetParameter(0,sigmaApprox[0]/2.);
  	    sqrtQuad->SetParameter(1,0.02);
  	    sqrtQuad->SetParameter(2,0.0002);
      rSigma = gSigma->Fit("sqrtQuad","NSR");
      if(rSigma->IsValid()) for(int i=0; i<3; i++) sigmaParameter[i] = sqrtQuad->GetParameter(i);
      else{
    	  sigmaParameter[0] = sigmaApprox[0]/2.;
    	  sigmaParameter[1] = 0.02;
    	  sigmaParameter[2] = 0.0002;
    	  }
  
      for(int i=0; i<3; i++) fpar[i] =  sigmaParameter[i];
      fpar[3] = 0.13; fpar[4] = 0.00035; //tau
      fpar[5] = 0.999; fpar[6] = -9.*pow(10.,-7.); //Htail
      for(int i=0; i<numberPeaksToFit; i++) {
        fpar[7+i*6] = peakApprox[i]; //mu
        fpar[8+i*6] = areaApprox[i]; //area
        fpar[9+i*6] = 0.005; //Hstep
        fpar[10+i*6] = yintApprox[i]; //yint of quad bkgd
        fpar[11+i*6] = 0.0; //slope of quad bkgd
        fpar[12+i*6] = 0.0; //quad of quad bkgd
    	}
      }
      
  else if(!result.IsEmpty()) for(int i=0; i<Npar; i++) fpar[i] = result.Parameter(i);
  	
  for(int i=0; i<4; i++){
  	fpar[3+i] = 0.;
    fixedPars.push_back(3+i); 
    }
  if(whichFit.compare("gaus")==0){
  	printf("%s\n%s\n \tf(x) = quadratic bgkd + gaussian \n", section, section);
  	printf("\tFixed Parameters: Htail=tau=0\n %s\n%s\n", section, section);
	}
  if(whichFit.compare("gaus_const")==0){
  	printf("%s\n%s\n\tf(x) = constant bgkd + gaussian \n", section, section);
  	printf("\tFixed Parameters: beta=gamma=Htail=tau=0\n %s\n%s\n", section, section);
	for(int i=0; i<numberPeaksToFit; i++) {
		fixedPars.push_back(11+i*6); //beta (slope)
		fixedPars.push_back(12+i*6); //gamma (quad)
		}
	}
  const int arraySize = fixedPars.size();
  fitter.Config().SetParamsSettings(Npar,fpar);
  for(int i=0; i<arraySize; i++) fitter.Config().ParSettings(fixedPars[i]).Fix();
  fixedPars.clear();
  }

void DeclareFitter::setupTail( ROOT::Fit::FitResult resultH, std::string whichFitH){  	
  double fparH[Npar];
  double par5[10];
  for(int i=0; i<10; i++) par5[i]=0.0+0.1*i;
  char tallyFinder[1];
  for(int i=0; i<3; i++) fparH[i] = resultH.Parameter(i); //sigma
  fparH[3] = 0.6*resultH.Parameter(0); // yint of tau = 0.6 * sigma0
  fparH[4] = pow(0.6*resultH.Parameter(1),2.)/(2.*fparH[3]); //slope of tau = ( 0.6 * sigma1 )^2 / ( 2 * yint_of_tau)
  fparH[5] = par5[0]; fparH[6] = -1.0*pow(10.,-6.); //Htail
  for(int i=0; i<numberPeaksToFit; i++){
	fparH[7+i*6] = resultH.Parameter(7+i*6); //mu
	fparH[8+i*6] = resultH.Parameter(8+i*6); //area
 	fparH[9+i*6] = resultH.Parameter(9+i*6); //Hstep
	fparH[10+i*6] = resultH.Parameter(10+i*6); //tempHist->GetBinContent(tempHist->FindBin(*(setOfRanges+2*i))); //alpha (yint)
	fparH[11+i*6] = resultH.Parameter(11+i*6); //0.0; //beta (slope)
	fparH[12+i*6] = resultH.Parameter(12+i*6); //0.0; //gamma (quad)
	}
  
  printf("%s\n%s\n\tf(x) = quadratic bgkd + step bkgd + gaussian + LE tail \n", section, section);
  printf("\tFixed Parameters: Htail's slope and y-int \n");
  fixedParsH.push_back(5); fixedParsH.push_back(6);
  if(whichFitH.compare(0,21,"full_fixedHtail_const")==0){
    printf("\tFixed Parameters: beta=gamma=0 \n");
  	for(int i=0; i<(sizeof(par5)/sizeof(*par5)); i++){
  		sprintf(tallyFinder,"%d",i);
  		if(whichFitH[23] == tallyFinder[0]) fparH[5] = par5[i];
  		}  	
	for(int i=0; i<numberPeaksToFit; i++) {
	    fparH[10+i*6] = tempHist->GetBinContent(tempHist->FindBin(*(setOfRanges+2*i)));
	    fparH[11+i*6] = 0.;
	    fparH[12+i*6] = 0.;
		fixedParsH.push_back(11+i*6); //beta (slope)
		fixedParsH.push_back(12+i*6); //gamma (quad)
		}
	}
  else{
	if(whichFitH.compare(0,22,"full_fixedHtail_linear")==0){
      printf("\tFixed Parameters: gamma=0 \n");
      for(int i=0; i<(sizeof(par5)/sizeof(*par5)); i++){
    	sprintf(tallyFinder,"%d",i);
    	if(whichFitH[24] == tallyFinder[0]) fparH[5] = par5[i];
    	}  	
  	  for(int i=0; i<numberPeaksToFit; i++) {
  	    fparH[10+i*6] = tempHist->GetBinContent(tempHist->FindBin(*(setOfRanges+2*i)));
  	    fparH[11+i*6] = 0.;
  	    fparH[12+i*6] = 0.;
  		fixedParsH.push_back(12+i*6); //gamma (quad)
  		}
  	  }
    else{ //full_fixedHtail_0n w/quad bkgd
  	  for(int i=0; i<(sizeof(par5)/sizeof(*par5)); i++){
  		sprintf(tallyFinder,"%d",i);
  		if(whichFitH[18] == tallyFinder[0]) fparH[5] = par5[i];
  		}  	
	  }
	}
	 
  if(fparH[5]==0.){
    fparH[6] = 0.;
    fparH[3] = 0.; fparH[4] = 0.;
    fixedParsH.push_back(3); fixedParsH.push_back(4);
    printf("\tFixed Parameters: Htail=tau=0 \n");
    }
  printf("%s\n%s\n", section, section);
  
  const int arraySizeH = fixedParsH.size();
  fitter.Config().SetParamsSettings(Npar,fparH);
  //fitter.Config().ParSettings(5).SetLimits(0,1); // 1 > y-int of Htail > 0
  fitter.Config().ParSettings(3).SetLimits(0,1); // 1 > y-int of tau > 0
  for(int i=0; i<arraySizeH; i++) fitter.Config().ParSettings(fixedParsH[i]).Fix();
  fixedParsH.clear();
  }

void DeclareFitter::setupAll( ROOT::Fit::FitResult resultA, std::string whichFitA){
  double fparA[Npar];
  for(int i=0; i<Npar; i++) fparA[i] = resultA.Parameter(i);
  
  printf("%s\n%s\n\tf(x) = quadratic bgkd + step bkgd + gaussian + LE tail \n", section, section);
  if(whichFitA.compare("full")==0) printf("\tFixed Parameters: none \n %s\n%s\n", section, section);
  if(whichFitA.compare("full_const")==0){
    printf("\tFixed Parameters: beta=gamma=0 \n %s\n%s\n", section, section);
    for(int i=0; i<numberPeaksToFit; i++) {
	  fparA[11+i*6] = 0.0; //beta (slope)
	  fparA[12+i*6] = 0.0; //gamma (quad)
	  fixedParsA.push_back(11+i*6); 
	  fixedParsA.push_back(12+i*6); 
	  }
	} 
  if(whichFitA.compare("full_linear")==0){
	printf("\tFixed Parameters: gamma=0 \n %s\n%s\n", section, section);
	for(int i=0; i<numberPeaksToFit; i++) fixedParsA.push_back(12+i*6); //gamma (quad)
	}

  const int arraySizeA = fixedParsA.size();
  fitter.Config().SetParamsSettings(Npar,fparA); //See if fitter.Config().SetFromFitResult(resultA) works
  for(int i=0; i<arraySizeA; i++) fitter.Config().ParSettings(fixedParsA[i]).Fix();
  fixedParsA.clear();
  } 
