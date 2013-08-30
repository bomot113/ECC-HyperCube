#include "boost/dynamic_bitset.hpp"
#include <map>
#include <vector>
#include <iostream>
using namespace std;
typedef unsigned int u_int;
typedef boost::dynamic_bitset<> BITSET;
enum ERROR {ENCODING, DECODING, LENGTH};
class HyperCube {
  protected:
    int _dims;
    map<u_int,bool> _fixedBits;
    vector<u_int> _elements;
  public:
    HyperCube(int dims);
    HyperCube(int dims, map<u_int,bool> const& fixedBits);
    u_int size();
    void setFixedBits(map<u_int,bool> const& fixedBits);
    void setElements(vector<u_int>& vector);
    vector<u_int> const& getElements() const;
    map<u_int,char>& getFixedBits();
    virtual ~HyperCube();
};
