#include "hypercube.h"
#include <memory>
using namespace std;
class UnitCube: public HyperCube {
  private:
    unsigned _pDimsOffsetNeeded;
    u_int _cubeIndex;
    vector<unique_ptr<HyperCube> > _parallelHCubes; 
    vector<map<u_int,bool> > _getParallelHCubes(u_int bits, map<u_int,bool> fixedBits);
    // Parity Dims offset needed to generate parallel cubes
  public:
    UnitCube(unsigned int dims, u_int cubeIndex, bool isCached=false);
    void initParallelHCubes(u_int limit, bool parallelCubeCached=false);
    vector<unique_ptr<HyperCube> > const& getParallelHCubes() const; 
    unsigned int getPDimsOffsetNeeded() const;
    virtual ~UnitCube();
};
