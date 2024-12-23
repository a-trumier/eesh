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
    
    while (1)
    {
        printf(env1.get_value("PS1").c_str());
        std::string input = clean_whitespace(read_input());
        std::vector<std::string> tokenized = tokenize_input(input); 
        std::vector<Command> full = generate_commands(tokenized, &env1);
       
        int code;
        for (unsigned int i = 0; i < full.size(); i++)
        {
            code = launch_command(&full[i], &env1);
        }
        /* Specific return codes from launch_commands */
        if (code == 231)
        {
            break;
        }
    }
    return 0;
}
