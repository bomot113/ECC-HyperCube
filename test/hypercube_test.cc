#include "../hypercube.h"
#include "gtest/gtest.h"
#include <list>
#include <map>
#include <memory>
using namespace std;

TEST(HypercubeTest, Initialization) {
  map<u_int,bool> fixedBits;
  unique_ptr<HyperCube> hCube(new HyperCube(4, fixedBits));
  EXPECT_EQ((u_int)16, hCube->size());
};

TEST(HypercubeTest, SetFixedBits) {
  map<u_int,bool> fixedBits;
  fixedBits[2]=1;
  fixedBits[3]=0;
  unique_ptr<HyperCube> hCube(new HyperCube(4, fixedBits));
  vector<u_int> expected;
  u_int arr[4]={4,5,6,7};
  expected.assign(arr,arr+4);
  vector<u_int>const& actual=hCube->getElements();
  EXPECT_EQ(expected, actual);
};

TEST(HypercubeTest, GetElements) {
  map<u_int,bool> fixedBits;
  fixedBits[0]=0;
  fixedBits[3]=1;
  unique_ptr<HyperCube> hCube(new HyperCube(4, fixedBits));
  vector<u_int>const& cubes = hCube->getElements();
  vector<u_int> expected;
  u_int arr[4]={8,10,12,14};
  expected.assign(arr,arr+4);
  EXPECT_EQ(expected, cubes);
};
