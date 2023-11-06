#ifndef LEXER_H
#define LEXER_H

enum token_id {
    TOKEN_START_LINE_COMMENT,
    TOKEN_START_BLOCK_COMMENT,
    TOKEN_END_BLOCK_COMMENT,
    TOKEN_TAB,
    TOKEN_SPACE,
    TOKEN_NEW_LINE,
    TOKEN_CARRIAGE_RETURN,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_LEFT_SQUARE_BRACE,
    TOKEN_RIGHT_SQUARE_BRACE,
    TOKEN_LEFT_SHIFT,
    TOKEN_RIGHT_SHIFT,
    TOKEN_LESS_THAN_EQ,
    TOKEN_GREATER_THAN_EQ,
    TOKEN_EQ,
    TOKEN_NOT_EQ,
    TOKEN_OR,
    TOKEN_AND,
    TOKEN_INCREMENT,
    TOKEN_DECREMENT,
    TOKEN_POINTER_ACCESS,
    TOKEN_ACCESS,
    TOKEN_LESS_THAN,
    TOKEN_GREATER_THAN,
    TOKEN_ADD,
    TOKEN_SUB,
    TOKEN_MULT,
    TOKEN_DIV,
    TOKEN_BIT_AND,
    TOKEN_BIT_OR,
    TOKEN_BIT_XOR,
    TOKEN_BIT_NEGATE,
    TOKEN_BANG,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_COLON,
    TOKEN_EQUAL,
    TOKEN_BACKSLASH,
    TOKEN_DOUBLE_QUOTE,
    TOKEN_SINGLE_QUOTE,
    TOKEN_MODULO,
    TOKEN_PREPROCESSOR_START,
    TOKEN_FOR,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_SWITCH,
    TOKEN_CASE,
    TOKEN_STRUCT,
    TOKEN_ENUM,
    TOKEN_UNION,
    TOKEN_CONST,
    TOKEN_STATIC,
    TOKEN_ATTRIBUTE,
    TOKEN_RETURN,
    TOKEN_BREAK,
    TOKEN_CONTINUE,
    TOKEN_SIZEOF,
    TOKEN_OFFSETOF,
    TOKEN_TYPEDEF,
    TOKEN_DEFAULT,
    TOKEN_GOTO,
    TOKEN_IDENT,
    // These tokens signify the start and end of the list
    TOKEN_LIST_START,
    TOKEN_LIST_END,
    TOKEN_COUNT
};

struct token {
    enum token_id id;
    char *ident_data;

    size_t offset;
    size_t line;

    struct token *next;
};

struct token_list {
    struct token *head;
    struct token *tail;

    const char *name;
};

extern const char special_chars[TOKEN_COUNT];
extern const char *token_strings[TOKEN_COUNT];
extern const char *token_names[TOKEN_COUNT];

void print_token_list(struct token_list *list);
void free_token_list(struct token_list *list);
int parse_tokens(const char *string, struct token_list *list);

int token_is_whitespace(struct token *token);

#endif
