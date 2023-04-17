#include "SemanticTree.h"
#define max(a,b) a<b? b : a

SemanticTree* SemanticTree::cur = NULL;
int SemanticTree::emptyCount = 1;

SemanticTree::SemanticTree(SemanticTree* left, SemanticTree* right, SemanticTree* up, Node* node)
{
	_left = left;
	_right = right;
	_up = up;
	_node = new Node();
	memcpy(_node, node, sizeof(Node));
}

SemanticTree::SemanticTree()
{
	_left = NULL;
	_right = NULL;
	_up = NULL;
	_node = new Node();
	_node->typeObject = ObjUndef;
	_node->typeVar = TypeUndef;

	//_node->param = std::vector<TypeVar>();
	//_node->dataAsInt = NULL;
	//_node->flagInit = false;
	_node->isEmpty = 0;
}

SemanticTree::~SemanticTree()
{
}

Node* SemanticTree::getNode()
{
	return _node;
}

Node* SemanticTree::getNodeByIdent(Lexem lexem)
{
	SemanticTree* temp = FindIdentUp(lexem, ObjVar);
	if (temp != NULL) {
		return temp->getNode();
	}
	else {
		temp = FindIdentUp(lexem, ObjConst);
		if (temp != NULL) {
			return temp->getNode();
		}
		else {
			std::string str(lexem);
			std::string s = "Не удалось найти вершину '" + str + "' для присвоения, потому чтo она не объявлена ранее\n";
			PrintError(s);
			return NULL;
		}
	}
}

SemanticTree* SemanticTree::getLchild()
{
	return _left;
}

SemanticTree* SemanticTree::getRchild()
{
	return _right;
}

SemanticTree* SemanticTree::getUp()
{
	return _up;
}

void SemanticTree::IncEmptyCount()
{
	emptyCount++;
}

int SemanticTree::GetEmptyCount()
{
	return emptyCount;
}

void SemanticTree::SetRightEmpty()
{
	Node* node = new Node();
	node->isEmpty = emptyCount;
	IncEmptyCount();

	SetCur(cur->SetRight(node));
}

SemanticTree* SemanticTree::SetLeft(Node* node)
{
	SemanticTree* a = new SemanticTree(NULL, NULL, this, node); // новая вершина
	_left = a;
	return a;
}

SemanticTree* SemanticTree::SetRight(Node* node)
{
	SemanticTree* a = new SemanticTree(NULL, NULL, this, node); // новая вершина
	_right = a;
	return a;
}

void SemanticTree::DeleteRight()
{
	cur->_right = nullptr;
}

//SemanticTree* SemanticTree::FillNode(Lexem lexem, TypeObject typeObject, TypeVar typeVar, std::vector<TypeVar>* param, DataValue data, bool flagInit)
SemanticTree* SemanticTree::FillNode(Lexem lexem, TypeObject typeObject, TypeVar typeVar)
{
	Node* node = new Node();
	memcpy(node->id, lexem, GetLexemLength(lexem));
	node->typeObject = typeObject;
	node->typeVar = typeVar;
	//if (typeObject == ObjFunc) {
	//	node->data.param = param;
	//}
	//else if (typeVar == TypeShort) {
	//	node->data.dataAsShort = data.dataAsShort;
	//}
	//else if (typeVar == TypeInt) {
	//	node->data.dataAsInt = data.dataAsInt;
	//}
	//else {

	//}
	//node->flagInit = flagInit;
	node->isEmpty = 0;

	SemanticTree* ret = cur->SetLeft(node);
	SetCur(ret);
	return ret;
}

//SemanticTree* SemanticTree::AddFunctionInTree(Lexem lexem, std::vector<TypeVar>* param)
SemanticTree* SemanticTree::AddFunctionInTree(Lexem lexem)
{
	if (FindIdentUp(lexem, ObjFunc) == NULL) {
		// Если до этого не было объекта такого типа с таким же именем
		//DataValue a;
		//a.param = param;
		//return FillNode(lexem, ObjFunc, TypeVoid, param, a, false);
		return FillNode(lexem, ObjFunc, TypeVoid);
	}
	else {
		std::string str(lexem);
		std::string s = "Не удалось добавить вершину с именем'" + str + "', потому чтo она объявлена ранее\n";
		PrintError(s);
	}
}

void SemanticTree::AddConstantInTree(Lexem lexem, TypeVar typeVar)
{
	if (FindIdentUp(lexem, ObjConst) == NULL) {
		// Если до этого не было объекта такого типа с таким же именем
		//std::vector<TypeVar>* empty = new std::vector<TypeVar>{};
		//FillNode(lexem, ObjConst, typeVar, empty, data, true);
		FillNode(lexem, ObjConst, typeVar);
	}
	else {
		std::string str(lexem);
		std::string s = "Не удалось добавить вершину с именем'" + str + "', потому чтo она объявлена ранее\n";
		PrintError(s);
	}
}

