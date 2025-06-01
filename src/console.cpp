#include "hercode_vm.h"
#include <string>
#include <iostream>
#include <fstream>

int main(int argc, char** args) 
{
    HercodeState state;
    hercode_initilize_state(&state);
    std::string line, error_msg;

    if (argc > 1)
    {
        std::ifstream in;
        in.open(args[1]);
        if (!in.is_open())
        {
            std::cerr << "无法打开文件: '" << args[1] << "'!" << std::endl;
            return -1;
        }
        while (std::getline(in, line)) 
        {
            if (hercode_eval(&state, line, error_msg))
            {
                std::cerr << error_msg << std::endl;
            }
        }
        in.close();
        return 0;
    }

    
    bool is_func = false;
    while (std::getline(std::cin, line)) 
    {
        if (line == "exit")
        {
            break;
        }

        if (hercode_eval(&state, line, error_msg))
        {
            std::cerr << error_msg << std::endl;
        }

        is_func = state.function_line != 0;
        if (is_func)
        {
            std::cout << ">> ";
        }
    }
    return 0;
}