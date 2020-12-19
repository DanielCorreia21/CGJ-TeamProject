#pragma once
#include <map>
#include <iostream>

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
	void remove(const std::string& id);
	void clear();

	//static instance
	//add
	//get
	//remove
	//clear
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
	this->map.insert(std::make_pair(id, value));
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
inline void Manager<E>::remove(const std::string& id)
{
	this->map.erase(id);
}

template<class E>
inline void Manager<E>::clear()
{
	this->map.clear();
}