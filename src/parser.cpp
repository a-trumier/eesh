#include <parser.hpp>

#include <string>
#include <vector>
using namespace std;
/* I hates it! */

vector<vector<string>> generate_parsed_tokens(vector<string> input)
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
                    perror("Error: Invalid use of a comma.\n");
                    return all_commands;
                }
                else
                {
                    /* Just don't do anything. Nice and simple. */
                    all_commands[cur_command].push_back(full_token);
                    all_commands.push_back(vector<string>());
                    cur_command++;
                    should_add = false;
                }
            }
            else
            {
                full_token += input[i][j];
            }
        }
        if (should_add)
            all_commands[cur_command].push_back(full_token);
    }
    return all_commands;
}
