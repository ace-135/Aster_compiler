#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

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

    char *ast_file = argv[1];
    int len = strlen(ast_file);

    if (strcmp(ast_file + len - 4, ".ast") != 0) {
        printf("no ast file found\n");
        exit(-1);
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
