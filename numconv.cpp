#include "numconv.h"
#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

//temp:
#include <cassert>

using namespace std;

const string usage = R"(
Usage
    numconv hex|dec|oct|bin --from-hex|--from-dec|--from-oct|--from-bin NUMBER
Commands
    hex     Convert to hexadecimal
    dec     Convert to decimal
    oct     Convert to octal
    bin     Convert to binary
    all     Convert to each of the above
Options
    --from-hex, -x  Parse argument as hexadecimal
    --from-dec, -d  Parse argument as decimal
    --from-oct, -o  Parse argument as octal
    --from-bin, -b  Parse argument as binary
Arguments
    NUMBER  An integer to be parsed according to the
            provided option
)";


// // repeat until result == 0
// 1. val1 = dec_num / 16
// 2. result1 = floor(val)
// 3. remainder1 = (val - result1) * 16 // IN DECIMAL
// 4. val2 = result1 / 16
// 5. result2 = floor(val2)
// 6. remainder2 = (val2 - result2) * 16
// 7. val3 = result2 / 16
// 8. result3 = floor(val3)
// 9. remainder3 = (val3 - result3) * 16e
// 10. hexnum = int(str(remainder3) + str(remainder2) + str(remainder1))


string padLeadingZeros(const string& num_str, size_t bgl) {
    size_t len = num_str.length();
    int width = len + ((bgl - (len % bgl)) % bgl);

    stringstream ss;
    ss << setfill('0') << setw(width) << num_str;

    return ss.str();
}

void trimLeadingZeros(string& num_str) {
    size_t first_non_zero = num_str.find_first_not_of('0');

    if (first_non_zero != string::npos)
        num_str.erase(0, first_non_zero);
    else
        num_str = "0";
}

string trimPrefix(const string& num_str, const string& prefix) {
    if (num_str.find(prefix) == 0)
        return num_str.substr(prefix.length());
    return num_str;
}

Hex& Hex::implFromDecimal(const int& num) {
    this->reset();

    val = num;
    do {
        quotient = this->getQuotient(val);
        this->setRemainder();
    } while (quotient > 0);

    for (int i = remainders.size() - 1; i > -1; --i) {
        hex_num_str.append(hex_map[remainders[i]]);
    }

    return *this;
}

Hex& Hex::implFromOctal(const string& num_str) {
    this->reset();

    string oct_val = trimPrefix(num_str, "0");
    string bin_str;
    int ct = 0;

    Binary* bin = new Binary();

    while (oct_val[ct]) {
        bin_str += bin->fromDecimal(oct_val[ct] - '0').getBinary();
        ct++;
    }

    delete bin;

    size_t bgl = 4;  // Bit group length
    string padded_num_str = padLeadingZeros(bin_str, bgl);
    string bin_group, hex_digit, temp_str;

    for (int i = padded_num_str.length() - bgl; i > -1; i -= bgl) {
        bin_group = padded_num_str.substr(i, bgl);
        hex_digit = this->fromBinary(bin_group).getHex();
        temp_str.insert(0, hex_digit);
    }

    hex_num_str = temp_str;

    return *this;
}

Hex& Hex::implFromBinary(const string& num_str) {
    Decimal* dec = new Decimal();
    int num = dec->fromBinary(num_str).getDecimal();
    delete dec;

    return this->fromDecimal(num);
}

Decimal& Decimal::implFromHex(const string& num_str) {
    this->reset();

    string hex_val = trimPrefix(num_str, "0x");
    size_t len = hex_val.length();

    if (len != 0) {
        for (int i = len - 1; i > -1; --i) {
            digit = hex_numbers.find(hex_val[i]);
            dec_num += (digit * pow(16, power));
            power++;
        }
    }

    return *this;
}

Decimal& Decimal::implFromOctal(const string& num_str) {
    this->reset();

    string oct_val = trimPrefix(num_str, "0");

    for (int i = oct_val.length() - 1; i > -1; --i) {
        temp = oct_val[i];
        digit = stoi(temp, &sz);
        dec_num += (digit * pow(8, power));
        power++;
    }

    return *this;
}

