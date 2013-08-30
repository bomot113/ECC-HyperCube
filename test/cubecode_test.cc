#include "../cubecode.h"
#include "gtest/gtest.h"
#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <assert.h>
template <typename E>
void equal(E* a1, E* a2){
  EXPECT_EQ(sizeof(a1), sizeof(a2));
  u_int size = sizeof(a1) /sizeof(E);
  for(u_int i=0;i<size;i++){
    EXPECT_EQ(a1[i],a2[i]);
  }
};

TEST(CubeCodeTest, Initialization) {
  CubeCode* code = new CubeCode(4,1);
  // Check bit2cubes property
  u_int bit2cubes[] = {15,14,13,11, 7,12,10,6, 9, 5, 3, 8, 4, 2, 1};
  const u_int* bit2cubes_ptr = bit2cubes;
  const u_int* act_bit2cubes = code->getBit2Cubes();
  equal(bit2cubes_ptr, act_bit2cubes);
  // Check cube2bits property
  u_int cube2bits[] = {-1,14,13,10,12,9,7,4,11,8,6,3,5,2,1,0};
  const u_int* cube2bits_ptr = cube2bits;
  const u_int* act_cube2bits = code->getCube2Bits();
  equal(cube2bits_ptr,act_cube2bits);
  // Check cubesInLevel property
  u_int cubesInLevel[] = {0,1,5,11,15};
  const u_int* cubesInLevel_ptr = cubesInLevel;
  const u_int* act_cubesInLevel = code->getCubesInLevel();
  equal(cubesInLevel_ptr, act_cubesInLevel);
  // Check data length
  EXPECT_EQ((u_int)11, code->getDataLength());
  // Check the UnitCubes and their ParallelHCubes
  vector<UnitCube*> cubes = code->getUnitCubes();
  EXPECT_EQ(NULL, cubes[0]);
  UnitCube* uCube = cubes[7];
  vector<unique_ptr<HyperCube> > pHCubes = uCube->getParallelHCubes();
  map<u_int,bool> fBits;
  vector<unique_ptr<HyperCube> >::iterator iter = pHCubes.begin();
  fBits = (*iter)->getFixedBits();
  EXPECT_EQ(1,fBits[0]);
  EXPECT_EQ(1,fBits[1]);
  EXPECT_EQ(1,fBits[2]);

};

