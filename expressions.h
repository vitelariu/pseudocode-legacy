#include <iostream>
#include <stack>
#include <limits.h>
#include <cmath>
#include <vector>

#define OR 0
#define AND 1
#define EGSD 2
#define ADD_SUBSTRACT 3
#define MULTIPLY_DIVIDE_MODULO 4
#define POWER 5

bool check_paranth(std::string n) {
	if(n[n.length() - 1] != ')' and (n[n.length() - 1] < '0' or n[n.length() - 1] > '9')) {
		return 0;
	}
	int paranth{};

	for(int i{}; i < n.length(); i++) {
		if(n[i] == '(') {
			paranth++;
		}
		else if(n[i] == ')') {
			paranth--;
			if(i == 0) return 0;
			if(n[i-1] == '(') return 0;
		}

		if(paranth < 0) return 0;
	}

	return paranth == 0;
} 



std::string cut(std::string n) {
	bool first = false;
	std::string new_str{};
	for(char x : n) {
		if(x != ' ' and x != '\t') {
			first = true;
		}

		if(first) {
			new_str += x;
		}
	}

	if(!first) return "";

	int point{};
	for(int i = new_str.length() - 1; i >= 0; i--) {
		if(new_str[i] != ' ' and new_str[i] != '\t') {
			point = i;
			break;
		}
	}
	
	std::string full{};
	for(int i{}; i <= point; i++) {
		full += new_str[i];
	}

	return full;
}



std::string get_str(std::string n, int i) {
	int paranth{1};
	std::string new_str{};
	
	i++;
	do {
		if(n[i] == '(') {
			paranth++;
		}
		else if(n[i] == ')') {
			paranth--;
		}

		if(paranth != 0) new_str += n[i];

		i++;
	}
	while(paranth != 0);

	return new_str;
}


