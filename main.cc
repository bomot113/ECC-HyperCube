#include <iostream>
#include <memory>
#include <string>
#include "includes/cubecode.h"
#include <time.h>

using namespace std;
int main() {
    int lines, n, r;
    char e;
    clock_t t,now;
    string s, result; 
    cin >> lines;
    for (int i=0; i<lines; i++) {
      cin >> n >> r >> e >> s;
      BITSET data(s); 

      now = clock();
      unique_ptr<CubeCode> cube(new CubeCode(n,r));// Initialization
      t = clock()-now;
      // Return code info
      cout<< "Initialization in:"<<((double)t)/CLOCKS_PER_SEC<<" sec."<< endl;
      cout<<"Meassage length(k):"<<cube->getDataLength()<<endl;
      cout<<"Block length(n):"<<cube->getCodewordLength()<<endl;
      cout<<"Rate(k/n):"<<((double)cube->getDataLength())/cube->getCodewordLength()<<endl;
      cout<<"Bit error tolerance:"<<((1<<r)-1)<<" bit(s)"<<endl;

      // encoding or decoding
      switch (e){
        case 'e':
          {
            now = clock();
            // Encoding
            BITSET encoded = cube->encode(data);
            t=clock()-now;
            cout<<"message:"<<s<<endl;
            cout<<"encoded:"<<encoded<<endl;
            cout<<"encoded in:"<<((double)t)/CLOCKS_PER_SEC<<" sec."<< endl;
            cout<<"--------------------------------"<<endl;
            break;
          }
        case 'd':
          {
            now = clock();
            // Decoding
            BITSET decoded = cube->decode(data);
            t=clock()-now;
            cout<<"received_code:"<<s<<endl;
            cout<<"______decoded:"<<decoded<<endl;
            cout<<"decoded in:"<<((double)t)/CLOCKS_PER_SEC<<" sec."<< endl;
            cout<<"--------------------------------"<<endl;
            break; 
          }
        default:
          continue;
      };
    }
    return 0;
}
