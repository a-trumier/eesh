#include <stdio.h>
#include <input.hpp>
#include <vector>
#include <launcher.hpp>
#include <common_classes.hpp>
#include <parser.hpp>
#include <path.hpp>

int main(int argc, char* argv[])
{

    Environment env1 = Environment();
    if (argc == 3)
    { 
        std::string input = clean_whitespace(string(argv[2]));
        if (input.compare("") == 0)
        {
            return 0;
        }
        else
        {
            std::vector<std::string> tokenized = tokenize_input(input); 
            std::vector<Command> full = generate_commands(tokenized, &env1);

            int code = 0;
            for (unsigned int i = 0; i < full.size(); i++)
            {
                full[i].redirect_streams();
                full[i].expand_variables(&env1);
                code = launch_command(&full[i], &env1);
                fprintf(stderr, "Command code = %i ", code);
            }
            return code;
        }
    }
    
    while (1)
    {
        fprintf(stdout, env1.get_value("PS1").c_str());
        std::string input = clean_whitespace(read_input());
        if (input.compare("") == 0)
        {
            continue;
        }
        std::vector<std::string> tokenized = tokenize_input(input); 
        std::vector<Command> full = generate_commands(tokenized, &env1);

        int code = 0;
        for (unsigned int i = 0; i < full.size(); i++)
        {
            full[i].redirect_streams();
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
