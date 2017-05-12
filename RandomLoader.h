#pragma once
#include "GraphLoader.h"
#include <string>
class RandomLoader :
	public GraphLoader
{
private:
	int vn;
	int maxDegree;
	int seed;
public:
	RandomLoader(int n, int maxDegree, int seed);
	virtual void load_names();
	virtual void load_edges();
	virtual void load_attributes() {};
	virtual void load_attributes_values() {};
	~RandomLoader();
};

