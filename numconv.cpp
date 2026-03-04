#include "numconv.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <string>

using namespace std;

const string usage = R"(
Usage
    numconv hex|dec|oct|bin --from-hex|--from-dec|--from-oct|--from-bin NUMBER
Commands
    hex     Convert to hexadecimal
    dec     Convert to decimal
    oct     Convert to octal
    bin     Convert to binary
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


Hex& Hex::implFromDecimal(const int& num) {
    reset();

    val = num;
    do {
        quotient = getQuotient(val);
        setRemainder();
    } while (quotient > 0);

    for (int i = remainders.size() - 1; i > -1; --i) {
        hex_num_str.append(hex_map[remainders[i]]);
    }

    return *this;
}

//Hex& Hex::implFromOctal(const int& num) {
//}

Hex& Hex::implFromBinary(const string& num_str) {
    Decimal* dec = new Decimal();
    int num = dec->fromBinary(num_str).getDecimal();
    delete dec;

    return fromDecimal(num);
}

Decimal& Decimal::implFromHex(const string& num_str) {
    reset();

    if (num_str.length() != 0) {
        for (int i = num_str.length() - 1; i > -1; --i) {
            digit = hex_numbers.find(num_str[i]);
            dec_num += (digit * pow(16, power));
            power++;
        }
    } else {
        cout << "no hexadecimal value" << endl;
    }

    return *this;
}

//Decimal& Decimal::implFromOctal(const int& num) {
//}

Decimal& Decimal::implFromBinary(const string& num_str) {
    reset();

    for (int i = num_str.length() - 1; i > -1; --i) {
        temp = num_str[ct];
        digit = stoi(temp, &sz);
        dec_num += (digit * pow(2, i));
        ct++;
    }

    return *this;
}

//Octal& Octal::implFromHex(const string& num_str) {
//}

Octal& Octal::implFromDecimal(const int& num) {
    reset();

    val = num;
    do {
        quotient = getQuotient(val);
        setRemainder();
    } while (quotient > 0);

    return *this;
}

//Octal& Octal::implFromBinary(const string& num_str) {
//}

Binary& Binary::implFromHex(const string& num_str) {
    reset();

    for (int i = 0; i < num_str.length(); i++) {
        temp = num_str[i];
        bin_num_str.append(bin_map[temp]);
    }

    return *this;
}

Binary& Binary::implFromDecimal(const int& num) {
    reset();

    val = num;
    do {
        quotient = getQuotient(val);
        setRemainder();
    } while (quotient > 0);

    return *this;
}

//Binary& Binary::implFromOctal(const int& num) {
//}

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
    string str_val;
    string::size_type sz;

    Hex hex;
    Decimal dec;
    Octal oct;
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
                    case NumType::Decimal:
                        num = dec.fromHex(num_str).getDecimal();
                        cout << num << endl;
                        break;
                    case NumType::Octal:
                        //TODO: implement
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
            case NumType::Binary:
                if (!valid_binary(num_str)) {
                    cout << "Not a valid binary number" << endl;
                    exit(EXIT_FAILURE);
                }

                switch (command) {
                    case NumType::Hexadecimal:
                        str_val = hex.fromBinary(num_str).getHex();
                        cout << str_val << endl;
                        break;
                    case NumType::Decimal:
                        num = dec.fromBinary(num_str).getDecimal();
                        cout << num << endl;
                        break;
                    case NumType::Octal:
                        //TODO: implement
                        break;
                    case NumType::Binary:
                        cout << num_str << endl;
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
                        str_val = hex.fromDecimal(num).getHex();
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
