#pragma once
#include <string.h>
#include <iostream>
#include <vector>
#include <string>
#include "Defines.h"
#include "Scaner.h"

class SemanticTree
{
protected:
	Node* _node; // данные таблицы
	SemanticTree* _up, * _left, * _right;
	Scaner* _ts;
public:
	SemanticTree(SemanticTree* left, SemanticTree* right, SemanticTree* up, Node* node);
	SemanticTree();
	~SemanticTree();

	Node* getNode();
	Node* getNodeByIdent(Lexem lexem);

	SemanticTree* getLchild();
	SemanticTree* getRchild();
	SemanticTree* getUp();


	static int emptyCount;
	void IncEmptyCount();
	int GetEmptyCount();

	static SemanticTree* cur;
	void SetCur(SemanticTree* a) { cur = a; }
	SemanticTree* GetCur(void) { return cur; }

	void SetRightEmpty();
	SemanticTree* SetLeft(Node* node);
	SemanticTree* SetRight(Node* node);
	//SemanticTree* FillNode(Lexem lexem, TypeObject typeObject, TypeVar typeVar, std::vector<TypeVar>* param, DataValue data, bool flagInit);
	SemanticTree* FillNode(Lexem lexem, TypeObject typeObject, TypeVar typeVar);
	//SemanticTree* AddFunctionInTree(Lexem lexem, std::vector<TypeVar>* param);
	SemanticTree* AddFunctionInTree(Lexem lexem);
	//void AddConstantInTree(Lexem lexem, TypeVar typeVar, DataValue data);
	void AddConstantInTree(Lexem lexem, TypeVar typeVar);
	//void AddDataInTree(Lexem lexem, TypeVar typeVar, DataValue data, bool flagInit);
	void AddDataInTree(Lexem lexem, TypeVar typeVar);
	void Print(void);
	void PrintError(std::string error);
	//void SetParam(std::vector<TypeVar>* param);
	//void SetData(Lexem lexem, DataValue data);
	void FindEmptyUp();
	//void CheckParamList(Lexem lexem, std::vector<TypeVar> param);
	SemanticTree* FindIdentUp(Lexem lexem, TypeObject typeObject);
	SemanticTree* FindIdentUp(Lexem lexem);
	bool vectorEquivalence(std::vector<TypeVar> f, std::vector<TypeVar> s);
	//void CheckInit(Lexem lexem);
	//void CheckInitByNode(Node* node);
	bool CheckAssignment(TypeVar left, TypeVar right);
	long ConvertTypeLexemToNumber(Lexem lexem);
	int GetLexemLength(Lexem lexem);
	std::string ConvertTypeObjectToString(TypeObject typeObject);
	std::string ConvertTypeVarToString(TypeVar typeVar);
	std::string ConvertFlaginitToString(bool flag);
	std::string ConvertValueToString(DataValue dataValue, TypeVar typeVar);
	void DeleteRight();
	bool findCopyUp(Lexem lexem);

	TypeVar ConvertTypeLexemToTypeVar(int typeLexem);
	TypeVar GetTypeVarByLexem(Lexem lexem);
};