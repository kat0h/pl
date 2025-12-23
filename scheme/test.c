#include <assert.h>
#include "main.h"
#include "continuation.h"

// Example test case
void test_example() {
    // A simple assertion that is always true
    assert(1 == 1);
}

int main(int argc, char **argv) {
    test_example();
    return 0;
}
