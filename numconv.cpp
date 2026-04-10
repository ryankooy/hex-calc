#include "numconv.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

const string usage = R"(
Usage
    numconv hex|dec|oct|bin|all --from-hex|--from-dec|--from-oct|--from-bin NUMBER
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


string padLeadingZeros(const string& num_str, size_t grp_len) {
    size_t num_len = num_str.length();
    size_t zero_count = (grp_len - (num_len % grp_len)) % grp_len;
    size_t width = num_len + zero_count;

    stringstream ss;
    ss << setfill('0') << setw(width) << num_str;

    return ss.str();
}

string lowerCase(const string& num_str) {
    string str = num_str;
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

void trimLeadingZeros(string& num_str) {
    size_t first_non_zero = num_str.find_first_not_of('0');

    if (first_non_zero != string::npos)
        num_str.erase(0, first_non_zero);
    else
        num_str = "0";
}

string trimPrefix(const string& num_str, const string& prefix) {
    size_t num_len = num_str.length();

    if (!num_len) return "0";

    size_t pr_len = prefix.length();

    if (num_str.find(prefix) == 0 && num_len > pr_len)
        return num_str.substr(pr_len);

    return num_str;
}

Hex& Hex::implFromDecimal(const string& num_str) {
    reset();

    try {
        dbl_val = stoi(num_str);

        do {
            setQuotient();
            setRemainder();
        } while (quotient > 0);

        for (int i = remainders.size() - 1; i > -1; --i) {
            result.append(hex_map[remainders[i]]);
        }
    } catch (const invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return *this;
}

Hex& Hex::implFromOctal(const string& num_str) {
    reset();

    string oct_val = trimPrefix(num_str, "0");
    string bin_grp_str, bin_grp, bin_val, oct_digit, hex_num_str;
    size_t ct = 0, grp_len = 3;  // Bit group length

    Binary* bin = new Binary();

    while (oct_val[ct]) {
        oct_digit += oct_val[ct];
        bin_val = bin->fromDecimal(oct_digit).get();
        bin_grp_str += padLeadingZeros(bin_val, grp_len);
        oct_digit = "";
        ct++;
    }

    grp_len = 4;
    bin_grp_str = padLeadingZeros(bin_grp_str, grp_len);

    for (int i = bin_grp_str.length() - grp_len; i > -1; i -= grp_len) {
        bin_grp = bin_grp_str.substr(i, grp_len);
        hex_num_str.insert(0, fromBinary(bin_grp).get());
    }

    result = hex_num_str;
    delete bin;

    return *this;
}

Hex& Hex::implFromBinary(const string& num_str) {
    Decimal* dec = new Decimal();
    string dec_str = dec->fromBinary(num_str).get();
    delete dec;

    return fromDecimal(dec_str);
}

Decimal& Decimal::implFromHex(const string& num_str) {
    reset();

    string hex_val = lowerCase(trimPrefix(num_str, "0x"));

    for (int i = hex_val.length() - 1; i > -1; --i) {
        digit = hex_numbers.find(hex_val[i]);
        dec_num += (digit * pow(16, power));
        power++;
    }

    result = to_string(dec_num);

    return *this;
}

Decimal& Decimal::implFromOctal(const string& num_str) {
    reset();

    string oct_val = trimPrefix(num_str, "0");

    try {
        for (int i = oct_val.length() - 1; i > -1; --i) {
            temp = oct_val[i];
            digit = stoi(temp);
            dec_num += (digit * pow(8, power));
            power++;
        }

        result = to_string(dec_num);
    } catch (const invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return *this;
}

Decimal& Decimal::implFromBinary(const string& num_str) {
    reset();

    try {
        for (int i = num_str.length() - 1; i > -1; --i) {
            temp = num_str[ct];
            digit = stoi(temp);
            dec_num += (digit * pow(2, i));
            ct++;
        }

        result = to_string(dec_num);
    } catch (const invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return *this;
}

Octal& Octal::implFromHex(const string& num_str) {
    reset();

    string hex_val = lowerCase(trimPrefix(num_str, "0x"));
    string bin_str, bin_group, bin_digit, oct_digit, hex_digit;
    size_t grp_len = 4;  // Bit group length

    Binary* bin = new Binary();
    Decimal* dec = new Decimal();

    for (int i = hex_val.length() - 1; i > -1; --i) {
        hex_digit = hex_val[i];
        bin_digit = bin->fromHex(hex_digit).get();
        bin_str.insert(0, padLeadingZeros(bin_digit, grp_len));
    }

    grp_len = 3;
    string padded_num_str = padLeadingZeros(bin_str, grp_len);

    for (int i = padded_num_str.length() - grp_len; i > -1; i -= grp_len) {
        bin_group = padded_num_str.substr(i, grp_len);
        oct_digit = dec->fromBinary(bin_group).get();
        result.insert(0, oct_digit);
    }

    trimLeadingZeros(result);

    delete bin;
    delete dec;

    return *this;
}

Octal& Octal::implFromDecimal(const string& num_str) {
    reset();

    try {
        dbl_val = stoi(num_str);
        do {
            setQuotient();
            setRemainder();
        } while (quotient > 0);
    } catch (const invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return *this;
}

Octal& Octal::implFromBinary(const string& num_str) {
    reset();

    size_t grp_len = 3;  // Bit group length
    string padded_num_str = padLeadingZeros(num_str, grp_len);
    string bin_group, oct_digit;

    Decimal* dec = new Decimal();

    for (int i = padded_num_str.length() - grp_len; i > -1; i -= grp_len) {
        bin_group = padded_num_str.substr(i, grp_len);
        oct_digit = dec->fromBinary(bin_group).get();
        result.insert(0, oct_digit);
    }

    delete dec;

    return *this;
}

Binary& Binary::implFromHex(const string& num_str) {
    reset();

    string hex_val = lowerCase(trimPrefix(num_str, "0x"));

    for (int i = 0; i < hex_val.length(); i++) {
        temp = hex_val[i];
        result.append(bin_map[temp]);
    }

    trimLeadingZeros(result);

    return *this;
}

Binary& Binary::implFromDecimal(const string& num_str) {
    reset();

    try {
        int_val = stoi(num_str);

        do {
            setRemainder();
            int_val /= base;
        } while (int_val > 0);

        trimLeadingZeros(result);
    } catch (const invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return *this;
}

Binary& Binary::implFromOctal(const string& num_str) {
    reset();

    string temp_str, bin_group, oct_val = trimPrefix(num_str, "0");
    string bin_num_str, bin_grp, oct_digit;
    size_t grp_len = 3;  // Bit group length

    for (int i = 0; i < oct_val.length(); i++) {
        oct_digit += oct_val[i];
        bin_grp = fromDecimal(oct_digit).get();
        bin_num_str.append(padLeadingZeros(bin_grp, grp_len));
        oct_digit = "";
    }

    result = bin_num_str;
    trimLeadingZeros(result);

    return *this;
}

NumType commandToNumType(const string& cmd) {
    if (cmd == "hex") return NumType::Hexadecimal;
    if (cmd == "dec") return NumType::Decimal;
    if (cmd == "oct") return NumType::Octal;
    if (cmd == "bin") return NumType::Binary;
    if (cmd == "all") return NumType::Any;
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
    string value = lowerCase(trimPrefix(num_str, "0x"));

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
    analyze("0x10", hex.fromDecimal("16").formatHex().get());
    analyze("0x10", hex.fromOctal("020").formatHex().get());
    analyze("0x10", hex.fromBinary("10000").formatHex().get());

    analyze("16", dec.fromHex("0x10").get());
    analyze("16", dec.fromOctal("020").get());
    analyze("16", dec.fromBinary("10000").get());

    analyze("20", oct.fromHex("0x10").get());
    analyze("20", oct.fromDecimal("16").get());
    analyze("20", oct.fromBinary("10000").get());

    analyze("10000", bin.fromHex("0x10").get());
    analyze("10000", bin.fromDecimal("16").get());
    analyze("10000", bin.fromOctal("020").get());
}

template<typename T>
string convertFromHex(Conv<T>& obj, const string& num_str) {
    return obj.fromHex(num_str).get();
}

template<typename T>
string convertFromDecimal(Conv<T>& obj, const string& num_str) {
    return obj.fromDecimal(num_str).get();
}

template<typename T>
string convertFromOctal(Conv<T>& obj, const string& num_str) {
    return obj.fromOctal(num_str).get();
}

template<typename T>
string convertFromBinary(Conv<T>& obj, const string& num_str) {
    return obj.fromBinary(num_str).get();
}

int main(int argc, char* argv[]) {
    Hex hex;
    Decimal dec;
    Octal oct;
    Binary bin;

    // Run unit tests
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
                        cout << convertFromHex(hex, num_str) << endl;
                        break;
                    case NumType::Decimal:
                        cout << convertFromHex(dec, num_str) << endl;
                        break;
                    case NumType::Octal:
                        cout << convertFromHex(oct, num_str) << endl;
                        break;
                    case NumType::Binary:
                        cout << convertFromHex(bin, num_str) << endl;
                        break;
                    case NumType::Any:
                        cout << "Hex: " << convertFromHex(hex, num_str) << endl;
                        cout << "Decimal: " << convertFromHex(dec, num_str) << endl;
                        cout << "Octal: " << convertFromHex(oct, num_str) << endl;
                        cout << "Binary: " << convertFromHex(bin, num_str) << endl;
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

                switch (command) {
                    case NumType::Hexadecimal:
                        cout << convertFromDecimal(hex, num_str) << endl;
                        break;
                    case NumType::Decimal:
                        cout << convertFromDecimal(dec, num_str) << endl;
                        break;
                    case NumType::Octal:
                        cout << convertFromDecimal(oct, num_str) << endl;
                        break;
                    case NumType::Binary:
                        cout << convertFromDecimal(bin, num_str) << endl;
                        break;
                    case NumType::Any:
                        cout << "Hex: " << convertFromDecimal(hex, num_str) << endl;
                        cout << "Decimal: " << convertFromDecimal(dec, num_str) << endl;
                        cout << "Octal: " << convertFromDecimal(oct, num_str) << endl;
                        cout << "Binary: " << convertFromDecimal(bin, num_str) << endl;
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
                        cout << convertFromOctal(hex, num_str) << endl;
                        break;
                    case NumType::Decimal:
                        cout << convertFromOctal(dec, num_str) << endl;
                        break;
                    case NumType::Octal:
                        cout << convertFromOctal(oct, num_str) << endl;
                        break;
                    case NumType::Binary:
                        cout << convertFromOctal(bin, num_str) << endl;
                        break;
                    case NumType::Any:
                        cout << "Hex: " << convertFromOctal(hex, num_str) << endl;
                        cout << "Decimal: " << convertFromOctal(dec, num_str) << endl;
                        cout << "Octal: " << convertFromOctal(oct, num_str) << endl;
                        cout << "Binary: " << convertFromOctal(bin, num_str) << endl;
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
                        cout << convertFromBinary(hex, num_str) << endl;
                        break;
                    case NumType::Decimal:
                        cout << convertFromBinary(dec, num_str) << endl;
                        break;
                    case NumType::Octal:
                        cout << convertFromBinary(oct, num_str) << endl;
                        break;
                    case NumType::Binary:
                        cout << convertFromBinary(bin, num_str) << endl;
                        break;
                    case NumType::Any:
                        cout << "Hex: " << convertFromBinary(hex, num_str) << endl;
                        cout << "Decimal: " << convertFromBinary(dec, num_str) << endl;
                        cout << "Octal: " << convertFromBinary(oct, num_str) << endl;
                        cout << "Binary: " << convertFromBinary(bin, num_str) << endl;
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

    cout << usage << endl;
	return 0;
}
