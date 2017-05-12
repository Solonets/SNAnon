#include "AConstraintGraph.h"
#include <gecode/gist.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <math.h>

AConstraintGraph::AConstraintGraph()
{
	n = 0;
	names = new std::vector<std::string>();
	atts_int = new std::vector<std::string>();
}

int AConstraintGraph::push_vertex(std::string name)
{
	names->push_back(name);
	return names->size() - 1;
}
int AConstraintGraph::push_attribute_int(std::string name)
{
	atts_int->push_back(name);
	return atts_int->size() - 1;
}
int AConstraintGraph::push_attribute_category(std::string name, std::vector<std::string> values)
{
	atts_category.push_back(name);
	atts_category_vals.push_back(values);
	return atts_category.size() - 1;

}
int AConstraintGraph::push_generalization(int att, std::string name, std::vector<std::string> c)
{
	assert(!this->atts_category_vals[att].empty());
	for (auto it = c.begin(); it != c.end(); ++it) {
		assert(std::find(atts_category_vals[att].begin(), atts_category_vals[att].end(), *it) != atts_category_vals[att].end());
	}
	return 0;
}
void AConstraintGraph::push_edge(int i, int j, int cl)
{
	Matrix<SetVarArray> classesMatrix(classes, classesNumber, n);
	if (i == j)
		return;
	//dom(*this, edges[i], SRT_SUP, j);
	//dom(*this, edges[j], SRT_SUP, i);
	dom(*this, classesMatrix(cl, i), SRT_SUP, j);
	dom(*this, classesMatrix(cl, j), SRT_SUP, i);
	//this->debug();
}
int AConstraintGraph::push_attribute_value(int i, int j, int v)
{
	atts_int_values[i][j] = v;
	if (i_min > v)
		i_min = v;
	if (i_max < v)
		i_max = v;
	if (mins[i] > v)
		mins[i] = v;
	if (maxs[i] < v)
		maxs[i] = v;
	return 0;
}
void AConstraintGraph::finalize_attributes()
{
	std::cout << "finalize attributes" << std::endl;
	if (ai == 0)
		ai = this->atts_int->size();
	if (ac == 0)
		ac = this->atts_category.size();
	if (n == 0)
		n = this->names->size();
	atts_int_values = new int*[ai];
	mins = new int[ai];
	maxs = new int[ai];
	info_lost_cost = new int[ai];
	for (int i = 0; i < ai; i++)
	{
		atts_int_values[i] = new int[n];
		maxs[i] = 0;
		mins[i] = 9999;
	}
}
void AConstraintGraph::finalize_vertexes()
{
	int maxCost = 1;
	std::cout << "finalize vertexes" << std::endl;
	if (n == 0)
		n = this->names->size();
	for (int i = 0; i < ai; i++)
	{
		int cost = 1;
		for (int j = 0; j < ai; j++)
			if (i != j)
			cost *= maxs[j] - mins[j] + 1;
		info_lost_cost[i] = cost;
		//std::cout << info_lost_cost[i] << " ";
		maxCost *= cost;

	}

	//std::cout << n;
	//assert(false);
	classes = SetVarArray(*this, n * classesNumber, IntSet::empty, IntSet(0, n - 1));
	Matrix<SetVarArray> classesMatrix(classes, classesNumber, n);
	
	AttributesIntLowerBound = IntVarArray(*this, n * ai, i_min, i_max);
	Matrix<IntVarArray> AttributesIntLowerBoundMatrix(AttributesIntLowerBound, ai, n);

	AttributesIntUpperBound = IntVarArray(*this, n * ai, i_min, i_max);
	Matrix<IntVarArray> AttributesIntUpperBoundMatrix(AttributesIntUpperBound, ai, n);

	for (int i = 0; i < ai; i++)
		for (int j = 0; j < n; j++)
		{
			rel(*this, AttributesIntLowerBoundMatrix(i, j), IRT_LQ, atts_int_values[i][j]);
			rel(*this, AttributesIntUpperBoundMatrix(i, j), IRT_GQ, atts_int_values[i][j]);
		}
		
	edges = SetVarArray(*this, n, IntSet::empty, IntSet(0, n - 1));
	for (int i = 0; i < n; i++)
	{
		rel(*this, SOT_DUNION, classesMatrix.row(i), edges[i]);
	}
	for (int i = 0; i < classesNumber; i++)
	{
		channel(*this, classesMatrix.col(i), classesMatrix.col(i));
	}
	classesDegreeSequences = IntVarArray(*this, n * classesNumber, 0, n - 1);
	classesDegreeSequencesSorted = IntVarArray(*this, n * classesNumber, 0, n - 1);
	Matrix<IntVarArray> degreeSequencesMatrix(classesDegreeSequences, classesNumber, n);
	Matrix<IntVarArray> degreeSequencesSortedMatrix(classesDegreeSequencesSorted, classesNumber, n);
	/*int *k = new int[classesNumber];
	int kf = 1;
	for (int i = classesNumber -1; i >= 0; i--, kf *= n)
		k[i] = kf;*/
	classesHashes = IntVarArray(*this, n, 0, n);
	classesHashesSorted = IntVarArray(*this, n, 0, n);
	sorted(*this, classesHashes, classesHashesSorted);
	
	/*for (int j = 0; j < n; j++)
	{
		linear(*this, IntArgs(classesNumber, k), degreeSequencesMatrix.row(j), IRT_EQ, classesHashes[j]);
	}*/
	for (int i = 0; i < classesNumber; i++)
	{
		IntVarArray m = IntVarArray(*this, n, 0, n);
		for (int j = 0; j < n; j++)
		{
			element(*this, m, classesHashes[j], degreeSequencesMatrix(i, j));
		}
	}
	for (int i = 0; i < ai; i++)
	{
		IntVarArray mup = IntVarArray(*this, n, i_min, i_max);
		IntVarArray mlw = IntVarArray(*this, n, i_min, i_max);
		for (int j = 0; j < n; j++)
		{
			element(*this, mup, classesHashes[j], AttributesIntLowerBoundMatrix(i, j));
			element(*this, mlw, classesHashes[j], AttributesIntUpperBoundMatrix(i, j));
		}
	}

	//delete k;
	for (int j = 0; j < classesNumber; j++)
	{
		for (int i = 0; i < n; i++)
		{
			cardinality(*this, classesMatrix(j, i), degreeSequencesMatrix(j, i));
		}
		sorted(*this, degreeSequencesMatrix.col(j), degreeSequencesSortedMatrix.col(j));
	}
	degreeSequence = IntVarArray(*this, n, 0, n - 1);
	degreeSequenceSorted = IntVarArray(*this, n, 0, n - 1);
	costValue = IntVar(*this, 0, n * maxCost);
    channel(*this, edges, edges);
	for (int i = 0; i < n; i++)
	{
		cardinality(*this, edges[i], degreeSequence[i]);
	}
	sorted(*this, degreeSequence, degreeSequenceSorted);
	IntVarArray Intervals = IntVarArray(*this, n * ai , 0, i_max - i_min + 1);
	Matrix<IntVarArray> IntervalsMatrix(Intervals, ai, n); 
	for (int i = 0; i < ai; i++)
	{
		for (int j = 0; j < n; j++)
		{
			IntervalsMatrix(i, j) = expr(*this, (AttributesIntUpperBoundMatrix(i, j)* info_lost_cost[i] - AttributesIntLowerBoundMatrix(i, j) * info_lost_cost[i]));
		}
		
	}
	for (int i = 0; i < n; i++)
	linear(*this, Intervals, IRT_EQ, costValue);
	//rel(*this, costValue == degreeSequencesMatrix(0, 0) + degreeSequencesMatrix(0, 1) + degreeSequencesMatrix(0, 2) + degreeSequencesMatrix(0, 7));
	//rel(*this, cost = )
	//assert(false);
}

