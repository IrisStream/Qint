#include "qint.h"

bool Qint::getBit(int pos)
{
	return (data[pos / 32] >> (pos % 32)) & 1;
}

void Qint::setBit(int pos, bool value)
{ 
	if (getBit(pos) == value)
		return;
	data[pos / 32] ^= (1 << (pos % 32));
}

void Qint::decToBin(string& str)
{
	bool isNegative = false;
	if (str[0] == '-')
	{
		str.erase(str.begin());
		isNegative = true;
	}
	BigInt num(str);
	string bits;
	while (num.isZero() == false)
	{
		bits = bits + char('0' + num % 2);
		num /= 2;
	}

	for (int i = 0; i < bits.size(); i++)
		setBit(i, bits[i] == '1');
	if (isNegative)
		this->toNegative();
}

void Qint::hexToBin(string& str)
{
	int counter = 0;
	for (int i = str.size() - 1; i >= 0; i--)
	{
		int value = 0;
		if (str[i] >= '0' && str[i] <= '9')
			value = str[i] - '0';
		else
			value = str[i] - 'A' + 10;
		for (int j = 0; j < 4; j++)
		{
			setBit(counter, (value >> j) & 1);
			counter++;
		}
	}
}

void Qint::toNegative()
{
	//Bù 1
	for (int i = 0; i < 4; i++)
		data[i] ^= -1;

	//Cộng 1
	for (int i = 0; i < 4; i++)
	{
		if (data[i] == -1)
		{
			data[i] = 0;
		}
		else
		{
			data[i] = data[i] + 1;
			break;
		}
	}
}

//Mặc định là số 0
Qint::Qint()
{
	memset(data, 0, sizeof(data));
}

Qint::Qint(int base, string num) : Qint()
{
	if (base == 10)
		decToBin(num);
	else
	if (base == 16)
		hexToBin(num);
	else
	{
		for (int i = num.size() - 1; i >= 0; i--)
			setBit(num.size() - i - 1, num[i] == '1');
	}
}

Qint::Qint(const Qint& other) : Qint()
{
	for (int i = 0; i < 4; i++)
	{
		data[i] = other.data[i];
	}
}

std::string Qint::getBin()
{
	string ans;
	int i = 127;
	while (getBit(i) == false)
		i--;
	for (; i >= 0; i--)
		ans = ans + char('0' + getBit(i));
	return ans;
}

std::string Qint::getDec()
{
	BigInt ans;
	BigInt Pow(1);
	for (int i = 0; i < 127; i++)
	{
		if (getBit(i))
			ans = ans + Pow;
		Pow = Pow * 2;
	}

	//Xét bit dấu
	if (getBit(127))
	{
		ans = Pow - ans;
		return string('-' + ans.getNum());
	}
	return ans.getNum();

}

std::string Qint::getHex()
{
	string ans;
	int digit = 0;
	for (int i = 0; i < 128; i++)
	{
		if(getBit(i))
			digit = digit | (1 << (i % 4));
		if (i % 4 == 3)
		{
			if (digit < 10)
				ans = char('0' + digit) + ans;
			else
				ans = char('A' + digit - 10) + ans;
			digit = 0;
		}
	}
	while (ans[0] == '0')
		ans.erase(ans.begin());
	return ans;
}

std::string Qint::getNum(int base)
{
	if (base == 2)
		return this->getBin();
	else if (base == 10)
		return this->getDec();
	else
		return this->getHex();
}

Qint& Qint::operator=(const Qint& other)
{
	if (this != &other)
	{
		for (int i = 0; i < 4; i++)
			data[i] = other.data[i];
	}
	return *this;
}

Qint Qint::operator>>(int times)
{
	Qint ans(*this);
	times = std::min(times, 128);
	for (int i = times; i < 128; i++)
		ans.setBit(i - times, ans.getBit(i));
	for (int i = 127; i > 127 - times; i--)
		ans.setBit(i, 0);
	return ans;
}

