#include "unitcube.h"
#include <memory>
using namespace std;
class CubeCode {
  private:
    int _cwDims;
    int _pDims;
    int _dataDims;
    CACHE _caType;
    u_int _cwLength;
    u_int _dataLength;
    u_int _votes;
    u_int _neededVotes;
    vector<unique_ptr<UnitCube> > _unitCubes;
    vector<u_int> _cubesInLevel;
    vector<u_int> _bit2Cubes;
    vector<u_int> _cube2Bits;
    vector<u_int> _nearestParents;
  public:
    CubeCode(int cwDims, int pDims, CACHE caType=CACHE::NONE);
    bool getBitByIndex(BITSET const & code, u_int index) const;
    void setBitByIndex(BITSET& code, u_int index, bool bit) const;
    bool flipBitByIndex(BITSET& code, u_int index) const;

    UnitCube* getUCubeByBitIndex(u_int bitIndex) const;
    u_int getDataLength();
    u_int getCodewordLength();
    
    virtual BITSET encode(BITSET code) const;
    virtual BITSET decode(BITSET received) const;
    BITSET transCodeParity(BITSET parity, int fromDim, 
                                            int toDim) const;
    bool calcParityFromSourceBit(BITSET const& code, HyperCube& hCube) const;
     
    vector<u_int> const& getBit2Cubes();
    vector<u_int> const& getCube2Bits();
    vector<u_int> const& getCubesInLevel();
    vector<u_int> const& getNearestParents() const;
    
    void getUnitCubes(int level, vector<u_int>& vector);
    vector<unique_ptr<UnitCube> > const& getUnitCubes();
    ~CubeCode();
};
