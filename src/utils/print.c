#include "minishell.h"

//TO DEBUG
// Helper: print token list
static void print_tokens_debug(t_token *token)
{
    while (token)
    {
        fprintf(stderr, "%s ", token->txt);
        token = token->next;
    }
    fflush(stderr);
}

// Recursive AST printer
void print_ast(t_node *node, int level)
{
    if (!node)
        return;

    // Indentation
    for (int i = 0; i < level; i++)//make norm happy
        fprintf(stderr,"  ");

    if (node->type == COMMAND)
    {
        // fprintf(stderr, "[DEBUG] COMMAND: ");
        DEBUG_PRINT("[DEBUG] COMMAND: ");
        if (node->u_data.cmd.tokens)
            print_tokens_debug(node->u_data.cmd.tokens);
        fprintf(stderr, "\n");
    }
    else if (node->type == OPERATOR)
    {
        // fprintf(stderr, "[DEBUG] OPERATOR: ");
        DEBUG_PRINT("[DEBUG] OPERATOR: ");
        switch (node->u_data.op.type)
        {
            case PIPE: fprintf(stderr, "|"); break;
            case AND: fprintf(stderr, "&&"); break;
            case OR: fprintf(stderr, "||"); break;
            case SEMICOLON: fprintf(stderr, ";"); break;
            default: fprintf(stderr, "UNKNOWN"); break;
        }
        fprintf(stderr, "\n");

        // Recurse into children
        print_ast(node->u_data.op.left, level + 1);
        print_ast(node->u_data.op.right, level + 1);
    }
    fflush(stderr);
}
