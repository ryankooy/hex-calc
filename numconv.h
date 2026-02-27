#ifndef NUMCONV_H
#define NUMCONV_H

#include <array>
#include <iostream>
#include <map>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

enum class NumType {
    Hexadecimal,
    Binary,
    Decimal,
    Unknown,
};

const string hex_numbers = "0123456789ABCDEF";

const array<string, 16> bin_numbers = {
  "0000", "0001", "0010", "0011", "0100",
  "0101", "0110", "0111", "1000", "1001",
  "1010", "1011", "1100", "1101", "1110",
  "1111"
};

class Super {
public:
    string getHex() const;
    int getDecimal() const;

protected:
    int remainder, dec_num, digit;
    string hex_num_str, bin_num_str;
};

class Hex : public Super {
public:
	Hex();
	virtual ~Hex();
	void toHexFromDecimal(int num);
	void toDecimalFromHex(string num_str);

private:
	inline void setHexmap() {
		for (int i = 0; i < hex_numbers.length(); i++) {
			hex_map[i] = hex_numbers[i];
		}
	}

	inline int setResult(double num) {
		val = num / 16;
		return floor(val);
	}

	inline void setRemainder() {
		remainder = (val - result) * 16;
		remainders.push_back(remainder);
	}

	int result, power;
	double val;
	vector<int> remainders;
	map<int, string> hex_map;
};

class Binary : public Super {
public:
	Binary();
	virtual ~Binary();
	void toBinaryFromDecimal(int num);
	void toDecimalFromBinary(string num_str);
	void toBinaryFromHex(string num_str);
	void toHexFromBinary(string num_str);
	string getBinary() const;

private:
	inline void setBinmap() {
		for (int i = 0; i < bin_numbers.size(); i++) {
			temp = hex_numbers[i];
			bin_map[temp] = bin_numbers[i];
		}
    }

	inline void setRemainder() {
		remainder = val % 2;
		remainders.insert(0, to_string(remainder));
		curr_num_str = remainders;
	}

	int val, ct;
	string remainders, temp, curr_num_str;
	string::size_type st;
	map<string, string> bin_map, bin_map2;
};

NumType stringToCommand(const string& cmd);
NumType optionToNumType(const string& opt);

#endif
