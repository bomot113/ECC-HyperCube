#include "includes/cubecode.h"
#include <assert.h>
#include <string>
#include <algorithm>

CubeCode::CubeCode(unsigned int cwDims, unsigned int pDims, CACHE caType)
        :_cwDims(cwDims), _pDims(pDims), _caType(caType)
{
  _cwLength = (1<<_cwDims)-1;
  _votes = (1<<(_pDims+1))-1;
  _neededVotes = 1<<_pDims;
  // Establishing caching mode
  bool unitCubeCached = false;
  bool parallelCubeCached = false;
  switch(_caType){
    case CACHE::PARTIAL:
      unitCubeCached=true;
      break;
    case CACHE::FULL:
      unitCubeCached=true;
      parallelCubeCached=true;
      break;
    default:
      break;
  }; 

  // initialize UnitCubes
  _unitCubes.push_back(nullptr); // unit cube 0 doesn't exist
  for(u_int i=1;i<_cwLength+1;i++){
    unique_ptr<UnitCube> uCube(new UnitCube(_cwDims, i, unitCubeCached));
    _unitCubes.push_back(std::move(uCube));
  }
  
  // initialize _bit2cubes Mapping and cubesInLevel
  _cubesInLevel=vector<u_int>(_cwDims+1,0);
  for(unsigned int i=0;i<_cwDims;i++){
    vector<u_int> uCubes;
    getUnitCubes(i, uCubes);
    _cubesInLevel[i+1] = _cubesInLevel[i]+uCubes.size();
    for(auto& item:uCubes){
      _bit2Cubes.push_back(item);
    }
  }
  
  // data length = codeword length - parity length
  _dataLength = _cubesInLevel[_cwDims-_pDims]; 
  
  // Init parallel HyperCubes   
  for(u_int bitIndex=0;bitIndex<_dataLength;bitIndex++){
    u_int cubeIndex = _bit2Cubes[bitIndex];
    _unitCubes[cubeIndex]->initParallelHCubes(_votes, parallelCubeCached);
  }

  // Initialize cube2Bits mapping
  _cube2Bits=vector<u_int>(_cwLength+1, 0);
  for(unsigned int i=0;i<_cwLength;i++){
    u_int cubeIndex = _bit2Cubes[i];
    _cube2Bits[cubeIndex] = i;
  }

};

void CubeCode::getUnitCubes(unsigned int level, vector<u_int>& vector){
  std::string bitmask(_cwDims-level, 1);
  bitmask.resize(_cwDims, 0);
  u_int val;
  do {
    val=0;
    for(auto& a:bitmask){
      val = a + (val<<1);
    };
    vector.push_back(val);
  } while (prev_permutation(bitmask.begin(), bitmask.end()));
};

vector<unique_ptr<UnitCube> > const& CubeCode::getUnitCubes(){
  return _unitCubes;
};


bool CubeCode::getBitByIndex(BITSET const& code, u_int index) const{
  return code[_cwLength-index-1];
};

void CubeCode::setBitByIndex(BITSET& code, u_int index, bool bit) const{
  code[_cwLength-index-1]=bit;  
};

BITSET CubeCode::encode(BITSET code) const{
  if (code.size()!=_dataLength)
    throw ERROR::LENGTH;
  bool parityBit=0;
  u_int cubeIndex;
  // make space for parity bits
  code.resize(_cwLength);
  code = code<<(_cwLength-_dataLength);
  // calculate parity bits and add them into source code
  for(u_int bitIndex=_dataLength;bitIndex<_cwLength;bitIndex++){
    cubeIndex = _bit2Cubes[bitIndex];
    UnitCube* uCube = _unitCubes[cubeIndex].get();
    parityBit = calcParityFromSourceBit(code, *uCube);
    if (parityBit){
      setBitByIndex(code, bitIndex, 1);
    }
  }
  return code;
};

BITSET CubeCode::getCodeFromParity(BITSET parity, u_int dataLength) const{
  // Loop through parity bits, retrieve its location in 
  // the HyperCube and its UnitCubes to compute the source bit
  BITSET code(_cwLength);
  for(u_int i=0;i<dataLength;i++){
    u_int cubeIndex = _bit2Cubes[i];
    UnitCube* uCube = _unitCubes[cubeIndex].get();
    vector<u_int> subUCubes;
    uCube->getElements(subUCubes);
    bool sourceBit = 0;
    u_int bitIndex=0;
    for (auto& item:subUCubes){
      bitIndex = _cube2Bits[item];
      sourceBit ^= getBitByIndex(parity, bitIndex);
    };
    if (sourceBit){
      setBitByIndex(code, i, 1);
    };
  };
  return code;
};

