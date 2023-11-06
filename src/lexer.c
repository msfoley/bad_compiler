#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

#include <util.h>
#include <log.h>
#include <lexer.h>

const char special_chars[TOKEN_COUNT] = "\t \n\r(){}[]<>=!|&+-*/^,;:\\\"'~%.";
const char *token_strings[TOKEN_COUNT] = {
    [TOKEN_START_LINE_COMMENT] = "//",
    [TOKEN_START_BLOCK_COMMENT] = "/*",
    [TOKEN_END_BLOCK_COMMENT] = "*/",
    [TOKEN_TAB] = "\t",
    [TOKEN_SPACE] = " ",
    [TOKEN_NEW_LINE] = "\n",
    [TOKEN_CARRIAGE_RETURN] = "\r",
    [TOKEN_LEFT_PAREN] = "(",
    [TOKEN_RIGHT_PAREN] = ")",
    [TOKEN_LEFT_BRACE] = "{",
    [TOKEN_RIGHT_BRACE] = "}",
    [TOKEN_LEFT_SQUARE_BRACE] = "[",
    [TOKEN_RIGHT_SQUARE_BRACE] = "]",
    [TOKEN_LEFT_SHIFT] = "<<",
    [TOKEN_RIGHT_SHIFT] = ">>",
    [TOKEN_LESS_THAN_EQ] = "<=",
    [TOKEN_GREATER_THAN_EQ] = ">=",
    [TOKEN_EQ] = "==",
    [TOKEN_NOT_EQ] = "!=",
    [TOKEN_OR] = "||",
    [TOKEN_AND] = "&&",
    [TOKEN_INCREMENT] = "++",
    [TOKEN_DECREMENT] = "--",
    [TOKEN_POINTER_ACCESS] = "->",
    [TOKEN_ACCESS] = ".",
    [TOKEN_LESS_THAN] = "<",
    [TOKEN_GREATER_THAN] = ">",
    [TOKEN_ADD] = "+",
    [TOKEN_SUB] = "-",
    [TOKEN_MULT] = "*",
    [TOKEN_DIV] = "/",
    [TOKEN_BIT_AND] = "&",
    [TOKEN_BIT_OR] = "|",
    [TOKEN_BIT_XOR] = "^",
    [TOKEN_BIT_NEGATE] = "~",
    [TOKEN_BANG] = "!",
    [TOKEN_COMMA] = ",",
    [TOKEN_SEMICOLON] = ";",
    [TOKEN_COLON] = ":",
    [TOKEN_EQUAL] = "=",
    [TOKEN_BACKSLASH] = "\\",
    [TOKEN_DOUBLE_QUOTE] = "\"",
    [TOKEN_SINGLE_QUOTE] = "'",
    [TOKEN_MODULO] = "%",
    [TOKEN_PREPROCESSOR_START] = "#",
    [TOKEN_FOR] = "for",
    [TOKEN_IF] = "if",
    [TOKEN_ELSE] = "else",
    [TOKEN_WHILE] = "while",
    [TOKEN_SWITCH] = "switch",
    [TOKEN_CASE] = "case",
    [TOKEN_STRUCT] = "struct",
    [TOKEN_ENUM] = "enum",
    [TOKEN_UNION] = "union",
    [TOKEN_CONST] = "const",
    [TOKEN_STATIC] = "static",
    [TOKEN_ATTRIBUTE] = "__attribute__",
    [TOKEN_RETURN] = "return",
    [TOKEN_BREAK] = "break",
    [TOKEN_CONTINUE] = "continue",
    [TOKEN_SIZEOF] = "sizeof",
    [TOKEN_OFFSETOF] = "offsetof",
    [TOKEN_TYPEDEF] = "typedef",
    [TOKEN_DEFAULT] = "default",
    [TOKEN_GOTO] = "goto",
};

