// Compiller CPP.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Scaner.h"
#include "LLK_Analyzer.h"

int main()
{
    setlocale(LC_ALL, "ru");
    Scaner* sc = new Scaner(_strdup("program.cpp"));
    LLK_Analyzer* llk = new LLK_Analyzer(sc);

    llk->makeAnalyze();

                                

    

    






}
