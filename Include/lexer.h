#ifndef LEXER_H
#define LEXER_H

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

extern const char *keywords[];
extern const char operators[];
extern const char *multi_operators[];
extern const char delimiters[];

int is_keyword(const char *str);
int is_operator(const char *str);
int is_delimiter(const char c);
int is_multi_operator(const char *str);
Token get_next_token(const char **input, int *line, int *column);

#endif
