#include <iostream>
#include <memory>
#include <string>
#include <time.h>
#include "includes/reedmuller.h"
using namespace std;
CACHE getCacheType(char c){
  switch (c) {
    case 'p':
      return CACHE::PARTIAL;
      break;
    case 'f':
      return CACHE::FULL;
      break;
    default:
      break;
  }
  return CACHE::NONE;
};
string getCacheName(char c){
  switch (c) {
    case 'p':
      return "partial";
      break;
    case 'f':
      return "full";
      break;
    default:
      break;
  }
  return "none";

};

int main() {
    int lines, m, r;
    char e,c;
    clock_t t,now;
    string s, result; 
    cin >> lines;
    for (int i=0; i<lines; i++) {
      cin >> r >> m >> e >> c >> s;
      BITSET data(s); 

      now = clock();
      unique_ptr<RM> cube(new RM(r,m,getCacheType(c)));// Initialization
      t = clock()-now;
      // Return code info
      cout<< "Initialization in:"<<((double)t)/CLOCKS_PER_SEC<<" sec."<< endl;
      cout<< "Cached:"<<getCacheName(c)<<endl;
      cout<<"Meassage length(k):"<<cube->getDataLength()<<endl;
      cout<<"Block length(n):"<<cube->getCodewordLength()+1<<endl;
      cout<<"Rate(k/n):"<<((double)cube->getDataLength()+1)/cube->getCodewordLength()<<endl;
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
