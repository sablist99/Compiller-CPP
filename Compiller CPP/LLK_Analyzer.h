#pragma once
#include "Scaner.h"
#include "SemanticTree.h"
#include "defines.h"
#include <iostream>
#include "Translate.h"

class LLK_Analyzer{

public:
	LLK_Analyzer(Scaner* _sc, Translate* _translate);
	void makeAnalyze(bool printTree);

private:
	Scaner* sc;
	Translate* translate;
	Lexem _lex; // ����������� ����������� �������
	int lexType;// ��� ������� - �������� ���������
	TOneSymb Mag[MaxLenMag];
	int MagPointer;
	bool flag;

	void epsilon();
	void handleError(std::string, std::string);



};