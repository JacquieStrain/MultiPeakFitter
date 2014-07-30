#ifndef GLOBALMINFUNC_HH
#define GLOBALMINFUNC_HH

#include "SingleMinFunc.hh"

class GlobalMinFunc 
{ 
  int fHowMany;
  SingleMinFunc* Psmf;
	
  public:
	GlobalMinFunc( SingleMinFunc smf[], int howMany );
	GlobalMinFunc();
	double operator() (const double* par) const { 
		const  ROOT::Math::IMultiGenFunction* fMF_j;
		double parameters[13];
		double mf_tot = 0;
		double mf_j;	
		for(int k=0; k<fHowMany; k++){
			for (int m=0; m<7; m++) parameters[m] = par[m]; //common parameters to all pks
			for (int m=7; m<13; m++) parameters[m] = par[m+6*k]; //unique parameters for each pk
			fMF_j = (*(Psmf+k)).fMF_i; 
			mf_j = (*fMF_j)(parameters);
			mf_tot += mf_j;
			}
		return mf_tot;
		}
};
#endif

//Comments on operator() function:
/*operator() is what the minimizer will call. The minimizer passes operator() the 
initial guesses of the parameters. Then it computes the value of the function that 
is to be minimized. After, it finds new parameters "guesses" accordingly, and passes 
those new parameter guesses to the operator() function once again. 
This repeats until the function is minimized.*/
//Comments on outer for() loop: 
/*For each peak, find the function that the peak needs to be fit with (fMF_j).
Then, for that peak, compute the value of the function that is to be 
minimized (mf_j) with the given parameter guesses (parameters).
Do this for each peak, and then add up each peak's minimizer function value. 
This total is what the minimization function.*/
