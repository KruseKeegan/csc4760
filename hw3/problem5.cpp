#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std;

void printHex(double val) {
    unsigned long long bits;
    memcpy(&bits, &val, sizeof(bits));
    cout << "0x" << hex << uppercase << setfill('0') << setw(16) << bits << dec;
}

int main(){
  const int N = 100000;
  const double val = 1e-18;

  double sum = 1.0;
  for (int i = 0; i < N; i++){
    sum += val;
  }

  double sum2 = 0.0;
  for (int i = 0; i < N; i++){
    sum2 += val;
  }
  sum2+= 1.0;

cout << fixed << setprecision(20);
cout << "Method 1: " << sum  << "  hex: "; printHex(sum);  cout << endl;
cout << "Method 2: " << sum2 << "  hex: "; printHex(sum2); cout << endl;
cout << "Difference (sum - sum2): " << (sum - sum2) << endl;



  return 0;
}