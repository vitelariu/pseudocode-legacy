#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <stack>
#include <chrono>
#include "expressions.h"
#include "errors.h"

auto time1 = std::chrono::high_resolution_clock::now();

std::string filename{};
int current_line{};

std::unordered_map<std::string, long long> vars_int{};
std::unordered_map<std::string, float> vars_float{};
std::unordered_map<std::string, std::string> vars_string{};

std::unordered_map<std::string, long long*> heap_int;
std::unordered_map<std::string, long long> heap_int_size;
long long *pointer_int{};

std::string global_line{};

int is_string(std::string n) {
	if(n[n.length() - 1] == ']') {
		std::string new_n{};
		int i{};
		while(true) {
			if(n[i] != '[') new_n += n[i];
			else break;
			i++;
		}
		n = new_n;
	}

	if(n[0] == '\"' and n[n.length() - 1] == '\"') return 1;
	if(vars_string[n].length() != 0) return 2;
	return 0;
}
 
bool is_var_int(std::string n) {
	return vars_int.count(n);
}

long long get_var_int_value(std::string n) {
	return vars_int[n];
}

std::string remove_quotes(std::string n) {
	std::string new_n{};
	for(int i{1}; i < n.length() - 1; i++) {
		new_n += n[i];
	}
	return new_n;
}





std::string conversion{};
bool is_num_conv = false, is_text_conv = false;



