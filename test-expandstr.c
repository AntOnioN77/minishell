//cc -g3 minishell.c expansor.c test-expandstr.c -L. -lft

#include "minishell.h"
#include <stdio.h>

void print_test_result(char *test_name, char *input, char *expected, char *result)
{
    printf("\n=== Test: %s ===\n", test_name);
    printf("Input   : [%s]\n", input);
    printf("Expected: [%s]\n", expected);
    printf("Got     : [%s]\n", result);
    printf("Result  : %s\n", (strcmp(expected, result) == 0) ? "✅ PASS" : "❌ FAIL");
}
/*
// Mock de ft_getenv para testing
char *ft_getenv(const char *name, char *envp[])
{
    if (strcmp(name, "VAR") == 0) return "value";
    if (strcmp(name, "EMPTY") == 0) return "";
    if (strcmp(name, "LONG") == 0) return "this_is_a_very_long_value";
    if (strcmp(name, "SPACE") == 0) return "value with spaces";
    if (strcmp(name, "QUOTES") == 0) return "value'with'quotes";
    return NULL;
}

void cleanup_garbage(t_garbage *garbage)
{
    for (int i = 0; i < garbage->current; i++)
    {
		printf("free garbage->pointers[%d]\n", i);
        free(garbage->pointers[i]);
    }
    free(garbage->pointers);
}
*/

void run_test(char *test_name, char *input, char *expected, char *envp[])
{
    t_garbage garbage = {0};
    garbage.size = 10;  // Asumimos máximo 10 expansiones por test
    garbage.pointers = malloc(sizeof(void *) * (garbage.size + 1));
    garbage.current = 0;

    char *str_original = strdup(input);
	char *str = str_original;
    if (expandstr(&str, &garbage, envp) == 0)
    {
        print_test_result(test_name, input, expected, str);
    }
    else
    {
        printf("\n=== Test: %s ===\n", test_name);
        printf("❌ FAIL: expandstr returned error\n");
    }
    
    cleanup_garbage(&garbage);
    //free(str);
}

int main(void)
{
    char *envp[] = {NULL};  // No necesitamos envp real porque mockeamos ft_getenv

    // Test 1: Simple variable expansion
    run_test("1.Simple expansion", 
             "$VAR", 
             "value",
             envp);

    // Test 2: Multiple variables together
    run_test("2.Multiple variables", 
             "$VAR$VAR", 
             "valuevalue",
             envp);

    // Test 3: Variables with text
    run_test("3.Variables with text", 
             "prefix $VAR suffix", 
             "prefix value suffix",
             envp);

    // Test 4: Single quotes
    run_test("4.Single quotes", 
             "'$VAR'", 
             "'$VAR'",
             envp);

    // Test 5: Double quotes
    run_test("5.Double quotes", 
             "\"$VAR\"", 
             "\"value\"",
             envp);

    // Test 6: Mixed quotes
    run_test("6.Mixed quotes", 
             "\"$VAR'$VAR'\"", 
             "\"value'$VAR'\"",
             envp);

    // Test 7: Non-existent variable
    run_test("7.Non-existent var", 
             "$NONEXISTENT", 
             "",
             envp);

    // Test 8: Empty variable
    run_test("8.Empty variable", 
             "$EMPTY", 
             "",
             envp);

    // Test 9: Variable with spaces
    run_test("9.Variable with spaces", 
             "$SPACE", 
             "value with spaces",
             envp);

    // Test 10: Complex mixed case
    run_test("10.Complex mixed", 
             "\"$VAR'$VAR'$VAR\"'$VAR'", 
             "\"value'$VAR'value\"'$VAR'",
             envp);

    // Test 11: Multiple types of whitespace
    run_test("11.Whitespace handling", 
             "$VAR\t$VAR\n$VAR", 
             "value\tvalue\nvalue",
             envp);

    // Test 12: Variable with quotes in value
    run_test("12.Quotes in variable", 
             "$QUOTES", 
             "value'with'quotes",
             envp);

    // Test 13: Long variable value
    run_test("13.Long variable", 
             "$LONG", 
             "this_is_a_very_long_value",
             envp);

    // Test 14: Just dollar sign
    run_test("14.Just dollar", 
             "$", 
             "$",
             envp);

    // Test 15: Dollar sign at end
    run_test("15.Dollar at end", 
             "text$", 
             "text$",
             envp);

    // Test 16: Multiple dollar signs
    run_test("16.Multiple dollars", 
             "$$$$VAR", 
             "$$$value",
             envp);

    // Test 17: Mixed with special characters
    run_test("17.Special chars", 
             "$VAR!@#$VAR", 
             "value",
             envp);

    // Test 18: Unclosed quotes
    run_test("18.Unclosed quotes", 
             "\"$VAR", 
             "\"value",
             envp);

    // Test 19: Variable name with invalid chars
    run_test("19.Invalid var chars", 
             "$VAR<NAME", 
             "value<NAME",
             envp);

    // Test 20: Empty string
    run_test("20.Empty string", 
             "", 
             "",
             envp);

    return 0;
}
