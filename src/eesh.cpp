#include <stdio.h>
#include <input.hpp>
#include <vector>
#include <launcher.hpp>
#include <environment.hpp>
#include <parser.hpp>

int main()
{
    Environment env1 = Environment();

    while (1)
    {
        printf(env1.get_variable("PS1").value.c_str());
        std::string input = read_input();
        std::vector<std::string> tokenized = tokenize_input(input); 
        std::vector<std::vector<std::string>> 
            full = generate_parsed_tokens(tokenized, env1);
        
        int code;
        for (unsigned int i = 0; i < full.size(); i++)
        {
            code = launch_command(full[i], env1);
        }
        /* Specific return codes from launch_commands */
        if (code == 231)
        {
            break;
        }
    }
    return 0;
}
