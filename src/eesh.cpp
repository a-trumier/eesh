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
        if (string(argv[1]).compare("-c") != 0)
        {
            fprintf(stderr, "Error: only command line argument should be -c\n");
            return 1;
        }
        std::string input = clean_whitespace(string(argv[2]));
        if (input.compare("") == 0)
        {
            return 0;
        }
        else
        {
            std::vector<std::string> tokenized = tokenize_input(input); 
            std::vector<Command> full = generate_commands(tokenized, &env1);

            if (full.size() == 0)
            {
                /* 
                 * This is really bad way to determine if an error has occurred.
                 * Really, this should go in the forked process. TODO: Not be
                 * crap?
                 */
                return 1;
            }
            int code = 0;
            for (unsigned int i = 0; i < full.size(); i++)
            {
                full[i].redirect_streams();
                full[i].expand_variables(&env1);
                code = launch_command(&full[i], &env1);
            }
            if (code > 255)
            {
                /*
                 * In this case, this is the kernel talking to us because
                 * it is a number that is big which include signal number,
                 * if a core dump was produced, and exit code. So we need to
                 * extract the 8th to 15th bits of this number.
                 * 
                 * TY geeks for geeks, helping!
                 * https://www.geeksforgeeks.org/extract-bits-in-c/
                 */
                int bitwise_shenan = 0;

                int begin = 8, end = 15;
                for (int i = begin; i <= end; i++)
                {
                    bitwise_shenan = bitwise_shenan | (1 << i);
                }
                return (code & bitwise_shenan) >> begin;
            }
            else
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
        if (full.size() == 0)
        {
            code = 1;
        }
        for (unsigned int i = 0; i < full.size(); i++)
        {
            full[i].redirect_streams();
            full[i].expand_variables(&env1);
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
