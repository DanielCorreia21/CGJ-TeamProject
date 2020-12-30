#pragma once
#include "Manager.h"

class PreDrawFunction : Mappable{
public:
	PreDrawFunction( void* f){
		this->function = f;
	}

private:
	string id;
	void* function;
};

class PreDrawFunctionManager : public Manager<PreDrawFunction> {};
