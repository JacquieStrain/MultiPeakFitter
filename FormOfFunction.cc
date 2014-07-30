#include "FormOfFunction.hh"

FormOfFunction::FormOfFunction(char* whichFit, char* whichFunc){
  if((strcmp(whichFunc,"fit")==0)){
  	sigmaPar0 = 0; sigmaPar1 = 1; sigmaPar2 = 2;
  	tauYintPar = 3; tauSlopePar = 4; 
  	HtailYintPar = 5; HtailSlopePar = 6;
  	muPar = 7; areaPar = 8; HstepPar = 9;
  	bkgdConstPar = 10; bkgdLinPar = 11; bkgdQuadPar = 12; }
  if((strcmp(whichFunc,"centroid")==0) || (strcmp(whichFunc,"tau")==0) || (strcmp(whichFunc,"Htail")==0)){
    sigmaPar0 = 100; sigmaPar1 = 100; sigmaPar2 = 100;
  	tauYintPar = 0; tauSlopePar = 1; 
  	HtailYintPar = 2; HtailSlopePar = 3;
  	muPar = 4; areaPar = 100; HstepPar = 100;
  	bkgdConstPar = 100; bkgdLinPar = 100; bkgdQuadPar = 100; }
  if(strcmp(whichFunc,"sigma")==0){
    sigmaPar0 = 0; sigmaPar1 = 1; sigmaPar2 = 2;
  	tauYintPar = 3; tauSlopePar = 4; 
  	HtailYintPar = 5; HtailSlopePar = 6;
  	muPar = 7; areaPar = 100; HstepPar = 100;
  	bkgdConstPar = 100; bkgdLinPar = 100; bkgdQuadPar = 100;}
    	
  int length = strlen(whichFit);	  
  if( (strncmp(whichFit,"gaus",4)==0) || 
  	  ((strncmp(whichFit,"full_fixedHtail_",16)==0) && (whichFit[length-1]=='0')) ) sprintf(centroidEq,"[%d]", muPar);
  //if(strncmp(whichFit,"full",4)==0){
  else{
	  sprintf(bTerm,"1.+[%d]*[%d]+[%d]*[%d]", tauSlopePar, HtailYintPar, tauYintPar, HtailSlopePar);
	  sprintf(sqrtTerm,"pow(%s,2.) - 4.*[%d]*[%d]*([%d]*[%d]-[%d])", 
						bTerm, HtailSlopePar, tauSlopePar, HtailYintPar, tauYintPar, muPar);
	  sprintf(centroidEq,"(-(%s) + sqrt(%s))/(2.*[%d]*[%d])", bTerm, sqrtTerm, HtailSlopePar, tauSlopePar);
	  }
    
  sprintf(sigma2Eq,"(pow([%d],2.)+%s*pow([%d],2.)+pow(%s*[%d],2.))", 
  						sigmaPar0, centroidEq, sigmaPar1, centroidEq, sigmaPar2);
  sprintf(tauEq,"([%d]+[%d]*%s)", tauYintPar, tauSlopePar, centroidEq);
  sprintf(HtailEq,"([%d]+[%d]*%s)", HtailYintPar, HtailSlopePar, centroidEq);
  
  //sprintf(signalGausConst, "[%d]*TMath::Abs(1.-%s)/(sqrt(2.*%s*TMath::Pi()))", 
  sprintf(signalGausConst, "[%d]*(1.-%s)/(sqrt(2.*%s*TMath::Pi()))",
  						areaPar, HtailEq, sigma2Eq);
  sprintf(signalGaus,"%s*exp(-0.5*pow(x-[%d],2.)/%s)", 
  						signalGausConst, muPar, sigma2Eq); 

  sprintf(signalTailConst, "[%d]*%s/(2.*%s)", areaPar, HtailEq, tauEq);
  sprintf(signalTailExp, "exp((x-[%d])/%s+0.5*%s/pow(%s,2.))", muPar, tauEq, sigma2Eq, tauEq);
  sprintf(signalTailErfc, "TMath::Erfc((x-[%d])/(sqrt(2.0*%s))+sqrt(%s)/(sqrt(2.0)*%s))",
  						muPar, sigma2Eq, sigma2Eq, tauEq);
  sprintf(signalTail,"%s*%s*%s", signalTailConst, signalTailExp, signalTailErfc);
  
  //sprintf(bkgdStep,"[%d]*[%d]*TMath::Erfc((x-[%d])/sqrt(%s*2.))", areaPar, HstepPar, muPar, sigma2Eq);
  sprintf(bkgdStep,"[%d]*TMath::Abs([%d])*TMath::Erfc((x-[%d])/sqrt(%s*2.))", areaPar, HstepPar, muPar, sigma2Eq);
  sprintf(bkgdQuad,"[%d]+[%d]*x+[%d]*pow(x,2.)", bkgdConstPar, bkgdLinPar, bkgdQuadPar);
  sprintf(sigmaEq, "sqrt(%s)", sigma2Eq);
			  
  if(strcmp(whichFunc,"fit")==0){
  	strcpy(function,bkgdQuad);
  	strcat(function,"+");
  	strcat(function,bkgdStep);
  	strcat(function,"+");
  	strcat(function,signalGaus);
  	strcat(function,"+");
  	strcat(function,signalTail);
  	}
  
  if(strcmp(whichFunc,"centroid")==0) strcpy(function,centroidEq);
  if(strcmp(whichFunc,"sigma")==0) strcpy(function,sigmaEq);
  if(strcmp(whichFunc,"tau")==0) strcpy(function,tauEq);
  if(strcmp(whichFunc,"Htail")==0) strcpy(function,HtailEq);
 
  funcSize = strlen(function);
  for(int i=0; i<=funcSize; i++) ffunction[i] = function[i];
  }
  