Decimal& Decimal::implFromBinary(const string& num_str) {
    this->reset();

    for (int i = num_str.length() - 1; i > -1; --i) {
        temp = num_str[ct];
        digit = stoi(temp, &sz);
        dec_num += (digit * pow(2, i));
        ct++;
    }

    return *this;
}

Octal& Octal::implFromHex(const string& num_str) {
    this->reset();

    size_t bgl = 4;  // Bit group length
    string hex_val = trimPrefix(num_str, "0x");
    string bin_str, bin_digit;
    int oct_digit;

    Binary* bin = new Binary();
    Decimal* dec = new Decimal();
    string hex_digit;

    for (int i = hex_val.length() - 1; i > -1; --i) {
        hex_digit = hex_val[i];
        bin_digit = bin->fromHex(hex_digit).getBinary();
        bin_str.insert(0, padLeadingZeros(bin_digit, bgl));
    }

    bgl = 3;
    string bin_group, padded_num_str = padLeadingZeros(bin_str, bgl);

    for (int i = padded_num_str.length() - bgl; i > -1; i -= bgl) {
        bin_group = padded_num_str.substr(i, bgl);
        oct_digit = dec->fromBinary(bin_group).getDecimal();
        oct_num_str.insert(0, to_string(oct_digit));
    }

    trimLeadingZeros(oct_num_str);

    delete bin;
    delete dec;

    return *this;
}

Octal& Octal::implFromDecimal(const int& num) {
    this->reset();

    val = num;
    do {
        quotient = this->getQuotient(val);
        this->setRemainder();
    } while (quotient > 0);

    return *this;
}

Octal& Octal::implFromBinary(const string& num_str) {
    this->reset();

    size_t bgl = 3;  // Bit group length
    string bin_group, padded_num_str = padLeadingZeros(num_str, bgl);
    int digit;

    Decimal* dec = new Decimal();

    for (int i = padded_num_str.length() - bgl; i > -1; i -= bgl) {
        bin_group = padded_num_str.substr(i, bgl);
        digit = dec->fromBinary(bin_group).getDecimal();
        oct_num_str.insert(0, to_string(digit));
    }

    delete dec;

    return *this;
}

Binary& Binary::implFromHex(const string& num_str) {
    this->reset();

    string hex_val = trimPrefix(num_str, "0x");

    for (int i = 0; i < hex_val.length(); i++) {
        temp = hex_val[i];
        bin_num_str.append(bin_map[temp]);
    }

    trimLeadingZeros(bin_num_str);

    return *this;
}

Binary& Binary::implFromDecimal(const int& num) {
    this->reset();

    int_val = num;
    do {
        this->setRemainder();
        int_val /= base;
    } while (int_val > 0);

    trimLeadingZeros(bin_num_str);

    return *this;
}

Binary& Binary::implFromOctal(const string& num_str) {
    this->reset();

    string temp_str, bin_group, oct_val = trimPrefix(num_str, "0");
    size_t bgl = 3;  // Bit group length

    for (int i = 0; i < oct_val.length(); i++) {
        bin_group = this->fromDecimal(oct_val[i] - '0').getBinary();
        temp_str.append(padLeadingZeros(bin_group, bgl));
    }

    bin_num_str = temp_str;
    trimLeadingZeros(bin_num_str);

    return *this;
}

NumType commandToNumType(const string& cmd) {
    if (cmd == "hex") return NumType::Hexadecimal;
    if (cmd == "dec") return NumType::Decimal;
    if (cmd == "oct") return NumType::Octal;
    if (cmd == "bin") return NumType::Binary;
    return NumType::Unknown;
}

NumType optionToNumType(const string& opt) {
    if (opt == "--from-hex" || opt == "-x")
        return NumType::Hexadecimal;
    if (opt == "--from-dec" || opt == "-d")
        return NumType::Decimal;
    if (opt == "--from-oct" || opt == "-o")
        return NumType::Octal;
    if (opt == "--from-bin" || opt == "-b")
        return NumType::Binary;
    return NumType::Unknown;
}

