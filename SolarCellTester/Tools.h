#pragma once
#include "float.h"

class Tools
{
public:
	bool dbIsEqu(double a,double b){return fabs(a-b)<DBL_EPSILON;}
	Tools(void);
public:
	~Tools(void);
};
