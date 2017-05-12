#pragma once
#include "AConstraintGraph.h"
#include <string>
#include <map>
class GraphLoader
{
protected:
	AConstraintGraph *graph;
	enum{INIT, NAMES, EDGES, ATRS} stage;
	std::map<std::string, int> indexV;
	std::map<std::string, int> indexAI;
	std::map<std::string, int> indexAC;
public:
	GraphLoader();
	virtual ~GraphLoader() {};
	virtual void load_names() {};
	virtual void load_edges() {};
	virtual void load_attributes() {};
	virtual void load_attributes_values() {};
	void push_vertex(std::string);
	void push_attribute_int(std::string);
	void push_attribute_category(std::string name, std::vector <std::string>);
	void push_attribute_value(std::string vname, std::string aname, int v);
	void GraphLoader::push_generalization(std::string att, std::string name, std::vector <std::string> c);
	void push_edge(int i, int j, int cl);
	void push_edge(std::string in, std::string out, int cl);
	void setVertexNumber(int n);
	void setClassesNumber(int n);
	AConstraintGraph* get();
};

