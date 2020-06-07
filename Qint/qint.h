#ifndef __QINT_H__
#define __QINT_H__
#include <iostream>
#include <algorithm>
#include <string>
#include "bigint.h"
using std::string;

class Qint
{
private:
	int data[4];
	bool getBit(int pos);				//DONE
	void setBit(int pos, bool value);	//DONE
	void decToBin(string& str);			//DONE
	void hexToBin(string& str);			//DONE
	void toNegative();					//DONE
public:
	//CONSTRUCTOR
	Qint();						//DONE
	Qint(int base, string num);	//DONE
	Qint(const Qint&);			//DONE
	
	//GETTER
	std::string getBin();	//DONE
	std::string getDec();	//DONE
	std::string getHex();	//DONE
	std::string getNum(int);  //DONE
	//OPERATOR
	//1.0
	Qint& operator=(const Qint&);								//DONE
	friend Qint operator+(const Qint&, const Qint&);			//DONE
	friend Qint operator-(const Qint&, const Qint&);			//DONE
	friend Qint operator*(Qint&, Qint&);			//DONE
	friend Qint operator/(Qint&, Qint&);			//DONE
	
	//2.0
	Qint operator >>(int);										//DONE
	Qint operator <<(int);										//DONE
	Qint sar(int); //Arithmetric right shift					//DONE
	
	//3.0
	friend Qint operator&(const Qint&, const Qint&);			//DONE
	friend Qint operator|(const Qint&, const Qint&);			//DONE
	friend Qint operator^(const Qint&, const Qint&);			//DONE
	Qint operator~();

	//4.0
	Qint rol();		//DONE
	Qint ror();		//DONE

	//ADDITIONAL FUNCTION
	bool isNegative();
	static int add(int, int, int&);						//DONE
	static bool getBit(int x, int pos);					//DONE
	static void setBit(int& x, int pos, int value);		//DONE
};
#endif
