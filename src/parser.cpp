#include <parser.hpp>
#include <environment.hpp>
#include <path.hpp>

#include <string>
#include <vector>
using namespace std;
/* I hates it! */

/*
 * Changes relative paths to absolute paths.
 *
 * Note: Returns -1 if ANY relative path in the set is bad. Otherwise
 * returns 0.
 */
int expand_relative_paths(vector<string> input, Environment* env)
{
    for (unsigned long int i = 0; i < input.size(); i++)
    {
        bool is_relative = false;
        for (unsigned long int j = 0; j < input[i].length(); j++)
        {
            if (input[i][j] == '/' && j == 0)
            {
                break; /* This is because this is absolute, so break out */
            }

            if (input[i][j] == '/')
            {
                is_relative = true;
                break;
            }
        }
        if (is_relative)
        {
            string parsed = parse_relative_path(input[i], env);
            if (parsed.compare("") == 0)
            {
                return -1;
            }
            else
            {
                input[i] = parsed;
            }
        }   
    }
    return 0;
}

vector<vector<string>> generate_parsed_tokens(vector<string> input, 
        Environment *env)
{
    vector<vector<string>> all_commands;
    all_commands.push_back(vector<string>());

    int cur_command = 0;

    for (unsigned long int i = 0; i < input.size(); i++)
    {
        string full_token = "";
        bool should_add = true;
        for (unsigned long int j = 0; j < input[i].length(); j++)
        {
            /* 
             * If the current character we are looking at is a semicolon
             * AND is the last of the string, then bingo we can create a 
             * new command.
             */
            if (input[i][j] == ';')
            {
                if (j != input[i].length() - 1)
                {
                    /* Error out in this case */
                    perror("Error: Invalid use of a semi-colon.\n");
                    return all_commands;
                }
                else
                {
                    /* Just add command and create new one. Nice and simple. */
                    all_commands[cur_command].push_back(full_token);
                    all_commands.push_back(vector<string>());
                    cur_command++;
                    should_add = false;
                }
            }
            else if (input[i][j] == '$')
            {
                string name = "";
                for (unsigned int k = (j + 1); k < input[i].size(); k++)
                {
                    name += input[i][k];
                }
                full_token += env->get_variable(name).value;
                break;
            }
            else
            {
                full_token += input[i][j];
            }
        }
        if (should_add)
            all_commands[cur_command].push_back(full_token);
    }

    for (unsigned int i = 0; i < all_commands.size(); i++)
    {
        all_commands[i][0] = find_command_path(all_commands[i][0], env);
        int ret_code = expand_relative_paths(all_commands[i], env);
        if (ret_code == -1)
        {
            perror("Error: Invalid relative path.\n");
            vector<vector<string>> s;
            return s;
        }
    }
    return all_commands;
}
