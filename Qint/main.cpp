#include "qint.h"

int main()
{
	Qint x(10, "651651651619");
	Qint y(10, "1642684142");
	x = x / y;
	std::cout << x.getBin() << std::endl;
	return 0;
}