std::string get_ints(std::string n) { // more like get_variables
	if(n.find(":") != std::string::npos) {
		for(int i{}; i < n.find(":"); i++) {
			conversion += n[i];
		}
		conversion = cut(conversion);

		if(conversion == "numar") {
			is_num_conv = true;
		}
		else if(conversion == "text" or conversion == "string") {
			is_text_conv = true;
		}
		else {
			errors::conversion_error(current_line, global_line, filename);
			return LLONG_MIN_STR; 
		}

		std::string aux{};
		for(int i = n.find(":") + 1; i < n.length(); i++) {
			aux += n[i];
		}
		n = cut(aux);
	}

	std::string possible_variable{}, expr{};

	long long count = LLONG_MAX;
	bool in_string = false;

	for(int i{}; i < n.length(); i++) {
		char x = n[i];
		

		if(x == '\"') {
			if(i != 0) {
				if(n[i - 1] != '\\') {
					in_string = !in_string;
					expr += x;
				}
				else {
					expr += x;
				}
			}
			else {
				in_string = !in_string;
				expr += x;
			}

		}
		else if(((x >= 'a' and x <= 'z') or (x >= 'A' and x <= 'Z') or (x == '_') or (x >= '0' and x <= '9' and possible_variable != "")) and !in_string) {
			possible_variable += x;
		}
		else if(x == '[' and !in_string) {
			std::string subexpr{};
			i++;

			int count_br{1};
			while(true) {
				if(n[i] == '[') count_br++;
				else if(n[i] == ']') count_br--;

				if(count_br != 0) subexpr += n[i];
				else break;
	
				i++;
			}
			

			
			subexpr = get_ints(subexpr);
			if(subexpr == LLONG_MIN_STR) return LLONG_MIN_STR;

			count = evaluate(subexpr);

			continue;
		}
		else if(possible_variable.length() != 0 and !in_string) {
			if(possible_variable == "sau" or possible_variable == "si") {
				expr += possible_variable;
				possible_variable = "";
				expr += x;
			}
			else if(is_var_int(possible_variable)) {
				expr += std::to_string(get_var_int_value(possible_variable)) + x;
				possible_variable = "";
			}
			else if(heap_int.count(possible_variable)) {
				if(count == LLONG_MAX) {
					expr += std::to_string(heap_int_size[possible_variable]);
					possible_variable = "";
					expr += x;
				}
				else {
					if(count < 0 or count >= heap_int_size[possible_variable]) {
						errors::unsafe_index(current_line, global_line, filename);
						return LLONG_MIN_STR;
					}

					long long *pointer = heap_int[possible_variable];
					expr += std::to_string(*(pointer + count));
					possible_variable = "";

					expr += x;
					count = LLONG_MAX;
				}
			}
			else if(vars_string.count(possible_variable)) {
				if(count == LLONG_MAX) {
					expr += vars_string[possible_variable];
					expr += x;
					possible_variable = "";
				}
				else {
					std::string aux = vars_string[possible_variable];
					expr += "\"";
					std::string without_quotes = remove_quotes(aux);
					if(count < 0 or count >= without_quotes.length()) {
						errors::unsafe_index(current_line, global_line, filename);
						return LLONG_MIN_STR;
					}
					expr += without_quotes[count];
					expr += "\"";

					expr += x;

					possible_variable = "";
					count = LLONG_MAX;
				}
			}
			else {
				errors::not_defined(current_line, global_line, filename);
				return LLONG_MIN_STR;
			}
		}
		else if(count != LLONG_MAX) {
			if(count < 0) {
				errors::unsafe_index(current_line, global_line, filename);
				return LLONG_MIN_STR;
			}
			int limit_two{}, i = expr.length() - 1;
			std::string last{};
			while(limit_two < 2) {
				if(expr[i] == '\"') {
					if(i == 0) {
						limit_two++;
					}
					else {
						if(n[i - 1] != '\\') {
							limit_two++;
						}
					}
				}

				i--;
			}
			i++;

			for(int k = i; k < expr.length(); k++) {
				last += expr[k];
			}

			std::string new_expr{}; 
			for(int j{}; j < i; j++) {
				new_expr += expr[j];
			}
			expr = new_expr;

			last = remove_quotes(last);
			expr += "\"";
			if(count >= last.length()) {
				errors::unsafe_index(current_line, global_line, filename);
				return LLONG_MIN_STR;
			} 
			expr += last[count];
			expr += "\"";
			count = LLONG_MAX;
		}
		else {
			expr += x;
		}
	}
	if(possible_variable.length() != 0) {
		if(is_var_int(possible_variable)) {
			expr += std::to_string(get_var_int_value(possible_variable));
		}
		else if(heap_int.count(possible_variable)) {
			if(count == LLONG_MAX) {
				expr += std::to_string(heap_int_size[possible_variable]);
			}
			else {	
				if(count < 0 or count >= heap_int_size[possible_variable]) {
					errors::unsafe_index(current_line, global_line, filename);
					return LLONG_MIN_STR;
				}
				long long *pointer = heap_int[possible_variable];
				expr += std::to_string(*(pointer + count));
			}

		}
		else if(vars_string.count(possible_variable)) {
			if(count == LLONG_MAX) {
				expr += vars_string[possible_variable];
			}
			else {
				std::string aux = vars_string[possible_variable];
				expr += "\"";
				std::string without_quotes = remove_quotes(aux);
				if(count < 0 or count >= without_quotes.length()) {
					errors::unsafe_index(current_line, global_line, filename);
					return LLONG_MIN_STR;
				}
				expr += without_quotes[count];
				expr += "\"";
			}
		}
		else {
			errors::not_defined(current_line, global_line, filename);
			return LLONG_MIN_STR;
		}

	}
	else if(count != LLONG_MAX) {
		if(count < 0) {
			errors::unsafe_index(current_line, global_line, filename);
			return LLONG_MIN_STR;
		} 
		int limit_two{}, i = expr.length() - 1;
		std::string last{};
		while(limit_two < 2) {
			if(expr[i] == '\"') {
				if(i == 0) {
					limit_two++;
				}
				else {
					if(n[i - 1] != '\\') {
						limit_two++;
					}
				}
			}

			i--;
		}
		i++;

		for(int k = i; k < expr.length(); k++) {
			last += expr[k];
		}


		std::string new_expr{}; 
		for(int j{}; j < i; j++) {
			new_expr += expr[j];
		}
		expr = new_expr;


		last = remove_quotes(last);
		expr += "\"";
		if(count >= last.length()) {
			errors::unsafe_index(current_line, global_line, filename);
			return LLONG_MIN_STR;
		}
		expr += last[count];
		expr += "\"";
	}


	return expr;
}


