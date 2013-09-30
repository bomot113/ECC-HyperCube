#include "includes/unitcube.h"
UnitCube::UnitCube(unsigned int dims, u_int cubeIndex, bool isCached):
              HyperCube(dims, vector<BitVal>(),HyperCube::_emptyElements, isCached){
  _pDimsOffsetNeeded=0;
  _cubeIndex = cubeIndex;
  for(unsigned int i=0; i<dims; i++){
    bool bit = (cubeIndex>>i)&1;
    if (bit){
      _fixedBits.push_back(std::make_pair(i,1));
    }
  }
  if (_isCached)
    generateElements(_cachedElements);
};

void UnitCube::initParallelHCubes(u_int limit, bool parallelCubeCached){
  u_int length = (1<<_fixedBits.size())-1;
  length = (length>limit)?limit:length;
  // If this unit cubes cached its elements, then we will use
  // it to accelerate 
  vector<u_int>const& elements = (_isCached)?_cachedElements:_emptyElements;
  for(u_int bits=_fixedBits.size()-1;bits>0;bits--){
    int radius = _fixedBits.size()-bits;
    std::string bitmask(radius, 0);
    bitmask.resize(_fixedBits.size(), 1);
    do {
      vector<BitVal> aComb; // a combination of bits
      aComb.reserve(_fixedBits.size());
      vector<BitVal>::iterator iter=_fixedBits.begin();
      for(u_int i=0;i<_fixedBits.size();i++){
        u_int pos=(*iter++).first;
        BitVal aPair = std::make_pair(pos,bitmask[i]);
        aComb.push_back(std::move(aPair));
      };
      // create a parallel hypercube and add to the result list 
      unique_ptr<HyperCube> aCube = unique_ptr<HyperCube>(new HyperCube(_dims, move(aComb), elements, parallelCubeCached));
      _parallelHCubes.push_back(std::move(aCube));
      // if we get enough parallel hypercubes used for correcting bits, return
      if(--length<=0) {
        _pDimsOffsetNeeded = radius;
        return;
      };
    } while(std::next_permutation(bitmask.begin(),bitmask.end()));
  }
};

void UnitCube::initNearestParents(int dimsOff){
  _nearestParents.clear();
  for(int bits=1;bits<=dimsOff;bits++){
    std::string bitmask(bits, 0);
    bitmask.resize(_fixedBits.size(), 1);
    do {
      vector<BitVal>::iterator iter=_fixedBits.begin();
      u_int parentIndex = _cubeIndex;
      for(u_int i=0;i<_fixedBits.size();i++){
        u_int pos=(*iter++).first;
        if (bitmask[i]==0) {
          parentIndex ^= (1<<pos);
        };
      };
      _nearestParents.push_back(parentIndex);
    } while(std::next_permutation(bitmask.begin(),bitmask.end()));
  }
};

vector<unique_ptr<HyperCube> > const& UnitCube::getParallelHCubes() const{
  return _parallelHCubes;
};

vector<u_int>const& UnitCube::getNearestParents() const {
  return _nearestParents;
};

unsigned int UnitCube::getPDimsOffsetNeeded() const {
  return _pDimsOffsetNeeded;
};

u_int UnitCube::getCubeIndex() const{
  return _cubeIndex;
};

UnitCube::~UnitCube(){
};
