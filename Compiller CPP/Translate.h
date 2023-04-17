#pragma once
#include "SemanticTree.h"

#include <stdio.h>
#include <stdlib.h>
class Translate
{
public:
	SemanticTree* st;
	GlobalData global;
	void deltaStartDecl();
	void deltaEndDecl();
	bool deltaSetIdent();

	void setTypeObject(TypeObject typeObject);
	void setTypeVar(int lexType);
	void setPrevLexem(Lexem lexem);
	void setIsConst(bool value);

	void upOnTree();
	void setRightEmpty();

	void Print();
	Translate(SemanticTree* tr);
};

