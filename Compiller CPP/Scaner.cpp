#include "Scaner.h"
				 


Lexem Keyword[KEY_WORD_COUNT] = {
	"int", "short", "long", "char", "void", "return", "break", "while", "const"
};

int IndexKeyword[KEY_WORD_COUNT] = {
	TInt, TShort, TLong, TChar, TVoid, TReturn, TBreak, TWhile, TConst
	//TInt, TShort, TVoid, TReturn, TBreak, TWhile, TConst
};

ScanerPosition Scaner::getScanerPosition()
{
	return { _pointer, _line, _position };
}

void Scaner::setScanerPosition(ScanerPosition sp)
{
	_pointer = sp.ptr;
	_position = sp.pos;
	_line = sp.line;
}

void Scaner::setPointer(int p)
{
	_pointer = p;
}

int Scaner::getPointer(void)
{
	return _pointer;
}

void Scaner::setPosition(int p)
{
	_position = p;
}

int Scaner::getPosition(void)
{
	return _position;
}

void Scaner::setLine(int l)
{
	_line = l;
}

int Scaner::getLine(void)
{
	return _line;
}

void Scaner::printError(char* error, char* lexem, int numberOfString, int numberOfPosition)
{
	if (numberOfString == -1 || numberOfPosition == -1)
	{
		if (lexem[0] == '\0')
			printf("Ошибка : %s\n", error);
		else
			printf("Ошибка : %s. Неверный символ %s\n", error, lexem);
	}
	else
		if (lexem[0] == '\0')
			printf("Ошибка : %s %s в строке номер %d на позиции %d\n", error, lexem, numberOfString + 1, numberOfPosition + 1);
		else
			printf("Ошибка : %s. Неверный символ %s в строке номер %d на позиции %d\n", error, lexem, numberOfString + 1, numberOfPosition + 1);

	//exit(0);
}