void AConstraintGraph::finalize()
{
	std::cout << "finalize" << std::endl;
	//cardinality(*this, in[0], 3, 3);
	//no self loops
	for (int i = 0; i < n; i++)
		dom(*this, edges[i], SRT_DISJ, i);
	//branch(*this, out, SET_VAR_NONE(), SET_VAL_MIN_INC());
}

AConstraintGraph::AConstraintGraph(bool share, AConstraintGraph & s) : IntMinimizeSpace(share, s)
{
	//std::cout << "copy" << std::endl;
	edges.update(*this, share, s.edges);
	degreeSequence.update(*this, share, s.degreeSequence);  
	degreeSequenceSorted.update(*this, share, s.degreeSequenceSorted);
	costValue.update(*this, share, s.costValue);
	classes.update(*this, share, s.classes);
	classesDegreeSequences.update(*this, share, s.classesDegreeSequences);   
	classesDegreeSequencesSorted.update(*this, share, s.classesDegreeSequencesSorted);  
	classesHashes.update(*this, share, s.classesHashes);
	classesHashesSorted.update(*this, share, s.classesHashesSorted);
	//m.update(*this, share, m);
	//mlw.update(*this, share, mlw);
	//mup.update(*this, share, mup);

	AttributesIntLowerBound.update(*this, share, s.AttributesIntLowerBound);
	AttributesIntUpperBound.update(*this, share, s.AttributesIntUpperBound);
	n = s.n;
	ai = s.ai;
	ac = s.ac;
	mins = s.mins;
	maxs = s.maxs;
	names = s.names;
	atts_int = s.atts_int;
	atts_int_values = s.atts_int_values;
	classesNumber = s.classesNumber;
	info_lost_cost = s.info_lost_cost;
	
	//c.update(*this, share, s.c);
	//debug();
}

