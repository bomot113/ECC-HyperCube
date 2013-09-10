#include "hypercube.h"
#include <memory>
using namespace std;
class UnitCube: public HyperCube {
  private:
    u_int _cubeIndex;
    vector<unique_ptr<HyperCube> > _parallelHCubes; 
    vector<map<u_int,bool> > _getParallelHCubes(u_int bits, map<u_int,bool> fixedBits);
  public:
    UnitCube(unsigned int dims, u_int cubeIndex,bool isCached=false);
    void initParallelHCubes(u_int limit);
    vector<unique_ptr<HyperCube> > const& getParallelHCubes() const; 
    virtual ~UnitCube();
};
