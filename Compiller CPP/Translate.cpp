#include "Translate.h"
#include <string>
#include <iostream>
#include <vector>

#include "LLK_Analyzer.h"
#include <string>
#include "SemanticTree.h"
#include <iostream>
#include <algorithm>
#include <iterator>
using namespace std;

void Translate::deltaStartDecl()
{
	global.flagDecl = true;
}

void Translate::deltaEndDecl()
{
	global.flagDecl = false;
}

bool  Translate::deltaSetIdent()
{
	if (!st->FindIdentUp(global.prevLex)) {
		if (global.typeObject == ObjFunc) {
            st->AddFunctionInTree(global.prevLex);
            st->SetRightEmpty();
				 
		}	
		else if (global.typeObject == ObjVar) {
			st->AddDataInTree(global.prevLex, global.typeVar);
		}
		else if (global.typeObject == ObjConst) {
			st->AddConstantInTree(global.prevLex, global.typeVar);
		}
        return true;
	}
	else {
        cout << "ѕроизошла попытка добавлени€ идентификатора, который уже есть в дереве - " << global.prevLex << endl;
        return false;
	}
}

void Translate::setTypeObject(TypeObject typeObject)
{
	global.typeObject = typeObject;
}

void Translate::setTypeVar(int lexType)
{
    if (lexType == TInt) {
        if (global.isConst) {
            global.typeVar = TypeConstInt;
        }
        else {
            global.typeVar = (TypeInt);
        }
    }
    else if (lexType == TShort) {
        if (global.isConst) {
            global.typeVar = (TypeConstInt);
        }
        else {
            global.typeVar = (TypeShort);
        }
    }
    else if (lexType == TChar) {
        if (global.isConst) {
            global.typeVar = (TypeConstChar);
        }
        else {
            global.typeVar = (TypeChar);
        }
    }
    else if (lexType == TLong) {
        if (global.isConst) {
            global.typeVar = (TypeConstInt);
        }
        else {
            global.typeVar = (TypeLong);
        }
    }
}

void Translate::setPrevLexem(Lexem l)
{
    for (int i = 0; i < sizeof(l); i++) {
        global.prevLex[i] = l[i];
    }
}

void Translate::setIsConst(bool value)
{
	global.isConst = value;
}



void Translate::upOnTree()
{
    st->FindEmptyUp();
}

void Translate::setRightEmpty()
{
    st->SetRightEmpty();
}

void Translate::Print()
{
    st->Print();
}

Translate::Translate(SemanticTree* tr)
{
    st = tr;
}
