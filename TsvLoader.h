#pragma once
#include "GraphLoader.h"
#include <string>
class TsvLoader : public GraphLoader
{
private:
	std::string names_file;
	std::string edges_file;
public:
	TsvLoader(std::string names_file, std::string edges_file) : names_file(names_file), edges_file(edges_file) {}
	virtual void load_names();
	virtual void load_edges();
	virtual void load_attributes() {};
	virtual void load_attributes_values() {};
	~TsvLoader() {};
};

