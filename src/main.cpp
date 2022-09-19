#include <stdlib.h>
#include <stdio.h>
#include "api.hpp"
#include "sample/sample.hpp"

int main(int argc, char * argv[]) {
    int return_value = 0;
    int launch_case = 0;
    {
        if (argc > 1) launch_case = atoi(argv[1]);
        switch (launch_case)
        {
            case 0:
                return_value = main_demo();
                break;
            case 1:
                return_value = blank_demo();
                break;
            case 2:     
                return_value = colormap_demo();
                break;
            case 3:
                return_value = normal_mapping_demo();
                break;
        }
    }
    return return_value;
}