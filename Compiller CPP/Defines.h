#pragma once
#ifndef __DEFINES
#define __DEFINES

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <deque>

#define MAX_TEXT_LENGHT 10000 // максимальная длина текста
#define MAX_LEXEM_LENGHT 100 // максимальная длина лексемы
#define MAX_TRIAD_COUNT 1000 // максимальная длина лексемы
#define KEY_WORD_COUNT 12 // число ключевых слов
typedef char Lexem[MAX_LEXEM_LENGHT];
typedef char TypeText[MAX_TEXT_LENGHT];

#define TInt 1
#define TShort 2
#define TLong 3
#define TChar 4
#define TVoid 5
#define TReturn 6
#define TWhile 7
#define TConst 8
#define TBreak 9
#define TConstInt 10
#define TConstHex 11
#define TConstChar 12
#define TIdent 13
#define TAssign 14
#define TPlus 15
#define TMinus 16
#define TMult 17
#define TDiv 18
#define TMod 19
#define TLSh 20
#define TRSh 21
#define TL 22
#define TR 23
#define TLE 24
#define TRE 25
#define TE 26
#define TNE 27
#define TLPar 28
#define TRPar 29
#define TFLPar 30
#define TFRPar 31
#define TSemicolon 32
#define TComma 33

#define T_S 101
#define	T_S_R 102
#define	T_DESC_CONST 103
#define	T_DESC 104
#define	T_DATA 105
#define	T_LIST_IDENT 106
#define	T_LIST_IDENT_R1 107
#define	T_LIST_IDENT_R2 108
#define	T_TYPE 109
#define	T_FUNC 110
#define	T_FORM_PARAM 111
#define	T_FORM_PARAM_R 112
#define	T_LIST_PARAM 113
#define	T_FUNC_CALL 114
#define	T_OPER 115
#define	T_SIMP_OPER 116
#define	T_COMB_OPER 117
#define	T_WHILE 118
#define	T_ASSIGN 119
#define	T_EQ 120
#define	T_SH 121
#define	T_OPER_2 122
#define	T_OPER_3 123
#define	T_EXPR 124
#define	T_EXPR_R 125
#define	T_EXPR_1 126
#define	T_EXPR_1_R 127
#define	T_EXPR_2 128
#define	T_EXPR_2_R 129
#define	T_EXPR_3 130
#define	T_EXPR_3_R 131
#define	T_SIMP_EXPR 132
#define T_CONST 133
#define T_SIMP_OPER_R 134
#define T_BODY_COMB_OPER 135

#define D_END_DECL 201
#define D_END_DECL_FUNC 202
#define D_START_DECL_DATA 203
#define D_START_DECL_CONST 204
#define D_START_DECL_FUNC 205
#define D_SET_IDENT 206	  
#define D_START_SUB 207	  
#define D_END_SUB 208	  

#define TR_MATCH 301	  
#define TR_MATCH_LEFT 302	  
#define TR_SET_ADDR 303	  
#define TR_GENER_IF 304	  
#define TR_FORM_IF 305	  
#define TR_GENER_GOTO 306	  
#define TR_GENER_NOP 307	  
#define TR_GENER_PLUS 308	  
#define TR_GENER_MINUS 309	  
#define TR_GENER_MULT 310	  
#define TR_GENER_DIV 311	  
#define TR_GENER_MOD 312	  
#define TR_GENER_L 313	  
#define TR_GENER_R 314	  
#define TR_GENER_LE 315	  
#define TR_GENER_RE 316	  
#define TR_GENER_E 317	  
#define TR_GENER_NE 318	  
#define TR_GENER_LSH 319	  
#define TR_GENER_RSH 320	  
#define TR_GENER_ASSIGN 321	  
#define TR_PUSH_IDENT 324	  
#define TR_PUSH_NUMBER 325	  
#define TR_START_FUNC 326	  
#define TR_END_FUNC 327	  
#define TR_GENER_PUSH_PARAM 328	  
#define TR_CALL_FUNC 329	  


#define TEnd 998
#define TError 999

#define FILE_DONT_EXIST "Файл не существует"
#define FILE_VERY_LONG "Файл слишком длинный"
#define ERROR_CHAR_CONAT "Неверная символьная константа"
#define ERROR_CHAR "Неверный символ"
#define EMPTY_STRING ""

#define CtoS "c->s"
#define CtoI "c->i"
#define CtoL "c->l"
#define StoC "s->c"
#define StoI "s->i"
#define StoL "s->l"
#define ItoC "i->c"
#define ItoS "i->s"
#define ItoL "i->l"
#define LtoC "l->c"
#define LtoS "l->s"
#define LtoI "l->i"

struct Operand {
	bool isLink;
	//  непосредственный операнд или ссылка
	int triadeNumber;    // номер триады-ссылки
	Lexem lex;         // непосредственный операнд
};

struct Triad {
	Lexem operation;  //  операция
	Operand operand1, operand2;
};


enum TypeObject {
	ObjConst = 1,   // константа
	ObjVar,         // переменная
	ObjFunc,        // функция
	ObjUndef = 10000
};

enum TypeVar {
	TypeConstChar = 1,
	TypeChar,
	TypeConstInt,
	TypeShort,
	TypeInt,
	TypeConstHex,
	TypeLong,
	TypeVoid,
	TypeIdent,
	TypeUndef = 10000
};

struct ScanerPosition {
	int ptr;
	int line;
	int pos;
};

union DataValue
{
	// для функций - количество параметров
	std::vector<TypeVar>* param;
	union
	{
		short dataAsShort;
		int dataAsInt;
	};
};


struct Node // информация об одной переменной
{
	Lexem id; // идентификатор переменной

	TypeObject typeObject;
	TypeVar typeVar;

	// ссылка на значение
	//DataValue data; // Значение переменной

	//bool flagInit; // Флаг начальной инициализации

	int isEmpty;
};

//	 Магазин

#define MaxLenMag 1000
struct TOneSymb
{ 
	bool IsTerm; // признак терминала
	int Type; // тип символа - численная константа
	Lexem Lex; // Изображение лексемы
};

struct GlobalData {
	Lexem prevLex;
	Lexem prevAllLex;
	bool flagDecl;
	TypeVar typeVar;
	TypeObject typeObject;
	bool isConst;
	std::deque<TypeVar> typesMagazine;
	std::deque<Operand> resultsMagazine;
	int triadIndex = 0;
	std::deque<Triad> triads;
	std::deque<int> triadesIndexMagazine;
};
#endif