#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
    TOKEN_KEYWORD,      // 0
    TOKEN_IDENTIFIER,   // 1
    TOKEN_NUMBER,       // 2
    TOKEN_OPERATOR,     // 3
    TOKEN_DELIMITER,    // 4
    TOKEN_STRING,       // 5
    TOKEN_UNKNOWN,      // 6
    TOKEN_EOF           // 7
} TokenType;

typedef struct {
    TokenType type;
    char value[256];
    int line;
    int column;
} Token;

const char *keywords[] = {"print", "println", "scan", "int", "double", "char", "string", "for", "while", "if", "else", "do"};
const char operators[] = "+-*/%=!";
const char *multi_operators[] = {"==", "!=", "<=", ">=", "++", "--", "+=", "-=", "*=", "/=", "%="};
const char delimiters[] = "();{}";

// Helper functions
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

// Function to check if a string is a multi-character operator
int is_multi_operator(const char *str) {
    int n = sizeof(multi_operators) / sizeof(multi_operators[0]);
    for (int i = 0; i < n; i++) {
        if (strcmp(str, multi_operators[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

Token get_next_token(const char *input[], int *line, int *column) {
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

    // Handle multi-character operators first
    if (*p == '=' || *p == '!' || *p == '<' || *p == '>' || *p == '+' || *p == '-') {
        // Try to check for multi-character operators
        char next_char[3] = {0}; // For storing two characters
        next_char[0] = *p;
        next_char[1] = *(p + 1);

        // If we have a valid multi-character operator, consume it
        if (is_multi_operator(next_char)) {
            token.value[0] = *p++;
            token.value[1] = *p++;
            token.value[2] = '\0';
            token.type = TOKEN_OPERATOR;
        } else {
            token.value[0] = *p++;
            token.value[1] = '\0';
            token.type = TOKEN_OPERATOR;
        }
    }
    // Handle numbers (integers and floating-point)
    else if (isdigit(*p) || (*p == '.' && isdigit(*(p + 1)))) {
        int i = 0;
        while (isdigit(*p) || *p == '.') {
            token.value[i++] = *p++;
        }
        token.value[i] = '\0';
        token.type = TOKEN_NUMBER;
    }
    // Handle identifiers and keywords
    else if (isalpha(*p)) {
        int i = 0;
        while (isalnum(*p)) {
            token.value[i++] = *p++;
        }
        token.value[i] = '\0';
        token.type = is_keyword(token.value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
    }
    // Handle string literals
    else if (*p == '"') {
        p++;  // Skip the opening quote
        int i = 0;
        while (*p != '"' && *p != '\0') {
            token.value[i++] = *p++;
        }
        if (*p == '"') {
            token.value[i] = '\0';
            token.type = TOKEN_STRING;
            p++;  // Skip the closing quote
        } else {
            fprintf(stderr, "Error: Unterminated string literal at line %d, column %d\n", *line, *column);
            exit(1);  // Exit on error
        }
    }
    // Handle single-character operators and delimiters
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
    // Handle unknown characters
    else {
        token.value[0] = *p++;
        token.value[1] = '\0';
        token.type = TOKEN_UNKNOWN;
    }

    *input = p;
    (*column)++;
    return token;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source_code_file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char source_code[file_size + 1];
    fread(source_code, 1, file_size, file);
    source_code[file_size] = '\0';
    fclose(file);

    int line = 1, column = 1;
    const char *input = source_code;

    printf("Tokens:\n");

    Token token;
    do {
        token = get_next_token(&input, &line, &column);
        printf("Type: %d, Value: %s, Line: %d, Column: %d\n", token.type, token.value, token.line, token.column);
    } while (token.type != TOKEN_EOF);

    return 0;
}