TEST(CubeCodeTest, GetBitByIndex){
  CubeCode* cube = new CubeCode(4,1);
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
  CubeCode* cube = new CubeCode(4,1);
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

TEST(CubeCodeTest, CalcParityFromSourceBit){
  CubeCode* cube = new CubeCode(4,1);
  // 18731 = '100100100101011'
  BITSET code= BITSET(string("100100100101011"));
  map<u_int,bool> fixedBits;
  fixedBits[0] = 0;
  fixedBits[3] = 1;
  unique_ptr<HyperCube> hCube(new HyperCube(4, fixedBits));
  vector<u_int> cubes = hCube->getElements();
  int parity = cube->calcParityFromSourceBit(code, *hCube);
  // 18731 = '100100100101011'
  //           ^   ^^    ^ 
  EXPECT_EQ(0, parity);
  fixedBits[0] = 1;
  fixedBits[3] = 0;
  hCube.reset(new HyperCube(4, fixedBits));
  parity = cube->calcParityFromSourceBit(code, *hCube);
  EXPECT_EQ(0, parity);
};

TEST(CubeCodeTest, GetCodeFromParity){
  CubeCode* cube = new CubeCode(4,1);
  // parity = '111011110000000'
  BITSET parity(string("111011110000000"));
  BITSET actual = cube->getCodeFromParity(parity, 7);
  BITSET expected(string("100100100000000"));
  EXPECT_EQ(expected, actual);
};

TEST(CubeCodeTest, EncodingCode1PDims){
  CubeCode* cube = new CubeCode(4,1);
  // code     = '10010010010'
  // encoded  = '100100100101011'
  //
  BITSET code(string("10010010010"));
  BITSET expected(string("100100100101011"));
  BITSET encoded = cube->encode(code);
  EXPECT_EQ(expected, encoded); 

};

TEST(CubeCodeTest, EncodingCode2PDims){
  CubeCode* cube = new CubeCode(4,2);
  // code     = 22    # 0b10110
  // encoded  = 22726 # 0b101100011000110
  //
  BITSET code(string("10110"));
  BITSET expected(string("101100011000110"));
  BITSET encoded = cube->encode(code);
  EXPECT_EQ(expected, encoded); 
};

TEST(CubeCodeTest, EncodingCode3PDims){
  CubeCode* cube = new CubeCode(4,3);
  // code     = 1     # 0b1
  // encoded  = 32767 # 0b111111111111111
  //
  BITSET code(string("1"));
  BITSET expected(string("111111111111111"));
  BITSET encoded = cube->encode(code);
  EXPECT_EQ(expected, encoded); 
};

TEST(CubeCodeTest, Decoding0BitErrorCode){
  CubeCode* cube = new CubeCode(4,1);
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
  CubeCode* cube = new CubeCode(4,1);
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
  CubeCode* cube = new CubeCode(4,2);
  // Data              : '0b10110'
  // Transmitting Code : '0b101100011000110'
  // Received          : '0b101110001000010'
  //                            ^  ^    ^
  //                        3 Bits different
  BITSET received(string("101110001000010"));
  BITSET expected(string("10110"));
  BITSET actual = cube->decode(received);
  EXPECT_EQ(expected, actual); 
  // Data              : '0b10110'
  // Transmitting Code : '0b101100011000110'
  // Received          : '0b101100011101111'
  //                                 ^ ^  ^
  //                        3 Bits different
  received = BITSET(string("101100011101111"));
  actual = cube->decode(received);
  EXPECT_EQ(expected, actual); 
  // Data              : '0b10110'
  // Transmitting code : '0b101100011000110'
  // Received          : '0b011000011000110'
  //                        ^^ ^
  //                       3 Bits different
  received = BITSET(string("011000011000110"));
  actual = cube->decode(received);
  EXPECT_EQ(expected, actual); 
};

TEST(CubeCodeTest, Decoding7BitErrorCodeWith3ParityDims){
/* Data              : '0b1010110101010111010101'
    Transmitting code : '0b101011010101011101010110111101000111101001000000010111100110111'
    Received          : '0b101001000101111101011110111101010111111001000000010111110110111'
                               ^  ^    ^       ^          ^     ^                 ^
                                            7 Bits different
*/
  CubeCode* cube = new CubeCode(6,3);
  BITSET received(string("101001000101111101011110111101010111111001000000010111110110111"));
  BITSET expected(string("1010110101010111010101"));
  BITSET actual = cube->decode(received);
  EXPECT_EQ(expected, actual); 
};

string random(u_int length){
  string result(length,'0');
  for(u_int i=0;i<length/2;i++){
    u_int r = rand()%length;
    result[r]='1';
  }
  return result;
};

TEST(CubeCodeTest, EncodeDecodeCombined){
  unsigned int cwDims = 7;
  unsigned int pDims = 3;
  CubeCode* cube = new CubeCode(cwDims, pDims);
  BITSET data(string("0011000000000110010101100111000000001011001010110111010000100100"));
  BITSET    received(string("0011000000000110010000100111000000001011001010111111010000100100111000000011100110101010111100011000111101110011100101010110111"));
  BITSET decode = cube->decode(received);
  EXPECT_EQ(decode,data); 

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

TEST(CubeCodeTest, EncodingDecodingLongCode){
  srand(time(0));
  unsigned int cwDims = 7;
  unsigned int pDims = 4;
  CubeCode* cube = new CubeCode(cwDims, pDims);
  u_int errs = (1<<pDims)-1;
  string rand_bits = random(cube->getDataLength()); 
  BITSET data(rand_bits);
  BITSET encoded = cube->encode(data);
  BITSET received = noiseChannel(encoded, errs);
  BITSET decoded = cube->decode(received);
  EXPECT_EQ(data, decoded);
};


