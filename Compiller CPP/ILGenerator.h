#include "SemanticTree.h"


class ILGenerator 
{
private:
	Lexem* getLexemFromString(std::string s);

public:
	SemanticTree* tree;
	GlobalData* global;
	int triadPointer;       //   индекс  первой  генерируемой  триады
	Triad triads[MAX_TRIAD_COUNT];      //  триады

	void setAddr();
	void generateIfTriad();
	void generateGoto();
	void generateNop();
	void generateFormIf();
	void deltaMatch(bool isLeftMatch);
	void deltaPushOperand(bool isConst);
	void generateTriade(std::string operationString, bool isOperation);
	bool isShortTriad(Lexem* operation);
	std::string getMatchLexem(TypeVar v1, TypeVar v2, bool* swap);
	ILGenerator(SemanticTree* tr, GlobalData* gl);
};