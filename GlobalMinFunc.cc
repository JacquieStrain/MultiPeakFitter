#include "GlobalMinFunc.hh"

#include <iostream>
#include "Math/IFunction.h"
#include "Math/WrappedMultiTF1.h"

GlobalMinFunc::GlobalMinFunc( SingleMinFunc smf[], int howMany ) : 
	fHowMany(howMany), Psmf(smf) {}
GlobalMinFunc::GlobalMinFunc() : 
	fHowMany(-1), Psmf(nullptr) {}
