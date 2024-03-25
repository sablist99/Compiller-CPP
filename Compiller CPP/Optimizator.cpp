#include "Optimizator.h"
#include "ILGenerator.h"
#include <cmath>

void Optimizator::SimplifyingCalculationMathematicalFunctions() {
	FirstFillRequiredUpdate();
	while (requiredUpdate.size() > 0) {
		int currentTriadNumber = requiredUpdate.back();
		requiredUpdate.pop_back();

		Triad* currentTriad = &(global->triads.at(currentTriadNumber));
		// ���������� ��� ������
		if (currentTriad->typeOperation == MathOperation) {
			// ���� ������� ������ - �������������� ��������
			if (currentTriad->operand1.isLink == false && currentTriad->operand2.isLink == false &&
				Scaner::isNumber((currentTriad->operand1.lex)[0]) &&
				Scaner::isNumber((currentTriad->operand2.lex)[0])) {
				// ���� �� ����� ��������� ��������, �� ���������
				Lexem* newValue = CalculateValue(currentTriad);

				// � ����������� ��� �� ��� �����, ��� ���� ������ �� ������� ������
				ReplaceLinkToNewValue(currentTriadNumber, newValue);

				// � ������� ������ �������� �� ������
				ReplaceTriadToNop(currentTriadNumber);
			}
		}
	}
}

Lexem* Optimizator::CalculateValue(Triad* triad) {
	long first, second, result = 0;
	first = LexemToNumber(&(triad->operand1.lex));
	second = LexemToNumber(&(triad->operand2.lex));

	int lexemSize = sizeof("+");
	int lexemLongSize = sizeof("<<");
	if (memcmp(triad->operation, "+", lexemSize) == 0) {
		result = first + second;
	}
	else if (memcmp(triad->operation, "-", lexemSize) == 0) {
		result = first - second;
	}
	else if (memcmp(triad->operation, "*", lexemSize) == 0) {
		result = first * second;
	}
	else if (memcmp(triad->operation, "/", lexemSize) == 0) {
		result = first / second;
	}
	else if (memcmp(triad->operation, "%", lexemSize) == 0) {
		result = first % second;
	}
	else if (memcmp(triad->operation, "<<", lexemLongSize) == 0) {
		result = first << second;
	}
	else if (memcmp(triad->operation, ">>", lexemLongSize) == 0) {
		result = first >> second;
	}
	else {
		std::cout << "������ ������������. ���������� ��������� ��������� �������� - �� ������� ������ ��������" << std::endl;
	}

	return NumberToLexem(result);
}

void Optimizator::ReplaceLinkToNewValue(int triadeNumber, Lexem* newValue) {
	// �������� ������ �� �������� �� ���� �������, ������� ��������� �� ���������� ������
	Lexem localLexem;
	memcpy(localLexem, newValue, sizeof(newValue));

	int triadsCount = global->triads.size();
	int currentTriadNumber = 0;
	while (currentTriadNumber < triadsCount) {
		Triad* currentTriad = &(global->triads.at(currentTriadNumber));	   
		bool added = false;	// ��������� �� ������� ������ � ������ �����, �� ������� ���� ����� ��������
		if (currentTriad->operand1.isLink && currentTriad->operand1.triadeNumber == triadeNumber) {
			currentTriad->operand1.isLink = false;
			memcpy(currentTriad->operand1.lex, newValue, sizeof(newValue));	  
			requiredUpdate.push_back(triadeNumber);
		}
		if (currentTriad->operand2.isLink && currentTriad->operand2.triadeNumber == triadeNumber) {
			currentTriad->operand2.isLink = false;
			memcpy(currentTriad->operand2.lex, newValue, sizeof(newValue));
			if (!added) {
				// ����� �� �������������
				requiredUpdate.push_back(triadeNumber);
			}
		}
		currentTriadNumber++;
	}
}

void Optimizator::ReplaceTriadToNop(int triadeNumber) {
	Lexem* operation = ILGenerator::getLexemFromString("nop");
	Lexem localLexem;
	memcpy(localLexem, operation, sizeof(operation));
	memcpy(global->triads.at(triadeNumber).operation, localLexem, sizeof(localLexem));
}

void Optimizator::FirstFillRequiredUpdate() {
	int triadsCount = global->triads.size();
	int currentTriadNumber = 0;
	while (currentTriadNumber < triadsCount) {
		requiredUpdate.push_back(currentTriadNumber);
		currentTriadNumber++;
	}
}

void Optimizator::SimplifyingLogicalOperations() {
	/* 
	* ���� ���������� ������� - ��� ����� ���� ������ ����� if, �������� ��������������� �������.
	* ���� ��� �������� - �����, �� ��������� �������������.
	* ��� ��� ��� �������� ���������� � ���� ������ ������� return, �� ����� �� while ����� ������ �� ������� � ����� while
	* � ���� ��� ������ ������ ��� �����, �� �� ���� ������ �����, �������� ��������������� �������.
	* � ����� ����������� �� ����� ������.
	* �������, ���� ������� �����, �� ������� ��������, if � �������� ������ while.
	* ���� ������� ������, �� ���������� ������.
	*/


}

long Optimizator::LexemToNumber(Lexem* newValue) {
	long result = 0;
	int mult = 0;
	int range = 0;
	while ((*newValue)[range] != 0) {
		// ���������� ���������� ��������
		range++;
	}
	// ���������� ���� ������ �����
	range--;

	while (range != -1) {
		result += (long)((*newValue)[range] - 48) * std::pow(10, mult);
		mult++;
		range--;
	}

	return result;
}

Lexem* Optimizator::NumberToLexem(long number) {
	Lexem l;
	for (int i = 0; i < MAX_LEXEM_LENGHT; i++) {
		l[i] = 0;
	}

	int range = -1;
	long buf = number;
	while (buf > 0) {
		range++;
		buf /= 10;
	}

	while (range != -1) {
		l[range] = (char) number % 10 + 48;
		number /= 10;
		range--;
	}

	return &l;
}

Optimizator::Optimizator(GlobalData* gl) {
	global = gl;
}