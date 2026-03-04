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
    Decimal,
    Octal,
    Binary,
    Unknown,
};

const string hex_numbers = "0123456789ABCDEF";

const array<string, 16> bin_numbers = {
  "0000", "0001", "0010", "0011", "0100",
  "0101", "0110", "0111", "1000", "1001",
  "1010", "1011", "1100", "1101", "1110",
  "1111"
};

template<typename T>
class Conv {
public:
    T& fromHex(const string& num_str) {
        return static_cast<T*>(this)->implFromHex(num_str);
    }

    T& fromDecimal(const int& num) {
        return static_cast<T*>(this)->implFromDecimal(num);
    }

    T& fromOctal(const int& num) {
        return static_cast<T*>(this)->implFromOctal(num);
    }

    T& fromBinary(const string& num_str) {
        return static_cast<T*>(this)->implFromBinary(num_str);
    }

protected:
    Conv() = default;
    ~Conv() = default;

    void setRemainder();
    void reset();

    inline int getQuotient(const double& num) {
        val = num / base;
        return floor(val);
    }

    int dec_num, quotient, remainder, base = 10;
	double val;
    string remainders_str, temp;
	string::size_type sz;
};

class Hex : public Conv<Hex> {
public:
    Hex() {
        setHexmap();
        base = 16;
    }

    Hex& implFromDecimal(const int& num);
    Hex& implFromOctal(const int& num);
    Hex& implFromBinary(const string& num_str);

    inline string getHex() const {
        return "0x" + hex_num_str;
    }

private:
	inline void setRemainder() {
		remainder = (val - quotient) * base;
		remainders.push_back(remainder);
	}

    inline void reset() {
        hex_num_str = "";
        remainders.clear();
    }

	inline void setHexmap() {
		for (int i = 0; i < hex_numbers.length(); i++) {
			hex_map[i] = hex_numbers[i];
		}
	}

    string hex_num_str;
	vector<int> remainders;
	map<int, string> hex_map;
};

class Decimal : public Conv<Decimal> {
public:
    Decimal& implFromHex(const string& num_str);
    Decimal& implFromOctal(const int& num);
    Decimal& implFromBinary(const string& num_str);

    inline int getDecimal() const {
        return dec_num;
    }

private:
    inline void reset() {
        dec_num = ct = power = 0;
    }

    int ct, digit, power;
};

class Octal : public Conv<Octal> {
public:
    Octal() {
        base = 8;
    }

    Octal& implFromHex(const string& num_str);
    Octal& implFromDecimal(const int& num);
    Octal& implFromBinary(const string& num_str);

    inline string getOctal() const {
        return oct_num_str;
    }

private:
	inline void setRemainder() {
		remainder = (val - quotient) * base;
		remainders_str.insert(0, to_string(remainder));
        oct_num_str = remainders_str;
    }

    inline void reset() {
        remainders_str = "";
    }

    string oct_num_str;
};

class Binary : public Conv<Binary> {
public:
    Binary() {
        setBinmap();
        base = 2;
    }

    Binary& implFromHex(const string& num_str);
    Binary& implFromDecimal(const int& num);
    Binary& implFromOctal(const int& num);

    inline string getBinary() const {
        return bin_num_str;
    }

private:
	inline void setRemainder() {
		remainder = quotient % base;
		remainders_str.insert(0, to_string(remainder));
		bin_num_str = remainders_str;
	}

    inline void reset() {
        bin_num_str = remainders_str = "";
    }

	inline void setBinmap() {
		for (int i = 0; i < bin_numbers.size(); i++) {
			temp = hex_numbers[i];
			bin_map[temp] = bin_numbers[i];
		}
    }

    inline string removeLeadingZeros() {
        string trimmed = bin_num_str;
        size_t first_one = trimmed.find_first_of('1');

        if (first_one != string::npos) {
            trimmed.erase(0, first_one);
            return trimmed;
        }
        return "0";
    }

    //int val;
	string bin_num_str;
	map<string, string> bin_map;
};

NumType commandToNumType(const string& cmd);
NumType optionToNumType(const string& opt);

#endif
