#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <util.h>

/*
 * statement = for_loop | if_statement | (expression term)
 *
 * for_loop = "for" "(" for_header ")" "{" for_body "}"
 * for_header = [ expressiion ] term [ expression ] term [ expression ]
 * for_body = { statement term }
 *
 * if_statment = "if" "(" expression ")" "{" if_body "}" { else_if } [ else ]
 * else_if = "else" "if" "(" expression ")" "{" if_body "}"
 * else = "else" "{" if_body "}"
 * if_body = { statement term }
 *
 * expression = [ lvalue "=" ] rvalue operator rvalue
 * rvalue = call | variable | integer | list_member | string | array | ( "(" expression ")" )
 * lvalue = variable | list_member
 *
 * list_member = variable "[" integer "]"
 * variable_declaration = type variable [ "[" integer "]" ]
 *
 * call = command "(" { argument_list } ")"
 * argument_list = [ rvalue { "," rvalue } ]
 *
 * array = "<" type ">" "{" array_list "}"
 * array_list = rvalue { "," rvalue }
 *
 * integer = ([ "-" ] { number }) | ("0x" hexadecimal_digit) | ("'" char "'")
 * string = '"' { char } '"'
 * char = letter | number | char_escape
 *
 * type = "uint8_t" | "uint16_t" | "uint32_t" | "uint64_t"
 * operator = "<<" | ">>" | "<=" | ">=" | "<" | ">" | "==" | "!=" | "+" | "-" | "*" | "/" | "&" | "|" | "^" | "||" | "&&"
 *
 * command = letter { letter | number }
 * variable = letter { letter | number }
 *
 */

const char special_chars[] = "\t \n\r(){}[]<>=!|&+-*/^,;:\\\"'";
const char *token_strings[] = {
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
    [TOKEN_LESS_THAN] = "<",
    [TOKEN_GREATER_THAN] = ">",
    [TOKEN_ADD] = "+",
    [TOKEN_SUB] = "-",
    [TOKEN_MULT] = "*",
    [TOKEN_DIV] = "/",
    [TOKEN_BIT_AND] = "&",
    [TOKEN_BIT_OR] = "|",
    [TOKEN_BIT_XOR] = "^",
    [TOKEN_COMMA] = ",",
    [TOKEN_SEMICOLON] = ";",
    [TOKEN_COLON] = ":",
    [TOKEN_EQUAL] = "=",
    [TOKEN_BACKSLASH] = "\\",
    [TOKEN_DOUBLE_QUOTE] = "\"",
    [TOKEN_SINGLE_QUOTE] = "'",
    [TOKEN_FOR] = "for",
    [TOKEN_IF] = "if",
    [TOKEN_ELSE] = "else",
};

const char *token_names[] = {
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
    [TOKEN_LESS_THAN] = "TOKEN_LESS_THAN",
    [TOKEN_GREATER_THAN] = "TOKEN_GREATER_THAN",
    [TOKEN_ADD] = "TOKEN_ADD",
    [TOKEN_SUB] = "TOKEN_SUB",
    [TOKEN_MULT] = "TOKEN_MULT",
    [TOKEN_DIV] = "TOKEN_DIV",
    [TOKEN_BIT_AND] = "TOKEN_BIT_AND",
    [TOKEN_BIT_OR] = "TOKEN_BIT_OR",
    [TOKEN_BIT_XOR] = "TOKEN_BIT_XOR",
    [TOKEN_COMMA] = "TOKEN_COMMA",
    [TOKEN_SEMICOLON] = "TOKEN_SEMICOLON",
    [TOKEN_COLON] = "TOKEN_COLON",
    [TOKEN_EQUAL] = "TOKEN_EQUAL",
    [TOKEN_BACKSLASH] = "TOKEN_BACKSLASH",
    [TOKEN_DOUBLE_QUOTE] = "TOKEN_DOUBLE_QUOTE",
    [TOKEN_SINGLE_QUOTE] = "TOKEN_SINGLE_QUOTE",
    [TOKEN_FOR] = "TOKEN_FOR",
    [TOKEN_IF] = "TOKEN_IF",
    [TOKEN_ELSE] = "TOKEN_ELSE",
    [TOKEN_IDENT] = "TOKEN_IDENT",
    [TOKEN_LIST_START] = "TOKEN_LIST_START",
    [TOKEN_LIST_END] = "TOKEN_LIST_END",
};

void print_token_list(struct token *head) {
    print_info("Token list: \n");
    while (head != null) {
        if (head->id != TOKEN_IDENT) {
            print_info("\t%s\n", token_names[head->id]);
        } else {
            print_info("\t%s (\"%s\")\n", token_names[head->id], head->ident_data);
        }

        head = head->next;
    }
}

void free_token_list(struct token *head) {
    struct token *curr, *next;

    curr = head;
    while (curr != NULL) {
        next = curr->next;

        if (curr->ident_data) {
            free(curr->ident_data);
        }
        free(curr);

        curr = next;
    }
}

int parse_tokens(const char *string, struct token **tokens) {
    struct token *head = NULL;
    struct token *tail = NULL;
    struct token *token = NULL;
    int ret = 0;

    if (!tokens || !head) {
        return EINVAL;
    }

    head = malloc(sizeof(head));
    if (!head) {
        return ENOMEM;
    }

    head->token_id = TOKEN_LIST_START;
    tail = head;

    while (*string != '\0') {
        enum token id = TOKEN_IDENT;
        char *ident_data = NULL;

        // Find special case tokens
        for (int i = 0; i < TOKEN_IDENT; i++) {
            const char *ts = token_strings[i];
            const char *s = string;

            while (*ts != '\0') {
                if (*s != *ts) {
                    break;
                }

                s++;
                ts++;
            }

            if (*ts == '\0') {
                id = i;
                // Advance string pointer to end of special token
                string = s;

                break;
            }
        }

        // If this doesn't match any special case, it must be a TOKEN_IDENT
        if (id == TOKEN_IDENT) {
            size_t len = 0;
            const char *s = string;

            // Collect characters until we hit a special character
            while (strchr(special_chars, *(s++)) == NULL) {
                len++;
            }

            if (len == 0) {
                // Fuck me, what do I do here?
                print_error("Zero-length TOKEN_IDENT\n");

                ret = EFAULT;
                goto error;
            }

            ident_data = malloc(sizeof(*ident_data) * (len + 1));
            if (!ident_data) {
                ret = ENOMEM;
                goto error;
            }
        }

        token = malloc(sizeof(*token));
        if (!token) {
            ret = ENOMEM;
            goto error;
        }

        token->id = id;
        token->ident_data = ident_data;
        token->next = NULL;

        tail->next = token;
        tail = token;
    }

    token = malloc(sizeof(*token));
    if (!token) {
        ret = ENOMEM;
        goto error;
    }

    token->id = TOKEN_LIST_END;
    token->next = NULL;

    tail->next = token;
    tail = token;

error:
    print_token_list(head);
    if (ret != 0) {
        free_token_list(head);
    } else {
        *tokens = head;
    }

    return ret;
}
