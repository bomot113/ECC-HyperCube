#include "includes/unitcube.h"
#include "gtest/gtest.h"
#include <list>
#include <map>
#include <memory>
using namespace std;

TEST(UnitCubeTest, Initialization) {
  unique_ptr<UnitCube> uCube(new UnitCube(4, 5));
  EXPECT_EQ((unsigned int)4, uCube->size());
  EXPECT_FALSE(uCube->isCached());
  uCube->initParallelHCubes(100);
  vector<unique_ptr<HyperCube> > const& pHCubes = uCube->getParallelHCubes();
  EXPECT_EQ((unsigned int)2, pHCubes.size());
  
  vector<unique_ptr<HyperCube> >::const_iterator iter;
  iter = pHCubes.begin();
  vector<BitVal>const& fBits = (*iter)->getFixedBits();
  EXPECT_EQ(0,fBits[0].second);
  EXPECT_EQ(1,fBits[1].second);
  iter++;
  vector<BitVal>const& fBits1 = (*iter)->getFixedBits();
  EXPECT_EQ(1,fBits1[0].second);
  EXPECT_EQ(0,fBits1[1].second);
};

TEST(UnitCubeTest, GetParallelHCubes3Bits) {
  unique_ptr<UnitCube> uCube(new UnitCube(4, 7));
  EXPECT_EQ((unsigned int)2, uCube->size());
  uCube->initParallelHCubes(3);
  vector<unique_ptr<HyperCube> > const& pHCubes = uCube->getParallelHCubes();
  EXPECT_EQ((unsigned int)3, pHCubes.size());
  
  vector<unique_ptr<HyperCube> >::const_iterator iter = pHCubes.begin();
  vector<BitVal>const& fBits = (*iter)->getFixedBits();
  EXPECT_EQ(0,fBits[0].second);
  EXPECT_EQ(1,fBits[1].second);
  EXPECT_EQ(1,fBits[2].second);
  iter++;
  vector<BitVal>const& fBits1 = (*iter)->getFixedBits();
  EXPECT_EQ(1,fBits1[0].second);
  EXPECT_EQ(0,fBits1[1].second);
  EXPECT_EQ(1,fBits1[2].second);
  iter++;
  vector<BitVal>const& fBits2 = (*iter)->getFixedBits();
  EXPECT_EQ(1,fBits2[0].second);
  EXPECT_EQ(1,fBits2[1].second);
  EXPECT_EQ(0,fBits2[2].second);
};

TEST(UnitCubeTest, GetElements){
  vector<u_int> a;
  unique_ptr<UnitCube> uCube(new UnitCube(4, 5));
  vector<u_int>elements;
  uCube->getElements(elements); 
  int arr[] = {5,7,13,15};
  vector<u_int> exp_elements(arr, arr + sizeof(arr)/sizeof(u_int));
  EXPECT_EQ(elements, exp_elements); 
};

TEST(UnitCubeTest, InitNearestParents){
  vector<u_int> a;
  unique_ptr<UnitCube> uCube(new UnitCube(4, 13, true));

  uCube->initNearestParents(1);
  vector<u_int> nearestParents = uCube->getNearestParents();
  int arr[] = {12,9,5};
  vector<u_int> exp_elements(arr, arr + 3);
  EXPECT_EQ(nearestParents, exp_elements); 

  uCube->initNearestParents(2);
  nearestParents = uCube->getNearestParents();
  int arr2[] = {12,9,5,8,4,1};
  exp_elements.assign(arr2, arr2 + 6);
  EXPECT_EQ(nearestParents, exp_elements); 

  uCube->initNearestParents(0);
  nearestParents = uCube->getNearestParents();
  exp_elements.clear();
  EXPECT_EQ(nearestParents, exp_elements); 
};


TEST(UnitCubeTest, GetElementsWithCache){
  vector<u_int> a;
  unique_ptr<UnitCube> uCube(new UnitCube(4, 5, true));
  EXPECT_TRUE(uCube->isCached());
  vector<u_int>elements;
  uCube->getElements(elements); 
  int arr[] = {5,7,13,15};
  vector<u_int> exp_elements(arr, arr + sizeof(arr)/sizeof(u_int));
  EXPECT_EQ(elements, exp_elements); 
};
