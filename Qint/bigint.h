#ifndef __BIGINT__
#define __BIGINT__
#include<algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

/*
Class định nghĩa các đối tượng số lớn có kích thước tối đa khoảng 10^8 chữ số
Số lớn được biểu diễn ở hệ 10^9 (Có thể tùy biến lại hệ trong tương lai).
Class gồm có: Mảng các biến kiểu int, mỗi phần tử được xem như 1 "chữ số" của 1 số lớn
Các lưu trữ: Các "chữ số" sẽ được lưu ngược với tự nhiên, các chữ số mang giá trị lớn sẽ nằm sau các chữ số mang giá trị nhỏ (Tiện cho việc tính toán)
Các phép toán hỗ trợ:
	-Chuyển 1 số nhỏ thành số lớn.
	-Cộng 2 số lớn.
	-Trừ 2 số lớn
	-Nhân số lớn với số nhỏ.
	-Chia lấy phần nguyên số lớn với số nhỏ.
	-Chia lấy phần dư số lớn với số nhỏ.
	-Nhân số lớn với số lớn.
	-Chia số lớn với số lớn.
	-Gán số nhỏ cho số lớn
	-Gán số lớn cho số lớn
	-Nhập xuất số lớn
*/

class BigInt
{
private:
	int* data;
	int size;
public:
	static const int maxSize = 2e3;
	static const int base = 1e9;

	int getSize();
	
	//CONSTRUCTOR
	//Mặc định giá trị là 0
	BigInt();				//DONE

	BigInt(int);			//DONE

	BigInt(std::string);	//DONE

	BigInt(const BigInt&);	//DONE

	//Operator
	friend BigInt operator+(const BigInt& a, const BigInt& b);	//DONE

	BigInt& operator+=(const BigInt& other);					//DONE

	friend BigInt operator-(const BigInt& a, const BigInt& b);	//DONE
	
	BigInt& operator-=(const BigInt& other);					//DONE	

	BigInt operator*(int);										//DONE

	friend BigInt operator*(const BigInt& a, const BigInt& b);	//DONE

	BigInt& operator*=(int);									//DONE

	friend BigInt operator/(const BigInt& a, int b);			//DONE

	BigInt& operator/=(int);									//DONE

	int operator%(int b);										//DONE

	BigInt& operator=(int);										//DONE

	BigInt& operator=(const BigInt& a);							//DONE

	friend std::istream& operator>>(std::istream& input, BigInt&);	//DONE

	friend std::ostream& operator<<(std::ostream& output, BigInt&);	//DONE
	
	void debug();												//DONE
	
	//DESTRUCTOR
	~BigInt();

	//addition function
	static int numOfDigits(int x);								//DONE								//DONE

	std::string getNum();										//DONE

//	void resize(int size);

	bool isZero();												//DOEN
};

#endif