#include "includes/hypercube.h"
#include <algorithm>
vector<u_int> const HyperCube::_emptyElements = vector<u_int>();

HyperCube::HyperCube(int dims, vector<BitVal> && fixedBits, 
  vector<u_int> const& elements, bool isCached): _dims(dims), 
                                  _fixedBits(fixedBits),
                                  _elements(elements),
                                  _isCached(isCached){
  if(_isCached&&(_fixedBits.size()!=0)){
    generateElements(_cachedElements);
  }
};

u_int HyperCube::size() {
  return 1<<(_dims-_fixedBits.size());
};

/* There're 2 ways to generate elements of a hypercube
   The faster one is to calculate from elements from 
   its cached unit_cube and return. This slower one is 
   to create them from the scratch. 
*/
void HyperCube::generateElements(vector<u_int>& v){
  if (_elements.empty()){
    /* Only UnitCubes have _elements empty
    */
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
    /*  If an UnitCube is cached, its Parallel HyperCubes can
        take advantage of unitcube elements which has been 
        generated to create their own elements. This way would
        gain a better performance than creating from the scratch
    */
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

/*
  Return the elements if cached; otherwise, generate them
*/
void HyperCube::getElements(vector<u_int>& v){
  if (_isCached){
    // Generate elements and stored in a local var
    if (_cachedElements.empty()) {
      generateElements(_cachedElements);
    };
    // stick the reference out of others using
    v=_cachedElements;
  } else {
    generateElements(v);
  };
};

vector<BitVal>const& HyperCube::getFixedBits(){
  return _fixedBits;
};

bool HyperCube::isCached() {
  return _cachedElements.size()==size();
}

int HyperCube::getDims(){
  return _dims;
};

HyperCube::~HyperCube(){
};
