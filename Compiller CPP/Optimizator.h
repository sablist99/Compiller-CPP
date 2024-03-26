#include "Defines.h"


class Optimizator
{
private:
	GlobalData* global;
	std::deque<int> requiredUpdate;

public:
	void FirstFillRequiredUpdate();
	void SimplifyingCalculationMathematicalFunctions();
	void SimplifyingLogicalOperations();
	void ReplaceLinkToNewValue(int triadeNumber, Lexem* newValue);
	long LexemToNumber(Lexem* newValue);
	Lexem* NumberToLexem(long number);
	void ReplaceTriadToNop(int triadeNumber);
	Lexem* CalculateMathValue(Triad* triad);
	bool CalculateLogicValue(Triad* triad);
	void PrintTriades();
	Optimizator(GlobalData* gl);
};