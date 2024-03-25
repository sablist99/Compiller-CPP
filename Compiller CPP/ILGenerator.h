#include "SemanticTree.h"


class ILGenerator 
{
private:

public:
	SemanticTree* tree;
	GlobalData* global;

	static Lexem* getLexemFromString(std::string s);

	void setAddr();
	void startFunc();
	void endFunc();
	void callFunc();
	void generatePushParamTriad();
	void generateIfTriad();
	void generateGoto();
	void generateNop();
	void generateFormIf();
	void deltaMatch(bool isLeftMatch);
	void deltaPushOperand(bool isConst);
	void generateTriade(std::string operationString, TypeOperation typeOperation);
	bool isShortTriad(Lexem* operation);
	std::string getMatchLexem(TypeVar v1, TypeVar v2, bool* swap);
	ILGenerator(SemanticTree* tr, GlobalData* gl);
};