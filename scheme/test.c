
#include <assert.h>
#include "main.h"
#include "continuation.h"


void test_quote_equivalence() {
    // (quote (1 2 3)) and '(1 2 3) should be parsed to the same structure
    value *a = parse_program("(quote (1 2 3))");
    value *b = parse_program("'(1 2 3)");
    assert(value_equal(a, b));
}

// Example test case
void test_example() {
    // A simple assertion that is always true
    assert(1 == 1);
}

int main(int argc, char **argv) {
    test_example();
    test_quote_equivalence();
    return 0;
}
