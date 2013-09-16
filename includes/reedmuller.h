#include "includes/cubecode.h"
#include "memory"
class RM:public CubeCode {
public:
  RM(unsigned int r, unsigned int m, CACHE caType=CACHE::NONE)
            :CubeCode(m, m-r-1, caType){}
  BITSET encode(BITSET code) const;
  BITSET decode(BITSET received) const;
};
