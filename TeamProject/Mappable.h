#pragma once
#include<string>

using namespace std;

class Mappable {

public:
	void setId(string id){
		this->id = id;
	}

	string getId() {
		return this->id;
	}
private:
	string id;
};