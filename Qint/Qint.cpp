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
	for (int i = 127; i >= 0; i--)
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
	return ans;
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
	for (int i = 0; i < 128 - times; i++)
		ans.setBit(i, ans.getBit(i + times));
	for (int i = 128 - times; i < 128; i++)
		ans.setBit(i, 0);

	return ans;
}

Qint Qint::operator<<(int times)
{
	Qint ans(*this);

	times = std::min(times, 128);
	for (int i = 127; i >= times; i--)
		ans.setBit(i, ans.getBit(i - times));
	for (int i = times - 1; i >= 0; i--)
		ans.setBit(i, 0);

	return ans;
}

Qint Qint::sar(int times)
{
	Qint ans(*this);

	//It's useless when we shift over 128 times 
	times = std::min(times, 128);

	/*
	We need to shift entire data[i] to data[i-1] for times/32 number of times
	After that we'll only have 4 - times/4 number less
	We need to reset all other number to 0
	*/
	for (int i = 0; i < 4 - times / 32; i++)
		ans.data[i] = ans.data[i + times / 32];
	for (int i = 4 - times / 32; i < 4; i++)
		ans.data[i] = 0;
	times %= 32;

	if (times == 0)
		return ans;

	long long memo = 0;
	for (int i = 3; i >= 0; i--)
	{
		memo = memo | ans.data[i];
		ans.data[i] = memo >> times;
		memo = memo << (64 - times) >> (32 - times);
	}

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
	int memo = getBit(ans.data[3],31);
	for (int i = 0; i < 4; i++)
	{
		int tmp = getBit(ans.data[i], 31);
		ans.data[i] = (ans.data[i] << 1) | memo;
		memo = tmp;
	}
	return ans;
}

Qint Qint::ror()
{
	Qint ans(*this);
	int memo = getBit(ans.data[0], 0);
	for (int i = 3; i >= 0; i--)
	{
		int tmp = getBit(ans.data[0], 0);
		ans.data[i] = (ans.data[i] >> 1);
		setBit(ans.data[i], 31, 0);
		ans.data[i] |= (memo << 31);
		memo = tmp;
	}
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

Qint operator*(const Qint& a, const Qint& b)
{
	bool memo = 0;
	Qint outSideRange;
	Qint ans(a);
	for (int i = 0; i < 128; i++)
	{
		int tmp = ans.getBit(0) * 2 + memo;
		if (tmp == 2)
			outSideRange = outSideRange - b;
		else if (tmp == 1)
			outSideRange = outSideRange + b;
		memo = ans.getBit(0);
		ans = (ans >> 1);
		ans.setBit(127, outSideRange.getBit(0));
		outSideRange = outSideRange.sar(1);
	}
	return ans;
}

Qint operator/(const Qint& a, const Qint& b)
{
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
