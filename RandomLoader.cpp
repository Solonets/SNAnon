#include "RandomLoader.h"
#include <cstdlib>

RandomLoader::RandomLoader(int vn, int maxDegree, int seed)
{
	this->vn = vn;
	this->maxDegree = maxDegree;
	this->seed = seed;
}


RandomLoader::~RandomLoader()
{
}
void RandomLoader::load_names()
{
	setVertexNumber(vn);
}

void RandomLoader::load_edges()
{
	std::srand(seed); 
	for (int i = 0; i < vn; i++)
	{
		int d = std::rand() % (maxDegree + 1);
		for (int j = 0; j < d; j++)
		{
			int out = std::rand() % vn;
			push_edge(i, out, 0);
		}
	}
}
