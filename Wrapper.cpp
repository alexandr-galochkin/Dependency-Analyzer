#include "Wrapper.h"

/*this class containing information about the file:
*name, path, number of inclusions in other files, dependencies.
*/
Wrapper::Wrapper(const path & file_path) {			
	this->file_path = file_path;			
	name = file_path.leaf().string();
	is_found = true;
	this->number_of_inclusions = 0;
};

Wrapper::Wrapper(string name) {
	vector <Wrapper> dependences;
	this->name = name;
	this->number_of_inclusions = 1;
};

void Wrapper::inc_numb_of_inc() {
	number_of_inclusions++;
};

/**/
void Wrapper::finder_in_file(vector <Wrapper> & existing_wrapper) {		//this function searches for header files
	std::ifstream file;													//and adds them to dependencies. 
	file.open(file_path.string());
	boost::iterator_range<std::string::iterator> r;
	if (!file.is_open()) {
		return;
	}
	string s;
	vector <string> vector_split;
	while (getline(file, s)) {
		if ((s.size() > 0) && (s[0] == '/')) {
			continue;
		}
		if (boost::algorithm::find_first(s, "#include")) {
			boost::algorithm::split(vector_split, s, boost::algorithm::is_any_of(" "));
			string new_wrapper_name = vector_split[1];
			new_wrapper_name.erase(new_wrapper_name.size()-1, 1);
			new_wrapper_name.erase(0, 1);
			bool flag = false;
			for (int i = 0; i < existing_wrapper.size(); i++) {
				if (existing_wrapper[i].name == new_wrapper_name) {
					existing_wrapper[i].inc_numb_of_inc();
					flag = true;
					this->dependencies.push_back(existing_wrapper[i]);
					break;
				}
			}
			if (!flag) {
				Wrapper *new_wrapper = new Wrapper(new_wrapper_name);
				if (vector_split[1][0] == '\"') {
					new_wrapper->is_path_relative = true;
				}					else {
					new_wrapper->is_path_relative = false;
				}
				existing_wrapper.push_back(*new_wrapper);
				this->dependencies.push_back(*new_wrapper);
			}
		}
	}
	file.close();
}

bool comparator(const Wrapper & a, const Wrapper & b) {			//this comparator is needed to sort vector of Wrapper.
	if (a.number_of_inclusions > b.number_of_inclusions) {
		return true;
	}
	else if (a.number_of_inclusions < b.number_of_inclusions) {
		return false;
	}
	return (a.name <= b.name);
}
