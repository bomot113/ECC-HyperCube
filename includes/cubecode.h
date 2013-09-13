#include "unitcube.h"
#include <memory>
using namespace std;
class CubeCode {
  private:
    unsigned int _cwDims;
    unsigned int _pDims;
    CACHE _caType;
    u_int _cwLength;
    u_int _dataLength;
    u_int _votes;
    u_int _neededVotes;
    vector<unique_ptr<UnitCube> > _unitCubes;
    vector<u_int> _cubesInLevel;
    vector<u_int> _bit2Cubes;
    vector<u_int> _cube2Bits;

  public:
    CubeCode(unsigned int cwDims, unsigned int pDims, CACHE caType=CACHE::NONE);
    bool getBitByIndex(BITSET const & code, u_int index) const;
    void setBitByIndex(BITSET& code, u_int index, bool bit) const;
    u_int getDataLength();
    u_int getCodewordLength();
    
    BITSET encode(BITSET code) const;
    BITSET decode(BITSET received) const;
    BITSET getCodeFromParity(BITSET parity, u_int dataLength) const;
    bool calcParityFromSourceBit(BITSET const& code, HyperCube& hCube) const;
     
    vector<u_int>  const& getBit2Cubes();
    vector<u_int> const& getCube2Bits();
    vector<u_int> const& getCubesInLevel();
    
    void getUnitCubes(unsigned int level, vector<u_int>& vector);
    vector<unique_ptr<UnitCube> > const& getUnitCubes();
    ~CubeCode();
};