void print_str(std::string n) {
	for(long long i{1}; i < n.length() - 1; i++) {
		if(n[i] == '\\' and n[i+1] == 'n') {
			std::cout << '\n';
			i++;
		}
		else if(n[i] == '\\' and n[i+1] == '\"') {
			std::cout << '\"';
			i++;
		}
		else if(n[i] == '\\' and n[i+1] == '\\') {
			std::cout << '\\';
			i++;
		}
		else {
			std::cout << n[i];
		}
	}
}




bool is_for_ev_str(std::string n) {
	bool in_str = false, return_val = false;
	bool is_string = false;

	for(int i{}; i < n.length(); i++) {
		if(n[i] == '\"') {
			is_string = true;
			if(i == 0) {
				in_str = !in_str;
			}
			else {
				if(n[i - 1] != '\\') {
					in_str = !in_str;
				}
			}
			i++;
		}

		if(!in_str) {
			if(n[i] == '+' and is_string) {
				return true;
			}
			else if((n[i] == '=' or n[i] == '>' or n[i] == '<' or n[i] == '!') and is_string) {
				return false;
			}
		}

	}
	
	if(is_string) return true;

	return false;
}

std::string convert(std::string expr) {
	if(is_for_ev_str(expr)) {
		std::string aux{};
		aux = evaluate_string(expr);
		if(is_num_conv) {
			aux = std::to_string((int) aux[1]);
		}
		expr = aux;
	}
	else {
		if(is_text_conv) {
			char aux = evaluate(expr);
			expr = "\"";
			expr += aux;
			expr += "\"";
		}
	}

	return expr;
}


int print_token(std::string token) {
	std::string expr{};
	token = cut(token);


	is_num_conv = false;
	is_text_conv = false;
	conversion = "";

	expr = get_ints(token);
	if(expr == LLONG_MIN_STR) return -1;

	
	expr = convert(expr);

	if(is_for_ev_str(expr)) print_str(evaluate_string(expr));
	else std::cout << evaluate(expr);

	return 0;
}

void read_token(std::string token, int type_read) {
	token = cut(token);
	if(type_read == 0) {
		std::cin >> vars_int[token];
	}
	else if(type_read == 1) {
		std::string x{};
		std::cin >> x;
		vars_string[token] = ('\"' + x + '\"');
	}
}



void put(std::string n, std::string var_name) {
	if(is_string(n) == 1) {
		vars_string[var_name] = n;
	}
	else if(is_string(n) == 2) {
		std::string str{}, N = vars_string[n];
		for(long long i{}; i < N.length(); i++) {
			str += N[i];
		}
		vars_string[var_name] = str;
	}
	else {

		std::string expr{};
		expr = cut(n);
		var_name = cut(var_name);
		vars_int[var_name] = evaluate(expr);
	}
}


void initiation_array(std::string var_name_without_brackets, long long expr, long long size) {

	long long *pointer{};
	pointer = heap_int[var_name_without_brackets];

	for(long long i{}; i < size; i++) {
		*(pointer + i) = expr;
	}
}


void put_elements_array(std::string var_name, std::vector<long long> aux, long long size) {
	
	long long *pointer{};
	pointer = heap_int[var_name];

	for(long long i{}; i < size; i++) {
		*(pointer + i) = aux[i];
	}
}


bool is_char(std::string n) {
	if(n.length() >= 3) return n[0] == '\"' and n[2] == '\"';
	return false;
}

int exist_forbidden_chars(std::string line) {
	bool fs = false;
	for(int i{}; i < line.length(); i++) {
		if(line[i] < '0' or line[i] > '9') {fs = true;}
		else if(line[i] >= '0' and line[i] <= '9' and !fs) {
			errors::var_name_error(current_line, global_line, filename);
			return 1;
		}

		if((line[i] >= 0 and line[i] <= 47) or (line[i] >= 58 and line[i] <= 64) or (line[i] == 92 or line[i] == 94) or line[i] == 96 or (line[i] >= 123 and line[i] <= 127)) {
			errors::var_name_error(current_line, global_line, filename);
			return 1;
		}
	}
	return 0;
}

