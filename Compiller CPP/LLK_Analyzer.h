#pragma once
#include "Scaner.h"
#include "defines.h"
#include <iostream>

class LLK_Analyzer{

private:
	Scaner* sc;
	Lexem _lex; // ����������� ����������� �������
	int lexType;// ��� ������� - �������� ���������
	TOneSymb Mag[MaxLenMag];
	int MagPointer;
	bool flag;

	void epsilon();
	void handleError(std::string, std::string);

public:
	LLK_Analyzer(Scaner*);
	void makeAnalyze();

};