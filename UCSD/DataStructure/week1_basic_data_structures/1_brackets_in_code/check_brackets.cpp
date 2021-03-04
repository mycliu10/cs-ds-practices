#include <iostream>
#include <stack>
#include <string>

struct Bracket {
    Bracket(char type, int position):
        type(type),
        position(position)
    {}

    bool Matchc(char c) {
        if (type == '[' && c == ']')
            return true;
        if (type == '{' && c == '}')
            return true;
        if (type == '(' && c == ')')
            return true;
        return false;
    }

    char type;
    int position;
};

int main() {
    std::string text;
    getline(std::cin, text);

    std::stack <Bracket> opening_brackets_stack;
    bool is_success = true;
    int error_position = 0;
    for (int position = 0; position < text.length(); ++position) {
        char next = text[position];

        if (next == '(' || next == '[' || next == '{') {
            // Process opening bracket, write your code here
            opening_brackets_stack.push(Bracket(next,position));
        }

        if (next == ')' || next == ']' || next == '}') {
            // Process closing bracket, write your code here
            if(opening_brackets_stack.empty()) {
                is_success = false;
                error_position = position + 1;
                break;
            }

            if(opening_brackets_stack.top().Matchc(next)) {
                opening_brackets_stack.pop();
            }
            else {
                is_success = false;
                error_position = position + 1;
                break;
            }
        }
    }

    if(is_success) {
        if(not opening_brackets_stack.empty()) {
            is_success = false;
            error_position = opening_brackets_stack.top().position + 1;
        }
    }

    // Printing answer, write your code here
    if(is_success) {
        std::cout << "Success" << std::endl;
    }
    else {
        std::cout << error_position << std::endl;
    }

    return 0;
}
