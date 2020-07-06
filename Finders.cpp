#include "Finders.h"

/*this finder searches for source files in directory dir_for_search
*and adds them to the vector.
*/
void finder_source_cod(const path & dir_for_search, vector <Wrapper> &source_cod) {
	if (!exists(dir_for_search)) return;
	directory_iterator end_itr;
	for (directory_iterator itr(dir_for_search); itr != end_itr; ++itr)
	{
		if (is_directory(itr->status())) // если файл - это папка
		{
			finder_source_cod(itr->path(), source_cod);
		}
		else if (extension(itr->path()) == ".cpp") // нашли исходник
		{
			Wrapper *new_source_cod = new Wrapper(itr->path());
			source_cod.push_back(*new_source_cod);
		}
	}
};

/*this finder searches header with the desired name 
*using the current path dir_for_search
*and add path to it to dir_for_header.
*/

bool finder_headers(const path & dir_for_search, path & dir_for_header, string name) {
	if (!exists(dir_for_search)) return false;
	directory_iterator end_itr;
	for (directory_iterator itr(dir_for_search); itr != end_itr; ++itr)
	{
		if (is_directory(itr->status()))
		{
			if (finder_headers(itr->path(), dir_for_header, name)) return true;
		}
		else if ((itr->path().leaf() == name) ||
			((itr->path().extension() == ".h")) && (itr->path().stem() == name)){
			dir_for_header = itr->path();
			return true;
		}
	}
	return false;
};

/*this function recurrently searches 
*all headers from a single file in current dir_path and dir_for_search.
*If header is found, this function called for the header.
*Vector all_headers stores found header to avoid cycles.
*/
void analyzer(const path & dir_path, vector <path> & dir_for_search, Wrapper &current_wrapper,
	vector <Wrapper> &all_headers)
{
	current_wrapper.finder_in_file(all_headers);
	for (int i = 0; i < current_wrapper.dependencies.size(); i++) {
		path path_for_header;
		bool is_find = false;
		if (current_wrapper.dependencies[i].is_path_relative) {
			if (!is_directory(dir_path)) {
				break;
			}
			is_find = finder_headers(dir_path, path_for_header, current_wrapper.dependencies[i].name);
		}
		else {
			for (int j = 0; j < dir_for_search.size(); j++) {
				if (!is_directory(dir_for_search[j])) {
					break;
				}
				is_find = finder_headers(dir_for_search[j], path_for_header, current_wrapper.dependencies[i].name);
				if (is_find) {
					break;
				}
			}
		}
		if (is_find) {
			current_wrapper.dependencies[i].file_path = path_for_header;
			current_wrapper.dependencies[i].is_found = true;
			analyzer(path_for_header.parent_path(), dir_for_search, current_wrapper.dependencies[i], all_headers);
		}
	}
	if (!exists(dir_path)) return;
};

/*this function calls traversal to recurrently find all dependencies and print them.
*/
void print_dependencies(Wrapper &first_wrapper) {
	vector <Wrapper>  traversed_wrapper;
	traversal(first_wrapper, traversed_wrapper, -1);
}

void traversal(Wrapper &current_wrapper, vector<Wrapper> &traversed_wrapper, int depth) {
	depth++;
	for (int i = 0; i < depth; i++) {
		cout << "..";
	}
	cout<< current_wrapper.name;
	if (!current_wrapper.is_found) {
		cout << "(!)" << endl;
	}
	else {
		cout << endl;
	}
	for (int i = 0; i < traversed_wrapper.size(); i++) {
		if (traversed_wrapper[i].name == current_wrapper.name) {
			return;
		}
	}
	traversed_wrapper.push_back(current_wrapper);
	for (int i = 0; i < current_wrapper.dependencies.size(); i++) {
		traversal(current_wrapper.dependencies[i], traversed_wrapper, depth);
	}
	traversed_wrapper.pop_back();
	depth--;
}

