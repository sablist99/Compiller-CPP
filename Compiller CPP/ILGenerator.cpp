#include "ILGenerator.h"

ILGenerator::ILGenerator(SemanticTree* tr, GlobalData* gl)
{
    tree = tr;
    global = gl;
}

Lexem* ILGenerator::getLexemFromString(std::string s) {
	Lexem lexem;
	memcpy(lexem, s.c_str(), sizeof(s.size()));
	return &lexem;
}

void ILGenerator::setAddr() {
	global->triadesIndexMagazine.push_back(global->triads.size());
}

void ILGenerator::generateIfTriad() {
	Triad triad{};

	Lexem* operation = getLexemFromString("if");
	memcpy(triad.operation, operation, sizeof(operation));

	Operand first{};
	first.isLink = true;
	first.triadeNumber = this->global->triads.size() + 1;	// ?

	triad.operand1 = first;
	this->global->triads.push_back(triad);
}

void ILGenerator::generateFormIf() {
	int triadNumber = global->triadesIndexMagazine.back();
	global->triadesIndexMagazine.pop_back();

	Operand second{};
	second.isLink = true;
	second.triadeNumber = global->triads.size() + 1; // ?

	global->triads.at(triadNumber).operand2 = second;
}

void ILGenerator::generateGoto() {
	int triadNumber = global->triadesIndexMagazine.back();
	global->triadesIndexMagazine.pop_back();

	Triad triad{};

	Lexem* operation = getLexemFromString("goto");
	memcpy(triad.operation, operation, sizeof(operation));

	Operand first{};
	first.isLink = true;
	first.triadeNumber = triadNumber;	// ?

	triad.operand1 = first;
	this->global->triads.push_back(triad);
}

void ILGenerator::generateNop() {
	Triad triad{};

	Lexem* operation = getLexemFromString("nop");
	memcpy(triad.operation, operation, sizeof(operation));

	this->global->triads.push_back(triad);
}

void ILGenerator::deltaMatch(bool isLeftMatch) {
	TypeVar v2 = global->typesMagazine.back();         // второй операнд
	global->typesMagazine.pop_back();
	TypeVar v1 = global->typesMagazine.back();         // первый операнд
	global->typesMagazine.pop_back();
    if (v1 == v2) {             //  типы равны, приведения нет
        global->typesMagazine.push_back(v1);
    }
	else {
		// Сопоставление по левой части (например, при присваивании) 
		bool swap = false;
		std::string matchLexem = getMatchLexem(v1, v2, &swap);
		if (isLeftMatch) {
			if (swap) {
				generateTriade(matchLexem, false);
			}
			global->typesMagazine.push_back(v1);
		}
		else {
			if (matchLexem != "") {
				// Если матчить все таки нужно
				// Определяем, какой тип положить в дек
				generateTriade(matchLexem, false);
				if (swap) {
					global->typesMagazine.push_back(v2);
				}
				else {
					global->typesMagazine.push_back(v1);
				}
			}
			else {
				global->typesMagazine.push_back(v1);
			}
		}
	}
}

void ILGenerator::deltaPushOperand(bool isConst) {
    Operand operand;
    operand.isLink = false;
    memcpy(operand.lex, this->global->prevLex, sizeof(this->global->prevLex));
    if (isConst) {
        global->typesMagazine.push_back(TypeConstInt);
    }
    else {
        // Ищем в дереве тип переменной по имени и кладем в магазин типов
        global->typesMagazine.push_back(tree->GetTypeVarByLexem(global->prevLex));
    }
    global->resultsMagazine.push_back(operand);
}