int assign(std::string line) {
	std::string var_name{};
	for(int i{}; i < line.find("<-"); i++) {
		var_name += line[i];
	}
	var_name = cut(var_name);
	if(exist_forbidden_chars(var_name)) {
		return -1;
	}

	int p = line.find("<-") + 2;
	std::string assigned_expr{};

	std::string var_name_without_brackets{};
	int first_bracket = var_name.find("[");

	if(var_name.find("[") != std::string::npos) {

		for(int j{}; j < first_bracket; j++) {
			var_name_without_brackets += var_name[j];
		}
	}



	if(var_name[var_name.length() - 1] == ']' and heap_int.count(var_name_without_brackets) == 0 and vars_string.count(var_name_without_brackets) == 0) {
		int i = first_bracket;
		//while(true) {
		//	if(var_name[i] == '[') break;
		//	i--;
		//}
		//int first_bracket = i;

		std::string allocation_size{};
		for(i = i + 1; i < var_name.length() - 1; i++) {
			allocation_size += var_name[i];
		}
		allocation_size = cut(allocation_size);
		allocation_size = get_ints(allocation_size);

		if(allocation_size == LLONG_MIN_STR) return -1;
	
		long long size = evaluate(allocation_size);
		pointer_int = (long long*) malloc(sizeof(long long) * size);
		if(pointer_int == nullptr) {
			std::cout << "FAILED ALLOCATION size: " << size << '\n';
		}
		

		heap_int[var_name_without_brackets] = pointer_int;
		heap_int_size[var_name_without_brackets] = size;

		if(line.find(",") == std::string::npos) {
			while(p < line.length() - 1) {
				assigned_expr += line[p];
				p++;
			}
			assigned_expr = cut(assigned_expr);
			assigned_expr = get_ints(assigned_expr);
			if(assigned_expr == LLONG_MIN_STR) return -1;
					
			initiation_array(var_name_without_brackets, evaluate(assigned_expr), size);

		}
		else {
			std::vector<long long> aux{};
			while(p < line.length() - 1) {
				if(line[p] == ',') {
					assigned_expr = cut(assigned_expr);
					assigned_expr = get_ints(assigned_expr);
					if(assigned_expr == LLONG_MIN_STR) return -1; 

					aux.push_back(evaluate(assigned_expr));
					assigned_expr = "";
				}
				else {
					assigned_expr += line[p];
				}

				p++;
			}
			assigned_expr = cut(assigned_expr);
			assigned_expr = get_ints(assigned_expr);
			if(assigned_expr == LLONG_MIN_STR) return -1;

			aux.push_back(evaluate(assigned_expr));
			assigned_expr = "";


			put_elements_array(var_name_without_brackets, aux, aux.size());
		}
		
	}
	else if(var_name[var_name.length() - 1] == ']') {
		std::string index_str{};
		int i = first_bracket;

		for(i = i + 1; i < var_name.length() - 1; i++) {
			index_str += var_name[i];
		}
		index_str = cut(index_str);
		index_str = get_ints(index_str);

		if(index_str == LLONG_MIN_STR) return -1;

		while(p < line.length() - 1) {
			assigned_expr += line[p];
			p++;
		}
		assigned_expr = cut(assigned_expr);
		assigned_expr = get_ints(assigned_expr);
		if(assigned_expr == LLONG_MIN_STR) return -1;


		long long index = evaluate(index_str);

		if(heap_int.count(var_name_without_brackets) != 0) {
			if(index < 0 or index >= heap_int_size[var_name_without_brackets]) {
				errors::unsafe_index(current_line, global_line, filename);
				return -1;
			}
			pointer_int = heap_int[var_name_without_brackets];
			*(pointer_int + index) = evaluate(assigned_expr);
		}
		else {
			if(index < 0 or index >= vars_string[var_name_without_brackets].length() - 1) {
				errors::unsafe_index(current_line, global_line, filename);
				return -1;
			}

			std::string aux = remove_quotes(vars_string[var_name_without_brackets]);
			aux[index] = remove_quotes(assigned_expr)[0];
			std::string auxf = "\"" + aux + "\"";
			vars_string[var_name_without_brackets] = auxf;
		}
	}
	else {
		while(p < line.length() - 1) {
			assigned_expr += line[p];
			p++;
		}
		assigned_expr = cut(assigned_expr);
		
		is_num_conv = false;
		is_text_conv = false;
		conversion = "";

		assigned_expr = get_ints(assigned_expr);
		if(assigned_expr == LLONG_MIN_STR) return -1;
		assigned_expr = convert(assigned_expr);


		if(is_for_ev_str(assigned_expr)) {
			vars_string[var_name] = evaluate_string(assigned_expr);
		}
		else {
			vars_int[var_name] = evaluate(assigned_expr);
		}

		
	}
	return 0;
}




