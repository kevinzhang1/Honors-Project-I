#include "translator.h"

/*
low_translator::low_translator(string path, ifstream* file) This constructor allows loops through each line in the Sil and passes each line through the main_interpretor
Inputs: string path, ifstream* file
*/

low_translator::low_translator(string path, ifstream* file) {
	Path = path;
	string line;
	while (getline(*file, line)) { main_interpretor(line);}
	write_racket_file();
}

/*
void low_translator::main_interpretor(string line) This function activates the variable state if a variable is about to be declared or it invokes the swift_variable_interpretor function if the state is already activated
Inputs: string line
*/

void low_translator::main_interpretor(string line) {
	if (v_state == None) {
		if (line.find("alloc_global @") != string::npos) {
			v_state = get_var_name; //alloc_global @ indicates new var
		}
	}
	else if (v_state == get_var_name || v_state == get_var_variable) {
		swift_variable_interpretor(v_state, line); //invoke interpretor if translator in an active state
	}
}

/*
void low_translator::swift_variable_interpretor(var_state state, string line) This function reads and interprets the Sil code based on the var_state
Inputs: var_state state, string line
*/

void low_translator::swift_variable_interpretor(var_state state, string line) {
	if (state == get_var_name) {
		size_t begin = line.find("Example")+7;
		begin += 1;
		while (line[begin] > '0' && line[begin] < '9') begin++;
		size_t lock = line.find_last_of("$");
		char type = line[lock - 6];
		size_t end = lock - 7;
		s_var.name = line.substr(begin, end-begin);
		switch (type) {
		case 'b':
			s_var.type = low_translator::swift_var::datatypes::boolean;
			break;
		case 'I':
			s_var.type = low_translator::swift_var::datatypes::integer;
			break;
		case 'd':
			s_var.type = low_translator::swift_var::datatypes::floats;
			break;
		case 'S':
			s_var.type = low_translator::swift_var::datatypes::String;
			break;
		}
		v_state = get_var_variable;
	}
	else {
		size_t lock;
		string string_int = "";
		string string_float = "";
		switch (s_var.type) {
		case low_translator::swift_var::datatypes::boolean:
			lock = line.find("Int1, ") + 6;
			if (line[lock] == '0') s_var.value = "false";
			else s_var.value = "true";
			break;
		case low_translator::swift_var::datatypes::integer:
			lock = line.find("Int64, ") + 7;
			while (line[lock] != ' ') {
				string_int += line[lock];
				lock++;
			}
			s_var.value = string_int;
			break;
		case low_translator::swift_var::datatypes::floats:
			lock = line.find("// ") + 3;
			while (line[lock] != ' ') {
				string_float += line[lock];
				lock++;
			}
			s_var.value = string_float;
			break;
		case low_translator::swift_var::datatypes::String:
			lock = line.find('"') + 1;
			string String = "";
			while (line[lock] != '"') {
				String += line[lock];
				lock++;
			}
			s_var.value = "\"" + String + "\"";
			break;
		}
		output.push_back(racket_variable_translator(s_var));
		for (int i = 0; i < output.size(); i++) {
			cout << output[i] << endl;
		}
		v_state = None;
	}
}

/*
string low_translator::racket_variable_translator(swift_var var) This function generates a Racket code line based on the variable information and returns it as a string
Inputs: swift_var var
*/

string low_translator::racket_variable_translator(swift_var var) {
	string racket_code = "(define " + s_var.name + " " + s_var.value + ")";
	return racket_code;
}

/*
void low_translator::write_racket_file() This function writes the translated Racket code into a Racket file
Inputs: None
*/

void low_translator::write_racket_file() {
	ofstream fout;
	fout.open(Path + "racket_result.rkt", ios::out);
	fout << "#lang racket" << endl;
	for (int i = 0; i < output.size(); i++) {
		string s = output[i];
		fout << s;
	}
	fout.close();
	system(("notepad " + Path + "racket_result.rkt").c_str());
}