// SemanticTree::AddDataInTree(Lexem lexem, TypeVar typeVar, DataValue data, bool flagInit)
void SemanticTree::AddDataInTree(Lexem lexem, TypeVar typeVar)
{
	if (FindIdentUp(lexem, ObjVar) == NULL) {
		// Если до этого не было объекта такого типа с таким же именем
		//std::vector<TypeVar>* empty = new std::vector<TypeVar>{};
		//FillNode(lexem, ObjVar, typeVar, empty, data, flagInit);
		FillNode(lexem, ObjVar, typeVar);
	}
	else {
		std::string str(lexem);
		std::string s = "Не удалось добавить вершину с именем'" + str + "', потому чтo она объявлена ранее\n";
		PrintError(s);
	}
}

void SemanticTree::Print(void)
{
	if (std::string(_node->id) == std::string(""))
	{
		printf("Область видимости (%d)", _node->isEmpty);
	}
	else {
		std::string id(_node->id);
		std::string obj = ConvertTypeObjectToString(_node->typeObject);
		std::string var = ConvertTypeVarToString(_node->typeVar);
		//std::string flag = ConvertFlaginitToString(_node->flagInit);
		//std::string value = ConvertValueToString(_node->data, _node->typeVar);
		//if (_node->flagInit) {
		//	std::cout << "Вершина с именем '" + id + "', данные о вершине --> (" + obj + ", " + var + ", " + flag + ", " + value + ")";
		//}
		//else {
		//	std::cout << "Вершина с именем '" + id + "', данные о вершине --> (" + obj + ", " + var + ", " + flag + ")";
		//}
		std::cout << "Вершина с именем '" + id + "', данные о вершине --> (" + obj + ", " + var + ")";
	}
	if (_left != NULL) printf("\n\t слева вершина с именем '%s'", _left->_node->id);
	if (_right != NULL) {
		if (std::string(_right->_node->id) == std::string(""))
		{
			printf("\n\t справа пустая вершина -> новая область видимости (%d)", _right->_node->isEmpty);
		}
		else {
			printf("\n\t справа вершина с именем '%s'", _right->_node->id);
		}
	}
	printf("\n\n");
	// АНАЛОГИЧНЫЙ ИФ
	if (_left != NULL) _left->Print();
	if (_right != NULL) _right->Print();
}

void SemanticTree::PrintError(std::string error)
{
	std::cout << error;
	//exit(0);
}

//void SemanticTree::SetParam(std::vector<TypeVar>* param)
//{
//	_node->data.param = param;
//}

void SemanticTree::FindEmptyUp()
{
	while (cur->getNode()->isEmpty == 0) {
		cur = cur->getUp();
	}
	cur = cur->getUp();
}

//void SemanticTree::SetData(Lexem lexem, DataValue data)
//{
//	SemanticTree* temp = FindIdentUp(lexem, ObjVar);
//	if (temp != NULL) {
//		temp->getNode()->data = data;
//		temp->getNode()->flagInit = true;
//	}
//	else {
//		std::string str(lexem);
//		std::string s = "Не удалось установить данные в вершину '" + str + "', потому чтo она не объявлена ранее\n";
//		PrintError(s);
//	}
//}

TypeVar SemanticTree::GetTypeVarByLexem(Lexem lexem)
{
	SemanticTree* temp = FindIdentUp(lexem);
	if (temp != NULL) {
		return temp->getNode()->typeVar;
	}
	return TypeUndef;
}

SemanticTree* SemanticTree::FindIdentUp(Lexem lexem, TypeObject typeObject)
{
	SemanticTree* temp = cur;
	while (std::string(temp->getNode()->id) != std::string(lexem) || temp->getNode()->typeObject != typeObject) {
		if (temp->getUp() != NULL)
			temp = temp->getUp();
		else
			return NULL;
	}
	return temp;
}

SemanticTree* SemanticTree::FindIdentUp(Lexem lexem)
{
	SemanticTree* temp = cur;
	while (std::string(temp->getNode()->id) != std::string(lexem)) {
		if (temp->getUp() != NULL)
			temp = temp->getUp();
		else
			return NULL;
	}
	return temp;
}

