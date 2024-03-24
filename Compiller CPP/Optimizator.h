#include "Defines.h"


class Optimizator
{
private:
	GlobalData* global;

public:
	void SimplifyingCalculationMathematicalFunctions();
	void SimplifyingLogicalOperations();
	Optimizator(GlobalData* gl);
};