#include "TsvLoader.h"
#include <iostream>
#include <fstream>

using namespace std;

void TsvLoader::load_names()
{
	ifstream in(this->names_file.c_str());
	std::string name;
	assert(in.is_open());
	while (getline(in, name))
	{
		this->push_vertex(name);
	}
}

void TsvLoader::load_edges()
{
	ifstream in(this->edges_file.c_str());
	std::string edge_in;
	std::string edge_out;
	assert(in.is_open());
	while (in >> edge_in)
	{
		in >> edge_out;
		this->push_edge(edge_in, edge_out, 1);
	}
}

