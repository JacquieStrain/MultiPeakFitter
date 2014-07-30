#include "SingleMinFunc.hh"

SingleMinFunc::SingleMinFunc( ROOT::Math::IMultiGenFunction &fi, int whichOne ) : 
	fMF_i(&fi), fWhichOne(whichOne) {}
SingleMinFunc::SingleMinFunc( ) : 
	fMF_i(NULL), fWhichOne(1001) {}
