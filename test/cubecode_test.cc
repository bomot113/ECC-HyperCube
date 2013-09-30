#include "includes/cubecode.h"
#include "gtest/gtest.h"
#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <assert.h>
#include <memory>
using namespace std;

template <typename E>
void equalVector(E& a1, E& a2){
  EXPECT_EQ(a1.size(), a2.size());
  for(u_int i=0;i<a1.size();i++){
    EXPECT_EQ(a1[i],a2[i]);
  }
};

TEST(CubeCodeTest, Initialization) {
  unique_ptr<CubeCode> code(new CubeCode(4,1));
  // Check bit2cubes property
  u_int bit2cubes[] = {15,14,13,11, 7,12,10, 9, 6, 5, 3, 8, 4, 2, 1};
  vector<u_int> bit2cubesV(bit2cubes,bit2cubes+sizeof(bit2cubes)/sizeof(u_int));
  vector<u_int> act_bit2cubes = code->getBit2Cubes();
  equalVector(act_bit2cubes, bit2cubesV);
  // Check cube2bits property
  u_int cube2bits[]={0,14,13,10,12, 9, 8, 4,11, 7, 6, 3, 5, 2, 1, 0};

  vector<u_int> cube2bitsV(cube2bits,cube2bits+sizeof(cube2bits)/sizeof(u_int));
  vector<u_int> act_cube2bits = code->getCube2Bits();
  equalVector(cube2bitsV, act_cube2bits);
  // Check cubesInLevel property
  u_int cubesInLevel[] = {0,1,5,11,15};
  vector<u_int> cubesInLevelV(cubesInLevel, 
                  cubesInLevel+sizeof(cubesInLevel)/sizeof(u_int));
  vector<u_int> act_cubesInLevel = code->getCubesInLevel();
  equalVector(act_cubesInLevel, cubesInLevelV);
  // Check data length
  EXPECT_EQ((u_int)11, code->getDataLength());
  // Check the UnitCubes and their ParallelHCubes
  vector<unique_ptr<UnitCube> > const& cubes = code->getUnitCubes();
  EXPECT_EQ(nullptr, cubes[0]);
  unique_ptr<UnitCube> const& uCube = cubes[7];
  vector<unique_ptr<HyperCube> > const& pHCubes = uCube->getParallelHCubes();
  vector<unique_ptr<HyperCube> >::const_iterator iter = pHCubes.begin();
  vector<BitVal>const& fBits = (*iter)->getFixedBits();
  EXPECT_EQ(0,fBits[0].second);
  EXPECT_EQ(1,fBits[1].second);
  EXPECT_EQ(1,fBits[2].second);
};

TEST(CubeCodeTest, InitializationFullCache) {
  unique_ptr<CubeCode> code(new CubeCode(4,1,CACHE::FULL));
  
  // Check the UnitCubes and their ParallelHCubes
  vector<unique_ptr<UnitCube> > const& cubes = code->getUnitCubes();
  EXPECT_EQ(nullptr, cubes[0]);
  unique_ptr<UnitCube> const& uCube = cubes[7];
  EXPECT_TRUE(uCube->isCached());
  vector<unique_ptr<HyperCube> > const& pHCubes = uCube->getParallelHCubes();
  vector<unique_ptr<HyperCube> >::const_iterator iter = pHCubes.begin();
  vector<BitVal>const& fBits = (*iter)->getFixedBits();
  EXPECT_TRUE((*iter)->isCached());
  EXPECT_EQ(0,fBits[0].second);
  EXPECT_EQ(1,fBits[1].second);
  EXPECT_EQ(1,fBits[2].second);
};

TEST(CubeCodeTest, InitializationPartialCache) {
  unique_ptr<CubeCode> code(new CubeCode(4,1,CACHE::PARTIAL));
  
  // Check the UnitCubes and their ParallelHCubes
  vector<unique_ptr<UnitCube> > const& cubes = code->getUnitCubes();
  EXPECT_EQ(nullptr, cubes[0]);
  unique_ptr<UnitCube> const& uCube = cubes[7];
  EXPECT_TRUE(uCube->isCached());
  vector<unique_ptr<HyperCube> > const& pHCubes = uCube->getParallelHCubes();
  vector<unique_ptr<HyperCube> >::const_iterator iter = pHCubes.begin();
  vector<BitVal>const& fBits = (*iter)->getFixedBits();
  EXPECT_FALSE((*iter)->isCached());
  EXPECT_EQ(0,fBits[0].second);
  EXPECT_EQ(1,fBits[1].second);
  EXPECT_EQ(1,fBits[2].second);
};
TEST(CubeCodeTest, GetBitByIndex){
  unique_ptr<CubeCode> cube(new CubeCode(4,1));
  // 18720 = '100100100100000'
  //          ^        ^
  //          0        9th
  BITSET code= BITSET(string("100100100100000"));
  int arr[]={1,0,0,1,0,0,1,0,0,1,0,0,0,0,0};
  for(int i=0;i<15;i++){
    int actual = cube->getBitByIndex(code,i);
    EXPECT_EQ(arr[i], actual);
  }
};

