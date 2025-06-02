#include "hercode_vm.h"
#include <cstdint>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

void hercode_initilize_state(HercodeState *state)
{
    state->stack_ptr = 0;
    state->total_line = 0;
    state->function_line = 0;
}

void hercode_clear_sate(HercodeState *state)
{
    state->functions.clear();
    state->codes.clear();
    state->stack_ptr = 0;
    state->total_line = 0;
    state->function_line = 0;
    state->function_name = "";
}

bool hercode_eval(HercodeState *state, std::string code, std::string& error_msg)
{
    std::string::size_type left = code.find_first_not_of(" ");
    std::string::size_type right = code.find_last_not_of(" ") + 1;
    std::string::size_type size = right - left;
    if (code == "" || size <= 0)
    {
        state->total_line += 1;
        state->codes.push_back({
            .line_num = state->total_line,
            .code = code,
            .type = HERCODE_LINE_TYPE_USELESS
        });
        return false;
    }

    std::string trim_code = code.substr(left, size);
    // printf("(%ld, %ld) \"%s\"", left, right, trim_code.c_str());
    
    if (trim_code == "start:")
    {
        state->function_name = MAIN_FUNCTION;
        state->total_line += 1;
        state->function_line = state->total_line;
        state->codes.push_back({
            .line_num = state->total_line,
            .code = code,
            .type = HERCODE_LINE_TYPE_DEFINE_FUNCTION,
            .function = MAIN_FUNCTION
        });
        return false;
    }

    if (trim_code == "end")
    {
        if (state->function_line != 0) 
        {
            state->total_line += 1;
            state->codes.push_back({
                .line_num = state->total_line,
                .code = code,
                .type = HERCODE_LINE_TYPE_END,
            });
            state->functions[state->function_name] = {
                state->function_line,
                state->total_line
            };
            state->function_line = 0;
            state->function_name = "";
        } else 
        {
            error_msg = "错误(行 " + std::to_string(state->total_line + 1) + "): 没有方法等待关闭!";
            return true;
        }
        

        // if (state->functions.end() != state->functions.find(MAIN_FUNCTION))
        // {
        //     // run main.
        // }
        return false;
    }

    if (size >= 8)
    {
        std::string str = code.substr(left, 8);
        if (str == "function") {
            if (size <= 10 && code[right] != ':') {
                error_msg = "错误(行 " + std::to_string(state->total_line + 1) + "): 定义一个方法时, 必须以 `function 方法名:` 格式定义!";
                return true;
            }
            else if (state->function_line != 0)
            {
                error_msg = "错误(行 " + std::to_string(state->total_line + 1) + "): 无法在一个方法定义内嵌套定义另一个方法!";
                return true;
            }
            str = code.substr(left + 8, right - left - 9);
            left = str.find_first_not_of(" ");
            right = str.find_last_not_of(" ") + 1;

            state->function_name = str.substr(left, right - left);
            state->total_line += 1;
            state->function_line = state->total_line;
            state->codes.push_back({
                .line_num = state->total_line,
                .code = code,
                .type = HERCODE_LINE_TYPE_DEFINE_FUNCTION,
                .function = state->function_name
            });
            return false;
        }
    }

    if (state->functions.end() != state->functions.find(trim_code))
    {
        state->total_line += 1;
        state->codes.push_back({
            .line_num = state->total_line,
            .code = code,
            .type = HERCODE_LINE_TYPE_CALL_FUNCTION,
            .function = trim_code
        });
        
        if (
            state->function_name == MAIN_FUNCTION ||
                state->function_line == 0
        ) {
            // TODO run code;
            return hercode_eval_line(state, state->total_line, error_msg);
        }

        return false;
    }
    
    if (size > 3) 
    {
        std::string str = code.substr(left, 3);
        if (str == "say")
        {
            state->total_line += 1;
            state->codes.push_back({
                .line_num = state->total_line,
                .code = code,
                .type = HERCODE_LINE_TYPE_FUNCTION_SAY,
            });

            if (
                state->function_name == MAIN_FUNCTION ||
                state->function_line == 0
            ) {
                // TODO run code;
                return hercode_eval_line(state, state->total_line, error_msg);
            }
            return false;
        }
    }
    error_msg = "错误(行 " + std::to_string(state->total_line + 1) + "): 不能识别代码: " + code;
    return true;
}

bool hercode_eval_line(HercodeState *state, uint32_t line_num, std::string &error_msg)
{
    const HercodeLine& line = state->codes[line_num - 1];
    switch (line.type) {
        case HERCODE_LINE_TYPE_FUNCTION_SAY:
        {
            const std::string code = line.code;
            std::string::size_type left = code.find_first_of("\"") + 1;
            std::string::size_type right = code.find_last_not_of("\"") + 1;
            printf("%s\n", code.substr(left, right - left).c_str());
            break;
        }
        case HERCODE_LINE_TYPE_CALL_FUNCTION:
        {
            if (hercode_call_function(state, line.function, error_msg))
            {
                error_msg = "错误(行 " + std::to_string(line_num) + "): " + error_msg;
                return true;
            }
        }   
    }
    return false;
}

bool hercode_call_function(HercodeState *state, std::string function_name, std::string &error_msg)
{
    if (state->functions.end() == state->functions.find(function_name))
    {
        error_msg = "没有名为 '" + function_name + "' 的方法!";
        return true;
    }

    const std::tuple<uint32_t, uint32_t> &func = state->functions[function_name];
    
    for (uint32_t begin = std::get<0>(func),
                    end = std::get<1>(func); begin < end; ++begin)
    {
        if (hercode_eval_line(state, begin, error_msg)) {
            return true;
        }
    }
    return false;
}