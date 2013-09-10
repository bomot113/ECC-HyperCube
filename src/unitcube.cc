#include "includes/unitcube.h"
UnitCube::UnitCube(unsigned int dims, u_int cubeIndex,bool isCached):
              HyperCube(dims, vector<BitVal>(), vector<u_int>(),isCached){
  _cubeIndex = cubeIndex;
  for(unsigned int i=0; i<dims; i++){
    bool bit = (cubeIndex>>i)&1;
    if (bit){
      _fixedBits.push_back(std::make_pair(i,1));
    }
  }
};

void UnitCube::initParallelHCubes(u_int limit){
  u_int length = (1<<_fixedBits.size())-1;
  length = (length>limit)?limit:length;
  vector<u_int> elements;
  getElements(elements);
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
      // create a parallel hypercube and add to the result list 
      unique_ptr<HyperCube> aCube(new HyperCube(_dims, aComb, elements));
      _parallelHCubes.push_back(std::move(aCube));
      // if we get enough parallel hypercubes used for correcting bits, return
      if(--length<=0) return;
    } while(std::prev_permutation(bitmask.begin(),bitmask.end()));
  }
};

vector<unique_ptr<HyperCube> > const& UnitCube::getParallelHCubes() const{
  return _parallelHCubes;
};

UnitCube::~UnitCube(){

