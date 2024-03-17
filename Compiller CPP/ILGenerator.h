#include "SemanticTree.h"


class ILGenerator 
{
public:
	SemanticTree* tree;
	GlobalData* global;
	int triadPointer;       //   ������  ������  ������������  ������
	Triad triads[MAX_TRIAD_COUNT];      //  ������

	void deltaFind();
	void deltaMatch(bool isLeftMatch);
	void deltaPushOperand(bool isConst);
	void generateTriade(Lexem* operation, bool isOperation);
	bool isShortTriad(Lexem* operation);
	Lexem* getMatchLexem(TypeVar v1, TypeVar v2, bool* swap);
	ILGenerator(SemanticTree* tr, GlobalData* gl);
};