int Scaner::scaner(Lexem l)
{
	int i; // текущая длина лексемы
	for (i = 0; i < MAX_LEXEM_LENGHT; i++)
		l[i] = 0;
	i = 0;
	bool flag = true;
	while (flag) {
		while ((_text[_pointer] == ' ') || (_text[_pointer] == '\n') || (_text[_pointer] == '\t'))
		{// Игнорируемые символы
			if (_text[_pointer] == '\n')
			{ // Нужно увеличить счетчик строк
				_line++;
				_errorPosition = _pointer;
				_position = 0;
			}
			_pointer++;
			_position++;
		}
		if ((_text[_pointer] == '/') && (_text[_pointer + 1] == '/')) // Однострочные комментарии
		{
			_pointer += 2;
			_position += 2;
			while ((_text[_pointer] != '\n') && (_text[_pointer] != '\0')) {
				_pointer++;
				_position++;
			}
		}
		else
			flag = false;
	}

	if (_text[_pointer] == '\0')
	{ // Конец файла
		l[0] = '#';
		return TEnd;
	}


	if (isNumber(_text[_pointer])) {
		if (_text[_pointer] == '0' && _text[_pointer + 1] == 'x') {
			l[i++] = _text[_pointer++];
			l[i++] = _text[_pointer++];
			_position += 2;
			while (isHex(_text[_pointer])) {
				if (i < MAX_LEXEM_LENGHT - 1)
					l[i++] = _text[_pointer++];
				else
					_pointer++;
				_position++;
			}
			return TConstHex;
		}
		l[i++] = _text[_pointer++];
		while (isNumber(_text[_pointer])) {
			if (i < MAX_LEXEM_LENGHT - 1)
				l[i++] = _text[_pointer++];
			else
				_pointer++;
			_position++;
		}

		return TConstInt;
	}

	else if (isLCase(_text[_pointer]) || isUCase(_text[_pointer]))
	{ //начался идентификатор
		l[i++] = _text[_pointer++];
		_position++;
		while (isNumber(_text[_pointer]) || isLCase(_text[_pointer]) || isUCase(_text[_pointer]) || _text[_pointer] == '_') {
			if (i < MAX_LEXEM_LENGHT - 1)
				l[i++] = _text[_pointer++];
			else
				_pointer++;
			_position++;
		}
		// если идентификатор слишком длинный обрезаем его
		int j; // проверяем идентификатора на принадлежность ключевым словам
		for (j = 0; j < KEY_WORD_COUNT; j++)
			if (strcmp(l, Keyword[j]) == 0) {
				return IndexKeyword[j];
			}
		return TIdent;
	}
	else {
		switch (_text[_pointer]) {
		case '\'':
			_pointer++; // не будем включать кавычки в константу
			_position++;
			if ((_text[_pointer] != '\'') && (_text[_pointer] != '\n') && (_text[_pointer] != '#'))
			{
				l[i++] = _text[_pointer++];
				_position++;
			}
			if (_text[_pointer] != '\'')
			{
				printError(_strdup(ERROR_CHAR_CONAT), l, _line, _pointer - _errorPosition);
				return TError;
			}
			_pointer++; // Закрывающая кавычка
			_position++;
			return TConstChar;

		case '=':
			l[i++] = _text[_pointer++];
			_position++;
			if (_text[_pointer] == '=') {
				l[i++] = _text[_pointer++];
				_position++;
				return TE;
			}
			return TAssign;

		case '+':
			l[i++] = _text[_pointer++];
			_position++;
			return TPlus;

		case '-':
			l[i++] = _text[_pointer++];
			_position++;
			return TMinus;

		case '*':
			l[i++] = _text[_pointer++];
			_position++;
			return TMult;

		case '/':
			l[i++] = _text[_pointer++];
			_position++;
			return TDiv;

		case '%':
			l[i++] = _text[_pointer++];
			_position++;
			return TMod;

		case '<':
			l[i++] = _text[_pointer++];
			_position++;
			if (_text[_pointer] == '<') {
				l[i++] = _text[_pointer++];
				_position++;
				return TLSh;
			}
			if (_text[_pointer] == '=') {
				l[i++] = _text[_pointer++];
				_position++;
				return TLE;
			}
			return TL;
		case '>':
			l[i++] = _text[_pointer++];
			_position++;
			if (_text[_pointer] == '>') {
				l[i++] = _text[_pointer++];
				_position++;
				return TRSh;
			}
			if (_text[_pointer] == '=') {
				l[i++] = _text[_pointer++];
				_position++;
				return TRE;
			}
			return TR;

		case '!':
			l[i++] = _text[_pointer++];
			_position++;
			if (_text[_pointer] == '=') {
				l[i++] = _text[_pointer++];
				_position++;
				return TNE;
			}
			else {
				printError(_strdup(ERROR_CHAR), l, _line, _pointer - _errorPosition);
				return TError;
			}

		case '(':
			l[i++] = _text[_pointer++];
			_position++;
			return TLPar;

		case ')':
			l[i++] = _text[_pointer++];
			_position++;
			return TRPar;

		case '{':
			l[i++] = _text[_pointer++];
			_position++;
			return TFLPar;

		case '}':
			l[i++] = _text[_pointer++];
			_position++;
			return TFRPar;

		case ';':
			l[i++] = _text[_pointer++];
			_position++;
			return TSemicolon;

		case ',':
			l[i++] = _text[_pointer++];
			_position++;
			return TComma;

		default:
			l[i] = _text[_pointer];
			printError(_strdup(ERROR_CHAR), l, _line, _pointer - _errorPosition);
			_pointer++;
			_position++;
			return TError;


		}
	}
}

void Scaner::readFile(char* fileName)
{
	FILE* in = fopen(fileName, "r");

	if (in == NULL) {
		printError(_strdup(FILE_DONT_EXIST), _strdup(EMPTY_STRING), -1, -1);
		exit(0);
	}
	int i = 0;
	char buf;
	while (!feof(in))
	{
		fscanf(in, "%c", &buf);
		if (!feof(in)) _text[i++] = buf;
		if (i >= MAX_TEXT_LENGHT - 1)
		{ // Если файл длинный
			printError(_strdup(FILE_VERY_LONG), _strdup(EMPTY_STRING), -1, -1);
			break;
		}
	}
	_text[i] = '\0';
	fclose(in);
}

bool Scaner::isNumber(char buf)
{
	if ((buf <= '9') && (buf >= '0'))
		return true;
	else return false;
}

bool Scaner::isHex(char buf)
{
	if (((buf >= '0') && (buf <= '9')) || (buf >= 'a') && (buf <= 'f') || (buf >= 'A') && (buf <= 'F'))
		return true;
	else return false;
	return false;
}

bool Scaner::isLCase(char buf)
{
	if ((buf >= 'a') && (buf <= 'z'))
		return true;
	else return false;
}

bool Scaner::isUCase(char buf)
{
	if ((buf >= 'A') && (buf <= 'Z'))
		return true;
	else return false;
}

Scaner::Scaner(char* fileName)
{
	readFile(fileName);
	setPointer(0);
	_position = 0;
	_line = 0;
	_errorPosition = 0;
}

Scaner::Scaner()
{
}

Scaner::~Scaner()
{
}


