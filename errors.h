#include <iostream>
#include <string>
#include <limits.h>

const std::string LLONG_MIN_STR = std::to_string(LLONG_MIN);


void generic(int line_num, std::string line, bool empty_terminal, std::string filename) {
	if(!empty_terminal) std::cout << "\n";
	std::cout << filename << ": Eroare pe linia =>\n";
		
	std::cout << ' ' << line_num << " | " << line.substr(0, line.length() - 1) << '\n';
	std::cout << ' ';
	for(int i{}; i < (std::to_string(line_num)).length(); i++) {
		std::cout << ' ';
	}
	std::cout << " |\n";

	std::cout << ' ';
	for(int i{}; i < (std::to_string(line_num)).length(); i++) {
		std::cout << ' ';
	}
	std::cout << " |\n";
}


namespace errors {
	void indentation_error(int line_num, std::string line, std::string filename) {
		generic(line_num, line, 0, filename);
		std::cout << "Nume de eroare: Identare incorecta (spatiile nu sunt considerate tabs)\n";
	}
	void syntax_error(int line_num, std::string line, std::string filename) {
		generic(line_num, line, 0, filename);
		std::cout << "Nume de eroare: Sintaxa gresita\n";
	}
	void conversion_error(int line_num, std::string line, std::string filename) {
		generic(line_num, line, 0, filename);
		std::cout << "Nume de eroare: Conversie necunoscuta -> Se astepta text / string / numar\n";
	}
	void forgot_keyword(int line_num, std::string line, std::string filename) {
		generic(line_num, line, 0, filename);
		std::cout << "Nume de eroare: Keyword scris gresit / inexistent\n";
	}
	void unsafe_index(int line_num, std::string line, std::string filename) {
		generic(line_num, line, 0, filename);
		std::cout << "Nume de eroare: Index in afara range-ului permis\n";
	}
	void not_defined(int line_num, std::string line, std::string filename) {
		generic(line_num, line, 0, filename);
		std::cout << "Nume de eroare: Variabila nedefinita\n";
	}
	void var_name_error(int line_num, std::string line, std::string filename) {
		generic(line_num, line, 0, filename);
		std::cout << "Nume de eroare: Variabila contine caractere nepermise / format gresit\n";
	}
	void not_finished(int line_num, std::string line, std::string filename) {
		generic(line_num, line, 0, filename);
		std::cout << "Nume de eroare: Nu exista \"pana cand\" -> expresia \"executa\" nu are conditie\n";
	}

}		
