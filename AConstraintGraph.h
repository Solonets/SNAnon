#pragma once
#include <gecode/set.hh>
#include <gecode/search.hh>
#include <string>
#include <vector>
#include <iostream>
#include <gecode/set.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
using namespace Gecode;

class AConstraintGraph : public IntMinimizeSpace 
{
protected:
	int n = 0;
	int ai = 0;
	int ac = 0;
	int i_min = 9999;
	int i_max = 0;
	int *mins = NULL;
	int *maxs = NULL;
	int *info_lost_cost = NULL;
	int classesNumber = 1;
	std::vector<std::string> *names;
	std::vector<std::string> *atts_int;
	std::vector<std::string> atts_category;
	int **atts_int_values = NULL;
	std::vector<std::vector<std::string>> atts_category_vals;
	SetVarArray classes;
	SetVarArray edges;
	IntVarArray degreeSequence;
	IntVarArray degreeSequenceSorted;
	IntVarArray classesDegreeSequences;
	IntVarArray classesDegreeSequencesSorted;
	IntVarArray classesHashes;
	IntVarArray classesHashesSorted;
	IntVarArray AttributesIntLowerBound;
	IntVarArray AttributesIntUpperBound;
	//IntVarArray m;
	//IntVarArray mup;
	//IntVarArray mlw;
	IntVar costValue;
public:
	AConstraintGraph();
	int push_vertex(std::string name);
	int push_attribute_int(std::string name);
	int push_attribute_category(std::string name, std::vector<std::string>);
	int push_generalization(int att, std::string name, std::vector<std::string>);
	void push_edge(int i, int j, int cl);
	int push_attribute_value(int i, int j, int v);
	void finalize_attributes();
	void finalize_vertexes();
	void finalize();
	AConstraintGraph(bool share, AConstraintGraph &s);
	virtual Space *copy(bool share) {
		return new AConstraintGraph(share, *this);
	}
	virtual IntVar cost(void) const;
	~AConstraintGraph();
	void applyKDegree(int k);
	void print(std::ostream& os) const;
	void setN(int n) { this->n = n; }
	void setClassesNumber(int n) { this->classesNumber = n; }
	void debug();
	void printDot(std::ostream& os) const;
};

