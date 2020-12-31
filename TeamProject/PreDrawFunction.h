#pragma once
#include "Manager.h"

typedef void (*fptr)();

class PreDrawFunction : public Mappable{
private:
	fptr f;
public:
	PreDrawFunction(fptr f);
	fptr getFunction();
};

class PreDrawFunctionManager : public Manager<PreDrawFunction> {};
