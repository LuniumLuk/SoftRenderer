#include <stdlib.h>
#include <stdio.h>
#include "api.hpp"
#include "test/test.hpp"

int main(int argc, char * argv[]) {
    int return_value = 0;
    int launch_case = 0;
    {
        if (argc > 1) launch_case = atoi(argv[1]);
        switch (launch_case)
        {
            case 0:
                return_value = test_pipeline();
                break;
            case 1:
                return_value = test_main();
                break;
            case 2:
                return_value = test_basic();
                break;
            case 3:
                return_value = test_shader();  
                break;
            case 4:     
                return_value = test_colormap();
                break;
        }
    }
    return return_value;
}