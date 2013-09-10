#include "includes/hypercube.h"
#include "gtest/gtest.h"
#include <list>
#include <map>
#include <memory>
using namespace std;

TEST(HypercubeTest, InitializationWithElementsNoCache) {
  vector<BitVal> fixedBits;
  fixedBits.push_back(std::make_pair(0,0));
  fixedBits.push_back(std::make_pair(3,1));
  vector<u_int> elements;
  u_int arr[4] = {9,11,13,15};
  elements.assign(arr, arr+4);
  // with elements, no cache
  unique_ptr<HyperCube> hCube(new HyperCube(4, std::move(fixedBits), elements));
  vector<u_int> actual;
  hCube->getElements(actual);
  u_int expected[4]={8,10,12,14};
  for(int i=0;i<4;i++){
    EXPECT_EQ(expected[i], actual[i]);
  };
  EXPECT_EQ(actual,actual);
};

TEST(HypercubeTest, InitializationWithElementsWithCache) {
  vector<BitVal> fixedBits;
  fixedBits.push_back(std::make_pair(0,0));
  fixedBits.push_back(std::make_pair(3,1));
  vector<u_int> elements;
  u_int arr[4] = {9,11,13,15};
  elements.assign(arr, arr+4);
  // with elements, no cache
  unique_ptr<HyperCube> hCube(new HyperCube(4, std::move(fixedBits), elements,true));
  vector<u_int> actual;
  hCube->getElements(actual);
  u_int expected[4]={8,10,12,14};
  for(int i=0;i<4;i++){
    EXPECT_EQ(expected[i], actual[i]);
  };
  EXPECT_EQ(actual,actual);
};

TEST(HypercubeTest, InitializationWithoutCacheOrElements) {
  vector<BitVal> fixedBits;
  fixedBits.push_back(std::make_pair(0,0));
  fixedBits.push_back(std::make_pair(3,1));
  vector<u_int> elements;
  u_int arr[4] = {9,11,13,15};
  elements.assign(arr, arr+4);
  // with elements, no cache
  unique_ptr<HyperCube> hCube(new HyperCube(4, std::move(fixedBits)));
  vector<u_int> actual;
  hCube->getElements(actual);
  u_int expected[4]={8,10,12,14};
  for(int i=0;i<4;i++){
    EXPECT_EQ(expected[i], actual[i]);
  };
  EXPECT_EQ(actual,actual);
};