void skip_and_add(std::string &line, int &line_num, int &indent, int &subtotal_lines, std::vector<std::string> &subprogram, std::vector<std::string> program) {
	do {
		int in{};
		line = program[line_num];
		if((line.length() == 1 and line[0] == ';') and program.size() - 1 > line_num) {
			line_num++;
			continue;
		}

		for(char x : line) {
			if(x == '\t') in++;
			else break;
		}

		if(in > indent) {
			subprogram.push_back(line);
			line_num++;
			subtotal_lines++;
		}
		else if(in <= indent) {
			line_num--;
			break;
		}
	}
	while(true);
	subtotal_lines++;
	subprogram.push_back(";");

}

std::string first_word(std::string line, int &c_indent) {
	std::string aux{};

	int i{}, indent{};
	while(line[i] == '\t') {
		indent++;
		i++;
	}

	c_indent = indent;
	

	std::string First_word{};
	while(line[i] != ' ' and line[i] != '<' and i < line.length() - 1) {
		First_word += line[i];
		i++;
	}

	return First_word;
}

bool is_indentation_correct(std::vector<std::string> program, int &line_num, std::string &line_error) {
	int expected_indent{};
	bool first = false;
	int c_indent{};
	for(int index{}; index < program.size(); index++) {
		std::string line = program[index];
		line_num++;
		if(line.length() == 1 and line[0] == ';') {
			if(!first or index < program.size() - 1) continue;
			else {
				return false;
			}
		}
		else if(line.length() >= 2 and line[0] == '/' and line[1] == '/') continue;

		c_indent = 0;
		std::string First_word = first_word(line, c_indent);

		if(c_indent > expected_indent) {
			line_error = line;
			return false;
		}
		else if(c_indent < expected_indent and first) {
			line_error = line;
			return false;
		}
		else if(c_indent == expected_indent and first) {
			line_error = line;
			first = false;
		}
		else {
			expected_indent = c_indent;
		}


		if(First_word == "daca" or First_word == "cat" or First_word == "pentru" or First_word == "executa" or First_word == "altfel") {

			expected_indent++;
			first = true;

		}
	}



	return true;
}




