#include "SampleLoader.h"




SampleLoader::SampleLoader()
{
	this->setClassesNumber(1);
}

void SampleLoader::load_names()
{
	this->push_vertex("Ivan");
	this->push_vertex("Fedor");
	this->push_vertex("Sam");
	this->push_vertex("Ho");
	this->push_vertex("Jane");
	this->push_vertex("John");
	this->push_vertex("Peter");
	this->push_vertex("Petra");
	this->push_vertex("Kim");
}


void SampleLoader::load_edges()
{
	this->push_edge("Ivan", "Peter", 0);
	this->push_edge("Fedor", "Sam", 0);
	this->push_edge("Fedor", "Ho", 0);
	this->push_edge("Fedor", "Jane", 0);
	this->push_edge("John", "Peter", 0);
	this->push_edge("Peter", "Sam", 0);
	this->push_edge("Peter", "Petra", 0);
	this->push_edge("Kim", "Ho", 0);
}

void SampleLoader::load_attributes()
{
	this->push_attribute_int("age");
	this->push_attribute_int("sex");
	std::vector<std::string>  origin = { "England", "Germany", "China", "Korea", "Nigeria", "USA", "Canada", "Peru" };
	std::vector<std::string>  europe = { "England", "Germany"};
	std::vector<std::string>  asia =   { "China", "Korea"};
	std::vector<std::string>  north_america = { "USA", "Canada"};
	std::vector<std::string>  eurasia = { "England", "Germany", "China", "Korea"};
	std::vector<std::string>  east = { "England", "Germany", "China", "Korea",  "Nigeria"};
	std::vector<std::string>  west = { "USA", "Canada", "Peru"};
	std::vector<std::string>  south = { "Nigeria", "Peru" };
	std::vector<std::string>  north = { "England", "Germany", "China", "Korea", "USA", "Canada"};
	//std::vector<std::string> origin =;
	//                                        *
	//                             /                        \
	//                        /                             \
	//                     East                              \ 
	//                 /            \                          \ 
	//           Eurasia             \         South          West
	//          /         \             \      /  \         /       \
	//      Europe           Asia         \  /     \     /    North America 
	//    /       \         /    \         |            /      /   \         
	//  England Gemany   China    Korea  Nigeria     Peru   USA    Canada     

	
	this->push_attribute_category("Origin", origin);
	this->push_generalization("Origin", "Europe", europe);
	this->push_generalization("Origin", "Asia", asia);
	this->push_generalization("Origin", "North America", north_america);
	this->push_generalization("Origin", "Eurasia", eurasia);
	this->push_generalization("Origin", "East", east);
	this->push_generalization("Origin", "West", west);
	this->push_generalization("Origin", "South", south);
	this->push_generalization("Origin", "North", north);

}

void SampleLoader::load_attributes_values()
{
	this->push_attribute_value("Ivan",  "age", 22);  
	this->push_attribute_value("Fedor", "age", 25);
	this->push_attribute_value("Sam",   "age", 35);
	this->push_attribute_value("Ho",    "age", 25);
	this->push_attribute_value("Jane",  "age", 43);
	this->push_attribute_value("John",  "age", 15);
	this->push_attribute_value("Peter", "age", 28);
	this->push_attribute_value("Petra", "age", 21);
	this->push_attribute_value("Kim",   "age", 40);

	this->push_attribute_value("Ivan",  "sex", 0);  
	this->push_attribute_value("Fedor", "sex", 0);
	this->push_attribute_value("Sam",   "sex", 1);
	this->push_attribute_value("Ho",    "sex", 1);
	this->push_attribute_value("Jane",  "sex", 1);
	this->push_attribute_value("John",  "sex", 0);
	this->push_attribute_value("Peter", "sex", 0);
	this->push_attribute_value("Petra", "sex", 1);
	this->push_attribute_value("Kim",   "sex", 0);
}
