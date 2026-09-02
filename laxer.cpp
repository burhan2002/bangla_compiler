#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;


// Token Types

string INTEGER = "INTEGER";
string IDENTIFIER = "IDENTIFIER";
string INTEGER_LITERAL = "INTEGER_LITERAL";
string ASSIGNMENT = "ASSIGNMENT";
string PRINT = "Ptr";
string PLUS = "PLUS";
string MINUS = "MINUS";
string MULTIPLY = "MULTIPLY";
string DIVIDE = "DIVIDE";
string LEFT_PAREN = "LEFT_PAREN";
string RIGHT_PAREN = "RIGHT_PAREN";
string EOF_TOKEN = "EOF";


// Token Class (Python er Token class er moto)

class Token {
public:
    string type_;
    string value;
    int line;

    Token(string t, string v, int l) {
        type_ = t;
        value = v;
        line = l;
    }

    void print() {
        if (value == "") {
            cout << "< " << type_ << " >" << endl;
        } else {
            cout << "< " << type_ << " , " << value << " >" << endl;
        }
    }
};


// Lexer Class (Python er Lexer class er moto)

class Lexer {
public:
    string code;
    int pos;
    int line;
    vector<Token> tokens;

    Lexer(string src) {
        code = src;
        pos = 0;
        line = 1;
    }

    // Current character dekhe
    char getChar() {
        if (pos < code.length()) {
            return code[pos];
        }
        return '\0';
    }

    // Porer character e jay
    void advance() {
        if (getChar() == '\n') {
            line++;
        }
        pos++;
    }

    // Main function: source code theke token banay
    vector<Token> tokenize() {
        string error = "";

        while (true) {
            char c = getChar();

            // Source code shesh hole break
            if (c == '\0') {
                break;
            }

            // Whitespace skip koro
            else if (c == ' ' || c == '\t' || c == '\n') {
                advance();
            }

            // Single line comment skip koro (#)
            else if (c == '#') {
                while (getChar() != '\0' && getChar() != '\n') {
                    advance();
                }
            }

            // Number hole (e.g., 10, 42)
            else if (isdigit(c)) {
                string num = "";
                while (getChar() != '\0' && isdigit(getChar())) {
                    num += getChar();
                    advance();
                }
                tokens.push_back(Token(INTEGER_LITERAL, num, line));
            }

            // Letter hole (keyword ba identifier)
            else if (isalpha(c) || c == '_') {
                string word = "";
                while (getChar() != '\0' && (isalnum(getChar()) || getChar() == '_')) {
                    word += getChar();
                    advance();
                }

                // Keyword check
                if (word == "integer") {
                    tokens.push_back(Token(INTEGER, "", line));
                }
                else if (word == "ptr") {
                    tokens.push_back(Token(PRINT, "", line));
                }
                else {
                    tokens.push_back(Token(IDENTIFIER, word, line));
                }
            }

            // Operators and delimiters
            else if (c == ':') {
                tokens.push_back(Token(ASSIGNMENT, ":", line));
                advance();
            }
            else if (c == '+') {
                tokens.push_back(Token(PLUS, "+", line));
                advance();
            }
            else if (c == '-') {
                tokens.push_back(Token(MINUS, "-", line));
                advance();
            }
            else if (c == '*') {
                tokens.push_back(Token(MULTIPLY, "*", line));
                advance();
            }
            else if (c == '/') {
                tokens.push_back(Token(DIVIDE, "/", line));
                advance();
            }
            else if (c == '(') {
                tokens.push_back(Token(LEFT_PAREN, "(", line));
                advance();
            }
            else if (c == ')') {
                tokens.push_back(Token(RIGHT_PAREN, ")", line));
                advance();
            }

            // Illegal character (Error)
            else {
                error = "LexerError: Illegal char '" + string(1, c) + "' at line " + to_string(line);
                cout << error << endl;
                break;
            }
        }

        // Sheshe EOF token add koro
        tokens.push_back(Token(EOF_TOKEN, "", line));

        return tokens;
    }
};

// Main Function (Test korar jonno)

int main() {

    string code = "integer a : 10\n"
                  "integer b : a - 5\n"
                  "b : (b * 10)\n"
                  "ptr b\n";

    cout << "=== SOURCE CODE ===" << endl;
    cout << code << endl;

    cout << "=== TOKENS ===" << endl;

    Lexer lexer(code);
    vector<Token> tokens = lexer.tokenize();

    for (int i = 0; i < tokens.size(); i++) {
        tokens[i].print();
    }

    return 0;
}