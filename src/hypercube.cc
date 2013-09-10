#include "includes/hypercube.h"
#include <algorithm>

HyperCube::HyperCube(int dims, vector<BitVal> const& fixedBits, 
  vector<u_int> const& elements, bool isCached): _dims(dims), 
                                  _fixedBits(fixedBits),
                                  _elements(elements),
                                  _isCached(isCached){
};

u_int HyperCube::size() {
  return 1<<(_dims-_fixedBits.size());
};

/* There're 2 ways to retrieve Elements of a hypercube
   The slow one is to calculate from  Elements from 
   its unit cube and return. This fastest one is to
   retrieve them from cache. 
*/
void HyperCube::generateElements(vector<u_int>& v){
  if (_elements.empty()){
    u_int leftBits, rightBits, value;
    u_int length = size();
    v=vector<u_int>();
    v.reserve(length);
    for(u_int i=0;i<length;i++){
      value = i;
      for(auto& item:_fixedBits){
        leftBits = value>>item.first;
        leftBits = leftBits<<item.first;
        rightBits = value^leftBits;
        value = (leftBits<<1) + ((item.second)<<(item.first)) + rightBits;
      }
      v.push_back(value);
    };
  } else {
    v = vector<u_int>(_elements);
    for(auto& bitval:_fixedBits){
      u_int mask = 1<<bitval.first;
      bool bit_1 = bitval.second;
      if (!bit_1) {
        for(auto& item:v){
          item &= (~mask);
        };
      };
    };
  };
};

void HyperCube::getElements(vector<u_int>& v){
  if (_isCached){
    if (_cachedElements.empty()) {
      generateElements(_cachedElements);
    };
    v=_cachedElements;
  };
  generateElements(v);
};

vector<BitVal>& HyperCube::getFixedBits(){
  return _fixedBits;
};

HyperCube::~HyperCube(){
};
