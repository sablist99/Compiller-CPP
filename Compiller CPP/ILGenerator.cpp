#include "ILGenerator.h"

ILGenerator::ILGenerator(SemanticTree* tr, GlobalData* gl)
{
    tree = tr;
    global = gl;
}

void ILGenerator::deltaFind() {

}

void ILGenerator::deltaMatch(bool isLeftMatch) {
	TypeVar v2 = global->typesMagazine.back();         // ������ �������
	global->typesMagazine.pop_back();
	TypeVar v1 = global->typesMagazine.back();         // ������ �������
	global->typesMagazine.pop_back();
    if (v1 == v2) {             //  ���� �����, ���������� ���
        global->typesMagazine.push_back(v1);
    }
	else {
		// ������������� �� ����� ����� (��������, ��� ������������) 
		bool swap = false;
		Lexem* matchLexem = getMatchLexem(v1, v2, &swap);
		if (isLeftMatch) {
			if (swap) {
				generateTriade(matchLexem, false);
			}
			global->typesMagazine.push_back(v1);
		}
		else {
			if (matchLexem != NULL) {
				// ���� ������� ��� ���� �����
				// ����������, ����� ��� �������� � ���
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
        // ���� � ������ ��� ���������� �� ����� � ������ � ������� �����
        global->typesMagazine.push_back(tree->GetTypeVarByLexem(global->prevLex));
    }
    global->resultsMagazine.push_back(operand);
}

void ILGenerator::generateTriade(Lexem* operation, bool isOperation) {
    bool isShort = isShortTriad(operation);
    Triad triad;
    memcpy(triad.operation, operation, sizeof(operation));

    Operand first = global->resultsMagazine.back();
    this->global->resultsMagazine.pop_back();
    triad.operand1 = first;

    if (!isShort) {
        // ���� � ������ ��� ���������
        Operand second = global->resultsMagazine.back();
        this->global->resultsMagazine.pop_back();
        triad.operand2 = second;
    }

    this->global->triads.push_back(triad);

    if (memcmp(operation, "=", sizeof("=")) != 0) {
        // ���� ���������� � ��� ����������� �������� ������ �� ������� ������
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
        memcmp(operation, StoI, sizeof(StoI)) == 0 ||
        memcmp(operation, StoL, sizeof(StoL)) == 0 ||
        memcmp(operation, ItoL, sizeof(ItoL)) == 0) {
        return true;
    }
    return false;
}

Lexem* ILGenerator::getMatchLexem(TypeVar v1, TypeVar v2, bool* swap) {
	if (v1 >= TypeVoid || v2 >= TypeVoid) {
		std::cout << "������ ����������. ����������� ��� �������� -  " << v1 << " ��� " << v2 << std::endl;
		return NULL;
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

	Lexem l;
	switch (v2) {
	case TypeConstChar:
		return NULL;
	case TypeConstInt:
		return NULL;
	case TypeChar:
		switch (v1) {
		case TypeShort:
			memcpy(l, CtoS, sizeof(CtoS));
			return &l;
		case TypeInt:
			memcpy(l, CtoI, sizeof(CtoI));
			return &l;
		case TypeLong:
			memcpy(l, CtoL, sizeof(CtoL));
			return &l;
		default:
			std::cout << "������ ����������. ���������� �������� -  " << v2 << " � " << v1 << std::endl;
			break;
		}
		break;
	case TypeShort:
		switch (v1) {
		case TypeChar:
			memcpy(l, StoC, sizeof(StoC));
			return &l;
		case TypeInt:
			memcpy(l, StoI, sizeof(StoI));
			return &l;
		case TypeLong:
			memcpy(l, StoL, sizeof(StoL));
			return &l;
		default:
			std::cout << "������ ����������. ���������� �������� -  " << v2 << " � " << v1 << std::endl;
			break;
		}
		break;
	case TypeInt:
		switch (v1) {
		case TypeChar:
			memcpy(l, ItoC, sizeof(ItoC));
			return &l;
		case TypeShort:
			memcpy(l, ItoS, sizeof(ItoS));
			return &l;
		case TypeLong:
			memcpy(l, ItoL, sizeof(ItoL));
			return &l;
		default:
			std::cout << "������ ����������. ���������� �������� -  " << v2 << " � " << v1 << std::endl;
			break;
		}
		break;
	case TypeLong:
		switch (v1) {
		case TypeChar:
			memcpy(l, LtoC, sizeof(LtoC));
			return &l;
		case TypeShort:
			memcpy(l, LtoS, sizeof(LtoS));
			return &l;
		case TypeInt:
			memcpy(l, LtoI, sizeof(LtoI));
			return &l;
		default:
			std::cout << "������ ����������. ���������� �������� -  " << v2 << " � " << v1 << std::endl;
			break;
		}
		break;
	default:
		std::cout << "������ ����������. ���������� �������� -  " << v2 << " � " << v1 << std::endl;
		break;
	}
	std::cout << "������ ����������. ���������� �������� -  " << v2 << " � " << v1 << std::endl;
	return NULL;
}