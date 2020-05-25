#include "bigint.h"

int BigInt::getSize()
{
	return size;
}

BigInt::BigInt()
{
	data = new int[maxSize];
	for (int i = 0; i < maxSize; i++)
		data[i] = 0;
	size = 1;
}

BigInt::BigInt(int num) : BigInt()
{
	data[0] = num;
	size = 1;
}

BigInt::BigInt(std::string num) : BigInt()
{
	size = 0;
	int temp = 0;
	std::reverse(num.begin(), num.end());
	while (num.size() > 1 && num[num.size() - 1] == '0')
		num.resize(num.size() - 1);
	for (int i = num.size() - 1; i >= 0; i--)
	{
		temp = temp * 10 + (num[i] - '0');
		if (i % 9 == 0)	//Tại các chữ số ở vị trí chia hết cho 9 sẽ là chữ số đơn vị của phần tử i/9
		{
			data[i/9] = temp;
			temp = 0;
			size++;
		}
	}
}

BigInt::BigInt(const BigInt& source) : BigInt()
{
	size = source.size;
	for (int i = 0; i < size; i++)
	{
		data[i] = source.data[i];
	}
}

BigInt& BigInt::operator+=(const BigInt& other)
{
	int memo = 0;
	size = std::max(size, other.size);
	for (int i = 0; i < size; i++)
	{
		memo = memo + data[i] + other.data[i];
		data[i] = memo % BigInt::base;
		memo = memo / BigInt::base;
	}
	if (memo > 0)
	{
		data[size] = memo;
		size++;
	}
	return *this;
}

BigInt& BigInt::operator-=(const BigInt& other)
{
	int memo = 0;
	for (int i = 0; i < size; i++)
	{
		memo = data[i] - other.data[i] - memo;
		if (memo < 0)
		{
			data[i] = memo + BigInt::base;
			memo = 1;
		}
		else
		{
			data[i] = memo;
			memo = 0;
		}
	}
	while (size > 1 && data[size - 1] == 0)
		size--;
	return *this;
}

BigInt& BigInt::operator=(int x)
{
	size = 1;
	data[0] = x;
	return *this;
}

BigInt& BigInt::operator=(const BigInt& source)
{
	if (this != &source)
	{
		size = source.size;
		for (int i = 0; i < source.size; i++)
			data[i] = source.data[i];
	}
	return *this;
}

void BigInt::debug()
{
	for (int i = 0; i < size; i++)	
		std::cout << data[i] << " ";
	std::cout << std::endl << size << std::endl;
}

BigInt::~BigInt()
{
	if (data == NULL)
		return;
	delete[] data;
	size = 0;
}

int BigInt::numOfDigits(int x)
{
	if (x == 0)
		return 1;
	return int(log10(x) + 1);
}

std::string BigInt::getNum()
{
	std::string ans;
	//Xử lý phần tử cao nhất
	ans = ans + std::to_string(data[size - 1]);
	
	//Các phần tử còn lại
	for (int i = size - 2; i >= 0; i--)
	{
		if (data[i] == 0)
			ans = ans + "000000000";
		else
		{
			for (int j = numOfDigits(data[i]); j < 9; j++)
				ans = ans + '0';
			ans = ans + std::to_string(data[i]);
		}
	}
	return ans;
}

//void BigInt::resize(int newSize)
//{
//	if(newSize < size)
//	{
//		//Nếu số lượng chữ số của phần tử cuối cùng nhiều hơn số lượng muốn resize 
//		//Thì bỏ những chữ số đó
//		//Bằng các % 10^x
//		//x là số lượng chữ số mong muốn
//		int pow = 1;
//		for (int i = 1; i <= newSize - (arraySize() - 1) * 9; i++)
//			pow = pow * 10;
//		data[arraySize() - 1] %= pow;
//	}
//	size = newSize;
//}

bool BigInt::isZero()
{
	if (size == 1 && data[0] == 0)
		return true;
	return false;
}

