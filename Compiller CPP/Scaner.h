#pragma once


#ifndef __SCANER
#define __SCANER
#include "defines.h"

class Scaner
{
private:
	TypeText _text; // тут храним текст
	int _pointer; // указатель на текущую позицию
	int _errorPosition; // номер позиции с ошибкой
	int _position;
	int _line; // номер строки с ошибкой
public:
	ScanerPosition getScanerPosition();
	void setScanerPosition(ScanerPosition sp);
	void setPointer(int);
	int getPointer(void);
	void setPosition(int);
	int getPosition(void);
	void setLine(int i);
	int getLine(void);
	void printError(char*, char*, int, int);
	int scaner(Lexem l);
	void readFile(char*);
	static bool isNumber(char);
	static bool isHex(char);
	static bool isLCase(char);
	static bool isUCase(char);
	Scaner(char*);
	Scaner();
	~Scaner();
};
#endif