void ILGenerator::generateTriade(std::string operationString, bool isOperation) {
	Lexem* operation = getLexemFromString(operationString);
    bool isShort = isShortTriad(operation);
	Triad triad{};
    memcpy(triad.operation, operation, sizeof(operation));

    Operand first = global->resultsMagazine.back();
    this->global->resultsMagazine.pop_back();
    triad.operand1 = first;

    if (!isShort) {
        // Если в триаде два параметра
        Operand second = global->resultsMagazine.back();
        this->global->resultsMagazine.pop_back();
        triad.operand2 = second;
    }

    this->global->triads.push_back(triad);

    if (memcmp(operation, "=", sizeof("=")) != 0) {
        // Если необходимо в дэк результатов положить ссылку на текущую триаду
        Operand triadeOperand;
        triadeOperand.isLink = true;
        triadeOperand.triadeNumber = this->global->triads.size() - 1;
        global->resultsMagazine.push_back(triadeOperand);
    }
}

bool ILGenerator::isShortTriad(Lexem* operation) {
    if (memcmp(operation, CtoS, sizeof(CtoS)) == 0 ||
        memcmp(operation, CtoI, sizeof(CtoI)) == 0 ||
        memcmp(operation, CtoL, sizeof(CtoL)) == 0 ||
        memcmp(operation, StoC, sizeof(StoC)) == 0 ||
		memcmp(operation, StoI, sizeof(StoI)) == 0 ||
		memcmp(operation, StoL, sizeof(StoL)) == 0 ||
		memcmp(operation, ItoC, sizeof(ItoC)) == 0 ||
		memcmp(operation, ItoS, sizeof(ItoS)) == 0 ||
		memcmp(operation, ItoL, sizeof(ItoL)) == 0 ||
		memcmp(operation, LtoC, sizeof(LtoC)) == 0 ||
		memcmp(operation, LtoS, sizeof(LtoS)) == 0 ||
        memcmp(operation, LtoI, sizeof(LtoI)) == 0) {
        return true;
    }
    return false;
}

std::string ILGenerator::getMatchLexem(TypeVar v1, TypeVar v2, bool* swap) {
	if (v1 >= TypeVoid || v2 >= TypeVoid) {
		std::cout << "Ошибка генератора. Неожиданный тип операнда -  " << v1 << " или " << v2 << std::endl;
		return "";
	}
	if (v1 < v2) {
		TypeVar buf = v1;
		v1 = v2;
		v2 = buf;
		*swap = true;
	}
	else {
		*swap = false;
	}

	switch (v2) {
	case TypeConstChar:
		return "";
	case TypeConstInt:
		return "";
	case TypeChar:
		switch (v1) {
		case TypeShort:
			return CtoS;
		case TypeInt:
			return CtoI;
		case TypeLong:
			return CtoL;
		default:
			std::cout << "Ошибка генератора. Невозможно привести -  " << v2 << " к " << v1 << std::endl;
			break;
		}
		break;
	case TypeShort:
		switch (v1) {
		case TypeChar:
			return StoC;
		case TypeInt:
			return StoI;
		case TypeLong:
			return StoL;
		default:
			std::cout << "Ошибка генератора. Невозможно привести -  " << v2 << " к " << v1 << std::endl;
			break;
		}
		break;
	case TypeInt:
		switch (v1) {
		case TypeChar:
			return ItoC;
		case TypeShort:
			return ItoS;
		case TypeLong:
			return ItoL;
		default:
			std::cout << "Ошибка генератора. Невозможно привести -  " << v2 << " к " << v1 << std::endl;
			break;
		}
		break;
	case TypeLong:
		switch (v1) {
		case TypeChar:
			return LtoC;
		case TypeShort:
			return LtoS;
		case TypeInt:
			return LtoI;
		default:
			std::cout << "Ошибка генератора. Невозможно привести -  " << v2 << " к " << v1 << std::endl;
			break;
		}
		break;
	default:
		std::cout << "Ошибка генератора. Невозможно привести -  " << v2 << " к " << v1 << std::endl;
		break;
	}
	std::cout << "Ошибка генератора. Невозможно привести -  " << v2 << " к " << v1 << std::endl;
	return "";
}