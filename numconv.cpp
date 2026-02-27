#include "numconv.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <string>

using namespace std;

const string usage = R"(
Usage
    numconv hex|bin|dec --from-hex|--from-bin|--from-dec NUMBER
Commands
    hex     Convert to hexadecimal
    bin     Convert to binary
    dec     Convert to decimal
Options
    --from-hex, -x  Parse argument as hexadecimal
    --from-bin, -b  Parse argument as binary
    --from-dec, -d  Parse argument as decimal
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


string Super::getHex() const {
    return "0x" + hex_num_str;
}

int Super::getDecimal() const {
    return dec_num;
}

Hex::Hex() {}
Hex::~Hex() {}

void Hex::toHexFromDecimal(int num) {
    val = num;
    setHexmap();

    do {
        result = setResult(val);
        setRemainder();
    } while (result > 0);

    for (int i = remainders.size() - 1; i > -1; --i) {
        hex_num_str.append(hex_map[remainders[i]]);
    }
}

void Hex::toDecimalFromHex(string num_str) {
    dec_num = power = 0;

    if (num_str.length() != 0) {
        for (int i = num_str.length() - 1; i > -1; --i) {
            digit = hex_numbers.find(num_str[i]);
            dec_num += (digit * pow(16, power));
            power++;
        }
    } else {
        cout << "no hexadecimal value" << endl;
    }
}

Binary::Binary() {}
Binary::~Binary() {}

void Binary::toBinaryFromDecimal(int num) {
    val = num;
    do {
        val /= 2;
        setRemainder();
    } while (val > 0);
}

void Binary::toDecimalFromBinary(string num_str) {
    dec_num = ct = 0;

    for (int i = num_str.length() - 1; i > -1; --i) {
        temp = num_str[ct];
        digit = stoi(temp, &st);
        dec_num += (digit * pow(2, i));
        ct++;
    }
}

void Binary::toBinaryFromHex(string num_str) {
    setBinmap();

    for (int i = 0; i < num_str.length(); i++) {
        temp = num_str[i];
        bin_num_str.append(bin_map[temp]);
    }
    curr_num_str = bin_num_str;
}

void Binary::toHexFromBinary(string num_str) {
    Hex* hex = new Hex();
    toDecimalFromBinary(num_str);
    hex->toHexFromDecimal(dec_num);
    hex_num_str = hex->getHex();
    delete hex;
}

string Binary::getBinary() const {
    return curr_num_str;
}

NumType commandToNumType(const string& cmd) {
    if (cmd == "hex") return NumType::Hexadecimal;
    if (cmd == "bin") return NumType::Binary;
    if (cmd == "dec") return NumType::Decimal;
    return NumType::Unknown;
}

NumType optionToNumType(const string& opt) {
    if (opt == "--from-hex" || opt == "-x")
        return NumType::Hexadecimal;
    if (opt == "--from-bin" || opt == "-b")
        return NumType::Binary;
    if (opt == "--from-dec" || opt == "-d")
        return NumType::Decimal;
    return NumType::Unknown;
}

bool valid_hex(string value) {
    int ct = 0;
    while (value[ct]) {
        if (hex_numbers.find(value[ct]) == -1)
            return false;
        ct++;
    }
    return true;
}

bool valid_binary(string value) {
    int ct = 0;
    while (value[ct]) {
        if (value[ct] != '0' && value[ct] != '1')
            return false;
        ct++;
    }
    return true;
}

bool valid_decimal(string value) {
    int ct = 0;
    while (value[ct]) {
        if (isalpha(value[ct]))
            return false;
        ct++;
    }
    return true;
}

int main(int argc, char* argv[]) {
    int num, ct = 0;
    string::size_type sz;
    Hex hex;
    Binary bin;

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
                if (!valid_hex(num_str)) {
                    cout << "Not a valid hexadecimal number" << endl;
                    exit(EXIT_FAILURE);
                }

                switch (command) {
                    case NumType::Hexadecimal:
                        cout << "0x" + num_str << endl;
                        break;
                    case NumType::Binary:
                        bin.toBinaryFromHex(num_str);
                        cout << bin.getBinary() << endl;
                        break;
                    case NumType::Decimal:
                        hex.toDecimalFromHex(num_str);
                        cout << hex.getDecimal() << endl;
                        break;
                    default:
                        cout << usage << endl;
                        exit(EXIT_FAILURE);
                }
                break;
            case NumType::Binary:
                if (!valid_binary(num_str)) {
                    cout << "Not a valid binary number" << endl;
                    exit(EXIT_FAILURE);
                }

                switch (command) {
                    case NumType::Hexadecimal:
                        bin.toHexFromBinary(num_str);
                        cout << bin.getHex() << endl;
                        break;
                    case NumType::Binary:
                        cout << num_str << endl;
                        break;
                    case NumType::Decimal:
                        bin.toDecimalFromBinary(num_str);
                        cout << bin.getDecimal() << endl;
                        break;
                    default:
                        cout << usage << endl;
                        exit(EXIT_FAILURE);
                }
                break;
            case NumType::Decimal:
                if (!valid_decimal(num_str)) {
                    cout << "Not a valid decimal number" << endl;
                    exit(EXIT_FAILURE);
                }
                num = stoi(num_str, &sz);

                switch (command) {
                    case NumType::Hexadecimal:
                        hex.toHexFromDecimal(num);
                        cout << hex.getHex() << endl;
                        break;
                    case NumType::Binary:
                        bin.toBinaryFromDecimal(num);
                        cout << bin.getBinary() << endl;
                        break;
                    case NumType::Decimal:
                        cout << num << endl;
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
		hex.toHexFromDecimal(num);
		cout << "hexadecimal -> " << hex.getHex() << endl;

		// DECIMAL TO BINARY
		bin.toBinaryFromDecimal(num);
		cout << "binary -> " << bin.getBinary() << endl;

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

		hex.toDecimalFromHex(input);
		cout << "decimal -> " << hex.getDecimal() << endl;

		// HEXADECIMAL TO BINARY
		bin.toBinaryFromHex(input);
		cout << "binary -> " << bin.getBinary() << endl;

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
		bin.toDecimalFromBinary(input);
		cout << "decimal -> " << bin.getDecimal() << endl;

		// BINARY TO HEXADECIMAL
		bin.toHexFromBinary(input);
		cout << "hexadecimal -> " << bin.getHex() << endl;

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
