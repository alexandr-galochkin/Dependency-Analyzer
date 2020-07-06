#pragma once
#include "Wrapper.h"
using namespace std;
using namespace boost::filesystem;

void finder_source_cod(const path & dir_for_search, vector <Wrapper> &source_cod);
bool finder_headers(const path & dir_for_search, path & dir_for_header, string name);
void analyzer(const path & dir_path, vector <path> & dir_for_search, Wrapper &current_wrapper,
	vector <Wrapper> &all_headers);

void print_dependencies(Wrapper &first_wrapper);
void traversal(Wrapper &current_wrapper, vector<Wrapper> &traversed_wrapper, int depth);