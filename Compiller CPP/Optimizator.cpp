#include "Optimizator.h"
#include "ILGenerator.h"
#include <cmath>

void Optimizator::SimplifyingCalculationMathematicalFunctions() {
	FirstFillRequiredUpdate();
	while (requiredUpdate.size() > 0) {
		int currentTriadNumber = requiredUpdate.back();
		requiredUpdate.pop_back();

		Triad* currentTriad = &(global->triads.at(currentTriadNumber));
		// Перебираем все триады
		if (currentTriad->typeOperation == MathOperation) {
			// Если текущая триада - математическая операция
			if (currentTriad->operand1.isLink == false && currentTriad->operand2.isLink == false &&
				Scaner::isNumber((currentTriad->operand1.lex)[0]) &&
				Scaner::isNumber((currentTriad->operand2.lex)[0])) {
				// Если мы можем вычислить значение, то вычисляем
				Lexem* newValue = CalculateMathValue(currentTriad);

				// И подставляем его во все места, где была ссылка на текущую триаду
				ReplaceLinkToNewValue(currentTriadNumber, newValue);

				// А текущую триаду заменяем на пустую
				ReplaceTriadToNop(currentTriadNumber);
			}
		}
	}
}

bool Optimizator::CalculateLogicValue(Triad* triad) {
	long first, second;
	bool result = false;
	first = LexemToNumber(&(triad->operand1.lex));
	second = LexemToNumber(&(triad->operand2.lex));

	int lexemSize = sizeof("<");
	int lexemLongSize = sizeof("<=");
	if (memcmp(triad->operation, "<", lexemSize) == 0) {
		result = second < first;
	}
	else if (memcmp(triad->operation, ">", lexemSize) == 0) {
		result = second > first;
	}
	else if (memcmp(triad->operation, "<=", lexemLongSize) == 0) {
		result = second <= first;
	}
	else if (memcmp(triad->operation, ">=", lexemLongSize) == 0) {
		result = second >= first;
	}
	else if (memcmp(triad->operation, "==", lexemLongSize) == 0) {
		result = second == first;
	}
	else if (memcmp(triad->operation, "!=", lexemLongSize) == 0) {
		result = second != first;
	}
	else {
		std::cout << "Ошибка оптимизатора. Невозможно вычислить очередное логическое значение - не нашлось нужной операции" << std::endl;
	}

	return result;
}

Lexem* Optimizator::CalculateMathValue(Triad* triad) {
	long first, second, result = 0;
	first = LexemToNumber(&(triad->operand1.lex));
	second = LexemToNumber(&(triad->operand2.lex));

	int lexemSize = sizeof("+");
	int lexemLongSize = sizeof("<<");
	if (memcmp(triad->operation, "+", lexemSize) == 0) {
		result = second + first;
	}
	else if (memcmp(triad->operation, "-", lexemSize) == 0) {
		result = second - first;
	}
	else if (memcmp(triad->operation, "*", lexemSize) == 0) {
		result = second * first;
	}
	else if (memcmp(triad->operation, "/", lexemSize) == 0) {
		result = second / first;
	}
	else if (memcmp(triad->operation, "%", lexemSize) == 0) {
		result = second % first;
	}
	else if (memcmp(triad->operation, "<<", lexemLongSize) == 0) {
		result = second << first;
	}
	else if (memcmp(triad->operation, ">>", lexemLongSize) == 0) {
		result = second >> first;
	}
	else {
		std::cout << "Ошибка оптимизатора. Невозможно вычислить очередное значение - не нашлось нужной операции" << std::endl;
	}

	return NumberToLexem(result);
}

