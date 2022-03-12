#include <stdlib.h>
#include <stdio.h>
#include "api.hpp"
#include "test/test.hpp"

int main() {
    int return_value = 0;
    {
        // return_value = test_main();
        return_value = test_basic();
        // return_value = test_shader();
    }
    return return_value;
}