IntVar AConstraintGraph::cost(void) const
{
	return costValue;
}

AConstraintGraph::~AConstraintGraph()
{
}

void AConstraintGraph::applyKDegree(int k)
{
	Matrix<IntVarArray> degreeSequencesSortedMatrix(classesDegreeSequencesSorted, classesNumber, n);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < classesNumber; j++)
		{
			count(*this, degreeSequencesSortedMatrix.slice(j, j + 1, fmax(0, i - k + 1), fmin(n, i + k)), degreeSequencesSortedMatrix(j, i), IRT_GQ, k);
		}
		count(*this, degreeSequenceSorted.slice(fmax(0, i - k + 1), 1, 2 * k - 1), degreeSequenceSorted[i], IRT_GQ, k);
		count(*this, classesHashesSorted.slice(fmax(0, i - k + 1), 1, 2 * k - 1), classesHashesSorted[i], IRT_GQ, k);
	}
	/*for (int i = 0; i < n; i += k)
	{
		count(*this, degreeSequenceSorted.slice(fmax(0, i - k + 1), 1, 2 * k - 1), degreeSequenceSorted[i], IRT_GQ, k);
		//nvalues(*this, degreeSequenceSorted.slice(i, -1, k), IRT_LQ, 2);
		nvalues(*this, degreeSequenceSorted.slice(i, 1, k), IRT_LQ, 2);
	}*/
	/*int *koef = new int[n]; 
	for (int i = 0; i < n; i++)
	{

		for (int j = 0; j < n; j++)
		{
			if (j < n - i)
				koef[j] = -1;
			else
				koef[j] = 1;
		}
		linear(*this,
			IntArgs(n, koef),
			degreeSequenceSorted,o
	delete[] koef;
	*/
	//nvalues(*this, degreeSequenceSorted, IRT_LQ, n / k);
	//branch(*this, edges, SET_VAR_NONE(), SET_VAL_MIN_EXC());
	//branch(*this, degreeSequenceSorted, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
	//branch(*this, degreeSequenceSorted, tiebreak(INT_VAR_DEGREE_MAX(), INT_VAR_NONE()), INT_VAL_MIN());
	//branch(*this, degreeSequenceSorted, INT_VAR_NONE(), INT_VAL_MIN());
	//branch(*this, degreeSequence, INT_VAR_NONE(), INT_VAL_MIN());
	//branch(*this, edges, SET_VAR_NONE(), SET_VAL_MIN_INC());
	branch(*this, classesHashesSorted, INT_VAR_NONE(), INT_VAL_MIN());
	branch(*this, classesHashes, INT_VAR_NONE(), INT_VAL_MIN());
	branch(*this, classesDegreeSequencesSorted, INT_VAR_NONE(), INT_VAL_MIN());
	branch(*this, classesDegreeSequences, INT_VAR_NONE(), INT_VAL_MIN());
	branch(*this, classes, SET_VAR_NONE(), SET_VAL_MIN_INC());
	branch(*this, AttributesIntLowerBound, INT_VAR_NONE(), INT_VAL_MAX());
	branch(*this, AttributesIntUpperBound, INT_VAR_NONE(), INT_VAL_MIN());
	
}