bool validHex(const string& num_str) {
    int i = 0;
    string value = trimPrefix(num_str, "0x");

    while (value[i]) {
        if (hex_numbers.find(value[i]) == -1)
            return false;
        i++;
    }
    return true;
}

bool validDecimal(const string& num_str) {
    int i = 0;

    while (num_str[i]) {
        if (isalpha(num_str[i])) return false;
        i++;
    }
    return true;
}

bool validOctal(const string& num_str) {
    int i = 0;
    string value = trimPrefix(num_str, "0");

    while (value[i]) {
        if (isalpha(value[i]) || value[i] == '8' || value[i] == '9')
            return false;
        i++;
    }
    return true;
}

bool validBinary(const string& num_str) {
    int i = 0;

    while (num_str[i]) {
        if (num_str[i] != '0' && num_str[i] != '1')
            return false;
        i++;
    }
    return true;
}

void analyze(string expected, string actual) {
    ASSERT_MSG(actual == expected, "Expected " + expected + ", got " + actual);
}

void test_conversions(Hex& hex, Decimal& dec, Octal& oct, Binary& bin) {
    analyze("0x10", hex.fromDecimal(16).formatHex().getHex());
    analyze("0x10", hex.fromBinary("10000").formatHex().getHex());

    analyze("16", to_string(dec.fromHex("10").getDecimal()));
    analyze("16", to_string(dec.fromBinary("10000").getDecimal()));

    analyze("20", oct.fromDecimal(16).getOctal());
    analyze("20", oct.fromBinary("10000").getOctal());

    analyze("10000", bin.fromHex("10").getBinary());
    //analyze("10000", bin.fromDecimal(16).getBinary());
}

