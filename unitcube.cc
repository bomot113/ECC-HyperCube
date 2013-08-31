#include "unitcube.h"
UnitCube::UnitCube(unsigned int dims, u_int cubeIndex):
              HyperCube(dims, vector<BitVal>(),vector<u_int>()){
  _cubeIndex = cubeIndex;
  for(unsigned int i=0; i<dims; i++){
    bool bit = (cubeIndex>>i)&1;
    if (bit){
      _fixedBits.push_back(std::make_pair(i,1));
    }
  }

  u_int leftBits, rightBits, value;
  u_int length = 1<<(_dims-_fixedBits.size());
  _elements.reserve(length);
  for(u_int i=0;i<length;i++){
    value = i;
    for(auto& item:_fixedBits){
      leftBits = value>>item.first;
      leftBits = leftBits<<item.first;
      rightBits = value^leftBits;
      value = (leftBits<<1) + ((item.second)<<(item.first)) + rightBits;
    }
    _elements.push_back(value);
  };
};

void UnitCube::initParallelHCubes(u_int limit){
  u_int length = (1<<_fixedBits.size())-1;
  length = (length>limit)?limit:length;
  for(u_int bits=_fixedBits.size();bits>0;bits--){
    std::string bitmask(bits, 1);
    bitmask.resize(_fixedBits.size(), 0);
    do {
      vector<BitVal> aComb; // a combination of bits
      aComb.reserve(_fixedBits.size());
      vector<BitVal>::iterator iter=_fixedBits.begin();
      for(u_int i=0;i<_fixedBits.size();i++){
        BitVal aPair = std::make_pair((*iter++).first,bitmask[i]);
        aComb.push_back(std::move(aPair));
      };
      unique_ptr<HyperCube> aCube(new HyperCube(_dims, aComb, _elements));
      _parallelHCubes.push_back(std::move(aCube));
      if(--length<=0) return;
    } while(std::prev_permutation(bitmask.begin(),bitmask.end()));
  }
};

vector<unique_ptr<HyperCube> > const& UnitCube::getParallelHCubes() const{
  return _parallelHCubes;
};

UnitCube::~UnitCube(){
};
