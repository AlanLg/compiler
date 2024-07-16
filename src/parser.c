#include "../include/parser.h"
#include "../include/errors.h"

symbol_t *table;

void parse(buffer_t *buffer, error_list *errors) {
    if (strcmp(lexer_getalpha(buffer, errors), "fonction") != 0) {
        add_error(errors,"first lexer was not \"fonction\"");
    }

    while (!buf_eof(buffer)) {
        symbol_t *result = analyze_function(buffer, errors);
        if (result) {
            sym_add(&table, result);
        } else {
            break;
        }
    }
}


symbol_t *analyze_function(buffer_t *buffer, error_list *errors) {
    char *name = lexer_getalphanum(buffer, errors);
    if (!name) return NULL;

    symbol_t *params = analyze_parameters(buffer, errors);
    var_type_e return_type = analyze_return(buffer, errors);
    if (return_type == UNKNOWN) return NULL;

    symbol_t *stmts = analyze_function_body(buffer, errors);
    if (!stmts) return NULL;

    ast_list_t *stmts_ast = NULL;
    symbol_t *temp_symb_stmts = stmts;
    while (temp_symb_stmts != NULL) {
        ast_list_add(&stmts_ast, temp_symb_stmts->ast_node);
        temp_symb_stmts = temp_symb_stmts->next;
    }

    ast_list_t *params_ast = NULL;
    symbol_t *temp_symb_params = params;
    while (temp_symb_params != NULL) {
        ast_list_add(&params_ast, temp_symb_params->ast_node);
        temp_symb_params = temp_symb_params->next;
    }

    ast_t *ast_function = ast_new_function(name, return_type, params_ast, stmts_ast);
    symbol_t *function_symbol = sym_new(name, SYM_FUNCTION, ast_function);
    function_symbol->function_table = params;
    sym_add(&function_symbol->function_table, stmts);
    sym_add(&table, function_symbol);

    return function_symbol;
}

symbol_t *analyze_parameters(buffer_t *buffer, error_list *errors) {
    printf("analyzing params\n");
    if (lexer_getchar(buffer, errors) != '(') {
        add_error(errors,"missing ( for function parameters\n");
    }

    char comma_or_parenthesis;

    printf("locking buffer\n");
    buf_lock(buffer);
    comma_or_parenthesis = lexer_getchar(buffer, errors);

    printf("checking if )\n");
    if (comma_or_parenthesis == ')') {
        buf_unlock(buffer);
        return NULL;
    }

    buf_rollback_and_unlock(buffer, 1);
    symbol_t *parameter_symbols = NULL;

    printf("starting do while\n");
    do {
        char *parameter_type_name = lexer_getalpha(buffer, errors);
        if (!parameter_type_name) return NULL;

        if (parameter_symbols != NULL) {
            printf("printing symbols before search: %s\n", parameter_symbols->name);
        }


        printf("type: %s\n", parameter_type_name);
        var_type_e parameter_type = get_var_type_from_string(parameter_type_name);

        if (parameter_type == UNKNOWN) {
          add_error(errors, "parameter type is not valid\n");
        }

        char *parameter_name = lexer_getalphanum(buffer, errors);
        if (!parameter_name) return NULL;

        if (sym_search(parameter_symbols, parameter_name) != NULL) {
          add_error(errors,"parameter name is already taken\n");
        }

        ast_t *ast = ast_new_variable(parameter_name, parameter_type);
        symbol_t *new_symbol = sym_new(parameter_name, SYM_PARAM, ast);
        sym_add(&parameter_symbols, new_symbol);

        comma_or_parenthesis = lexer_getchar(buffer, errors);

    } while (comma_or_parenthesis == ',');

    if (comma_or_parenthesis != ')') {
      add_error(errors,"missing ) at end of parameters\n");
    }

    if (buffer->lock == true) {
        buf_unlock(buffer);
    }

    printf("finished param symbol analysis\n");
    return parameter_symbols;
}

var_type_e analyze_return(buffer_t *buffer, error_list *errors) {
    if (lexer_getchar(buffer, errors) != ':') {
        add_error(errors,"missing : for function return type");
    }
    var_type_e return_type = get_var_type_from_string(lexer_getalpha(buffer, errors));
    if (return_type == UNKNOWN) {
        add_error(errors,"unknown type for function return type");
    }
    return return_type;
}

symbol_t *analyze_function_body(buffer_t *buffer, error_list *errors) {
    if (lexer_getchar(buffer, errors) != '{') {
        add_error(errors,"missing { for function body\n");
    }

    symbol_t *table_function_body = NULL;
    do {
        buf_lock(buffer);
        char *alphanum = lexer_getalphanum(buffer, errors);
        if (!alphanum) {
            buf_unlock(buffer);
            return NULL;
        }

        symbol_t *actual_function_body_symbol = NULL;
        if (is_alpha(alphanum)) {
            char *alpha = alphanum;
            if (strcmp(alpha, "si") == 0) {
                actual_function_body_symbol = analyze_conditional_branching(buffer, errors);
            } else if (strcmp(alpha, "tantque") == 0) {
                actual_function_body_symbol = analyze_loop(buffer, errors);
            } else {
                actual_function_body_symbol = analyze_declaration(buffer, errors);
            }
        } else {
            actual_function_body_symbol = analyze_assignment(buffer, errors);
        }
        sym_add(&table_function_body, actual_function_body_symbol);
        buf_unlock(buffer);
    } while (lexer_getchar(buffer, errors) != '}');

    printf("finished analaysing function body");

    return table_function_body;
}

symbol_t *analyze_declaration(buffer_t *buffer, error_list *errors) {
    char *parameter_type_name = lexer_getalpha(buffer, errors);

    var_type_e parameter_type = get_var_type_from_string(parameter_type_name);

    if (parameter_type == UNKNOWN) {
      add_error(errors, "parameter type is not valid\n");
    }

    char *parameter_name = lexer_getalphanum(buffer, errors);
    ast_t *ast = ast_new_variable(parameter_name, parameter_type);
    symbol_t *new_symbol = sym_new(parameter_name, SYM_PARAM, ast);

    return new_symbol;
}

symbol_t *analyze_assignment(buffer_t *buffer, error_list *errors) {
    return NULL;
}

symbol_t *analyze_loop(buffer_t *buffer, error_list *errors) {
    return NULL;
}

symbol_t *analyze_conditional_branching(buffer_t *buffer, error_list *errors) {
    const char *conditional_branching_start = lexer_getalpha(buffer, errors);
    if (strcmp(conditional_branching_start, "si") != 0) {
        add_error(errors, "invalid conditional branching start");
    }

    const char parenthesis = lexer_getchar(buffer, errors);

    if (parenthesis != '(') {
        add_error(errors, "missing (");
    }

    return NULL;
}

ast_t *analyse_expression(buffer_t *buffer, error_list *errors) {

}