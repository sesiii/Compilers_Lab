#ifndef EVALEXPR_HPP
#define EVALEXPR_HPP

#include <string>
#include <vector>
#include <iostream>

enum TokenType {
    NUMBER,
    IDENTIFIER,
    OPERATOR,
    END
};

struct Token {
    TokenType type;
    std::string value;

    Token() : type(NUMBER), value("") {}
    Token(TokenType t, const std::string& v = "") : type(t), value(v) {}
};

struct Node {
    TokenType type;
    std::string value;
    Node* left;
    Node* right;

    Node(TokenType t, const std::string& v = "") : type(t), value(v), left(nullptr), right(nullptr) {}
};

class Parser {
private:
    std::vector<Token> tokens;
    size_t index;
    Token currentToken;

public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens), index(0) {
        currentToken = tokens[index];
    }

    void advance() {
        if (index < tokens.size() - 1) {
            currentToken = tokens[++index];
        } else {
            currentToken.type = END;
        }
    }

    Node* parse() {
        return parseExpression();
    }

private:
    Node* parseExpression() {
        Node* node = parseArgument();
        while (currentToken.type == OPERATOR) {
            Node* opNode = parseOperator();
            opNode->left = node;
            opNode->right = parseArgument();
            node = opNode;
        }
        return node;
    }

    Node* parseArgument() {
        if (currentToken.type == NUMBER || currentToken.type == IDENTIFIER) {
            Node* node = new Node(currentToken.type, currentToken.value);
            advance();
            return node;
        } else if (currentToken.type == OPERATOR && currentToken.value == "(") {
            advance();
            Node* node = parseExpression();
            advance(); // consume ')'
            return node;
        }
        return nullptr;
    }

    Node* parseOperator() {
        TokenType opType = OPERATOR;
        std::string opValue = currentToken.value;
        Node* node = new Node(opType, opValue);
        advance();
        return node;
    }
};

#endif