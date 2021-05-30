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
  void toDecimal() {
    
  }
  string getHex() const {
    return hexstr;
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
  int hexnum, remainder, result, currentDigit, power;
  double val;
  string hexstr = "";
  vector<int> remainders;
  map<int, string> hexmap;
};

int main(int argc, char* argv[]) {
  int num;
  string::size_type sz;
  string input;
  cout << "Enter a number: ";
  cin >> input;
  num = stoi(input, &sz);
  Hex hex(num);
  hex.toHex();
  cout << input << " to hexadecimal is " << hex.getHex() << endl;
  return 0;
}
