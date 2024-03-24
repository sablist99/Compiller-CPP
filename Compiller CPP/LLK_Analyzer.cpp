#include "LLK_Analyzer.h"
#include <string>
#include "SemanticTree.h"

LLK_Analyzer::LLK_Analyzer(Scaner* _sc, Translate* _translate, ILGenerator* _generator) {
	sc = _sc;
	translate = _translate;
	generator = _generator;
}



void LLK_Analyzer::makeAnalyze(bool printTree) {
	MagPointer = 0;
	Mag[MagPointer].IsTerm = false;
	Mag[MagPointer].Type = T_S;

	flag = true;
	lexType = sc->scaner(_lex);

	TOneSymb symb;
	while (flag) {
		symb = Mag[MagPointer];
		if (symb.IsTerm) {// Если терминальный симовл
			if (symb.Type == lexType) { // Если это ТОТ терминальный символ, который ждали
				if (symb.Type == TEnd) { // Если это конец программы
					flag = 0;
				}
				else {
					MagPointer--;
					translate->setPrevLexem(_lex);
					lexType = sc->scaner(_lex);
				}
			}
			else {
				// Ошибка
				handleError(std::to_string(symb.Type), "termHandler");
			}
		}
		else {  // Если нетерминал
			switch (symb.Type) {
			case TR_MATCH:
				generator->deltaMatch(false);
				epsilon();
				break;

			case TR_MATCH_LEFT:
				generator->deltaMatch(true);
				epsilon();
				break;

			case TR_GENER_PLUS:
				generator->generateTriade("+", MathOperation);
				epsilon();
				break;

			case TR_GENER_MINUS:
				generator->generateTriade("-", MathOperation);
				epsilon();
				break;

			case TR_GENER_MOD:
				generator->generateTriade("%", MathOperation);
				epsilon();
				break;

			case TR_GENER_DIV:
				generator->generateTriade("/", MathOperation);
				epsilon();
				break;

			case TR_GENER_MULT:
				generator->generateTriade("*", MathOperation);
				epsilon();
				break;

			case TR_GENER_L:
				generator->generateTriade("<", LogicOperation);
				epsilon();
				break;

			case TR_GENER_R:
				generator->generateTriade(">", LogicOperation);
				epsilon();
				break;

			case TR_GENER_LE:
				generator->generateTriade("<=", LogicOperation);
				epsilon();
				break;

			case TR_GENER_RE:
				generator->generateTriade(">=", LogicOperation);
				epsilon();
				break;

			case TR_GENER_E:
				generator->generateTriade("==", LogicOperation);
				epsilon();
				break;

			case TR_GENER_NE:
				generator->generateTriade("!=", LogicOperation);
				epsilon();
				break;

			case TR_GENER_LSH:
				generator->generateTriade("<<", LogicOperation);
				epsilon();
				break;

			case TR_GENER_RSH:
				generator->generateTriade(">>", LogicOperation);
				epsilon();
				break;

			case TR_GENER_ASSIGN:
				generator->generateTriade("=", NoneOperation);
				epsilon();
				break;

			case TR_PUSH_IDENT:
				generator->deltaPushOperand(false);
				epsilon();
				break;

			case TR_PUSH_NUMBER:
				generator->deltaPushOperand(true);
				epsilon();
				break;

			case TR_GENER_NOP:
				generator->generateNop();
				epsilon();
				break;

			case TR_FORM_IF:
				generator->generateFormIf();
				epsilon();
				break;

			case TR_GENER_GOTO:
				generator->generateGoto();
				epsilon();
				break;

			case TR_GENER_IF:
				generator->generateIfTriad();
				epsilon();
				break;

			case TR_SET_ADDR:
				generator->setAddr();
				epsilon();
				break;

			case TR_START_FUNC:
				generator->startFunc();
				epsilon();
				break;

			case TR_END_FUNC:
				generator->endFunc();
				epsilon();
				break;

			case TR_GENER_PUSH_PARAM:
				generator->generatePushParamTriad();
				epsilon();
				break;

			case TR_CALL_FUNC:
				generator->callFunc();
				epsilon();
				break;



			case D_START_DECL_DATA:
				translate->setTypeObject(ObjVar);
				translate->deltaStartDecl();
				epsilon();
				break;

			case D_START_DECL_CONST:
				translate->setTypeObject(ObjConst);
				translate->deltaStartDecl();
				epsilon();
				break;

			case D_START_DECL_FUNC:
				translate->setTypeVar(TVoid);
				translate->setTypeObject(ObjFunc);
				translate->deltaStartDecl();
				epsilon();
				break;

			case D_END_DECL_FUNC:
				translate->upOnTree();
				epsilon();
				break;

			case D_START_SUB:
				translate->setRightEmpty();
				epsilon();
				break;

			case D_END_SUB:
				translate->upOnTree();
				epsilon();
				break;

			case D_END_DECL:
				translate->setIsConst(false);
				epsilon();
				break;

			case D_SET_IDENT:
				if (!translate->deltaSetIdent()) {
					return;
				}
				epsilon();
				break;

			case T_S:
				if (lexType == TConst || lexType == TInt || lexType == TShort || lexType == TVoid || lexType == TChar || lexType == TLong) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_S_R;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_DESC;
				}
				else {
					handleError("const, int, short, void, long, char", "T_S");
				}
				break;

			case T_S_R:
				if (lexType == TConst || lexType == TInt || lexType == TShort || lexType == TVoid || lexType == TChar || lexType == TLong) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_S;
				}
				else if (lexType == TEnd) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TEnd;
				}
				else {
					handleError("const, int, short, void, long, char", "T_S_R");
				}
				break;

			case T_DESC_CONST:
				if (lexType == TConst) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = D_END_DECL;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TSemicolon;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_CONST;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TAssign;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = D_SET_IDENT;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TIdent;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = D_START_DECL_CONST;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_TYPE;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TConst;

					translate->setIsConst(true);
				}
				else {
					handleError("const", "T_DESC_CONST");
				}
				break;

			case T_CONST:
				if (lexType == TConstInt) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TConstInt;
				}
				else if (lexType == TConstHex) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TConstHex;
				}
				else if (lexType == TConstChar) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TConstChar;
				}
				else {
					handleError("коснтанта символьная/десятиричная/шестнадцатиричная", "T_CONST");
				}
				break;

			case T_DESC:
				if (lexType == TConst) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_DESC_CONST;
				}
				else if (lexType == TInt || lexType == TShort || lexType == TChar || lexType == TLong) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_DATA;
				}
				else if (lexType == TVoid) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_FUNC;
				}
				else {
					handleError("const, int, short, void, char, long", "T_DESC");
				}
				break;

			case T_DATA:
				if (lexType == TInt || lexType == TShort || lexType == TChar || lexType == TLong) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = D_END_DECL;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TSemicolon;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_LIST_IDENT;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = D_START_DECL_DATA;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_TYPE;
				}
				else {
					handleError("int, short, char, long", "T_DATA");
				}
				break;

			case T_LIST_IDENT:
				if (lexType == TIdent) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = TR_GENER_ASSIGN;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = TR_MATCH_LEFT;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_LIST_IDENT_R1;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = TR_PUSH_IDENT;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = D_SET_IDENT;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TIdent;

				}
				else {
					handleError("Идентификатор", "T_LIST_IDENT");
				}
				break;

			case T_LIST_IDENT_R1:
				if (lexType == TAssign) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_LIST_IDENT_R2;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_EXPR;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TAssign;
				}
				else if (lexType == TComma) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_LIST_IDENT;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TComma;
				}
				else if (lexType == TRPar || lexType == TSemicolon) {
					epsilon();
				}
				else {
					handleError("')', ',', '='", "T_LIST_IDENT_R1");
				}
				break;

			case T_LIST_IDENT_R2:
				if (lexType == TComma) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_LIST_IDENT;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TComma;
				}
				else if (lexType == TRPar || lexType == TSemicolon) {
					epsilon();
				}
				else {
					handleError("')', ','", "T_LIST_IDENT_R2");
				}
				break;

			case T_TYPE:
				if (lexType == TInt) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TInt;
					translate->setTypeVar(TInt);
				}
				else if (lexType == TShort) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TShort;
					translate->setTypeVar(TShort);
				}
				else if (lexType == TChar) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TChar;
					translate->setTypeVar(TChar);
				}
				else if (lexType == TLong) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TLong;
					translate->setTypeVar(TLong);
				}
				else {
					handleError("int, short, long, char", "T_TYPE");
				}
				break;

			case T_FUNC:
				if (lexType == TVoid) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = D_END_DECL_FUNC;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = TR_END_FUNC;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_OPER;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = TR_START_FUNC;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TRPar;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_FORM_PARAM;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TLPar;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = D_SET_IDENT;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = D_START_DECL_FUNC;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = TR_PUSH_IDENT;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TIdent;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TVoid;
				}
				else {
					handleError("void", "T_FUNC");
				}
				break;

			case T_FORM_PARAM:
				if (lexType == TInt || lexType == TShort || lexType == TChar || lexType == TLong) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_FORM_PARAM_R;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = D_END_DECL;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = D_SET_IDENT;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TIdent;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = D_START_DECL_DATA;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_TYPE;
				}
				else if (lexType == TRPar) {
					epsilon();
				}
				else {
					handleError("int, short, long, char, ')'", "T_FORM_PARAM");
				}
				break;

			case  T_FORM_PARAM_R:
				if (lexType == TComma) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_FORM_PARAM_R;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = D_END_DECL;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = D_SET_IDENT;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TIdent;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = D_START_DECL_DATA;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_TYPE;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TComma;
				}
				else if (lexType == TRPar) {
					epsilon();
				}
				else {
					handleError("',', ')'", "T_FORM_PARAM_R");
				}
				break;

			case T_LIST_PARAM:
				if (lexType == TIdent) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_LIST_IDENT;
				}
				else if (lexType == TRPar) {
					epsilon();
				}
				else {
					handleError("Идентификатор, ')'", "T_LIST_PARAM");
				}
				break;

			case T_FUNC_CALL:
				if (lexType == TLPar) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TRPar;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_LIST_PARAM;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TLPar;
				}
				else {
					handleError("'('", "T_FUNC_CALL");
				}
				break;

			case T_OPER:
				if (lexType == TIdent || lexType == TReturn || lexType == TBreak || lexType == TWhile) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_SIMP_OPER;
				}
				else if (lexType == TSemicolon) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TSemicolon;
				}
				else if (lexType == TFLPar) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_COMB_OPER;
				}
				else {
					handleError("Идентификатор, ';', '{', return, break, while", "T_OPER");
				}
				break;

			case T_SIMP_OPER:
				if (lexType == TIdent) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_SIMP_OPER_R;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = TR_PUSH_IDENT;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TIdent;
				}
				else if (lexType == TReturn) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TSemicolon;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TReturn;
				}
				else if (lexType == TBreak) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TSemicolon;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TBreak;
				}
				else if (lexType == TWhile) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_WHILE;
				}
				else {
					handleError("Идентификатор, return, break, while", "T_SIMP_OPER");
				}
				break;

			case T_SIMP_OPER_R:
				if (lexType == TLPar) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TSemicolon;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = TR_CALL_FUNC;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_FUNC_CALL;
				}
				else if (lexType == TAssign) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = TR_GENER_ASSIGN;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = TR_MATCH_LEFT;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TSemicolon;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_ASSIGN;
				}
				else {
					handleError("'(', '='", "T_SIMP_OPER_R");
				}
				break;

			case T_COMB_OPER:
				if (lexType == TFLPar) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = D_END_SUB;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TFRPar;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_BODY_COMB_OPER;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = D_START_SUB;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TFLPar;
				}
				else {
					handleError("'{'", "T_COMB_OPER");
				}
				break;

			case T_BODY_COMB_OPER:
				if (lexType == TIdent || lexType == TSemicolon || lexType == TReturn || lexType == TBreak || lexType == TFLPar || lexType == TWhile) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_BODY_COMB_OPER;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_OPER;
				}
				else if (lexType == TConst) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_BODY_COMB_OPER;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_DESC_CONST;
				}
				else if (lexType == TInt || lexType == TShort || lexType == TChar || lexType == TLong) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_BODY_COMB_OPER;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_DATA;
				}
				else if (lexType == TFRPar) {
					epsilon();
				}
				else {
					handleError("'{', '}', int, short, long, char, const, ';', return, break, while, идентификатор", "T_BODY_COMB_OPER");
				}
				break;

			case T_WHILE:
				if (lexType == TWhile) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = TR_GENER_NOP;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = TR_GENER_GOTO;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = TR_FORM_IF;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_OPER;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = TR_GENER_IF;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = TR_SET_ADDR;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TRPar;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_EXPR;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TLPar;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = TR_SET_ADDR;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TWhile;
				}
				else {
					handleError("while", "T_WHILE");
				}
				break;

			case T_ASSIGN:
				if (lexType == TAssign) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_EXPR;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TAssign;
				}
				else {
					handleError("'='", "T_ASSIGN");
				}
				break;

			case T_EQ:
				if (lexType == TL) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TL;
				}
				else if (lexType == TR) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TR;
				}
				else if (lexType == TLE) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TLE;
				}
				else if (lexType == TRE) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TRE;
				}
				else if (lexType == TE) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TE;
				}
				else if (lexType == TNE) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TNE;
				}
				else {
					handleError("'==', '!=', '<', '>', '<=', '>='", "T_EQ");
				}
				break;

			case T_SH:
				if (lexType == TLSh) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TLSh;
				}
				else if (lexType == TRSh) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TRSh;
				}
				else {
					handleError("'<=', '>='", "T_SH");
				}
				break;

			case T_OPER_2:
				if (lexType == TPlus) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TPlus;
				}
				else if (lexType == TMinus) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TMinus;
				}
				else {
					handleError("'+', '-'", "T_OPER_2");
				}
				break;

			case T_OPER_3:
				if (lexType == TMult) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TMult;
				}
				else if (lexType == TDiv) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TDiv;
				}
				else if (lexType == TMod) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TMod;
				}
				else {
					handleError("'*', '/', '%'", "T_OPER_3");
				}
				break;

			case T_EXPR:
				if (lexType == TIdent || lexType == TConstInt || lexType == TLPar) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_EXPR_R;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_EXPR_1;
				}
				else {
					handleError("'(', число, идентификатор", "T_EXPR");
				}
				break;

			case T_EXPR_1:
				if (lexType == TIdent || lexType == TConstInt || lexType == TLPar) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_EXPR_1_R;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_EXPR_2;
				}
				else {
					handleError("'(', число, идентификатор", "T_EXPR_1");
				}
				break;

			case T_EXPR_2:
				if (lexType == TIdent || lexType == TConstInt || lexType == TLPar) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_EXPR_2_R;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_EXPR_3;
				}
				else {
					handleError("'(', число, идентификатор", "T_EXPR_2");
				}
				break;

			case T_EXPR_3:
				if (lexType == TIdent || lexType == TConstInt || lexType == TLPar) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_EXPR_3_R;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_SIMP_EXPR;
				}
				else {
					handleError("'(', число, идентификатор", "T_EXPR_3");
				}
				break;

			case T_SIMP_EXPR:
				if (lexType == TIdent) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = TR_PUSH_IDENT;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TIdent;
				}
				else if (lexType == TConstInt) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = TR_PUSH_NUMBER;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TConstInt;
				}
				else if (lexType == TLPar) {
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TRPar;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_EXPR;

					MagPointer++;
					Mag[MagPointer].IsTerm = true;
					Mag[MagPointer].Type = TLPar;
				}
				else {
					handleError("'(', число, идентификатор", "T_SIMP_EXPR");
				}
				break;

			case T_EXPR_R:
				if (lexType == TIdent || lexType == TConst || lexType == TInt || lexType == TChar || lexType == TLong ||
					lexType == TShort || lexType == TVoid || lexType == TRPar ||
					lexType == TComma || lexType == TSemicolon || lexType == TReturn ||
					lexType == TBreak || lexType == TFLPar || lexType == TFRPar ||
					lexType == TWhile || lexType == TEnd) {
					epsilon();
				}
				else if (lexType == TL || lexType == TR || lexType == TLE ||
					lexType == TRE || lexType == TE || lexType == TNE) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_EXPR_R;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					switch (lexType) {
					case TL:
						Mag[MagPointer].Type = TR_GENER_L;
						break;
					case TR:
						Mag[MagPointer].Type = TR_GENER_R;
						break;
					case TLE:
						Mag[MagPointer].Type = TR_GENER_LE;
						break;
					case TRE:
						Mag[MagPointer].Type = TR_GENER_RE;
						break;
					case TE:
						Mag[MagPointer].Type = TR_GENER_E;
						break;
					case TNE:
						Mag[MagPointer].Type = TR_GENER_NE;
						break;
					default:
						handleError("Неожиданный операнд", "T_EXPR_1_R");
					}

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = TR_MATCH;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_EXPR_1;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_EQ;
				}
				else {
					handleError("Ну тут дофига всего может быть. Когда-нибудь напишу:)", "T_EXPR_R");
				}
				break;

			case T_EXPR_1_R:
				if (lexType == TIdent || lexType == TConst || lexType == TInt || lexType == TChar || lexType == TLong ||
					lexType == TShort || lexType == TVoid || lexType == TRPar ||
					lexType == TComma || lexType == TSemicolon || lexType == TReturn ||
					lexType == TBreak || lexType == TFLPar || lexType == TFRPar ||
					lexType == TWhile || lexType == TEnd || lexType == TL ||
					lexType == TR || lexType == TLE || lexType == TRE ||
					lexType == TE || lexType == TNE) {
					epsilon();
				}
				else if (lexType == TLSh || lexType == TRSh) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_EXPR_1_R;	 

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					switch (lexType) {
					case TLSh:
						Mag[MagPointer].Type = TR_GENER_LSH;
						break;
					case TRSh:
						Mag[MagPointer].Type = TR_GENER_RSH;
						break;
					default:
						handleError("Неожиданный операнд", "T_EXPR_1_R");
					}

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = TR_MATCH;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_EXPR_2;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_SH;
				}
				else {
					handleError("Ну тут дофига всего может быть. Когда-нибудь напишу:)", "T_EXPR_1_R");
				}
				break;

			case T_EXPR_2_R:
				if (lexType == TIdent || lexType == TConst || lexType == TInt || lexType == TChar || lexType == TLong ||
					lexType == TShort || lexType == TVoid || lexType == TRPar ||
					lexType == TComma || lexType == TSemicolon || lexType == TReturn ||
					lexType == TBreak || lexType == TFLPar || lexType == TFRPar ||
					lexType == TWhile || lexType == TEnd || lexType == TL ||
					lexType == TR || lexType == TLE || lexType == TRE ||
					lexType == TE || lexType == TNE ||
					lexType == TLSh || lexType == TRSh) {
					epsilon();
				}
				else if (lexType == TPlus || lexType == TMinus) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_EXPR_2_R;
					
					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					switch (lexType) {
					case TPlus:
						Mag[MagPointer].Type = TR_GENER_PLUS;
						break;
					case TMinus:
						Mag[MagPointer].Type = TR_GENER_MINUS;
						break;
					default:
						handleError("Неожиданный операнд", "T_EXPR_2_R");
					}

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = TR_MATCH;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_EXPR_3;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_OPER_2;
				}
				else {
					handleError("Ну тут дофига всего может быть. Когда-нибудь напишу:)", "T_EXPR_2_R");
				}
				break;

			case T_EXPR_3_R:
				if (lexType == TIdent || lexType == TConst || lexType == TInt || lexType == TChar || lexType == TLong ||
					lexType == TShort || lexType == TVoid || lexType == TRPar ||
					lexType == TComma || lexType == TSemicolon || lexType == TReturn ||
					lexType == TBreak || lexType == TFLPar || lexType == TFRPar ||
					lexType == TWhile || lexType == TEnd || lexType == TL ||
					lexType == TR || lexType == TLE || lexType == TRE ||
					lexType == TE || lexType == TNE ||
					lexType == TLSh || lexType == TRSh ||
					lexType == TPlus || lexType == TMinus) {
					epsilon();
				}
				else if (lexType == TMult || lexType == TDiv || lexType == TMod) {
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_EXPR_3_R;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					switch (lexType) {
					case TMult:
						Mag[MagPointer].Type = TR_GENER_MULT;
						break;
					case TDiv:
						Mag[MagPointer].Type = TR_GENER_DIV;
						break;
					case TMod:
						Mag[MagPointer].Type = TR_GENER_MOD;
						break;
					default:
						handleError("Неожиданный операнд", "T_EXPR_3_R");
					}

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = TR_MATCH;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_SIMP_EXPR;

					MagPointer++;
					Mag[MagPointer].IsTerm = false;
					Mag[MagPointer].Type = T_OPER_3;
				}
				else {
					handleError("Ну тут дофига всего может быть. Когда-нибудь напишу:)", "T_EXPR_3_R");
				}
				break;

			default:
				handleError("Хотя бы что-то", "default case");

				break;
			}
		}
	}

	if (printTree) {
		translate->Print();
	}
}

void  LLK_Analyzer::epsilon() {
	MagPointer--;
}

void LLK_Analyzer::handleError(std::string wait, std::string block) {
	std::cout << "Ошибка анализатора в блоке " << block << ". Ожидался '" << wait << "'" << std::endl;
	std::cout << "Встречен '" << _lex << "'" << std::endl;
	flag = false;
}
