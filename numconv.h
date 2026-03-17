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
    Any,
    Unknown,
};

const string hex_numbers = "0123456789abcdef";
const array<string, 16> bin_numbers = {
  "0000", "0001", "0010", "0011", "0100",
  "0101", "0110", "0111", "1000", "1001",
  "1010", "1011", "1100", "1101", "1110",
  "1111"
};

#ifndef NDEBUG
    #define ASSERT_MSG(condition, message) \
        do { \
            if (!(condition)) { \
                cerr << "Assertion failed: (" #condition ") at " \
                     << __FILE__ << ":" << __LINE__ << " - " \
                     << message << endl; \
                abort(); \
            } \
        } while (false)
#else
    #define ASSERT_MSG(condition, message) do { } while (false)
#endif

template<typename Derived>
class Conv {
public:
    Derived& fromHex(const string& num_str) {
        return static_cast<Derived*>(this)->implFromHex(num_str);
    }

    Derived& fromDecimal(const string& num_str) {
        return static_cast<Derived*>(this)->implFromDecimal(num_str);
    }

    Derived& fromOctal(const string& num_str) {
        return static_cast<Derived*>(this)->implFromOctal(num_str);
    }

    Derived& fromBinary(const string& num_str) {
        return static_cast<Derived*>(this)->implFromBinary(num_str);
    }

    inline string get() const {
        return result;
    }

    NumType type;

protected:
    Conv() = default;
    ~Conv() = default;

    void setRemainder();
    void reset();

    inline void setQuotient() {
        dbl_val /= base;
        quotient = floor(dbl_val);
    }

    int quotient, remainder, base;
	double dbl_val;
    string temp, result;
};

class Hex : public Conv<Hex> {
public:
    Hex() {
        setHexmap();
        base = 16;
        type = NumType::Hexadecimal;
    }

    Hex& implFromDecimal(const string& num_str);
    Hex& implFromOctal(const string& num_str);
    Hex& implFromBinary(const string& num_str);

    inline Hex& implFromHex(const string& num_str) {
        result = num_str;
        return *this;
    }

    inline Hex& formatHex() {
        if (result.length() != 0)
            result = "0x" + result;

        return *this;
    }

private:
	inline void setRemainder() {
		remainder = (dbl_val - quotient) * base;
		remainders.push_back(remainder);
	}

    inline void reset() {
        result = "";
        remainders.clear();
    }

	inline void setHexmap() {
		for (int i = 0; i < hex_numbers.length(); i++) {
			hex_map[i] = hex_numbers[i];
		}
	}

	vector<int> remainders;
	map<int, string> hex_map;
};

class Decimal : public Conv<Decimal> {
public:
    Decimal() {
        base = 10;
        type = NumType::Decimal;
    }

    Decimal& implFromHex(const string& num_str);
    Decimal& implFromOctal(const string& num_str);
    Decimal& implFromBinary(const string& num_str);

    inline Decimal& implFromDecimal(const string& num_str) {
        result = num_str;
        return *this;
    }

private:
    inline void reset() {
        result = "";
        dec_num = ct = power = 0;
    }

    int ct, dec_num, digit, power;
};

class Octal : public Conv<Octal> {
public:
    Octal() {
        base = 8;
        type = NumType::Octal;
    }

    Octal& implFromHex(const string& num_str);
    Octal& implFromDecimal(const string& num_str);
    Octal& implFromBinary(const string& num_str);

    inline Octal& implFromOctal(const string& num_str) {
        result = num_str;
        return *this;
    }

private:
	inline void setRemainder() {
		remainder = (dbl_val - quotient) * base;
		result.insert(0, to_string(remainder));
    }

    inline void reset() {
        result = "";
    }
};

class Binary : public Conv<Binary> {
public:
    Binary() {
        setBinmap();
        base = 2;
        type = NumType::Binary;
    }

    Binary& implFromHex(const string& num_str);
    Binary& implFromDecimal(const string& num_str);
    Binary& implFromOctal(const string& num_str);

    inline Binary& implFromBinary(const string& num_str) {
        result = num_str;
        return *this;
    }

private:
	inline void setRemainder() {
		remainder = int_val % base;
        result.insert(0, to_string(remainder));
	}

    inline void reset() {
        result = "";
        int_val = 0;
    }

	inline void setBinmap() {
		for (int i = 0; i < bin_numbers.size(); i++) {
			temp = hex_numbers[i];
			bin_map[temp] = bin_numbers[i];
		}
    }

    int int_val;
	map<string, string> bin_map;
};

string padLeadingZeros(const string& num_str, size_t grp_len);
string lowerCase(const string& num_str);
void trimLeadingZeros(string& num_str);
string trimPrefix(const string& num_str, const string& prefix);

NumType commandToNumType(const string& cmd);
NumType optionToNumType(const string& opt);

#endif