TEST(CubeCodeTest, SetBitByIndex){
  unique_ptr<CubeCode> cube(new CubeCode(4,1));
  // 18720 = '100100100100000'
  //          ^        ^
  //          0        9th
  BITSET code= BITSET(string("100100100100000"));
  cube->setBitByIndex(code, 9, 0);
  cube->setBitByIndex(code, 6, 0);
  EXPECT_EQ((u_int)18432, code.to_ulong());
  cube->setBitByIndex(code, 1, 1);
  EXPECT_EQ((u_int)26624, code.to_ulong());
};

TEST(CubeCodeTest, TransCodeParity){
  unique_ptr<CubeCode> cube(new CubeCode(4,1));
  // parity = '111011110000000'
  BITSET parity(string("111011110000000"));
  BITSET actual = cube->transCodeParity(parity,0,1);
  BITSET expected(string("100100000000000"));
  EXPECT_EQ(expected, actual);
};

TEST(CubeCodeTest, EncodingCode1PDims){
  unique_ptr<CubeCode> cube(new CubeCode(4,1));
  // code     = '10010010010'
  // encoded  = '100100100101011'
  //
  BITSET code(string("10010010010"));
  BITSET expected(string("100100100101011"));
  BITSET encoded = cube->encode(code);
  EXPECT_EQ(expected, encoded); 

};

TEST(CubeCodeTest, EncodingCode2PDims){
  unique_ptr<CubeCode> cube(new CubeCode(4,2));
  // code     = 22    # 0b10110
  // encoded  = 22726 # 0b101100011000110
  //
  BITSET code(string("10110"));
  BITSET expected(string("101100011000110"));
  BITSET encoded = cube->encode(code);
  EXPECT_EQ(expected, encoded); 
};

TEST(CubeCodeTest, EncodingCode3PDims){
  unique_ptr<CubeCode> cube(new CubeCode(4,3));
  // code     = 1     # 0b1
  // encoded  = 32767 # 0b111111111111111
  //
  BITSET code(string("1"));
  BITSET expected(string("111111111111111"));
  BITSET encoded = cube->encode(code);
  EXPECT_EQ(expected, encoded); 
};

string random(u_int length){
  string result(length,'0');
  for(u_int i=0;i<length/2;i++){
    u_int r = rand()%length;
    result[r]='1';
  }
  return result;
};

BITSET noiseChannel(BITSET bits, u_int flips){
  BITSET result(bits);
  for(u_int i=0;i<flips;i++){
    u_int r= rand()%bits.size();
    bits[r]^=1;  
  };
  BITSET diff = result^bits;
  assert (diff.count() <= flips);
  return bits;
};

TEST(CubeCodeTest, Decoding0BitErrorCode){
  unique_ptr<CubeCode> cube(new CubeCode(4,1)); // Hamming Code
  /* Oringinal : '0b100100100101011' (18731)
     Received  : the same
     Decoded   : '0b10010010010'
  */
  BITSET received(string("100100100101011"));
  BITSET expected(string("10010010010"));
  BITSET actual = cube->decode(received);
  EXPECT_EQ(expected, actual); 

};

TEST(CubeCodeTest, Decoding1BitErrorCode){
  unique_ptr<CubeCode> cube(new CubeCode(4,1)); // Hamming code
  // Data              : '0b10010010010'
  // Transmitting Code : '0b100100100101011'
  // Received          : '0b100100110101011'
  //                                ^
  //                        one Bit different
  BITSET received(string("100100110101011"));
  BITSET expected(string("10010010010"));
  BITSET actual = cube->decode(received);
  EXPECT_EQ(expected, actual); 
};

TEST(CubeCodeTest, Decoding3BitErrorCodeWith2ParityDims){
  unique_ptr<CubeCode> cube(new CubeCode(4,2)); // first order RM
  string bitmask(3,'1');
  bitmask.resize(15, '0');
  BITSET data(string("101100011000110"));
  BITSET expected(string("10110"));
  do {
    BITSET errors(bitmask);
    BITSET received = errors ^ data;
    BITSET actual = cube->decode(received);
    EXPECT_EQ(expected, actual);
  } while(prev_permutation(bitmask.begin(), bitmask.end()));
};


TEST(CubeCodeTest, EncodingDecodingLongCode){
  srand(time(0));
  unsigned int cwDims = 15;
  unsigned int pDims = 7;
  unique_ptr<CubeCode> cube(new CubeCode(cwDims, pDims,CACHE::FULL));
  u_int errs = (1<<pDims)-1;
  string rand_bits = random(cube->getDataLength()); 
  BITSET data(rand_bits);
  BITSET encoded = cube->encode(data);
  BITSET received = noiseChannel(encoded, errs);
  BITSET decoded = cube->decode(received);
  EXPECT_EQ(data, decoded);
};
