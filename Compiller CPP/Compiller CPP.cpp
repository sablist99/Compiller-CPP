﻿// Compiller CPP.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Scaner.h"
#include "LLK_Analyzer.h"
#include "SemanticTree.h"

int main()
{
    setlocale(LC_ALL, "ru");
    Scaner* sc = new Scaner(_strdup("program.cpp"));
    SemanticTree* tr = new SemanticTree();
    Translate* translate = new Translate(tr);
    tr->SetCur(tr);
    LLK_Analyzer* llk = new LLK_Analyzer(sc, translate);

    llk->makeAnalyze(true);

                                

    

    






}
