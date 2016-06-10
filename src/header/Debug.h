#include <cassert>
#include <iostream>
#ifdef DEBUG_MODE

extern unsigned int MASK;
#define DBG(A, B) {if ((A) & MASK) {B; } }

#else
#define DBG(A, B)
#endif


#define D1(a) DBG(1, a)
#define D2(a) DBG(2, a)
#define D3(a) DBG(4, a)

