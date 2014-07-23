#ifndef SINGLEMINFUNC_HH
#define SINGLEMINFUNC_HH

#include <cstddef>
#include "Math/IFunction.h"

class SingleMinFunc 
{
  int fWhichOne;
  const ROOT::Math::IMultiGenFunction* fMF_i;
  
  public:
	SingleMinFunc( ROOT::Math::IMultiGenFunction &fi, int whichOne );
	SingleMinFunc( );
	friend class GlobalMinFunc;
};
#endif