long long evaluate(std::string n) {
	if(n.length() == 0) return LLONG_MAX;

	std::stack<long long> S;
	std::stack<int> Sop;
	std::stack<std::string> Sop_char;
	int i{};
	
	bool negative = false, negate = false;
	if(n[i] == '!') {
		i++;
		negate = true;
	}

	if(n[i] == '+') {
		i++;
	}
	else if(n[i] == '-') {
		i++;
		negative = true;
	}	


	long long t{};
	while(n[i] != 's' and n[i] != '=' and n[i] != '!' and n[i] != '>' and n[i] != '<' and n[i] != '+' and n[i] != '-' and n[i] != '*' and n[i] != '/' and n[i] != '^' and n[i] != '%' and i < n.length()) {
		if(n[i] == '(') {
			std::string sub_string = get_str(n, i);
			t = evaluate(sub_string);
			i += sub_string.length() + 2;
			break;
		}
		if(n[i] == '\"') {
			char sub_char = n[++i];
			t = (long long) sub_char;
			i += 2;
			break;
		}
		if(n[i] == ' ') {
			i++;
			break;
		}
		t = t * 10 + ((int) n[i] - '0');
		i++;
	}
	if(negative) t = t * (-1);
	if(negate) t = !t;
	S.push(t);
	
	while(i < n.length() - 1) {
		while(n[i] == ' ') {
			i++;
		}
		
		std::string current_op{};
		int current_op_num{};
		if(i < n.length() - 3) {
			if(n[i] == 's' and n[i+1] == 'a' and n[i+2] == 'u') {
				current_op += "or";
				current_op_num = OR;
				i += 2;
			}
		}
		if(i < n.length() - 2 and current_op.length() == 0) {
			if(n[i] == 's' and n[i+1] == 'i') {
				current_op += "and";
				current_op_num = AND;
				i++;
			}
		}
		if(current_op.length() == 0) {
			if(n[i] == '!') {
				current_op += "!=";
				current_op_num = EGSD;
				i++;
			}
			else if(n[i] == '=') {
				current_op += "=";
				current_op_num = EGSD;
			}
			else if(n[i] == '>' or n[i] == '<') {
				current_op_num = EGSD;
				if(n[i] == '>') current_op += '>';
				else current_op += '<';
				if(i + 1 < n.length() - 1) {
					if(n[i + 1] == '=') current_op += '=';
					i++;
				}
			}
			else{
				current_op += n[i];
				if(current_op == "+" or current_op == "-") {current_op_num = ADD_SUBSTRACT;}
				else if(current_op == "*" or current_op == "/" or current_op == "%") {current_op_num = MULTIPLY_DIVIDE_MODULO;}
				else if(current_op == "^") {current_op_num = POWER;}
			}
		}

		i++;

		while(n[i] == ' ') {
			i++;
		}
		
		negative = false, negate = false;
		if(n[i] == '!') {
			i++;
			negate = true;
		}

		if(n[i] == '-') {
			i++;
			negative = true;
		}
		else if(n[i] == '+') {
			i++;
		}
		
		t = 0;
		while(n[i] != 's' and n[i] != '=' and n[i] != '!' and n[i] != '>' and n[i] != '<' and n[i] != '+' and n[i] != '-' and n[i] != '*' and n[i] != '/' and n[i] != '%' and n[i] != '^' and i < n.length()) {
			if(n[i] == '(') {
				std::string sub_string = get_str(n, i);
				t = evaluate(sub_string);
				i += sub_string.length() + 2;
				break;
			}
			if(n[i] == '\"') {
				char sub_char = n[++i];
				t = (long long) sub_char;
				i += 2;
				break;
			}
			if(n[i] == ' ') {
				i++;
				continue;
			}
			t = t * 10 + (int) (n[i] - '0');
			i++;
		}
		if(negative) t = t * (-1);
		if(negate) t = !t;


		if(current_op == "^") {
			long long a = S.top();
			S.pop();
			S.push(pow(a, t));
		}
		else if(S.size() >= 2 and current_op_num <= Sop.top()) {
			long long b = S.top();
			S.pop();
			long long a = S.top();
			S.pop();

			if(Sop_char.top() == "+") S.push(a + b);
			else if(Sop_char.top() == "-") S.push(a - b);
			else if(Sop_char.top() == "*") S.push(a * b);
			else if(Sop_char.top() == "/") S.push(a / b);
			else if(Sop_char.top() == "%") S.push(a % b);
			else if(Sop_char.top() == "=") S.push(a == b);
			else if(Sop_char.top() == "!=") S.push(a != b);
			else if(Sop_char.top() == ">") S.push(a > b);
			else if(Sop_char.top() == "<") S.push(a < b);
			else if(Sop_char.top() == ">=") S.push(a >= b);
			else if(Sop_char.top() == "<=") S.push(a <= b);
			else if(Sop_char.top() == "and") S.push(a && b);
			else if(Sop_char.top() == "or") S.push(a || b);
			

			Sop.push(current_op_num);
			Sop_char.push(current_op);
			S.push(t);
		}
		else if(S.size() >= 2 and current_op_num > Sop.top()) {
			long long a = S.top();
			S.pop();
			if(current_op == "=") S.push(a == t);
			else if(current_op == "!=") S.push(a != t);
			else if(current_op == ">") S.push(a > t);
			else if(current_op == "<") S.push(a < t);
			else if(current_op == ">=") S.push(a >= t);
			else if(current_op == "<=") S.push(a <= t);
			else if(current_op == "and") S.push(a && t);
			else if(current_op == "or") S.push(a || t);
			else if(current_op == "+") S.push(a + t);
			else if(current_op == "-") S.push(a - t);
			else if(current_op == "*") S.push(a * t);
			else if(current_op == "/") S.push(a / t);
			else if(current_op == "%") S.push(a % t);
		}
		else {
			Sop.push(current_op_num);
			Sop_char.push(current_op);
			S.push(t);
		}
		

	}
	if(S.size() == 2) {
		long long b = S.top();
		S.pop();
		long long a = S.top();
		S.pop();
		if(Sop_char.top() == "=") S.push(a == b);
		else if(Sop_char.top() == "!=") S.push(a != b);
		else if(Sop_char.top() == ">") S.push(a > b);
		else if(Sop_char.top() == "<") S.push(a < b);
		else if(Sop_char.top() == ">=") S.push(a >= b);
		else if(Sop_char.top() == "<=") S.push(a <= b);
		else if(Sop_char.top() == "and") S.push(a && b);
		else if(Sop_char.top() == "or") S.push(a || b);
		else if(Sop_char.top() == "+") S.push(a + b);
		else if(Sop_char.top() == "-") S.push(a - b);
		else if(Sop_char.top() == "*") S.push(a * b);
		else if(Sop_char.top() == "/") S.push(a / b);
		else if(Sop_char.top() == "%") S.push(a % b);
	}


	return S.top();
}




std::string evaluate_string(std::string n) {
	bool in_string = false;
	bool con = false;
	std::string final_expr{};

	int i_bak{};

	int limit_two = 0;
	for(int i{}; i < n.length(); i++) {
		if(n[i] == '\"' and limit_two == 0) limit_two++;
		else if(n[i] == '\"' and n[i - 1] != '\\') limit_two++;
		if(limit_two == 2) {
			i_bak = i + 1;
			break;
		}
		final_expr += n[i];
	}



	bool c = false;

	for(int i = i_bak; i < n.length(); i++) {
		if(n[i] == ' ' and !in_string) continue;
		if(n[i] == '+') {
			c = true;
			continue;
		}

		if(n[i] == '\"' and n[i - 1] != '\\') {
			in_string = !in_string;
			i++;
		}

		if(in_string and c) {
			final_expr += n[i];
		}
		else {
			c = false;
		}
	}

	return final_expr + '\"';
}