Qint Qint::operator<<(int times)
{
	Qint ans(*this);
	times = std::min(times, 128);
	for (int i = 127-times; i >=0; i--)
		ans.setBit(i + times, ans.getBit(i));
	for (int i = 0; i < times; i++)
		ans.setBit(i, 0);
	return ans;
}

Qint Qint::sar(int times)
{
	Qint ans(*this); 
	bool sign = this->getBit(127);
	times = std::min(times, 128);
	for (int i = times; i < 128; i++)
		ans.setBit(i - times, ans.getBit(i));
	for (int i = 127; i > 127 - times; i--)
		ans.setBit(i, sign);
	return ans;
}

Qint Qint::operator~()
{
	Qint ans(*this);
	for (int i = 0; i < 4; i++)
		ans.data[i] = ~ans.data[i];
	return ans;
}

Qint Qint::rol()
{
	Qint ans(*this);
	bool memo = ans.getBit(127);
	ans = ans << 1;
	ans.setBit(0, memo);
	return ans;
}

Qint Qint::ror()
{
	Qint ans(*this);
	bool memo = ans.getBit(0);
	ans = ans >> 1;
	ans.setBit(127, memo);
	return ans;
}

bool Qint::isNegative()
{
	return getBit(127);
}

int Qint::add(int x, int y, int& memo)
{
	int xMSB = (x >> 31) & 1;
	if (xMSB == 1)
		x ^= (1 << 31);

	int yMSB = (y >> 31) & 1;
	if (yMSB == 1)
		y ^= (1 << 31);

	int ans = x + y + memo;
	int tmp = Qint::getBit(ans, 31) + xMSB + yMSB;
	Qint::setBit(ans, 31, Qint::getBit(tmp, 0));
	memo = tmp >> 1;

	return ans;
}

bool Qint::getBit(int x, int pos)
{
	return (x >> pos) & 1;
}

void Qint::setBit(int& x, int pos, int value)
{
	if (getBit(x, pos) == value)
		return;
	x ^= (1 << pos);
}

Qint operator+(const Qint& a, const Qint& b)
{
	Qint c;
	int memo = false;
	for (int i = 0; i < 4; i++)
	{
		c.data[i] = Qint::add(a.data[i], b.data[i], memo);
	}
	return c;
}

Qint operator-(const Qint& a, const Qint& b)
{
	Qint c(b);
	c.toNegative();
	c = c + a;
	return c;
}

Qint operator*(Qint& a, Qint& b)
{
	Qint ans;
	for (int i = 0; i < 128; i++)
	{
		if (b.getBit(i) == true)
			ans = ans + (a << i);
	}
	return ans;
}

Qint operator/(Qint& a, Qint& b)
{
	int sign = 1;
	if (a.isNegative())
	{
		a.toNegative();
		sign *= -1;
	}
	if (b.isNegative())
	{
		b.toNegative();
		sign *= -1;
	}
	Qint rest;
	Qint ans(a);
	if (ans.isNegative())
		rest = ~rest;
	for (int i = 0; i < 128; i++)
	{
		rest = rest << 1;
		rest.setBit(0, ans.getBit(127));
		ans = ans << 1;
		rest = rest - b;
		if (rest.isNegative())
		{
			ans.setBit(0, 0);
			rest = rest + b;
		}
		else
		{
			ans.setBit(0, 1);
		}
	}
	if (sign == -1)
		ans.toNegative();
	return ans;
}

Qint operator&(const Qint& a, const Qint& b)
{
	Qint c;
	for (int i = 0; i < 4; i++)
	{
		c.data[i] = a.data[i] & b.data[i];
	}
	return c;
}

Qint operator|(const Qint& a, const Qint& b)
{
	Qint c;
	for (int i = 0; i < 4; i++)
	{
		c.data[i] = a.data[i] | b.data[i];
	}
	return c;
}

Qint operator^(const Qint& a, const Qint& b)
{
	Qint c;
	for (int i = 0; i < 4; i++)
	{
		c.data[i] = a.data[i] ^ b.data[i];
	}
	return c;
}
