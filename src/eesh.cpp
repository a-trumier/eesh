#include <stdio.h>
#include <input.hpp>
#include <vector>
#include <launcher.hpp>
#include <common_classes.hpp>
#include <parser.hpp>
#include <path.hpp>

int main()
{
    Environment env1 = Environment();
    
    History hist = History("/home/alex/.eesh_history", 10);
    while (1)
    {
        printf(env1.get_value("PS1").c_str());
        std::string input = clean_whitespace(read_input());
        hist.add_to_history(input);
        std::vector<std::string> tokenized = tokenize_input(input); 
        std::vector<std::vector<std::string>> 
            full = generate_parsed_tokens(tokenized, &env1);
       
        int code;
        for (unsigned int i = 0; i < full.size(); i++)
        {
            code = launch_command(full[i], &env1, &hist);
        }
        /* Specific return codes from launch_commands */
        if (code == 231)
        {
            break;
        }
    }
    return 0;
}
