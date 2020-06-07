#include "qint.h"
#include <fstream>
#include <conio.h>
int main(int argc, char* argv[])
{
	std::ifstream input(argv[1]);
	std::ofstream output(argv[2]);
	
	while (input.eof() == false)
	{
		string base1;
		string base2;
		input >> base1 >> base2;
		if (base1 == "")
			break;
		if (base2 == "2" || base2 == "10" || base2 == "16")
		{
			string str;
			input >> str;
			Qint num(std::stoi(base1), str);
			output << num.getNum(std::stoi(base2)) << std::endl;
		}
		else if (base2 == "ror" || base2 == "rol" || base2 == "~")
		{
			string str;
			input >> str;
			Qint num(std::stoi(base1), str);
			if (base2 == "ror")
				num = num.ror();
			else if (base2 == "rol")
				num = num.rol();
			else
				num = ~num;
			output << num.getNum(std::stoi(base1)) << std::endl;
		}
		else
		{
			string opr;
			input >> opr;
			Qint firstNum(std::stoi(base1), base2);
			Qint num;
			if (opr == "<<")
			{
				int times;
				input >> times;
				num = firstNum << times;
				output << num.getNum(std::stoi(base1)) << std::endl;
				continue;
			}
			else if (opr == ">>")
			{

				int times;
				input >> times;
				num = firstNum.sar(times);
				output << num.getNum(std::stoi(base1)) << std::endl;
				continue;
			}
			string str2;
			input >> str2;
			Qint secondNum(std::stoi(base1), str2);
			if (opr == "+")
				num = firstNum + secondNum;
			else if (opr == "-")
				num = firstNum - secondNum;
			else if (opr == "*")
				num = firstNum * secondNum;
			else if (opr == "/")
				num = firstNum / secondNum;
			else if (opr == "&")
				num = firstNum & secondNum;
			else if (opr == "|")
				num = firstNum | secondNum;
			else if (opr == "^")
				num = firstNum ^ secondNum; 
			output << num.getNum(std::stoi(base1)) << std::endl;
		}
	}
	input.close();
	output.close();
	return 0;
}