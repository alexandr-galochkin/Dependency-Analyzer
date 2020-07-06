#pragma once
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
using namespace std;
using namespace boost::filesystem;

class Wrapper{
public:
	Wrapper(const path  & file_path);
	Wrapper(string name);

	path file_path;
	string name;
	bool is_found = false;
	bool is_path_relative;
	vector <Wrapper> dependencies;
	int number_of_inclusions;
	void inc_numb_of_inc();
	void finder_in_file(vector<Wrapper>& existing_wrapper);
};
bool comparator(const Wrapper & a, const Wrapper & b);
