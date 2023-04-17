
int d;
const int conI = 0xab2;
const short conS = 0xab2;
const char conC = 0xab2;
const long conL = 0xab2;

void abc(int i, short s, char c, long l) {
	const long conLabc = 0xab2;
	int in = 10;
	short sh = 10;
	char cn;
	long lo;
}

void abc1(int i, short s, char c, long l) {
	const long conLabc = 0xab2;
	int in = 10;
	short sh = 10;
	char cn;
	long lo;
	{
		int sub1;
		const long conLSUB = 0xab2;
	}
}

void main() {
	abc();
	abc1(y);
	while (a < b) {
		b = a;
		const long conLmain = 0xab2;
	}
}


