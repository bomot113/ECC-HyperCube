#include "hypercube.h"
#include <memory>
using namespace std;
class UnitCube: public HyperCube {
  private:
    // Parity Dims offset needed to generate parallel cubes
    unsigned _pDimsOffsetNeeded;
    u_int _cubeIndex;
    vector<unique_ptr<HyperCube> > _parallelHCubes; 
    vector<map<u_int,bool> > _getParallelHCubes(u_int bits, map<u_int,bool> fixedBits);
    vector<u_int> _nearestParents;
  public:
    UnitCube(unsigned int dims, u_int cubeIndex, bool isCached=false);
    void initParallelHCubes(u_int limit, bool parallelCubeCached=false);
    void initNearestParents(int dimsOff);
    vector<unique_ptr<HyperCube> > const& getParallelHCubes() const; 
    unsigned int getPDimsOffsetNeeded() const;
    vector<u_int> const& getNearestParents() const;
    u_int getCubeIndex() const;
    virtual ~UnitCube();
};
