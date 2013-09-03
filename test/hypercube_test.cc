#include "includes/hypercube.h"
#include "gtest/gtest.h"
#include <list>
#include <map>
#include <memory>
using namespace std;

TEST(HypercubeTest, InitializationWithElements) {
  vector<BitVal> fixedBits;
  fixedBits.push_back(std::make_pair(0,0));
  fixedBits.push_back(std::make_pair(3,1));
  vector<u_int> elements;
  u_int arr[4] = {9,11,13,15};
  elements.assign(arr, arr+4);
  
  unique_ptr<HyperCube> hCube(new HyperCube(4, std::move(fixedBits), elements));
  vector<u_int>const& actual = hCube->getElements();
  
  vector<u_int> expected;
  u_int arr1[4]={8,10,12,14};
  expected.assign(arr1,arr1+4);
  
  EXPECT_EQ(expected, actual);

};

