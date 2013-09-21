#include "includes/cubecode.h"
#include "memory"
class RM:public CubeCode {
public:
  unsigned int blockLength;
  unsigned int messageLength;
  double dataRate;
  unsigned int errBitTol;
  RM(unsigned int r, unsigned int m, CACHE caType=CACHE::NONE);
  BITSET encode(BITSET code) const;
  BITSET decode(BITSET received) const;
};
