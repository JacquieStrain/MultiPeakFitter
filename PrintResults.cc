#include "PrintResults.hh"

PrintResults::PrintResults( std::string ffitOpt, ROOT::Fit::FitResult r2p, TH1D* histArray[], int* rangeArray ){
  resultToPrint = r2p;
  numberPeaksToFit = (resultToPrint.NPar()-7)/6;  
  hArray = histArray;
  rArray = rangeArray;
  fitOpt=ffitOpt;
  s0 = resultToPrint.Parameter(0); ds0 = resultToPrint.ParError(0);
  s1 = resultToPrint.Parameter(1); ds1 = resultToPrint.ParError(1);
  s2 = resultToPrint.Parameter(2); ds2 = resultToPrint.ParError(2);
  bT = resultToPrint.Parameter(3); dbT = resultToPrint.ParError(3);
  mT = resultToPrint.Parameter(4); dmT = resultToPrint.ParError(4);
  bH = resultToPrint.Parameter(5); dbH = resultToPrint.ParError(5);
  mH = resultToPrint.Parameter(6); dmH = resultToPrint.ParError(6);
  cov_s0_s1 = resultToPrint.CovMatrix(0,1);
  cov_s0_s2 = resultToPrint.CovMatrix(0,2);
  cov_s0_bT = resultToPrint.CovMatrix(0,3);
  cov_s0_bH = resultToPrint.CovMatrix(0,5);
  cov_s0_mT = resultToPrint.CovMatrix(0,4);
  cov_s0_mH = resultToPrint.CovMatrix(0,6);
  cov_s1_s2 = resultToPrint.CovMatrix(1,2);
  cov_s1_bT = resultToPrint.CovMatrix(1,3);
  cov_s1_bH = resultToPrint.CovMatrix(1,5);
  cov_s1_mT = resultToPrint.CovMatrix(1,4);
  cov_s1_mH = resultToPrint.CovMatrix(1,6);
  cov_s2_bT = resultToPrint.CovMatrix(2,3);
  cov_s2_bH = resultToPrint.CovMatrix(2,5);
  cov_s2_mT = resultToPrint.CovMatrix(2,4); 
  cov_s2_mH = resultToPrint.CovMatrix(2,6);  
  cov_bT_bH = resultToPrint.CovMatrix(3,5); 
  cov_bT_mT = resultToPrint.CovMatrix(3,4); 
  cov_bT_mH = resultToPrint.CovMatrix(3,6);
  cov_bH_mT = resultToPrint.CovMatrix(5,4);
  cov_bH_mH = resultToPrint.CovMatrix(5,6);
  cov_mT_mH = resultToPrint.CovMatrix(4,6);  
  }	 

PrintResults::PrintResults( ){
  numberPeaksToFit = 0;
  hArray = NULL;
  fArray = NULL;
  rArray = NULL;
  ROOT::Fit::FitResult nullFit;
  resultToPrint = nullFit;
  //fitOpt[0] = '\0';
  s0 = s1 = s2 = bT = mT = bH = mH = 0;
  ds0 = ds1 = ds2 = dbT = dmT = dbH = dmH = 0;
  cov_s0_s1 = cov_s0_s2 = cov_s0_bT = cov_s0_bH = cov_s0_mT = cov_s0_mH = 0;
  cov_s1_s2 = cov_s1_bT = cov_s1_bH = cov_s1_mT = cov_s1_mH = 0;
  cov_s2_bT = cov_s2_bH = cov_s2_mT = cov_s2_mH = 0;
  cov_bT_bH = cov_bT_mT = cov_bT_mH = 0;
  cov_bH_mT = cov_bH_mH = 0;
  cov_mT_mH = 0;
  }  

