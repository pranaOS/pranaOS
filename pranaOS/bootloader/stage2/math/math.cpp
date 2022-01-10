#include <math.h>

using namespace math;

int pow(int x, int n) {
	int r = x;
	for (int i = 0; i < n-1; i++){
		r *= x;
	}
	return r;	
}

void
