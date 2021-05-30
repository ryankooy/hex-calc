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
#include <iostream>
#include <map>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

const string hex_numbers = "0123456789ABCDEF";

class Hex {
public:
  Hex(int orignum) : val(orignum) {}
  ~Hex() {}
  void toHex() {
    setHexmap();
    do {
      result = setResult(val);
      setRemainder();
    } while (result > 0);
    for (int i = remainders.size() - 1; i > -1; --i) {
      hexstr.append(hexmap[remainders[i]]);
    }
  }
  void toDecimal(string hexnum) {
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
  void toBinary(int orignum) {
    val = orignum;
    do {
      val /= 2;
      setRemainder();
    } while (val > 0);
  }
  void toDecimal(string binnum) {
    decimal = ct = 0;
    for (int i = binnum.length() - 1; i > -1; --i) {
      temp = binnum[ct];
      digit = stoi(temp, &st);
      decimal += (digit * pow(2, i));
      ct++;
    }
  }
  string getBinary() const {
    return remainders;
  }
  int getDecimal() const {
    return decimal;
  }
private:
  void setRemainder() {
    remainder = val % 2;
    remainders.insert(0, to_string(remainder));
  }
  int val, remainder, bitnum, digit, decimal, ct;
  string remainders, temp;
  string::size_type st;
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

    // TO HEXADECIMAL
    Hex hex(num);
    hex.toHex();
    cout << "hexadecimal -> " << hex.getHex() << endl;

    // TO BINARY
    Binary bin;
    bin.toBinary(num);
    cout << "binary -> " << bin.getBinary() << endl;

    // TO DECIMAL FROM HEX
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
    hex.toDecimal(input);
    cout << "decimal -> " << hex.getDecimal() << endl;

    // TO DECIMAL FROM BINARY
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
    bin.toDecimal(input);
    cout << "decimal -> " << bin.getDecimal() << endl;

    // CONTINUE?
    cout << "Again? (y/n): ";
    cin >> input;
    while (true) {
      if (input == "y" || input == "Y")
        break;
      else if (input == "n" || input == "N")
        exit(EXIT_SUCCESS);
      else {
        cout << "Again? (y/n): ";
        cin >> input;
      }
    }
  }
  return 0;
}