void PrintResults::PrintFitResults(){

  char subsection[500] = "---------------------------------------------------------------";
  double alpha[numberPeaksToFit], beta[numberPeaksToFit], gamma[numberPeaksToFit];
  double Mu[numberPeaksToFit], dMu[numberPeaksToFit];
  double Area[numberPeaksToFit], dArea[numberPeaksToFit];
  double Hstep[numberPeaksToFit], dHstep[numberPeaksToFit];
  double cov_s0_mu[numberPeaksToFit], cov_s1_mu[numberPeaksToFit], cov_s2_mu[numberPeaksToFit];
  double cov_bT_mu[numberPeaksToFit], cov_bH_mu[numberPeaksToFit];
  double cov_mT_mu[numberPeaksToFit], cov_mH_mu[numberPeaksToFit];
  double FWHM[numberPeaksToFit], dFWHM[numberPeaksToFit];
  for(int i=0; i<numberPeaksToFit; i++){
  	alpha[i] = resultToPrint.Parameter(10+i*6);
  	beta[i] = resultToPrint.Parameter(11+i*6);
  	gamma[i] = resultToPrint.Parameter(12+i*6);
  	Mu[i] = resultToPrint.Parameter(7+i*6);
  	dMu[i] = resultToPrint.ParError(7+i*6);		
	Area[i] = resultToPrint.Parameter(8+i*6);
	dArea[i] = resultToPrint.ParError(8+i*6);
	Hstep[i] = TMath::Abs(resultToPrint.Parameter(9+i*6));
	dHstep[i] = resultToPrint.ParError(9+i*6);
  	cov_s0_mu[i] = resultToPrint.CovMatrix(0,7+i*6);
  	cov_s1_mu[i] = resultToPrint.CovMatrix(1,7+i*6);
  	cov_s2_mu[i] = resultToPrint.CovMatrix(2,7+i*6);
  	cov_bT_mu[i] = resultToPrint.CovMatrix(3,7+i*6); 
  	cov_bH_mu[i] = resultToPrint.CovMatrix(5,7+i*6);
  	cov_mT_mu[i] = resultToPrint.CovMatrix(4,7+i*6);
  	cov_mH_mu[i] = resultToPrint.CovMatrix(6,7+i*6);
	}
  double Centroid[numberPeaksToFit], dCentroid[numberPeaksToFit];
  double Sigma[numberPeaksToFit], dSigma[numberPeaksToFit]; 
  double Tau[numberPeaksToFit], dTau[numberPeaksToFit];
  double Htail[numberPeaksToFit], dHtail[numberPeaksToFit];
  double SqrtVar[numberPeaksToFit], dSqrtVar[numberPeaksToFit];
  
  for(int i=0; i<numberPeaksToFit; i++){
  	if((fitOpt.compare(0,4,"gaus")==0) || ((bH==0.) && (mH==0.))){
  	  Centroid[i] = Mu[i];
  	  dCentroid[i] = dMu[i];
  	  dCdbT = dCdbH = dCdmT = dCdmH = 0.;
  	  dSVdbT = dSVdbH = dSVdmT = dSVdmH = 0.;
  	  dCdMu = 1.;
  	  Tau[i] = 0.;
  	  dTau[i] = 0.;
  	  Htail[i] = 0.;
  	  dHtail[i] = 0.;
  	  }
  	else{
  	  bTerm = 1. + bT*mH + mT*bH; 
      sqrtTerm = pow(bTerm,2.) - 4.*mT*mH* (bT*bH-Mu[i]);
	  Numerator = -bTerm + sqrt(sqrtTerm);
  	  Centroid[i] = Numerator/(2.*mT*mH); 	
  	  dCdbT = (-mH + (mH*bTerm - 2.*mT*mH*bH)/sqrt(sqrtTerm)) / (2.*mT*mH);
	  dCdbH = (-mT + (mT*bTerm - 2.*mT*mH*bT)/sqrt(sqrtTerm)) / (2.*mT*mH);		
	  dNdmT = -bH + (bH*bTerm - 2.*mH*(bT*bH-Mu[i]))/sqrt(sqrtTerm);
	  dNdmH = -bT + (bT*bTerm - 2.*mT*(bT*bH-Mu[i]))/sqrt(sqrtTerm);
	  dCdmT = (mT*dNdmT - Numerator)/(2.*mT*mT*mH);
	  dCdmH = (mH*dNdmH - Numerator)/(2.*mH*mT*mH);
	  dCdMu = 1./sqrt(sqrtTerm);
  	  dCentroid[i] = sqrt( pow(dCdbT*dbT,2.) + pow(dCdbH*dbH,2.) + pow(dCdmT*dmT,2.) + 
  						   pow(dCdmH*dmH,2.) + pow(dCdMu*dMu[i],2.) + 
  						   2.*( 
  						   dCdbT*( cov_bT_bH*dCdbH + cov_bT_mT*dCdmT + 
  						   cov_bT_mH*dCdmH + cov_bT_mu[i]*dCdMu ) + 
  						   dCdbH*( cov_bH_mT*dCdmT + cov_bH_mH*dCdmH +
  						   cov_bH_mu[i]*dCdMu ) + 
  						   dCdmT*( cov_mT_mH*dCdmH + cov_mT_mu[i]*dCdMu ) + 
  						   dCdmH*cov_mH_mu[i]*dCdMu ) );
  	  Tau[i] = bT + mT*Centroid[i];
  	  dTdbT = 1. + mT*dCdbT;
  	  dTdbH = dCdbH;
  	  dTdmT = mT*dCdmT + Centroid[i];
  	  dTdmH = dCdmH;
  	  dTdMu = dCdMu;
  	  dTau[i] = sqrt( pow(dTdbT*dbT,2.) + pow(dTdbH*dbH,2.) + pow(dTdmT*dmT,2.) +
  					  pow(dTdmH*dmH,2.) + pow(dTdMu*dMu[i],2.) +
  					  2.*(
  					  dTdbT*( cov_bT_bH*dTdbH + cov_bT_mT*dTdmT + cov_bT_mH*dTdmH +
  					  cov_bT_mu[i]*dTdMu ) +
  					  dTdbH*( cov_bH_mT*dTdmT + cov_bH_mH*dTdmH + cov_bH_mu[i]*dTdMu ) +
  					  dTdmT*( cov_mT_mH*dTdmH + cov_mT_mu[i]*dTdMu ) +
  					  dTdmH*cov_mH_mu[i]*dTdMu ) );
  	  Htail[i] = bH + mH*Centroid[i];
  	  dHdbT = dCdbT;
  	  dHdbH = 1. + mH*dCdbH;
  	  dHdmT = dCdmT;
  	  dHdmH = mH*dCdmH + Centroid[i];
  	  dHdMu = dCdMu;
  	  dHtail[i] = sqrt( pow(dHdbT*dbT,2.) + pow(dHdbH*dbH,2.) + pow(dHdmT*dmT,2.) +
  					  	pow(dHdmH*dmH,2.) + pow(dHdMu*dMu[i],2.) +
  						2.*(
  						dHdbT*( cov_bT_bH*dHdbH + cov_bT_mT*dHdmT + cov_bT_mH*dHdmH +
  						cov_bT_mu[i]*dHdMu ) +
  						dHdbH*( cov_bH_mT*dHdmT + cov_bH_mH*dHdmH + cov_bH_mu[i]*dHdMu ) +
  						dHdmT*( cov_mT_mH*dHdmH + cov_mT_mu[i]*dHdMu ) +
  						dHdmH*cov_mH_mu[i]*dHdMu ) );	
  	  }
  	Sigma[i] = sqrt( pow(s0,2.) + Centroid[i]*pow(s1,2.) + pow(Centroid[i]*s2,2.) );
  	dSds0 = s0/Sigma[i];
  	dSds1 = s1*Centroid[i]/Sigma[i];
  	dSds2 = s2*pow(Centroid[i],2.)/Sigma[i];
  	dSdC = (0.5*pow(s1,2.) + pow(s2,2.)*Centroid[i])/Sigma[i];
  	dSdbT = dSdC*dCdbT;
  	dSdbH = dSdC*dCdbH;
  	dSdmT = dSdC*dCdmT;
  	dSdmH = dSdC*dCdmH;
  	dSdMu = dSdC*dCdMu;
  	dSigma[i] = sqrt( pow(dSds0*ds0,2.) + pow(dSds1*ds1,2.) + pow(dSds2*ds2,2.) + 
  					  pow(dSdbT*dbT,2.) + pow(dSdbH*dbH,2.) + pow(dSdmT*dmT,2.) + 
  					  pow(dSdmH*dmH,2.) + pow(dSdMu*dMu[i],2.) + 
  					  pow(dSdC*dCdMu*dMu[i],2.) +
  					  2.*( 
  					  dSds0*( cov_s0_s1*dSds1 + cov_s0_s2*dSds2 + cov_s0_bT*dSdbT +
  					  cov_s0_bH*dSdbH + cov_s0_mT*dSdmT + cov_s0_mH*dSdmH + 
  					  cov_s0_mu[i]*dSdMu ) + 
  					  dSds1*( cov_s1_s2*dSds2 + cov_s1_bT*dSdbT + cov_s1_bH*dSdbH +
  					  cov_s1_mT*dSdmT + cov_s1_mH*dSdmH + cov_s1_mu[i]*dSdMu ) + 
  					  dSds2*( cov_s2_bT*dSdbT + cov_s2_bH*dSdbH + cov_s2_mT*dSdmT + 
  					  cov_s2_mH*dSdmH + cov_s2_mu[i]*dSdMu ) +
  					  dSdbT*( cov_bT_bH*dSdbH + cov_bT_mT*dSdmT + cov_bT_mH*dSdmH + 
  					  cov_bT_mu[i]*dSdMu ) +
  					  dSdbH*( cov_bH_mT*dSdmT + cov_bH_mH*dSdmH + cov_bH_mu[i]*dSdMu ) +
  					  dSdmT*( cov_mT_mH*dSdmH + cov_mT_mu[i]*dSdMu ) +
  					  dSdmH*cov_mH_mu[i]*dSdMu ) );				
  	SqrtVar[i] = sqrt( pow(Sigma[i],2.) - pow(Tau[i]*Htail[i],2.) + 2.*pow(Tau[i],2.)*Htail[i] );
  	dSVds0 = s0/SqrtVar[i];
  	dSVds1 = s1*Centroid[i]/SqrtVar[i];
  	dSVds2 = s2*pow(Centroid[i],2.)/SqrtVar[i];
  	tauComp = 0.5*pow(s1,2.) + pow(s2,2.)*Centroid[i] + mT*Tau[i]*Htail[i]*(2.-Htail[i]) +
  			  pow(Tau[i],2.)*(1.-Htail[i]);
  	htailComp = 0.5*pow(s1,2.) + pow(s2,2.)*Centroid[i] + Tau[i]*Htail[i]*(2.-Htail[i]) + 
  				mH*pow(Tau[i],2.)*(1.-Htail[i]);
  	dSVdbT = Tau[i]*Htail[i]*(2.-Htail[i])/SqrtVar[i] + tauComp*dCdbT/SqrtVar[i];
  	dSVdbH = Tau[i]*Tau[i]*(1.-Htail[i])/SqrtVar[i] + htailComp*dCdbH/SqrtVar[i];
  	dSVdmT = Centroid[i]*Tau[i]*Htail[i]*(2.-Htail[i])/SqrtVar[i] + tauComp*dCdmT/SqrtVar[i];
  	dSVdmH = Centroid[i]*pow(Tau[i],2.)*(1.-Htail[i])/SqrtVar[i] + htailComp*dCdmH/SqrtVar[i];
  	dSVdMu = dCdMu/SqrtVar[i]*( 0.5*pow(s1,2.) + pow(s2,2.)*Centroid[i] + 
  			 mT*Tau[i]*Htail[i]*(2.-Htail[i]) + mH*pow(Tau[i],2.)*(1.-Htail[i]) );
  	dSqrtVar[i] = sqrt( pow(dSVds0*ds0,2.) + pow(dSVds1*ds1,2.) + pow(dSVds2*ds2,2.) +
  						pow(dSVdbT*dbT,2.) + pow(dSVdbH*dbH,2.) + pow(dSVdmT*dmT,2.) +
  					  	pow(dSVdmH*dmH,2.) + pow(dSVdMu*dMu[i],2.) +
  					  	2.*(
  					  	dSVds0*( cov_s0_s1*dSVds1 + cov_s0_s2*dSVds2 + cov_s0_bT*dSVdbT +
  					  	cov_s0_bH*dSVdbH + cov_s0_mT*dSVdmT + cov_s0_mH*dSVdmH + 
  					  	cov_s0_mu[i]*dSVdMu ) +
  					  	dSVds1*( cov_s1_s2*dSVds2 + cov_s1_bT*dSVdbT + cov_s1_bH*dSVdbH + 
  					  	cov_s1_mT*dSVdmT + cov_s1_mH*dSVdmH + cov_s1_mu[i]*dSVdMu ) +
  					  	dSVds2*( cov_s2_bT*dSVdbT + cov_s2_bH*dSVdbH + cov_s2_mT*dSVdmT + 
  					  	cov_s2_mH*dSVdmH + cov_s2_mu[i]*dSVdMu ) +
  					  	dSVdbT*( cov_bT_bH*dSVdbH + cov_bT_mT*dSVdmT + cov_bT_mH*dSVdmH + 
  					  	cov_bT_mu[i]*dSVdMu ) +
  					  	dSVdbH*( cov_bH_mT*dSVdmT + cov_bH_mH*dSVdmH + cov_bH_mu[i]*dSVdMu ) +
  					  	dSVdmT*( cov_mT_mH*dSVdmH + cov_mT_mu[i]*dSVdMu ) +
  					  	dSVdmH*cov_mH_mu[i]*dSVdMu ) );
  			
  	printf("%s\n", subsection);
  	printf("Peak %d\n", i);
  	printf("\tMu = %.4f \t +/- \t %.4f\n", Mu[i], dMu[i]);
  	printf("\tArea from Fit = %.4f \t +/- %.4f \n", Area[i], dArea[i]);
  	printf("\tHstep %% = %.4f \t +/- %.4f \n", Hstep[i], dHstep[i]);
  	printf("\tSigma = %.4f \t +/- \t %.4f \n", Sigma[i], dSigma[i]);
  	printf("\tTau = %.4f \t +/- \t %.4f \n", Tau[i], dTau[i]);
  	printf("\tHtail %% = %.4f \t +/- \t %.4f \n", Htail[i], dHtail[i]);
  	printf("\tGaus %% = %.4f \n", 1.-Htail[i]);
  	printf("\tCentroid = %.4f \t +/- \t %.4f \n", Centroid[i], dCentroid[i]);
  	printf("\tSqrt Var = %.4f \t +/- \t %.4f \n", SqrtVar[i], dSqrtVar[i]);
  	}
  printf("%s\n For M +/- 10*sqrtVar: \n int setOfRanges[] = { ", subsection);
  for(int i=0; i<(numberPeaksToFit-1); i++) 
  	printf("%.0f, %.0f, ", Centroid[i]-10.*SqrtVar[i], Centroid[i]+10.*SqrtVar[i]);
  printf("%.0f, %.0f } \n %s\n", Centroid[numberPeaksToFit-1]-10.*SqrtVar[numberPeaksToFit-1], 
  	Centroid[numberPeaksToFit-1]+10.*SqrtVar[numberPeaksToFit-1], subsection);
  	
  //Plot peaks with their fits	
  c1 = new TCanvas("c1","Simultaneous fit of N histograms",50,10,1400,700);
  canvasWidth = ceil(numberPeaksToFit/2.0);
  c1->Divide(canvasWidth,2);
  //gStyle->SetOptFit(1111);
  gStyle->SetOptStat(0);
  c1->SetFillColor(0);
  c1->SetFrameBorderMode(0); 
    
  bkgdQ = new TF1*[numberPeaksToFit];
  bkgdS = new TF1*[numberPeaksToFit];
  signalG = new TF1*[numberPeaksToFit];
  signalT = new TF1*[numberPeaksToFit];
  signal = new TF1*[numberPeaksToFit];
  fArray = new TF1*[numberPeaksToFit];
  for(int i=0; i<numberPeaksToFit; i++){
  	bkgdQ[i] = new TF1(Form("pk%d_bkgdQ",i),
  		"[0]+[1]*x+[2]*pow(x,2.)", rArray[2*i], rArray[2*i+1]);
  	bkgdS[i] = new TF1(Form("pk%d_bkgdS",i),
  		"[0]+[1]*x+[2]*pow(x,2.)+[3]*[4]*TMath::Erfc((x-[5])/([6]*sqrt(2.)))", rArray[2*i], rArray[2*i+1]);
  	signalG[i] = new TF1(Form("pk%d_signalG",i),
  		"[0]+[1]*x+[2]*pow(x,2.)+[3]*[4]*TMath::Erfc((x-[5])/([6]*sqrt(2.)))+[3]*(1-[7])/([6]*sqrt(2.*TMath::Pi()))*exp(-0.5*pow((x-[5])/[6],2.))", rArray[2*i], rArray[2*i+1]);
  	signalT[i] = new TF1(Form("pk%d_signalT",i),
  		"[0]+[1]*x+[2]*pow(x,2.)+[3]*[4]*TMath::Erfc((x-[5])/([6]*sqrt(2.)))+[3]*[7]/(2*[8])*exp((x-[5])/[8]+pow([6]/(sqrt(2.)*[8]),2.))*TMath::Erfc((x-[5])/(sqrt(2.)*[6])+[6]/([8]*sqrt(2.)))", rArray[2*i], rArray[2*i+1]);
  	signal[i] = new TF1(Form("pk%d_signal",i),
  		"[0]*(1-[1])/([2]*sqrt(2.*TMath::Pi()))*exp(-0.5*pow((x-[3])/[2],2.))+[0]*[1]/(2*[4])*exp((x-[3])/[4]+pow([2]/(sqrt(2.)*[4]),2.))*TMath::Erfc((x-[3])/(sqrt(2.)*[2])+[2]/([4]*sqrt(2.)))", rArray[2*i], rArray[2*i+1]);
  	fArray[i] = new TF1(Form("pk%d_fArray",i),
  		"[0]+[1]*x+[2]*pow(x,2.)+[3]*[4]*TMath::Erfc((x-[5])/([6]*sqrt(2.)))+[3]*(1-[7])/([6]*sqrt(2.*TMath::Pi()))*exp(-0.5*pow((x-[5])/[6],2.))+[3]*[7]/(2*[8])*exp((x-[5])/[8]+pow([6]/(sqrt(2.)*[8]),2.))*TMath::Erfc((x-[5])/(sqrt(2.)*[6])+[6]/([8]*sqrt(2.)))", rArray[2*i], rArray[2*i+1]);
  		
  	bkgdQ[i]->FixParameter(0, alpha[i]);
  	bkgdQ[i]->FixParameter(1, beta[i]);
  	bkgdQ[i]->FixParameter(2, gamma[i]);
  	
  	bkgdS[i]->FixParameter(0, alpha[i]);
  	bkgdS[i]->FixParameter(1, beta[i]);
  	bkgdS[i]->FixParameter(2, gamma[i]);
  	bkgdS[i]->FixParameter(3, Area[i]); 
  	bkgdS[i]->FixParameter(4, Hstep[i]);
  	bkgdS[i]->FixParameter(5, Mu[i]);
  	bkgdS[i]->FixParameter(6, Sigma[i]);
  	
  	signalG[i]->FixParameter(0, alpha[i]);
  	signalG[i]->FixParameter(1, beta[i]);
  	signalG[i]->FixParameter(2, gamma[i]);
  	signalG[i]->FixParameter(3, Area[i]);
  	signalG[i]->FixParameter(4, Hstep[i]);
  	signalG[i]->FixParameter(5, Mu[i]);
  	signalG[i]->FixParameter(6, Sigma[i]);
  	signalG[i]->FixParameter(7, Htail[i]);
  	
  	signalT[i]->FixParameter(0, alpha[i]);
  	signalT[i]->FixParameter(1, beta[i]);
  	signalT[i]->FixParameter(2, gamma[i]);
  	signalT[i]->FixParameter(3, Area[i]);
  	signalT[i]->FixParameter(4, Hstep[i]);
  	signalT[i]->FixParameter(5, Mu[i]);
  	signalT[i]->FixParameter(6, Sigma[i]);
  	signalT[i]->FixParameter(7, Htail[i]);
  	signalT[i]->FixParameter(8, Tau[i]);
  	
  	signal[i]->FixParameter(0, Area[i]);
  	signal[i]->FixParameter(1, Htail[i]);
  	signal[i]->FixParameter(2, Sigma[i]);
  	signal[i]->FixParameter(3, Mu[i]);
  	signal[i]->FixParameter(4, Tau[i]);
  	MaxXFit = signal[i]->GetMaximumX();
    HalfMaxXFit = signal[i]->Eval(MaxXFit)/2.;
    lowFWHM = signal[i]->GetX(HalfMaxXFit, MaxXFit-5.*SqrtVar[i], MaxXFit);
    highFWHM = signal[i]->GetX(HalfMaxXFit, MaxXFit, MaxXFit+5.*SqrtVar[i]);
    FWHM[i] = highFWHM - lowFWHM;
    dFWHM[i] = dSqrtVar[i]*FWHM[i]/SqrtVar[i];
  	
  	fArray[i]->FixParameter(0, alpha[i]);
  	fArray[i]->FixParameter(1, beta[i]);
  	fArray[i]->FixParameter(2, gamma[i]);
  	fArray[i]->FixParameter(3, Area[i]);
  	fArray[i]->FixParameter(4, Hstep[i]);
  	fArray[i]->FixParameter(5, Mu[i]);
  	fArray[i]->FixParameter(6, Sigma[i]);
  	fArray[i]->FixParameter(7, Htail[i]);
  	fArray[i]->FixParameter(8, Tau[i]);
  	}
  
  for(int i=0; i<numberPeaksToFit; i++){
  	c1->cd(i+1);
  	bkgdQ[i]->SetLineColor(50); //salmon
  	bkgdS[i]->SetLineColor(39); //gray
  	signalG[i]->SetLineColor(30); //green
  	signalT[i]->SetLineColor(28); //brown
  	fArray[i]->SetLineColor(9); //purple
  	hArray[i]->GetListOfFunctions()->Add(fArray[i]);
  	hArray[i]->GetListOfFunctions()->Add(bkgdQ[i]);
  	hArray[i]->GetListOfFunctions()->Add(bkgdS[i]);
  	hArray[i]->GetListOfFunctions()->Add(signalG[i]);
  	hArray[i]->GetListOfFunctions()->Add(signalT[i]);
  	hArray[i]->Draw();
  	}

  sprintf(outputFileName, "%s_fit.png", fitOpt.c_str());
  c1->Print(outputFileName);
  c1->Clear();
  
  for(int i=0; i<numberPeaksToFit; i++) hArray[i]->GetListOfFunctions()->Add(signal[i]);
  
  //Plot trends wrt PkCentroid
  gHtail = new TGraphErrors(numberPeaksToFit, Centroid, Htail, dCentroid, dHtail);
  	gHtail->SetMarkerColor(14);
  	gHtail->SetMarkerSize(0.8);
  	gHtail->SetMarkerStyle(23);
  	gHtail->GetXaxis()->SetTitle("Peak Centroid (keV)");
  	gHtail->GetYaxis()->SetTitle("Htail");
  	gHtail->Draw("ap");	
  fHtail = new TF1("fHtail", "[0]+[1]*x", rArray[0], rArray[2*numberPeaksToFit-1]);
  	fHtail->FixParameter(0, bH);
  	fHtail->FixParameter(1, mH);
  	fHtail->SetLineColor(32);
  	fHtail->Draw("same");
  c1->Print("Htail.png");
  c1->Clear();
  
  gTau = new TGraphErrors(numberPeaksToFit, Centroid, Tau, dCentroid, dTau);
  	gTau->SetMarkerColor(14);
  	gTau->SetMarkerSize(0.8);
  	gTau->SetMarkerStyle(23);
  	gTau->GetXaxis()->SetTitle("Peak Centroid (keV)");
  	gTau->GetYaxis()->SetTitle("Tau (keV)");
  	gTau->Draw("ap");
  fTau = new TF1("fTau", "[0]+[1]*x", rArray[0], rArray[2*numberPeaksToFit-1]);
  	fTau->FixParameter(0, bT);
  	fTau->FixParameter(1, mT);
  	fTau->SetLineColor(32);
  	fTau->Draw("same");
  c1->Print("tau.png");
  c1->Clear();
    		
  gSigma = new TGraphErrors(numberPeaksToFit, Centroid, Sigma, dCentroid, dSigma);
  	gSigma->SetMarkerColor(14);
  	gSigma->SetMarkerSize(0.8);
  	gSigma->SetMarkerStyle(23);
  	gSigma->GetXaxis()->SetTitle("Peak Centroid (keV)");
  	gSigma->GetYaxis()->SetTitle("Sigma (keV)");
  	gSigma->Draw("ap");
  fSigma = new TF1("fSigma", "sqrt( pow([0],2.) + pow([1],2.)*x + pow([2]*x,2.) )", rArray[0], rArray[2*numberPeaksToFit-1]);
  	fSigma->FixParameter(0, s0);
  	fSigma->FixParameter(1, s1);
  	fSigma->FixParameter(2, s2);
  	fSigma->SetLineColor(32);
  	fSigma->Draw("same");
  c1->Print("sigma.png");
  c1->Clear();
  
  gSqrtVar = new TGraphErrors(numberPeaksToFit, Centroid, SqrtVar, dCentroid, dSqrtVar);
  	gSqrtVar->SetMarkerColor(14);
  	gSqrtVar->SetMarkerSize(0.8);
  	gSqrtVar->SetMarkerStyle(23);
  	gSqrtVar->GetXaxis()->SetTitle("Peak Centroid (keV)");
  	gSqrtVar->GetYaxis()->SetTitle("SqrtVar (keV)");
  	gSqrtVar->Draw("ap");
  c1->Print("sqrtVar.png");
  c1->Clear();  
  
  gFWHM = new TGraphErrors(numberPeaksToFit, Centroid, FWHM, dCentroid, dFWHM);
  	gFWHM->SetMarkerColor(14);
  	gFWHM->SetMarkerSize(0.8);
  	gFWHM->SetMarkerStyle(23);
  	gFWHM->GetXaxis()->SetTitle("Peak Centroid (keV)");
  	gFWHM->GetYaxis()->SetTitle("FWHM (keV)");
  	gFWHM->Draw("ap");
  fFWHM = new TF1("fFWHM", "sqrt( pow([0],2.) + pow([1],2.)*x + pow([2]*x,2.) )", rArray[0], rArray[2*numberPeaksToFit-1]);
  	fFWHM->SetParameter(0, s0);
  	fFWHM->SetParameter(1, s1);
  	fFWHM->SetParameter(2, s2);
  	fFWHM->SetLineColor(32);
  	
  	gFWHM->Fit("fFWHM","RN");
  	fFWHM->Draw("same");
  c1->Print("FWHM.png");
  c1->Clear();
    
  gHstep = new TGraphErrors(numberPeaksToFit, Centroid, Hstep, dCentroid, dHstep);
  	gHstep->SetMarkerColor(14);
  	gHstep->SetMarkerSize(0.8);
  	gHstep->SetMarkerStyle(23);
  	gHstep->GetXaxis()->SetTitle("Peak Centroid (keV)");
  	gHstep->GetYaxis()->SetTitle("Hstep");
  	gHstep->Draw("ap");    
  c1->Print("Hstep.png");
  
  hPar = new TH1D(Form("hPar_%.0f",bT*100),"",resultToPrint.NPar(),0.,resultToPrint.NPar());
  hParErr = new TH1D(Form("hParErr_%.0f",bT*100),"",resultToPrint.NPar(),0.,resultToPrint.NPar());
  for(int i=0; i<resultToPrint.NPar(); i++){
    hPar->SetBinContent(hPar->FindBin(i),resultToPrint.Parameter(i));
    hParErr->SetBinContent(hParErr->FindBin(i),resultToPrint.ParError(i));
    }
  
  sprintf(outputFileName, "%s_fit.root", fitOpt.c_str());
  rootResults = new TFile(outputFileName,"update");
  hPar->Write("hPar");
  hParErr->Write("hParErr");
  for(int i=0; i<numberPeaksToFit; i++){
  	sprintf(histTitle,"hPeak%d", i);
  	hArray[i]->Write(histTitle);
  	}
  gHtail->Write("gHtail");
  gTau->Write("gTau");
  gSigma->Write("gSigma");
  gSqrtVar->Write("gSqrtVar");
  gFWHM->Write("gFWHM");
  gHstep->Write("gHstep");
  rootResults->Close();
  delete c1;
  }
 