int parse(int total_lines, std::vector<std::string> program) {
	for(int line_num{}; line_num < total_lines; line_num++) {
		std::string line = program[line_num];
		global_line = line;
		


		current_line++;

		if(line.length() == 1) continue;
		if(line.length() >= 3) {
			if(line[0] == '/' and line[1] == '/') continue;
		}
		
		int c_indent{};
		std::string First_word = first_word(line, c_indent);




		if(First_word == "scrie") {




			std::string token{};
			int p = c_indent + 6;
			bool in_str = false;
			while(true) {
				if(line[p] == '\"') {
					if(p == 6) {
						in_str = !in_str;
					}
					else if(line[p - 1] != '\\') {
						in_str = !in_str;
					}
				}

				if(p == line.length() - 1) {
					int x = print_token(token);
					if(x == -1) return -1;
					token = "";
					break;
				}
				else if(line[p] == ',' and !in_str) {
					int x = print_token(token);
					if(x == -1) return -1;
					token = "";
				}
				else {
					token += line[p];
				}
				p++;
			}	
		}
		else if(First_word == "citeste") {
			int type_read{};
			if(line.find("(") != std::string::npos and line[line.length() - 2] == ')') {
				std::string type_str{};
				for(int i = line.find("("); i < line.length(); i++) {
					type_str += line[i];
				}
				if(type_str.find("intregi") != std::string::npos or type_str.find("natural") != std::string::npos) type_read = 0;
				else if(type_str.find("string") != std::string::npos or type_str.find("text") != std::string::npos) type_read = 1;

			}
			std::string token{};
			int p = c_indent + 8;
			while(true) {


				if(line[p] == '(' or p == line.length() - 1) {
					read_token(token, type_read);
					token = "";
					break;
				}
				else if(line[p] == ',') {
					read_token(token, type_read);
					token = "";
				}
				else {
					token += line[p];
				}

				p++;
			}

		}
		else if(line.find("<-") != std::string::npos and First_word != "pentru") {
			int y = assign(line);
			if(y == -1) return -1;

		}
		else if(First_word == "daca") {
			int indent = c_indent;
			int p = indent + 5;

			std::string expr{};

			int y = line.find(" atunci");
			if(y + 7 != line.length() - 1 or y == std::string::npos) {
				errors::forgot_keyword(current_line, global_line, filename);
				return -1;
			}

			while(p < y) {
				expr += line[p];
				p++;
			} 
			expr = cut(expr);
			expr = get_ints(expr);
			if(expr == LLONG_MIN_STR) return -1;


			if(evaluate(expr)) {
				std::vector<std::string> subprogram;
				line_num++;

				int subtotal_lines{};


				skip_and_add(line, line_num, indent, subtotal_lines, subprogram, program);
				line_num++;
				if(parse(subtotal_lines, subprogram) == -1) return -1;
				

				line = program[line_num];
				line_num++;
					
				y = line.find("altfel");
				if(y != std::string::npos) {
					if(y + 6 != line.length() - 1) {
						errors::syntax_error(current_line, line, filename);
					}
				}

				if(y == indent) {
					do {
						line = program[line_num];
						int in = 0; 

						for(char x : line) {
							if(x == '\t') in++;
							else break;
						}

						if(in > indent) {
							line_num++;
						}
						else if(in <= indent) {
							line_num--;
							break;
						}

					}
					while(true);
				}
				else {
					line_num -= 2;
				}

			}
			else {
				line_num++;
				line = program[line_num];

				do {
					int in{};
					line = program[line_num];

					for(char x : line) {
						if(x == '\t') in++;
						else break;
					}

					if(in > indent) {
						line_num++;
						continue;
					}
					else if(in <= indent) {
						if(line.find("altfel") == std::string::npos) line_num--;
						break;
					}
				}
				while(true);
			}

		}
		else if(First_word == "cat") {
			int indent = c_indent;
			std::string substr = line.substr(indent + 4, 4);
			if(substr != "timp") {
				errors::forgot_keyword(current_line, global_line, filename);
				return -1;
			}
			int p = indent + 9;

			std::string expr{};

			int y = line.find(" executa");
			if(y + 8 != line.length() - 1 or y == std::string::npos) {
				errors::forgot_keyword(current_line, global_line, filename);
				return -1;
			}


			while(p < y) {
				expr += line[p];
				p++;
			}
			expr = cut(expr);

			line_num++;
			int subtotal_lines{};

			std::vector<std::string> subprogram{};

			skip_and_add(line, line_num, indent, subtotal_lines, subprogram, program);


						
			std::string original_expr = expr;
			expr = get_ints(original_expr);
			if(expr == LLONG_MIN_STR) return -1;


			while(evaluate(expr)) {
				int copy_current_line = current_line;
				if(parse(subtotal_lines, subprogram) == -1) return -1;
				current_line = copy_current_line;
				expr = get_ints(original_expr);
				if(expr == LLONG_MIN_STR) return -1;
			}
		} 
		else if(First_word == "executa") {
			int indent = c_indent;
			int subtotal_lines{};
			
			line_num++;
			
			std::vector<std::string> subprogram{};
			skip_and_add(line, line_num, indent, subtotal_lines, subprogram, program);
			line_num++;

			line = program[line_num];
			line_num++;

			int indent2{};
			First_word = first_word(line, indent2);
			if(First_word != "pana") {
				errors::not_finished(current_line, global_line, filename);
				return -1;
			}
			else {
				std::string next_token{};
				indent2 += 5;
				while(line[indent2] != ' ' and indent2 < line.length()) {
					next_token += line[indent2];
					indent2++;
				}
				if(next_token != "cand") {
					errors::not_finished(current_line, global_line, filename);
					return -1;
				}
			}
			int p = indent2 + 1;
			std::string expr{};
			while(p < line.size() - 1) {
				expr += line[p];
				p++;
			}
			expr = cut(expr);


			std::string original_expr = expr;
			expr = get_ints(original_expr);
			if(expr == LLONG_MIN_STR) return -1;

			do {
				int copy_current_line = current_line;
				if(parse(subtotal_lines, subprogram) == -1) return -1;
				current_line = copy_current_line;
				expr = get_ints(original_expr);
				if(expr == LLONG_MIN_STR) return -1;
			}
			while(evaluate(expr));

			line_num--;

		}
		else if(First_word == "pentru") {
			int indent = c_indent;
			int p = indent + 7;


			std::string var_name{};
			bool var_stop = false;

			std::string first_expr{};
			int x = line.find(",");
			if(x == std::string::npos) {
				errors::syntax_error(current_line, global_line, filename);
				return -1;
			}
			while(p < x) {
				if(line[p] == '<') {
					if(p <= line.length() - 13) {
						if(line[p + 1] == '-') {
							var_stop = true;
						}
						else {
							errors::syntax_error(current_line, global_line, filename);
							return -1;
						}
					}
					else {
						errors::syntax_error(current_line, global_line, filename);
						return -1;
					}
				}
				if(!var_stop) var_name += line[p];
				first_expr += line[p];
				p++;
			}
			if(!var_stop) {
				errors::syntax_error(current_line, global_line, filename);
				return -1;
			}

			var_name = cut(var_name);
			first_expr = cut(first_expr) + ";";
			if(assign(first_expr) == -1) return -1;

			p++;
			std::string new_str{};
			for(int i = p; i < line.length(); i++) {
				new_str += line[i];
			}
			line = new_str;
			p = 0;
				
			std::string second_expr = "";
			int limit{};
			if(line.find(",") != std::string::npos) {limit = line.find(",");}


			int y = line.find(" executa");
			if(y == std::string::npos) {
				errors::forgot_keyword(current_line, global_line, filename);
				return -1;
			}
			else if(y + 8 != line.length() - 1) {
				errors::forgot_keyword(current_line, global_line, filename);
				return -1;
			}
			if(limit == 0) limit = y;
			

			while(p < limit) {
				second_expr += line[p];
				p++;
			}
			second_expr = cut(second_expr);

			if(second_expr == "") {
				errors::syntax_error(current_line, global_line, filename);
				return -1;
			}
			
			std::string original_line = line;
			std::string third_expr = var_name + "<-" + var_name;
			std::string third_expr_core = "";
			
			if(line.find(",") == limit) {
				p++;
				while(p < line.find(" executa")) {
					third_expr_core += line[p];
					p++;
				}
				third_expr_core = cut(third_expr_core);
				if(third_expr_core == "") {
					errors::syntax_error(current_line, global_line, filename);
					return -1;
				}
			}

			int subtotal_lines{};
			line_num++;
			std::vector<std::string> subprogram{};

			skip_and_add(line, line_num, indent, subtotal_lines, subprogram, program);
			line_num++;

			
			line = original_line;

			std::string second_expr_aux = get_ints(second_expr);
			if(second_expr_aux == "") {

				errors::syntax_error(current_line, global_line, filename);
				return -1;
			}
			if(second_expr_aux == LLONG_MIN_STR) return -1;
			long long for_limit = evaluate(second_expr_aux);

			
			if(line.find(" executa") == limit) {
				for(vars_int[var_name]; vars_int[var_name] != for_limit + 1; vars_int[var_name]++) {
					int copy_current_line = current_line;
					if(parse(subtotal_lines, subprogram) == -1) return -1;
					current_line = copy_current_line;
				}
				line_num--;
			}
			else {
				if(vars_int[var_name] < for_limit) {
					while(vars_int[var_name] <= for_limit) {
						int copy_current_line = current_line;
						if(parse(subtotal_lines, subprogram) == -1) return -1;
						current_line = copy_current_line;
						
						std::string third_expr_core_aux = get_ints(third_expr_core);
						if(third_expr_core_aux == LLONG_MIN_STR) return -1;

						third_expr += third_expr_core_aux + ";";
						if(assign(third_expr) == -1) return -1;
						

						third_expr = var_name + "<-" + var_name;
					}
				}
				else if(vars_int[var_name] > for_limit) {
	
					while(vars_int[var_name] >= for_limit) {
						int copy_current_line = current_line;
						if(parse(subtotal_lines, subprogram) == -1) return -1;
						current_line = copy_current_line;
						
						std::string g_aux = get_ints(third_expr_core);
						if(g_aux == LLONG_MIN_STR) return -1;
						third_expr += g_aux + ";";
						if(assign(third_expr) == -1) return -1;

						third_expr = var_name + "<-" + var_name;
					}
				
				}
				else {
					while(vars_int[var_name] == for_limit) {
						int copy_current_line = current_line;
						if(parse(subtotal_lines, subprogram) == -1) return -1;
						current_line = copy_current_line;

						std::string g_aux = get_ints(third_expr_core);
						if(g_aux == LLONG_MIN_STR) return -1;
						third_expr += g_aux + ";";
						if(assign(third_expr) == -1) return -1;

						third_expr = var_name + "<-" + var_name;
					}
				}
				line_num--;
			}
		}
		else {
			errors::syntax_error(current_line, line, filename);	
			return -1;
		}
	}



	return 0;
}



