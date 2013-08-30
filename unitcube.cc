#include "unitcube.h"
UnitCube::UnitCube(unsigned int dims, u_int cubeIndex):HyperCube(dims){
  this->_cubeIndex = cubeIndex;
  map<u_int,bool> fixedBits;
  for(unsigned int i=0; i<dims; i++){
    bool bit = (cubeIndex>>i)&1;
    if (bit){
      fixedBits[i]=1;
    }
  }
  setFixedBits(fixedBits);
};

void UnitCube::initParallelHCubes(u_int limit){
  u_int length = (1<<_fixedBits.size())-1;
  length = (length>limit)?limit:length;
  for(u_int bits=_fixedBits.size();bits>0;bits--){
    std::string bitmask(bits, 1);
    bitmask.resize(_fixedBits.size(), 0);
    do {
      map<u_int,bool> aComb; // a combination of bits
      map<u_int,bool>::iterator iter=_fixedBits.begin();
      for(u_int i=0;i<_fixedBits.size();i++){
        aComb[(*iter++).first]=bitmask[i];
      }
      unique_ptr<HyperCube> aCube(new HyperCube(_dims, aComb));
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
