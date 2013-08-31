#include "../unitcube.h"
#include "gtest/gtest.h"
#include <list>
#include <map>
#include <memory>
using namespace std;

TEST(UnitCubeTest, Initialization) {
  unique_ptr<UnitCube> uCube(new UnitCube(4, 5));
  EXPECT_EQ((unsigned int)4, uCube->size());
  uCube->initParallelHCubes(100);
  vector<unique_ptr<HyperCube> > const& pHCubes = uCube->getParallelHCubes();
  vector<unique_ptr<HyperCube> >::const_iterator iter;
  iter = pHCubes.begin();
  vector<BitVal>& fBits = (*iter)->getFixedBits();
  EXPECT_EQ(1,fBits[0].second);
  EXPECT_EQ(1,fBits[1].second);
  vector<u_int> s;
  s = (*iter)->getElements();
  iter++;
  fBits = (*iter)->getFixedBits();
  EXPECT_EQ(1,fBits[0].second);
  EXPECT_EQ(0,fBits[1].second);
  iter++;
  fBits = (*iter)->getFixedBits();
  EXPECT_EQ(0,fBits[0].second);
  EXPECT_EQ(1,fBits[1].second);
};

TEST(UnitCubeTest, GetParallelHCubes3Bits) {
  unique_ptr<UnitCube> uCube(new UnitCube(4, 7));
  EXPECT_EQ((unsigned int)2, uCube->size());
  uCube->initParallelHCubes(3);
  vector<unique_ptr<HyperCube> > const& pHCubes = uCube->getParallelHCubes();
  EXPECT_EQ((unsigned int)3, pHCubes.size());
  
  vector<unique_ptr<HyperCube> >::const_iterator iter = pHCubes.begin();
  vector<BitVal>& fBits = (*iter)->getFixedBits();
  EXPECT_EQ(1,fBits[0].second);
  EXPECT_EQ(1,fBits[1].second);
  EXPECT_EQ(1,fBits[2].second);
  iter++;
  fBits = (*iter)->getFixedBits();
  EXPECT_EQ(1,fBits[0].second);
  EXPECT_EQ(1,fBits[1].second);
  EXPECT_EQ(0,fBits[2].second);
  iter++;
  fBits = (*iter)->getFixedBits();
  EXPECT_EQ(1,fBits[0].second);
  EXPECT_EQ(0,fBits[1].second);
  EXPECT_EQ(1,fBits[2].second);
};

TEST(UnitCubeTest, GetElements){
  unique_ptr<UnitCube> uCube(new UnitCube(4, 5));
  vector<u_int> elements = uCube->getElements();
  int arr[] = {5,7,13,15};
  vector<u_int> exp_elements(arr, arr + sizeof(arr)/sizeof(u_int));
  EXPECT_EQ(elements, exp_elements); 
};