bool CubeCode::calcParityFromSourceBit(BITSET const& code, HyperCube& hCube) const{
  vector<u_int> uCubes;
  hCube.getElements(uCubes);
  u_int bitIndex;
  bool bit = 0;
  bool parity=0;
  for(auto& item:uCubes){
    bitIndex = _cube2Bits[item];
    bit = getBitByIndex(code, bitIndex);
    parity ^= bit;
  }
  return parity;
};

BITSET CubeCode::decode(BITSET received) const{
  if (received.size()!=_cwLength)
    throw ERROR::LENGTH;

  // formalized code
  BITSET code(received);
  u_int bitIndex, cubeIndex;
  bool parityBit, sourceBit;
  bool error=false;
  // Check if there's any error received
  for(bitIndex=_dataLength;bitIndex<_cwLength;bitIndex++){
    cubeIndex = _bit2Cubes[bitIndex];
    UnitCube* uCube = _unitCubes[cubeIndex].get();
    parityBit = calcParityFromSourceBit(code, *uCube);
    if(parityBit){
      error = true;
      break;
    };
  };
  // Correct errors if needed
  if(error){
    // Correct parity bits first
    unsigned int dim, upperDim;
    unsigned int dataDims = _cwDims-_pDims;
    bool bit;
    BITSET parity(_cwLength);
    for(dim=dataDims;dim-->0;){
      BITSET fixedParity(_cwLength); // fixed parity of the current dim
      for(bitIndex=_cubesInLevel[dim];bitIndex<_cubesInLevel[dim+1];bitIndex++){
        cubeIndex = _bit2Cubes[bitIndex];
        UnitCube* uCube = _unitCubes[cubeIndex].get();
        vector<unique_ptr<HyperCube> > const& parallelCubes 
                                          = uCube->getParallelHCubes();
        // Voting
        parityBit = 0;
        u_int vote[] = {0,0};
        bit=0;
        for(auto& cube:parallelCubes){
          bit = calcParityFromSourceBit(code, *cube);
          vote[bit]++;
          if (vote[bit]==_neededVotes){
            parityBit = bit;
            break;
          };
        };
        if (parityBit){
          setBitByIndex(fixedParity, bitIndex, 1);
        };
      };

      // update parity
      parity|=fixedParity;
      BITSET mask(fixedParity);

      // Because the lower dim will only need pDims dims to fix
      // itself, we will travel through pDims-1 higher Dims to change.
      for(upperDim=dim+1;upperDim<(dim+_pDims);upperDim++){
        for(bitIndex=_cubesInLevel[upperDim];
            bitIndex<_cubesInLevel[upperDim+1];bitIndex++){
          cubeIndex = _bit2Cubes[bitIndex];
          UnitCube* uCube = _unitCubes[cubeIndex].get();
          vector<u_int> subUCubes;
          uCube->getElements(subUCubes);
          sourceBit = 0;
          for (auto& cube:subUCubes){
            sourceBit ^= getBitByIndex(mask, _cube2Bits[cube]);
          };
          if (sourceBit){
            setBitByIndex(mask, bitIndex, 1);
          };
        };
      };
      code ^= mask;
    };
    code = getCodeFromParity(parity,_dataLength);
  };

  // shrink parity bits and return only data bits
  BITSET result = code>>(_cwLength-_dataLength);
  result.resize(_dataLength);
  return result;
};

vector<u_int>  const& CubeCode::getBit2Cubes(){
  return _bit2Cubes;
};

vector<u_int>  const& CubeCode::getCube2Bits(){
  return _cube2Bits;
};

vector<u_int>  const& CubeCode::getCubesInLevel(){
  return _cubesInLevel;
};

u_int CubeCode::getDataLength(){
  return _dataLength;
};

u_int CubeCode::getCodewordLength(){
  return _cwLength;
};

CubeCode::~CubeCode(){
};
