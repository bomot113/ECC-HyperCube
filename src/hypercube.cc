#include "includes/hypercube.h"
#include <algorithm>

HyperCube::HyperCube(int dims, vector<BitVal> const& fixedBits, vector<u_int> const& elements):_dims(dims), _fixedBits(fixedBits), _elements(elements) {
  for(auto& bitval:_fixedBits){
    u_int mask = 1<<bitval.first;
    bool bit_1 = bitval.second;
    if (!bit_1) {
      for(auto& item:_elements){
        item &= (~mask);
      };
    };
  };
};

u_int HyperCube::size() {
  return 1<<(_dims-_fixedBits.size());
};

vector<u_int> const& HyperCube::getElements() const{
  return _elements;
};

vector<BitVal>& HyperCube::getFixedBits(){
  return _fixedBits;
};

HyperCube::~HyperCube(){
};
