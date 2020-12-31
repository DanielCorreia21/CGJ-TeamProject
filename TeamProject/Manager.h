#pragma once
#include <map>
#include <iostream>
#include<vector>

#include "Mappable.h"

/*	AUTHORS
*	Group: 4
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*	Antoine Pontallier - 98316
*	André Santos - 91000
*/
using namespace std;

template<class E>
class Manager
{
protected:
	static Manager* instance;
	Manager();
	std::map<std::string, E*> map;
public:
	static Manager* getInstance();
	virtual ~Manager();
	void add(const std::string& id, E* value);
	E* get(const std::string& id);
	std::string get(E* value);
	vector<E*> getAllValues();
	void remove(const std::string& id);
	void clear();
};

template<class E>
Manager<E>* Manager<E>::instance = nullptr;

template<class E>
Manager<E>* Manager<E>::getInstance() {
	if (instance == nullptr) {
		instance = new Manager<E>();
	} 
	return instance;
}

template<class E>
Manager<E>::Manager() {

}

template<class E>
Manager<E>::~Manager() {
	typename std::map<std::string, E*>::iterator it = this->map.begin();
	while (it != this->map.end())
	{
		delete it->second;
		it++;
	}
}

template<class E>
inline void Manager<E>::add(const std::string& id, E* value)
{
	value->setId(id);
	if ( !(this->map.insert(std::make_pair(id, value)).second) ) {
		this->map[id] = value;
	}
}

template<class E>
inline E* Manager<E>::get(const std::string& id)
{
	typename std::map<std::string, E*>::iterator it = this->map.find(id);
	if (it != this->map.end()) {
		return it->second;
	}
	return NULL;
}

template<class E>
inline std::string Manager<E>::get(E* value) {
	typename std::map<std::string, E*>::iterator it = this->map.begin();

	while ( it != map.end()) {

		if (it -> second == value) {
			return it->first;
		}
		it++;
	}
	return "";
}

template<class E>
inline vector<E*> Manager<E>::getAllValues()
{
	typename std::map<std::string, E*>::iterator it = this->map.begin();

	typename std::vector<E*> values;

	while (it != map.end()) {

		values.push_back(it->second);
		it++;
	}
	return values;
}



template<class E>
inline void Manager<E>::remove(const std::string& id)
{
	this->map.erase(id);
}

template<class E>
inline void Manager<E>::clear()
{
	this->map.clear();
}