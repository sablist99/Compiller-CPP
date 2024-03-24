// Compiller CPP.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Scaner.h"
#include "LLK_Analyzer.h"
#include "SemanticTree.h"
#include "Optimizator.h"

int main()
{
    setlocale(LC_ALL, "ru");
    Scaner* sc = new Scaner(_strdup("program.cpp"));
    GlobalData* global = new GlobalData();
    SemanticTree* tr = new SemanticTree();
    Translate* translate = new Translate(tr, global);
    ILGenerator* generator = new ILGenerator(tr, global);
    Optimizator* optimizator = new Optimizator(global);
    optimizator->SimplifyingCalculationMathematicalFunctions();
    optimizator->SimplifyingLogicalOperations();

    tr->SetCur(tr);
    LLK_Analyzer* llk = new LLK_Analyzer(sc, translate, generator);

    llk->makeAnalyze(true);
}
