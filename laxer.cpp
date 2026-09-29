#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

enum class TokenType {
    Keyword, IntType, FloatType, Identifier, Number, Decimal,
    Assign, Plus, Minus, Star, Slash,
    Semicolon, LParen, RParen, LBrace, RBrace,
    Less, Greater, LessEqual, GreaterEqual, Equal, NotEqual,
    End, Invalid
};

struct Token {
    TokenType type;
    string text;
    int line;
};

class Lexer {
    string source;
    size_t pos = 0;
    int line = 1;

    bool atEnd() const { return pos >= source.size(); }
    
    char peek(size_t offset = 0) const {
        return pos + offset < source.size() ? source[pos + offset] : '\0';
    }
    
    Token make(TokenType t, const string& s, int ln) {
        return Token{t, s, ln};
    }

public:
    explicit Lexer(string s) : source(std::move(s)) {}

    vector<Token> tokenize() {
        vector<Token> out;
        while (!atEnd()) {
            char c = peek();
            if (c == ' ' || c == '\t' || c == '\r') { ++pos; continue; }
            if (c == '\n') { ++line; ++pos; continue; }

            int tokenLine = line;

            // Single-line comments: // ... or # ...
            if (c == '#' || (c == '/' && peek(1) == '/')) {
                if (c == '#') {
                    while (!atEnd() && peek() != '\n') ++pos;
                } else {
                    pos += 2;
                    while (!atEnd() && peek() != '\n') ++pos;
                }
                continue;
            }

            if (isdigit(static_cast<unsigned char>(c))) {
                string number;
                while (isdigit(static_cast<unsigned char>(peek()))) {
                    number += peek(); ++pos;
                }
                if (peek() == '.' && isdigit(static_cast<unsigned char>(peek(1)))) {
                    number += peek(); ++pos;
                    while (isdigit(static_cast<unsigned char>(peek()))) {
                        number += peek(); ++pos;
                    }
                    out.push_back(make(TokenType::Decimal, number, tokenLine));
                } else {
                    out.push_back(make(TokenType::Number, number, tokenLine));
                }
                continue;
            }

            // ASCII identifiers and UTF-8 Bangla words. Non-ASCII bytes are
            // consumed as part of a word; punctuation remains ASCII.
            if (isalpha(static_cast<unsigned char>(c)) || c == '_' ||
                static_cast<unsigned char>(c) >= 128) {
                string word;
                while (!atEnd()) {
                    unsigned char ch = static_cast<unsigned char>(peek());
                    if (isalnum(ch) || ch == '_' || ch >= 128) {
                        word += peek(); ++pos;
                    } else break;
                }
                if (word == "ধরি") out.push_back(make(TokenType::Keyword, word, tokenLine));
                else if (word == "যদি" || word == "নাহলে" || word == "যতক্ষণ" ||
                         word == "দেখাও")
                    out.push_back(make(TokenType::Keyword, word, tokenLine));
                else if (word == "সংখ্যা") out.push_back(make(TokenType::IntType, word, tokenLine));
                else if (word == "দশমিক") out.push_back(make(TokenType::FloatType, word, tokenLine));
                else out.push_back(make(TokenType::Identifier, word, tokenLine));
                continue;
            }

            switch (c) {
                case '=':
                    if (peek(1) == '=') { out.push_back(make(TokenType::Equal, "==", tokenLine)); pos += 2; }
                    else { out.push_back(make(TokenType::Assign, "=", tokenLine)); ++pos; }
                    break;
                case '!':
                    if (peek(1) == '=') { out.push_back(make(TokenType::NotEqual, "!=", tokenLine)); pos += 2; }
                    else { out.push_back(make(TokenType::Invalid, "!", tokenLine)); ++pos; }
                    break;
                case '<':
                    if (peek(1) == '=') { out.push_back(make(TokenType::LessEqual, "<=", tokenLine)); pos += 2; }
                    else { out.push_back(make(TokenType::Less, "<", tokenLine)); ++pos; }
                    break;
                case '>':
                    if (peek(1) == '=') { out.push_back(make(TokenType::GreaterEqual, ">=", tokenLine)); pos += 2; }
                    else { out.push_back(make(TokenType::Greater, ">", tokenLine)); ++pos; }
                    break;
                case '+': out.push_back(make(TokenType::Plus, "+", tokenLine)); ++pos; break;
                case '-': out.push_back(make(TokenType::Minus, "-", tokenLine)); ++pos; break;
                case '*': out.push_back(make(TokenType::Star, "*", tokenLine)); ++pos; break;
                case '/': out.push_back(make(TokenType::Slash, "/", tokenLine)); ++pos; break;
                case ';': out.push_back(make(TokenType::Semicolon, ";", tokenLine)); ++pos; break;
                case '(': out.push_back(make(TokenType::LParen, "(", tokenLine)); ++pos; break;
                case ')': out.push_back(make(TokenType::RParen, ")", tokenLine)); ++pos; break;
                case '{': out.push_back(make(TokenType::LBrace, "{", tokenLine)); ++pos; break;
                case '}': out.push_back(make(TokenType::RBrace, "}", tokenLine)); ++pos; break;
                default: out.push_back(make(TokenType::Invalid, string(1,c), tokenLine)); ++pos; break;
            }
        }
        out.push_back(make(TokenType::End, "", line));
        return out;
    }
};
