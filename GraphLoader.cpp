#include "GraphLoader.h"



GraphLoader::GraphLoader()
{
	this->graph = new AConstraintGraph();
	this->stage = INIT;
}
void GraphLoader::setVertexNumber(int n)
{
	assert(this->stage == INIT || this->stage == NAMES);
	this->stage = NAMES;
	graph->setN(n);
}
void GraphLoader::setClassesNumber(int n)
{
	assert(this->stage == INIT || this->stage == NAMES);
	graph->setClassesNumber(n);
}
void GraphLoader::push_vertex(std::string name)
{
	assert(this->stage == INIT || this->stage == ATRS  || this->stage == NAMES);
	if (this->stage == ATRS)
	{
		this->stage = NAMES;
	}
	indexV[name] = this->graph->push_vertex(name);
}
void GraphLoader::push_attribute_int(std::string name)
{
	assert(this->stage == INIT || this->stage == ATRS);
	this->stage = ATRS;
	indexAI[name] = this->graph->push_attribute_int(name);
}
void GraphLoader::push_attribute_category(std::string name, std::vector<std::string> c)
{
	assert(this->stage == INIT || this->stage == ATRS);
	this->stage = ATRS;
	indexAC[name] = this->graph->push_attribute_category(name, c);
}
void GraphLoader::push_attribute_value(std::string vname, std::string aname, int v)
{
	std::map<std::string, int>::iterator it_v = indexV.find(vname);
	std::map<std::string, int>::iterator it_a = indexAI.find(aname);
	assert(it_a != indexAI.end() && it_v != indexV.end());
	this->graph->push_attribute_value(it_a->second, it_v->second, v);
}
void GraphLoader::push_generalization(std::string att, std::string name, std::vector<std::string> c)
{
	std::map<std::string, int>::iterator it_in = indexAC.find(att);
	assert(it_in != indexAC.end());
	this->graph->push_generalization(it_in->second, name, c);
}
void GraphLoader::push_edge(int i, int j, int cl)
{
	assert(this->stage == NAMES || this->stage == EDGES);
	if (this->stage == NAMES)
	{
		this->graph->finalize_vertexes();
		this->stage = EDGES;
	}
	graph->push_edge(i, j, cl);
}

void GraphLoader::push_edge(std::string in, std::string out, int cl)
{
	std::map<std::string, int>::iterator it_in = indexV.find(in);
	std::map<std::string, int>::iterator it_out = indexV.find(out);
	assert(it_in != indexV.end() && it_out != indexV.end());
	//std::cout << it_in->second << " " << it_out->second << std::endl;
	this->push_edge(it_in->second, it_out->second, cl);
}

AConstraintGraph * GraphLoader::get()
{
	this->load_attributes();
	this->load_names();
	this->graph->finalize_attributes();
	this->load_attributes_values();

	this->load_edges();
	graph->finalize();
	return graph;
}