//void SemanticTree::CheckInit(Lexem lexem)
//{
//	SemanticTree* temp = FindIdentUp(lexem);
//	if (temp != NULL) {
//		if (temp->getNode()->flagInit == false) {
//			std::string str(lexem);
//			std::string s = "Переменная '" + str + "' не инициализирована перед использованием\n";
//			PrintError(s);
//		}
//	}
//	else {
//		std::string str(lexem);
//		std::string s = "Не удалось проверить инициализацию переменной '" + str + "', потому чтo она не объявлена ранее\n";
//		PrintError(s);
//	}
//}

//void SemanticTree::CheckInitByNode(Node* node) {
//	if (node->flagInit) {
//		return;
//	}
//	else {
//		std::string str(node->id);
//		std::string s = "Переменная '" + str + "' не инициализирована перед использованием\n";
//		PrintError(s);
//	}																 
//}

std::string SemanticTree::ConvertTypeVarToString(TypeVar typeVar)
{
	switch (typeVar)
	{
		case TypeConstChar:
			return "TypeConstChar";
		case TypeChar:
			return "TypeChar";
	case TypeShort:
		return "TypeShort";
	case TypeConstInt:
		return "TypeConstInt";
	case TypeInt:
		return "TypeInt";
		case TypeConstHex:
			return "TypeConstHex";
		case TypeLong:
			return "TypeLong";
	case TypeVoid:
		return "TypeVoid";
	case TypeIdent:
		return "TypeIdent";
	case TypeUndef:
		return "TypeUndef";
	default:
		break;
	}
}

TypeVar SemanticTree::ConvertTypeLexemToTypeVar(int typeLexem)
{
	switch (typeLexem)
	{
		//case TChar:
		//	return TypeChar;
	case TShort:
		return TypeShort;
	case TInt:
		return TypeInt;
		//case TLong:
		//	return TypeLong;
	case TConstInt:
		return TypeConstInt;
		//case TConstChar:
		//	return TypeConstChar;
		//case TConstHex:
		//	return TypeConstHex;
	case TVoid:
		return TypeVoid;
	case TIdent:
		return TypeIdent;
	default:
		return TypeUndef;
	}
}

std::string SemanticTree::ConvertFlaginitToString(bool flag)
{
	if (flag) {
		return std::string("инициализирована");
	}
	else {
		return std::string("не инициализирована");
	}
}

std::string SemanticTree::ConvertTypeObjectToString(TypeObject typeObject)
{
	switch (typeObject)
	{
	case ObjConst:
		return "ObjConst";
	case ObjVar:
		return "ObjVar";
	case ObjFunc:
		return "ObjFunc";
	case ObjUndef:
		return "ObjUndef";
	default:
		break;
	}
}

std::string SemanticTree::ConvertValueToString(DataValue dataValue, TypeVar typeVar) {
	if (typeVar == TypeInt) {
		return std::to_string(dataValue.dataAsInt);
	}
	else if (typeVar == TypeShort) {
		return std::to_string(dataValue.dataAsShort);
	}
	return std::string("");
}


int SemanticTree::GetLexemLength(Lexem lexem)
{
	int len = 0;
	for (int i = 0; i < MAX_LEXEM_LENGHT; i++)
		if (lexem[i] == 0) {
			len = i;
			break;
		}
	return len;
}

bool SemanticTree::CheckAssignment(TypeVar left, TypeVar right)
{
	if (left >= right) {
		return true;
	}
	else {
		std::string strLeft(ConvertTypeVarToString(left));
		std::string strRight(ConvertTypeVarToString(right));
		std::string s = "Переменной типа '" + strLeft + "' нельзя присвоить значение типа '" + strRight + "'\n";
		PrintError(s);
		return false;
	}
}

bool SemanticTree::vectorEquivalence(std::vector<TypeVar> f, std::vector<TypeVar> s)
{
	if (f.size() == s.size()) {
		int count = f.size();
		for (int i = 0; i < count; i++) {
			if (f[i] != s[i])
				return false;
		}
		return true;
	}
	else return false;
}

//void SemanticTree::CheckParamList(Lexem lexem, std::vector<TypeVar> param)
//{
//	SemanticTree* temp = FindIdentUp(lexem, ObjFunc);
//	if (!vectorEquivalence(*(temp->getNode()->data.param), param)) {
//		std::string str(lexem);
//		std::string s = "Параметры функции '" + str + "' при вызове не совпадают с параметрами при объявлении\n";
//		PrintError(s);
//	}
//}

long SemanticTree::ConvertTypeLexemToNumber(Lexem lexem)
{
	int len = GetLexemLength(lexem);

	long result = 0;
	for (int i = 0; i < len; i++) {
		result *= 10;
		result += lexem[i] - 48;
	}
	return result;
}