int main(int argc, char* argv[]) {
    int num, ct = 0;
    string str_val;
    size_t sz;

    Hex hex;
    Decimal dec;
    Octal oct;
    Binary bin;

    //temp:
    test_conversions(hex, dec, oct, bin);

    if (argc > 1) {
        if (argv[1] == "--help" || argv[1] == "-h") {
            cout << usage << endl;
            return 0;
        }

        if (argc < 4) {
            cout << usage << endl;
            exit(EXIT_FAILURE);
        }

        NumType command = commandToNumType(argv[1]);
        NumType option = optionToNumType(argv[2]);
        string num_str = argv[3];

        switch (option) {
            case NumType::Hexadecimal:
                if (!validHex(num_str)) {
                    cout << "Not a valid hexadecimal number" << endl;
                    exit(EXIT_FAILURE);
                }

                switch (command) {
                    case NumType::Hexadecimal:
                        cout << num_str << endl;
                        break;
                    case NumType::Decimal:
                        num = dec.fromHex(num_str).getDecimal();
                        cout << num << endl;
                        break;
                    case NumType::Octal:
                        str_val = oct.fromHex(num_str).getOctal();
                        cout << str_val << endl;
                        break;
                    case NumType::Binary:
                        str_val = bin.fromHex(num_str).getBinary();
                        cout << str_val << endl;
                        break;
                    default:
                        cout << usage << endl;
                        exit(EXIT_FAILURE);
                }
                break;
            case NumType::Decimal:
                if (!validDecimal(num_str)) {
                    cout << "Not a valid decimal number" << endl;
                    exit(EXIT_FAILURE);
                }
                num = stoi(num_str, &sz);

                switch (command) {
                    case NumType::Hexadecimal:
                        str_val = hex.fromDecimal(num)
                            .formatHex()
                            .getHex();
                        cout << str_val << endl;
                        break;
                    case NumType::Decimal:
                        cout << num << endl;
                        break;
                    case NumType::Octal:
                        str_val = oct.fromDecimal(num).getOctal();
                        cout << str_val << endl;
                        break;
                    case NumType::Binary:
                        str_val = bin.fromDecimal(num).getBinary();
                        cout << str_val << endl;
                        break;
                    default:
                        cout << usage << endl;
                        exit(EXIT_FAILURE);
                }
                break;
            case NumType::Octal:
                if (!validOctal(num_str)) {
                    cout << "Not a valid octal number" << endl;
                    exit(EXIT_FAILURE);
                }

                switch (command) {
                    case NumType::Hexadecimal:
                        str_val = hex.fromOctal(num_str)
                            .formatHex()
                            .getHex();
                        cout << str_val << endl;
                        break;
                    case NumType::Decimal:
                        num = dec.fromOctal(num_str).getDecimal();
                        cout << num << endl;
                        break;
                    case NumType::Octal:
                        cout << num_str << endl;
                        break;
                    case NumType::Binary:
                        str_val = bin.fromOctal(num_str).getBinary();
                        cout << str_val << endl;
                        break;
                    default:
                        cout << usage << endl;
                        exit(EXIT_FAILURE);
                }
                break;
            case NumType::Binary:
                if (!validBinary(num_str)) {
                    cout << "Not a valid binary number" << endl;
                    exit(EXIT_FAILURE);
                }

                switch (command) {
                    case NumType::Hexadecimal:
                        str_val = hex.fromBinary(num_str)
                            .formatHex()
                            .getHex();
                        cout << str_val << endl;
                        break;
                    case NumType::Decimal:
                        num = dec.fromBinary(num_str).getDecimal();
                        cout << num << endl;
                        break;
                    case NumType::Octal:
                        str_val = oct.fromBinary(num_str).getOctal();
                        cout << str_val << endl;
                        break;
                    case NumType::Binary:
                        cout << num_str << endl;
                        break;
                    default:
                        cout << usage << endl;
                        exit(EXIT_FAILURE);
                }
                break;
            default:
                cout << usage << endl;
                exit(EXIT_FAILURE);
        }

        return 0;
    }

    string input;

	while (true) {
		cout << "Enter a decimal number: ";
		cin >> input;

		while (input[ct]) {
			if (isalpha(input[ct])) {
				cout << "Not a valid decimal number" << endl;
				exit(EXIT_FAILURE);
			}
			ct++;
		}

		num = stoi(input, &sz);

		// DECIMAL TO HEXADECIMAL
		str_val = hex.fromDecimal(num).getHex();
		cout << "hexadecimal -> " << str_val << endl;

		// DECIMAL TO BINARY
		str_val = bin.fromDecimal(num).getBinary();
		cout << "binary -> " << str_val << endl;

		// HEXADECIMAL TO DECIMAL
		cout << "Enter a hexadecimal number: ";
		cin >> input;
		ct = 0;

		while (input[ct]) {
			if (hex_numbers.find(input[ct]) == -1) {
				cout << "Not a valid hexadecimal number" << endl;
				exit(EXIT_FAILURE);
			}
			ct++;
		}

		num = dec.fromHex(input).getDecimal();
		cout << "decimal -> " << num << endl;

		// HEXADECIMAL TO BINARY
		str_val = bin.fromHex(input).getBinary();
		cout << "binary -> " << str_val << endl;

		// BINARY TO DECIMAL
		cout << "Enter a binary number: ";
		cin >> input;
		ct = 0;

		while (input[ct]) {
			if (input[ct] != '0' && input[ct] != '1') {
				cout << "Not a valid binary number" << endl;
				exit(EXIT_FAILURE);
			}
			ct++;
		}

		num = dec.fromBinary(input).getDecimal();
		cout << "decimal -> " << num << endl;

		// BINARY TO HEXADECIMAL
		str_val = hex.fromBinary(input).getHex();
		cout << "hexadecimal -> " << str_val << endl;

		// CONTINUE?
		cout << "Again? (y/n): ";
		cin >> input;

		while (true) {
			if (input == "y" || input == "Y")
				break;
			else if (input == "n" || input == "N") {
				cout << "Bye.\n";
				exit(EXIT_SUCCESS);
			} else {
				cout << "Again? (y/n): ";
				cin >> input;
			}
		}
	}

	return 0;
}
