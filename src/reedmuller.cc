#include "includes/reedmuller.h"
BITSET RM::encode(BITSET code) const {
  BITSET encoded = CubeCode::encode(code);
  bool parity = bool(encoded.count()%2);
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