void Optimizator::ReplaceLinkToNewValue(int triadeNumber, Lexem* newValue) {
	// Заменяем ссылку на значение во всех триадах, которые ссылались на упрощенную триаду
	Lexem localLexem;
	memcpy(localLexem, newValue, sizeof(newValue));

	int triadsCount = global->triads.size();
	int currentTriadNumber = 0;
	while (currentTriadNumber < triadsCount) {
		Triad* currentTriad = &(global->triads.at(currentTriadNumber));
		bool added = false;	// Добавлена ли текущая триада в список триад, по которым тоже нужно пройтись
		if (currentTriad->operand1.isLink && currentTriad->operand1.triadeNumber == triadeNumber) {
			currentTriad->operand1.isLink = false;
			memcpy(currentTriad->operand1.lex, newValue, sizeof(newValue));
			requiredUpdate.push_back(currentTriadNumber);
		}
		if (currentTriad->operand2.isLink && currentTriad->operand2.triadeNumber == triadeNumber) {
			currentTriad->operand2.isLink = false;
			memcpy(currentTriad->operand2.lex, newValue, sizeof(newValue));
			if (!added) {
				// Чтобы не дублироваться
				requiredUpdate.push_back(currentTriadNumber);
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
	* Ищем логические условия - они могут быть только перед if, согласно индивидуальному заданию.
	* Если оба значения - числа, то результат предопределен.
	* Так как нет условных операторов и есть только команда return, то выйти из while можно только по условию в самом while
	* А если оно всегда истино или ложно, то из него нельзя выйти, согласно индивидуальному заданию.
	* В такой конструкции не будет смысла.
	* Поэтому, если условие ложно, то удаляем проверку, if и операнды внутри while.
	* Если условие истино, то генерируем ошибку.
	*/
	FirstFillRequiredUpdate();
	while (requiredUpdate.size() > 0) {
		int currentTriadNumber = requiredUpdate.back();
		requiredUpdate.pop_back();

		Triad* currentTriad = &(global->triads.at(currentTriadNumber));
		// Перебираем все триады
		if (currentTriad->typeOperation == LogicOperation) {
			// Если текущая триада - логическая операция
			if (currentTriad->operand1.isLink == false && currentTriad->operand2.isLink == false &&
				Scaner::isNumber((currentTriad->operand1.lex)[0]) &&
				Scaner::isNumber((currentTriad->operand2.lex)[0])) {
				// Если мы можем вычислить значение, то вычисляем
				bool newValue = CalculateLogicValue(currentTriad);

				if (newValue) {
					// Ошибка
				}
				else {
					// Удаляем все триады с текущей до [goto 'текущая триада']
					int ifTriadeNumber = currentTriadNumber;
					while (!(memcmp(currentTriad->operation, "goto", sizeof("goto")) == 0 &&
						currentTriad->operand1.isLink == false &&
						currentTriad->operand1.triadeNumber < ifTriadeNumber))
						//	triadeNumber < ifTriadeNumber Потому что мы не знаем, в какой момент начались вычисления выражения в while
						// Но точно можем сказать, что не наткнемся на goto от вложенного while, потому что triadeNumber будет больше ifTriadeNumber
					{
						ReplaceTriadToNop(currentTriadNumber);
						currentTriadNumber++;
						currentTriad = &(global->triads.at(currentTriadNumber));
					}
					ReplaceTriadToNop(currentTriadNumber); // Удаление самого goto
				}
			}
		}
	}
}

long Optimizator::LexemToNumber(Lexem* newValue) {
	long result = 0;
	int mult = 0;
	int range = 0;
	while ((*newValue)[range] != 0) {
		// Определяем количество разрядов
		range++;
	}
	// Отмытываем один разряд назад
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
		l[range] = (char)number % 10 + 48;
		number /= 10;
		range--;
	}

	return &l;
}

void Optimizator::PrintTriades() {
	for (int i = 0; i < global->triads.size(); i++) {
		Triad* currentTriad = &(global->triads.at(i));
		std::cout << i << ") " << currentTriad->operation << " ";
		if (memcmp(currentTriad->operation, "nop", sizeof("nop")) != 0)
		{
			if (!ILGenerator::isShortTriad(&currentTriad->operation)) {
				if (currentTriad->operand2.isLink) {
					std::cout << "[" << currentTriad->operand2.triadeNumber << "] ";
				}
				else {
					std::cout << currentTriad->operand2.lex << " ";
				}
			}
			if (currentTriad->operand1.isLink) {
				std::cout << "[" << currentTriad->operand1.triadeNumber << "] ";
			}
			else {
				std::cout << currentTriad->operand1.lex;
			}
		}
		std::cout << std::endl;
	}
}

Optimizator::Optimizator(GlobalData* gl) {
	global = gl;
}