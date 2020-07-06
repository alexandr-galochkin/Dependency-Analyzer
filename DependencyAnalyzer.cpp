#include "Wrapper.h"
#include "Finders.h"

using namespace boost::filesystem;
using namespace std;
using namespace boost::algorithm;

int main(int argc, char* argv)
{
	string in;
	vector <string> vector_split;
	getline(cin, in);
	split(vector_split, in, is_any_of(" "));
	vector <path> dir_for_search;
	path dir_path;
	try {
		for (int i = 0; i < vector_split.size(); i++) {
			if (vector_split[i] == "analyser") {
				dir_path = vector_split[i + 1];
				break;
			}
		}
		for (int i = 0; i < vector_split.size(); i++) {
			if (vector_split[i] == "-I") {
				dir_for_search.push_back(vector_split[i+1]);
				i++;
			}
		}
	}catch (const std::exception&){
		cout << "exception: wrong input." << endl;
	}

	vector <Wrapper> source_cod_wrapper;
	vector <Wrapper> headers_wrapper;

	finder_source_cod(dir_path, source_cod_wrapper);					//find all source cod.
	for (int i = 0; i < source_cod_wrapper.size(); i++) {
		analyzer(source_cod_wrapper[i].file_path.parent_path(),
			dir_for_search, source_cod_wrapper[i], headers_wrapper);	//find dependencies for a single source file.
		print_dependencies(source_cod_wrapper[i]);						//print a tree from them.
	}
	cout << endl;

	sort(headers_wrapper.begin(), headers_wrapper.end(), comparator);
	sort(source_cod_wrapper.begin(), source_cod_wrapper.end(), comparator);

	for (int i = 0; i < headers_wrapper.size(); i++) {
		cout << headers_wrapper[i].name << " " << headers_wrapper[i].number_of_inclusions << endl;
	}
	for (int i = 0; i < source_cod_wrapper.size(); i++) {
		cout << source_cod_wrapper[i].name << " " << source_cod_wrapper[i].number_of_inclusions << endl;
	}
	return 0;
}