void calculate_duration() {
	auto time2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1);
	std::cout << "\nProgram terminat in ";
	std::cout << duration.count() << "ms" << " [" << duration.count() / 1000.0 << " s] " << '\n';
}

int main(int argc, char **argv) {
	argc--;
	argv++;

	if(argc == 0) {
		auto start = std::chrono::system_clock::now();
		auto end = std::chrono::system_clock::now();
	 
		std::chrono::duration<double> elapsed_seconds = end-start;
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);
		std::string current_time = std::ctime(&end_time);
		std::string current_time_fixed{};
		for(int i{}; i < current_time.length() - 1; i++) {
			current_time_fixed += current_time[i];
		}

		std::cout << "Pseudocod (" << current_time_fixed << ") [versiunea 1.0.0]\n";
		std::cout << "Interpretor! Ai intrat in modul [EVALUATOR]\n";
		std::cout << "Scrie \"ajutor\" pentru mai multe informatii\n";
		while(true) {
			std::string command{};
			std::cout << ">>> ";
			getline(std::cin, command);
			long long rez = evaluate(command);
			if(rez != LLONG_MAX) {std::cout << rez; std::cout << '\n';}
		}
	}


	std::vector<std::string> program;

	filename = *argv;
	std::ifstream fin(filename);
	std::string line{};
	int total_lines{};

	bool commented = false;
	

	while(getline(fin, line)) {
		int index = line.length() - 1;
		if(index != -1) { 
			while(line[index] == ' ' or line[index] == '\t') {
				index--;
				if(index == -1) break;
			}

			std::string aux{};
			for(int index_aux{}; index_aux <= index; index_aux++) {
				aux += line[index_aux];
			}
			line = aux;
		}
		
		

		line += ';';
		program.push_back(line);
		total_lines++;
		
	}

	program.push_back(";");

	int line_num_error{};
	std::string line_error{};

	if(!is_indentation_correct(program, line_num_error, line_error)) {
		errors::syntax_error(line_num_error, line_error, filename);	
		return -1;
	}

	if(parse(total_lines, program) == -1) {
		calculate_duration();
		return -1;
	}


	fin.close();



	for(auto x : heap_int) {
		free(x.second);
	}


	
	calculate_duration();
	

	return 0;
}