void AConstraintGraph::debug()
{
	print(std::cout);
}
void AConstraintGraph::printDot(std::ostream & os) const
{
	Matrix<SetVarArray> classesMatrix(classes, classesNumber, n);
	Matrix<IntVarArray> AttributesIntLowerBoundMatrix(AttributesIntLowerBound, ai, n);
	Matrix<IntVarArray> AttributesIntUpperBoundMatrix(AttributesIntUpperBound, ai, n);
	os << "graph { " << std::endl;
	for (std::size_t i = 0; i != names->size(); ++i)
	{
		os << "\t" << (*names)[i] << "[label=<" << (*names)[i] << "<BR/>" << std::endl;
		for (int j = 0; j < ai; j++)
		{
			os << "<FONT POINT-SIZE=\"10\">"<< (*atts_int)[j] <<": ("<< AttributesIntLowerBoundMatrix(j, i) 
				<< " - "  << AttributesIntUpperBoundMatrix(j, i) << ")  <B>"
				<< atts_int_values[j][i] << "</B></FONT><BR/>" << std::endl;
		}
		os << " >];" << std::endl;
	}
	for (int i = 0; i < classesNumber; i++)
	{
		for (int j = 0; j < n; j++)
			for (int k = j + 1; k < n; k++)
				if (classesMatrix(i, j).contains(k))
				{
					os << '\t' << (*names)[j] << " -- " << (*names)[k] << std::endl;
				}
	}
	os << "}" << std::endl;
}
void AConstraintGraph::print(std::ostream& os) const {
	Matrix<SetVarArray> classesMatrix(classes, classesNumber, n);
	Matrix<IntVarArray> degreeSequencesMatrix(classesDegreeSequences, classesNumber, n);
	Matrix<IntVarArray> degreeSequencesSortedMatrix(classesDegreeSequencesSorted, classesNumber, n);
	Matrix<IntVarArray> AttributesIntLowerBoundMatrix(AttributesIntLowerBound, ai, n);
	Matrix<IntVarArray> AttributesIntUpperBoundMatrix(AttributesIntUpperBound, ai, n);
	//os << "test" << std::endl;
	for (int i = 0; i < classesNumber; i++)
	{
		os << "class " << (i + 1)  << ":" << std::endl;
		os << "  Edges" << classesMatrix.col(i) << std::endl;
		os << "  Degree sequence:" << degreeSequencesMatrix.col(i) << std::endl;
		os << "  Degree sequence sorted :" << degreeSequencesSortedMatrix.col(i) << std::endl;
	}
	for (int i = 0; i < ai; i++)
	{
		os << (*atts_int)[i] << std::endl;
		os << "  Lower Bounds:" << AttributesIntLowerBoundMatrix.col(i) << std::endl;
		os << "  Upper Bounds:" << AttributesIntUpperBoundMatrix.col(i) << std::endl;
	}
	os << std::endl << "Edges" << edges << std::endl;
	os <<  "Degree sequence :" << degreeSequence << std::endl;
	os << "Degree sequence sorted :" << degreeSequenceSorted << std::endl;
	os << "Hashes :" << classesHashes << std::endl;
	os << "Hashes Sorted :" << classesHashesSorted << std::endl;
	os << "Cost: " << costValue << std::endl;
}