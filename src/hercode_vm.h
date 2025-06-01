#include <cstdint>
#include <string>
#include <cstring>
#include <unordered_map>
#include <vector>

#define MAIN_FUNCTION "  start  "

#define HERCODE_LINE_TYPE_USELESS           0x00000000
#define HERCODE_LINE_TYPE_DEFINE_FUNCTION   0x00000001
#define HERCODE_LINE_TYPE_CALL_FUNCTION     0x00000002
#define HERCODE_LINE_TYPE_END               0x00000003
#define HERCODE_LINE_TYPE_FUNCTION_SAY      0x10000001

struct HercodeState;
struct HercodeLine;

struct HercodeState 
{
    std::unordered_map<std::string, std::vector<HercodeLine>> functions;
    std::vector<HercodeLine> codes;
    uint32_t stack_ptr;
    uint32_t total_line;

    uint32_t function_line;
    std::string function_name;
};

struct HercodeLine 
{
    uint32_t line_num;
    std::string code;
    
    uint32_t type;
    std::string function;
};

void hercode_initilize_state(HercodeState* state);

void hercode_clear_sate(HercodeState* state);

bool hercode_eval(HercodeState* state, std::string code, std::string& error_msg);

bool hercode_eval_line(HercodeState* state, uint32_t line_num, std::string& error_msg);

bool hercode_call_function(HercodeState* state, std::string function_name, std::string& error_msg);