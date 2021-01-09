#include "PreDrawFunction.h"

PreDrawFunction::PreDrawFunction(fptr f) {
	this->f = f;
}

fptr PreDrawFunction::getFunction() {
	return f;
}