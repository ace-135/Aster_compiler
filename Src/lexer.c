#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

const char *keywords[] = {"print", "println", "scan", "int", "double", "char", "string", "for", "while", "if", "else", "do"};
const char operators[] = "+-*/%=!";
const char *multi_operators[] = {"==", "!=", "<=", ">=", "++", "--", "+=", "-=", "*=", "/=", "%="};
const char delimiters[] = "();{}";

int is_keyword(const char *str) {
    int n = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < n; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_operator(const char *str) {
    return strchr(operators, str[0]) != NULL;
}

int is_delimiter(const char c) {
    return strchr(delimiters, c) != NULL;
}

int is_multi_operator(const char *str) {
    int n = sizeof(multi_operators) / sizeof(multi_operators[0]);
    for (int i = 0; i < n; i++) {
        if (strcmp(str, multi_operators[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

Token get_next_token(const char **input, int *line, int *column) {
    Token token = {TOKEN_UNKNOWN, "", *line, *column};
    const char *p = *input;

    while (isspace(*p)) {
        if (*p == '\n') {
            (*line)++;
            *column = 0;
        }
        p++;
        (*column)++;
    }

    if (*p == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        *input = p;
        return token;
    }

    if (*p == '=' || *p == '!' || *p == '<' || *p == '>' || *p == '+' || *p == '-') {

        char next_char[3] = {0};
        next_char[0] = *p;
        next_char[1] = *(p + 1);

        if (is_multi_operator(next_char)) {
            token.value[0] = *p++;
            token.value[1] = *p++;
            token.value[2] = '\0';
            token.type = TOKEN_OPERATOR;
        } 
        else {
            token.value[0] = *p++;
            token.value[1] = '\0';
            token.type = TOKEN_OPERATOR;
        }
    }

    else if (isdigit(*p) || (*p == '.' && isdigit(*(p + 1)))) {
        int i = 0;
        while (isdigit(*p) || *p == '.') {
            token.value[i++] = *p++;
        }
        token.value[i] = '\0';
        token.type = TOKEN_NUMBER;
    }

    else if (isalpha(*p)) {
        int i = 0;
        while (isalnum(*p)) {
            token.value[i++] = *p++;
        }
        token.value[i] = '\0';
        token.type = is_keyword(token.value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
    }

    else if (*p == '"') {
        p++;
        int i = 0;
        while (*p != '"' && *p != '\0') {
            token.value[i++] = *p++;
        }
        if (*p == '"') {
            token.value[i] = '\0';
            token.type = TOKEN_STRING;
            p++;
        } else {
            fprintf(stderr, "Error: Unterminated string literal at line %d, column %d\n", *line, *column);
            exit(1);
        }
    }

    else if (is_operator(p)) {
        token.value[0] = *p++;
        token.value[1] = '\0';
        token.type = TOKEN_OPERATOR;
    }

    else if (is_delimiter(*p)) {
        token.value[0] = *p++;
        token.value[1] = '\0';
        token.type = TOKEN_DELIMITER;
    }
    
    else {
        token.value[0] = *p++;
        token.value[1] = '\0';
        token.type = TOKEN_UNKNOWN;
    }

    *input = p;
    (*column)++;
    return token;
}