const char *token_names[TOKEN_COUNT] = {
    [TOKEN_START_LINE_COMMENT] = "TOKEN_START_LINE_COMMENT",
    [TOKEN_START_BLOCK_COMMENT] = "TOKEN_START_BLOCK_COMMENT",
    [TOKEN_END_BLOCK_COMMENT] = "TOKEN_END_BLOCK_COMMENT",
    [TOKEN_TAB] = "TOKEN_TAB",
    [TOKEN_SPACE] = "TOKEN_SPACE",
    [TOKEN_NEW_LINE] = "TOKEN_NEW_LINE",
    [TOKEN_CARRIAGE_RETURN] = "TOKEN_CARRIAGE_RETURN",
    [TOKEN_LEFT_PAREN] = "TOKEN_LEFT_PAREN",
    [TOKEN_RIGHT_PAREN] = "TOKEN_RIGHT_PAREN",
    [TOKEN_LEFT_BRACE] = "TOKEN_LEFT_BRACE",
    [TOKEN_RIGHT_BRACE] = "TOKEN_RIGHT_BRACE",
    [TOKEN_LEFT_SQUARE_BRACE] = "TOKEN_LEFT_SQUARE_BRACE",
    [TOKEN_RIGHT_SQUARE_BRACE] = "TOKEN_RIGHT_SQUARE_BRACE",
    [TOKEN_LEFT_SHIFT] = "TOKEN_LEFT_SHIFT",
    [TOKEN_RIGHT_SHIFT] = "TOKEN_RIGHT_SHIFT",
    [TOKEN_LESS_THAN_EQ] = "TOKEN_LESS_THAN_EQ",
    [TOKEN_GREATER_THAN_EQ] = "TOKEN_GREATER_THAN_EQ",
    [TOKEN_EQ] = "TOKEN_EQ",
    [TOKEN_NOT_EQ] = "TOKEN_NOT_EQ",
    [TOKEN_OR] = "TOKEN_OR",
    [TOKEN_AND] = "TOKEN_AND",
    [TOKEN_INCREMENT] = "TOKEN_INCREMENT",
    [TOKEN_DECREMENT] = "TOKEN_DECREMENT",
    [TOKEN_POINTER_ACCESS] = "TOKEN_POINTER_ACCESS",
    [TOKEN_ACCESS] = "TOKEN_ACCESS",
    [TOKEN_LESS_THAN] = "TOKEN_LESS_THAN",
    [TOKEN_GREATER_THAN] = "TOKEN_GREATER_THAN",
    [TOKEN_ADD] = "TOKEN_ADD",
    [TOKEN_SUB] = "TOKEN_SUB",
    [TOKEN_MULT] = "TOKEN_MULT",
    [TOKEN_DIV] = "TOKEN_DIV",
    [TOKEN_BIT_AND] = "TOKEN_BIT_AND",
    [TOKEN_BIT_OR] = "TOKEN_BIT_OR",
    [TOKEN_BIT_XOR] = "TOKEN_BIT_XOR",
    [TOKEN_BIT_NEGATE] = "TOKEN_BIT_NEGATE",
    [TOKEN_BANG] = "TOKEN_BANG",
    [TOKEN_COMMA] = "TOKEN_COMMA",
    [TOKEN_SEMICOLON] = "TOKEN_SEMICOLON",
    [TOKEN_COLON] = "TOKEN_COLON",
    [TOKEN_EQUAL] = "TOKEN_EQUAL",
    [TOKEN_BACKSLASH] = "TOKEN_BACKSLASH",
    [TOKEN_DOUBLE_QUOTE] = "TOKEN_DOUBLE_QUOTE",
    [TOKEN_SINGLE_QUOTE] = "TOKEN_SINGLE_QUOTE",
    [TOKEN_MODULO] = "TOKEN_MODULO",
    [TOKEN_PREPROCESSOR_START] = "TOKEN_PREPROCESSOR_START",
    [TOKEN_FOR] = "TOKEN_FOR",
    [TOKEN_IF] = "TOKEN_IF",
    [TOKEN_ELSE] = "TOKEN_ELSE",
    [TOKEN_WHILE] = "TOKEN_WHILE",
    [TOKEN_SWITCH] = "TOKEN_SWITCH",
    [TOKEN_CASE] = "TOKEN_CASE",
    [TOKEN_STRUCT] = "TOKEN_STRUCT",
    [TOKEN_ENUM] = "TOKEN_ENUM",
    [TOKEN_UNION] = "TOKEN_UNION",
    [TOKEN_CONST] = "TOKEN_CONST",
    [TOKEN_STATIC] = "TOKEN_STATIC",
    [TOKEN_ATTRIBUTE] = "TOKEN_ATTRIBUTE",
    [TOKEN_RETURN] = "TOKEN_RETURN",
    [TOKEN_BREAK] = "TOKEN_BREAK",
    [TOKEN_CONTINUE] = "TOKEN_CONTINUE",
    [TOKEN_SIZEOF] = "TOKEN_SIZEOF",
    [TOKEN_OFFSETOF] = "TOKEN_OFFSETOF",
    [TOKEN_TYPEDEF] = "TOKEN_TYPEDEF",
    [TOKEN_DEFAULT] = "TOKEN_DEFAULT",
    [TOKEN_GOTO] = "TOKEN_GOTO",
    [TOKEN_IDENT] = "TOKEN_IDENT",
    [TOKEN_LIST_START] = "TOKEN_LIST_START",
    [TOKEN_LIST_END] = "TOKEN_LIST_END",
};

