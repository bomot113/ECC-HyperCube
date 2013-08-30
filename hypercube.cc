#include "HyperCube.h"
#include <algorithm>

HyperCube::HyperCube(int dims){
  _dims = dims;
}

HyperCube::HyperCube(int dims, map<u_int,bool> const& fixedBits):HyperCube(dims) {
  setFixedBits(fixedBits);
};

void HyperCube::setFixedBits(map<u_int,bool> const& fixedBits){
  _fixedBits = fixedBits;
  setElements(_elements);
};

u_int HyperCube::size() {
  return 1<<(_dims-_fixedBits.size());
};

void HyperCube::setElements(vector<u_int>& vector){
  vector.clear();
  u_int leftBits, rightBits, value;
  for(u_int i=0;i<size();i++){
    value = i;
    for(auto& item:_fixedBits){
      leftBits = value>>item.first;
      leftBits = leftBits<<item.first;
      rightBits = value^leftBits;
      value = (leftBits<<1) + ((item.second)<<(item.first)) + rightBits;
    }
    vector.push_back(value);
  }
};

vector<u_int> const& HyperCube::getElements() const{
  return _elements;
};

map<u_int,bool>& HyperCube::getFixedBits(){
  return _fixedBits;
};

HyperCube::~HyperCube(){
};
