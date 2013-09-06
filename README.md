ECC-HyperCube
=============

Fast algorithm for encoding and decoding Reed Muller Error Correcting Codes based on HyperCubes

Installation:
  - $make
Run:
  - $cat input | ./cubecode > output
  - Input file format:
      + First line: number of activities (or number of lines following)
      + From second line to the end: each line defines n, r, activity type, and data respectively. 
       ==> r is the order and n is the length (2^n) of reed muller codes. From my perspective, n would be the number of dimensions of block code and r would be the number of dimensions of the parity bits used to correct bit errors.
       ==> activity type: e-> encode, d-> decode.
       ==> data: in encoding type, data is the message need encoding, while in decoding type, data is the message received from a noisy channel with error bits included.
      + Restrictions: 
         0<n<16
         0<r<n
         actvity: e or d
         data: binary string with length = C(n,0)+C(n,1)+..+C(n-r-1)
               with C(m,n) = m!/(n!(m-n)!)
               So, for n=4,r=2 code will have a length=C(4,0)+C(4,1)=1+4=5
      + Input example:
         input
         -----------
         1
         4 2 e 10110
      There's 1 activity required to process which is encoding reed muller code (4,2) with message 10110
      + Output:
         Initialization in:0 sec.
         Meassage length(k):5
         Block length(n):15
         Rate(k/n):0.333333
         Bit error tolerance:3 bit(s)
         message:10110
         encoded:101100011000110
         encoded in:0 sec.

