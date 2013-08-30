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
  map<u_int,bool>& fBits = (*iter)->getFixedBits();
  EXPECT_EQ(1,fBits[0]);
  EXPECT_EQ(1,fBits[2]);
  iter++;
  fBits = (*iter)->getFixedBits();
  EXPECT_EQ(1,fBits[0]);
  EXPECT_EQ(0,fBits[2]);
  iter++;
  fBits = (*iter)->getFixedBits();
  EXPECT_EQ(0,fBits[0]);
  EXPECT_EQ(1,fBits[2]);
};

TEST(UnitCubeTest, GetParallelHCubes3Bits) {
  unique_ptr<UnitCube> uCube(new UnitCube(4, 7));
  EXPECT_EQ((unsigned int)2, uCube->size());
  uCube->initParallelHCubes(3);
  vector<unique_ptr<HyperCube> > const& pHCubes = uCube->getParallelHCubes();
  EXPECT_EQ((unsigned int)3, pHCubes.size());
  
  vector<unique_ptr<HyperCube> >::const_iterator iter = pHCubes.begin();
  map<u_int,bool>& fBits = (*iter)->getFixedBits();
  EXPECT_EQ(1,fBits[0]);
  EXPECT_EQ(1,fBits[1]);
  EXPECT_EQ(1,fBits[2]);
  iter++;
  fBits = (*iter)->getFixedBits();
  EXPECT_EQ(1,fBits[0]);
  EXPECT_EQ(1,fBits[1]);
  EXPECT_EQ(0,fBits[2]);
  iter++;
  fBits = (*iter)->getFixedBits();
  EXPECT_EQ(1,fBits[0]);
  EXPECT_EQ(0,fBits[1]);
  EXPECT_EQ(1,fBits[2]);
};

