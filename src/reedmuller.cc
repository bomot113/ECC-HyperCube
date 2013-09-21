#include "includes/reedmuller.h"

RM::RM(unsigned int r, unsigned int m, CACHE caType)
            :CubeCode(m, m-r-1, caType){
  messageLength = getDataLength();
  blockLength = getCodewordLength()+1;
  dataRate = (double)messageLength/blockLength;
  errBitTol = (1<<(m-r-1))-1;
};

BITSET RM::encode(BITSET code) const {
  BITSET encoded = CubeCode::encode(code);
  bool parity = (encoded.count()&1);
  encoded.resize(encoded.size()+1);
  encoded<<=1;
  encoded[0]=parity;
  return encoded;
};

BITSET RM::decode(BITSET received) const {
  received>>=1;
  received.resize(received.size()-1);
  BITSET decoded = CubeCode::decode(received);
  return decoded;
};
