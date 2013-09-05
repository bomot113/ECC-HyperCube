#include <iostream>
#include <memory>
#include <string>
#include "includes/cubecode.h"
using namespace std;
int main() {
    int lines, n, r;
    char e;
    string s, result; 
    cin >> lines;
    for (int i=0; i<lines; i++) {
      cin >> n >> r >> e >> s;
      BITSET data(s); 
      switch (e){
        case 'e':
          {
            unique_ptr<CubeCode> cube(new CubeCode(n,r));
            BITSET encoded = cube->encode(data);
            boost::to_string(encoded, result);
            cout<<result<<endl;
            break;
          }
        case 'd':
          {
            unique_ptr<CubeCode> cube(new CubeCode(n,r));
            BITSET decoded = cube->decode(data);
            cout<<decoded<<endl;
            break; 
          }
        default:
          continue;
      };
    }
    return 0;
}
