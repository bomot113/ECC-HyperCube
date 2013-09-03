#include "boost/dynamic_bitset.hpp"
#include <map>
#include <vector>
#include <iostream>
using namespace std;
typedef unsigned int u_int;
typedef boost::dynamic_bitset<> BITSET;
typedef pair<u_int,bool> BitVal;
enum ERROR {ENCODING, DECODING, LENGTH};
class HyperCube {
  protected:
    int _dims;
    vector<BitVal> _fixedBits;
    vector<u_int> _elements;
  public:
    HyperCube(int dims, vector<BitVal> const& fixedBits, vector<u_int> const& elements);
    u_int size();
    vector<u_int> const& getElements() const;
    vector<BitVal>& getFixedBits();
    virtual ~HyperCube();
};
