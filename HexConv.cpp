// // repeat until result == 0
// 1. val1 = decnum / 16
// 2. result1 = floor(val)
// 3. remainder1 = (val - result1) * 16 // IN DECIMAL
// 4. val2 = result1 / 16
// 5. result2 = floor(val2)
// 6. remainder2 = (val2 - result2) * 16
// 7. val3 = result2 / 16
// 8. result3 = floor(val3)
// 9. remainder3 = (val3 - result3) * 16e
// 10. hexnum = int(str(remainder3) + str(remainder2) + str(remainder1))

#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

const string hex_numbers = "0123456789ABCDEF";
const array<string, 16> bin_numbers = {
  "0000", "0001", "0010", "0011", "0100",
  "0101", "0110", "0111", "1000", "1001",
  "1010", "1011", "1100", "1101", "1110",
  "1111"
};


class Hex {
public:
  Hex() {}
  ~Hex() {}
  void toHexFromDecimal(int orignum) {
    val = orignum;
    setHexmap();
    do {
      result = setResult(val);
      setRemainder();
    } while (result > 0);
    for (int i = remainders.size() - 1; i > -1; --i) {
      hexstr.append(hexmap[remainders[i]]);
    }
  }
  void toDecimalFromHex(string hexnum) {
    if (hexnum.length() != 0) {
      for (int i = hexnum.length() - 1; i > -1; --i) {
        currentDigit = hex_numbers.find(hexnum[i]);
        decnum += (currentDigit * pow(16, power));
        power++;
      }
    } else {
      cout << "no hexadecimal value" << endl;
    }
  }
  string getHex() const {
    return "0x" + hexstr;
  }
  int getDecimal() const {
    return decnum;
  }
private:
  void setHexmap() {
    for (int i = 0; i < hex_numbers.length(); i++) {
      hexmap[i] = hex_numbers[i];
    }
  }
  int setResult(double value) {
    val = value / 16;
    return floor(val);
  }
  void setRemainder() {
    remainder = (val - result) * 16;
    remainders.push_back(remainder);
  }
  int remainder, result, currentDigit, power = 0, decnum = 0;
  double val;
  string hexstr = "";
  vector<int> remainders;
  map<int, string> hexmap;
};

class Binary {
public:
  Binary() {}
  ~Binary() {}
  void toBinaryFromDecimal(int orignum) {
    val = orignum;
    do {
      val /= 2;
      setRemainder();
    } while (val > 0);
  }
  void toDecimalFromBinary(string binnum) {
    decimal = ct = 0;
    for (int i = binnum.length() - 1; i > -1; --i) {
      temp = binnum[ct];
      digit = stoi(temp, &st);
      decimal += (digit * pow(2, i));
      ct++;
    }
  }
  void toBinaryFromHex(string hexnum) {
    setBinmap();
    for (int i = 0; i < hexnum.length(); i++) {
      temp = hexnum[i];
      binarynum.append(binmap[temp]);
    }
    currentnum = binarynum;
  }
  void toHexFromBinary(string binnum) {
    Hex* hex = new Hex();
    toDecimalFromBinary(binnum);
    hex->toHexFromDecimal(decimal);
    hexadecnum = hex->getHex();
    delete hex;
  }
  string getHex() const {
    return hexadecnum;
  }
  string getBinary() const {
    return currentnum;
  }
  int getDecimal() const {
    return decimal;
  }
private:
  void setBinmap() {
    for (int i = 0; i < bin_numbers.size(); i++) {
      temp = hex_numbers[i];
      binmap[temp] = bin_numbers[i];
    }
  }
  void setRemainder() {
    remainder = val % 2;
    remainders.insert(0, to_string(remainder));
    currentnum = remainders;
  }
  int val, remainder, bitnum, digit, decimal, ct;
  string remainders, temp, binarynum, hexadecnum, currentnum;
  string::size_type st;
  map<string, string> binmap, binmap2;
};

int main(int argc, char* argv[]) {
  int num, ct = 0;
  string::size_type sz;
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
    Hex hex;
    hex.toHexFromDecimal(num);
    cout << "hexadecimal -> " << hex.getHex() << endl;

    // DECIMAL TO BINARY
    Binary bin;
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
