#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_UNKNOWN,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[256];
} Token;

const char *keywords[] = {"if", "else", "while", "return"};
const char operators[] = "+-*/=!";
const char delimiters[] = "();{}";

int is_keyword(const char *str) {
    int n = sizeof(keywords) / sizeof(keywords[0]);
    for (int i=0; i<n; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

Token get_next_token(const char **input) {
    Token token = {TOKEN_UNKNOWN, ""};
    const char *p = *input;

    while (isspace(*p)) {
        p++;
    }

    if (*p == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        *input = p;
        return token;
    }

    if (isdigit(*p)) {
        int i = 0;
        while (isdigit(*p)) {
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

    else if (strchr(operators, *p)) {
        token.value[0] = *p++;
        token.value[1] = '\0';
        token.type = TOKEN_OPERATOR;
    }

     else if (strchr(delimiters, *p)) {
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
    return token;

}

int main() {

    const char *source_code = "printf('Hello World');";
    const char *input = source_code;

    printf("Source Code : %s\n", source_code);
    printf("Tokens : \n");

    Token token;
    do {
        token = get_next_token(&input);
        printf("Type : %d, Value : %s\n", token.type, token.value);
    }while (token.type != TOKEN_EOF);

    return 0;
}