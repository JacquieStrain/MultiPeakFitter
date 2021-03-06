#ifndef FORMOFFUNCTION_HH
#define FORMOFFUNCTION_HH

#include <string.h>
#include <stdio.h>
#include <iostream>

class FormOfFunction 
{ 
  char centroidEq[300];
  	char bTerm[50];
  	char sqrtTerm[100];
  
  char sigma2Eq[700]; 
  char tauEq[400]; 
  char HtailEq[500];
  
  char signalGaus[1000];
  	char signalGausConst[1000]; 
   
  char signalTail[1500];
	char signalTailConst[1500];
  	char signalTailExp[1500];
  	char signalTailErfc[1500];
  
  char bkgdStep[300]; 
  char bkgdQuad[30]; 
  
  char sigmaEq[750];
  char function[3000];
  
  int areaPar, bkgdConstPar, bkgdLinPar, bkgdQuadPar, HstepPar, HtailYintPar, HtailSlopePar;
  int muPar, sigmaPar0, sigmaPar1, sigmaPar2, tauSlopePar, tauYintPar;
  
  public:
  	int funcSize;
  	char ffunction[3000];
  	FormOfFunction(std::string whichFit, std::string whichFunc);
};
#endif