BigInt operator+(const BigInt& a, const BigInt& b)
{
	BigInt c;
	c.size = std::max(a.size, b.size);
	int memo = 0;
	for (int i = 0; i < c.size; i++)
	{
		memo = memo + a.data[i] + b.data[i];
		c.data[i] = memo % BigInt::base;
		memo = memo / BigInt::base;
	}
	if (memo > 0)
	{
		c.data[c.size] = memo;
		c.size++;
	}
	return c;
}

BigInt operator-(const BigInt& a, const BigInt& b)
{
	int memo = 0;
	BigInt c;
	c.size = std::max(a.size, b.size);
	for (int i = 0; i < c.size; i++)
	{
		memo = a.data[i] - b.data[i] - memo;
		if (memo < 0)
		{
			c.data[i] = memo + BigInt::base;
			memo = 1;
		}
		else
		{
			c.data[i] = memo;
			memo = 0;
		}
	}
	while (c.size > 1 && c.data[c.size - 1] == 0)
		c.size--;
	return c;
}

BigInt BigInt::operator*(int b) 
{
	BigInt c;
	if (b == 0 || isZero())
		return c;
	c.size = size;
	long long memo = 0;
	for (int i = 0; i < c.size; i++)
	{ 
		memo = memo + long long(data[i]) * long long(b);
		long long tmp = memo % BigInt::base;
		c.data[i] = tmp;
		memo = memo / BigInt::base;
	}
	if (memo > 0)
	{
		c.data[c.size] = memo;
		c.size++;
	}
	return c;
}

BigInt& BigInt::operator*=(int num)
{
	long long memo = 0;
	for (int i = 0; i < size; i++)
	{
		memo = memo + long long(data[i]) * num;
		data[i] = memo % BigInt::base;
		memo = memo / BigInt::base;
	}
	if (memo > 0)
	{
		data[size] = memo;
		size++;
	}
	return *this;
}

BigInt operator*(const BigInt& a, const BigInt&  b)
{
	BigInt c;
	if ((a.data[0] == 0 && a.size == 1) ||
		(b.data[0] == 0 && b.size == 1))
		return c;
	c.size = a.size + b.size;
	for (int i = 0; i < b.size; i++)
	{
		if (b.data[i] == 0)
			continue;
		long long memo = 0;
		for (int j = 0; j < a.size; j++)
		{
			memo = memo + long long(a.data[j]) * long long(b.data[i]) + c.data[i + j];
			long long tmp = memo % BigInt::base;
			c.data[i + j] = tmp;
			memo = memo / BigInt::base;
		}
		if (memo > 0)
			c.data[i + a.size] = memo;
	}
	return c;
}

BigInt operator/(const BigInt& a, int b)
{
	BigInt ans;
	ans.size = a.size;
	long long memo = 0;
	for (int i = ans.size - 1; i >= 0; i--)
	{
		memo = memo * BigInt::base + a.data[i];
		long long tmp = memo / b;
		ans.data[i] = tmp;
		memo = memo % b;
	}
	while (ans.size > 1 && ans.data[ans.size - 1] == 0)
		ans.size--;
	return ans;
}

BigInt& BigInt::operator/=(int b)
{
	long long memo = 0;
	for (int i = size - 1; i >= 0; i--)
	{
		memo = memo * BigInt::base + data[i];
		long long tmp = memo / b;
		data[i] = tmp;
		memo = memo % b;
	}
	while (size > 1 && data[size - 1] == 0)
		size--;
	return *this;
}

int BigInt::operator%(int b)
{
	long long memo = 0;
	for (int i = size - 1; i >= 0; i--)
	{
		memo = memo * BigInt::base + data[i];
		memo = memo % b;
	}
	return memo;
}

std::istream& operator>>(std::istream& input, BigInt& num)
{
	std::string str;
	input >> str;
	num = BigInt(str);
	return input;
}

std::ostream& operator<<(std::ostream& output, BigInt& num)
{
	output << num.getNum();
	return output;
}
