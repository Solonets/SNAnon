#pragma once
#include "GraphLoader.h"
#include <string>
class SampleLoader :
	public GraphLoader
{
public:
	SampleLoader();
	virtual void load_names();
	virtual void load_edges();
	virtual void load_attributes();
	virtual void load_attributes_values();
	~SampleLoader() {};
};

