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
    vector<u_int>const& _elements;
    bool _isCached;
    vector<u_int> _cachedElements;
    void generateElements(vector<u_int>& v);
  public:
    HyperCube(int dims, vector<BitVal> const& fixedBits,
                        vector<u_int> const& elements=vector<u_int>(),
                        bool isCached=false);
    u_int size();
    void getElements(vector<u_int>& v);
    vector<BitVal>& getFixedBits();
    virtual ~HyperCube();
};
