#include "boost/dynamic_bitset.hpp"
#include <map>
#include <vector>
#include <iostream>
using namespace std;
typedef unsigned int u_int;
typedef boost::dynamic_bitset<> BITSET;

/*
    BitVal is a pair of a position in integer, and 
    a bit value in bool. 
*/
typedef pair<u_int,bool> BitVal;

enum ERROR {ENCODING, DECODING, LENGTH};

/* Caching mode:
    + NONE: don't catch at all
    + PARTIAL: will cache only unitcube's elements
    + FULL: will cache all elements in both types of
            cubes - unit and its parallel cubes
*/
enum CACHE {NONE,PARTIAL,FULL};

class HyperCube {
  protected:
    int _dims;
    vector<BitVal> _fixedBits;
    vector<u_int>const& _elements;
    bool _isCached;
    vector<u_int> _cachedElements;
    void generateElements(vector<u_int>& v);
  public:
    static vector<u_int> const _emptyElements;
    HyperCube(int dims, vector<BitVal>&& fixedBits,
                        vector<u_int> const& elements,
                        bool isCached=false);
    u_int size();
    void getElements(vector<u_int>& v);
    vector<BitVal>const& getFixedBits();
    bool isCached();
    virtual ~HyperCube();
};