void print_token_list(struct token_list *list) {
    struct token *t;

    if (list == NULL) {
        return;
    }

    print_debug("Token list:\n");

    t = list->head;
    while (t != NULL) {
        print_debug("\t%s [%s:%zu %zu]\n", token_names[t->id], list->name, t->line, t->offset);
        if (t->ident_data) {
            print_debug("\t\t= \"%s\"\n", t->ident_data);
        }

        t = t->next;
    }
}

void free_token_list(struct token_list *list) {
    struct token *curr, *next;

    if (!list->head) {
        return;
    }

    curr = list->head;
    while (curr) {
        next = curr->next;

        if (curr->ident_data) {
            free(curr->ident_data);
        }
        free(curr);

        curr = next;
    }

    list->head = NULL;
}

int add_token_to_list(struct token_list *list, enum token_id id, char *ident_data, size_t line, size_t offset) {
    struct token *token;

    if (!list) {
        return EINVAL;
    }

    token = malloc(sizeof(*token));
    if (!token) {
        return ENOMEM;
    }

    token->id = id;
    token->ident_data = ident_data;
    token->line = line;
    token->offset = offset;
    token->next = NULL;

    if (!list->head) {
        list->head = token;
    }

    if (list->tail) {
        list->tail->next = token;
    }
    list->tail = token;

    return 0;
}

// Return 0 if token_string does not match the beginning of string,
// otherwise return the length of token_string
int compare_to_token_string(const char *string, const char *token_string) {
    int len = 0;

    while (token_string[len] != '\0') {
        if (string[len] == '\0') {
            return 0;
        }

        if (string[len] != token_string[len]) {
            break;
        }

        len++;
    }

    if (token_string[len] != '\0') {
        return 0;
    }

    return len;
}

int parse_tokens(const char *string, struct token_list *list) {
    struct token *head = NULL;
    struct token *tail = NULL;
    struct token *token = NULL;
    const char *s = NULL;
    const char *line_start = string;
    size_t line = 0;
    size_t offset;
    int ret = 0;
    int len = 0;

    if (!list || !string) {
        return EINVAL;
    }

    list->head = NULL;
    list->tail = NULL;

    add_token_to_list(list, TOKEN_LIST_START, NULL, line, (size_t) (string - line_start));

    while (*string != '\0') {
        enum token_id id = TOKEN_IDENT;
        char *ident_data = NULL;

        // Find special case tokens
        for (int i = 0; i < TOKEN_IDENT; i++) {
            len = compare_to_token_string(string, token_strings[i]);
            if (len != 0) {
                id = i;
                // Advance string pointer to end of special token
                string += len;

                break;
            }
        }

        // This is a token made of special characters
        if (id < TOKEN_IDENT) {
            add_token_to_list(list, id, NULL, line, (size_t) (string - line_start));

            if (id == TOKEN_NEW_LINE) {
                line_start = string;
                line++;
            }

            continue;
        }

        // Otherwise this is TOKEN_IDENT
        s = string;
        len = 0;

        while (strchr(special_chars, *(s++)) == NULL) {
            // Collect characters until we hit a special character
            len++;
        }

        if (len == 0) {
            // Fuck me, what do I do here?
            print_error("Zero-length TOKEN_IDENT\n");

            ret = EFAULT;
            goto error;
        }

        // If this doesn't match any special case, it must be a TOKEN_IDENT
        ident_data = malloc(sizeof(*ident_data) * (len + 1));
        if (!ident_data) {
            ret = ENOMEM;
            goto error;
        }

        strncpy(ident_data, string, len);
        ident_data[len] = '\0';
        string += len;

        add_token_to_list(list, id, ident_data, line, (size_t) (string - line_start));
    }

    add_token_to_list(list, TOKEN_LIST_END, NULL, line, (size_t) (string - line_start));

error:
    print_token_list(list);
    if (ret != 0) {
        free_token_list(list);
    }

    return ret;
}
