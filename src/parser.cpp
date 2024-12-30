#include <parser.hpp>
#include <common_classes.hpp>
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

vector<Command> generate_commands(vector<string> input, 
        Environment *env)
{
    vector<Command> all_commands;
    all_commands.push_back(Command(vector<string>()));

    int cur_command = 0;

    for (unsigned long int i = 0; i < input.size(); i++)
    {
        string full_token = "";
        bool should_add = true;
        bool quotation = false;
        bool dbl_quote = false;
        
        if (input[i].compare("&&") == 0)
        {
            all_commands.push_back(Command(vector<string>()));
            cur_command++;
            continue;
        }
        if (input[i].compare("|") == 0)
        {
            Command new_cmd = Command(vector<string>());
            all_commands[cur_command].set_pipe(&new_cmd);
            all_commands.push_back(new_cmd);
            cur_command++;
            continue;
        }
        for (unsigned long int j = 0; j < input[i].length(); j++)
        {
            /* 
             * If the current character we are looking at is a semicolon
             * AND is the last of the string, then bingo we can create a 
             * new command.
             */
            if (input[i][j] == ';' && !quotation)
            {
                if (j != input[i].length() - 1)
                {
                    /* Error out in this case */
                    fprintf(stderr, "Error: Invalid use of a semi-colon.\n");
                    return all_commands;
                }
                else
                {
                    /* Just add command and create new one. Nice and simple. */
                    all_commands[cur_command].add_token(full_token);
                    all_commands.push_back(Command(vector<string>()));
                    cur_command++;
                    should_add = false;
                }
            }
            /* Single quote statements */
            else if (input[i][j] == 39 && !quotation)
            {
                quotation = true;
            }
            else if (input[i][j] == 39 && quotation)
            {
                quotation = false;
            }
            else if (input[i][j] == '"')
            {
                if (dbl_quote)
                    dbl_quote = false;
                else
                    dbl_quote = true;
            }
            else
            {
                full_token += input[i][j];
            }
        }
        if (should_add)
            all_commands[cur_command].add_token(full_token);
        if (quotation)
        {
            /*
             * This only happens if we reach the end of a token and have not
             * found a second quotation mark. In this case, say its invalid
             * and don't do anything.
             */
            fprintf(stderr, "Error: Invalid use of quotation mark.\n");
            return vector<Command>();
        }
        if (dbl_quote)
        {
            fprintf(stderr, "Error: Invalid use of quotation mark.\n");
            return vector<Command>();
        }
    }

    for (unsigned int i = 0; i < all_commands.size(); i++)
    {
        if (all_commands[i].get_command().compare("") == 0)
        {
            fprintf(stderr, "Error: Invalid command.\n");
            vector<Command> s;
            return s;
        }
        all_commands[i].set_command(find_command_path
                (all_commands[i].get_command(), env));
        int ret_code = expand_relative_paths
            (all_commands[i].get_tokens_vec(), env);
        if (ret_code == -1)
        {
            vector<Command> s;
            return s;
        }
    }
    printf("Returning commands\n");
    return all_commands;
}

string clean_whitespace(string s)
{
    string ret_s = "";
    unsigned long int cur_counter = 0;
    /* First, clean whitespace prior to first character */
    while (cur_counter < s.length())
    {
        if (s[cur_counter] != ' ')
        {
            break;
        }
        cur_counter++;
    }
    /* Clean the rest of the string */
    while (cur_counter < s.length())
    {
        if (cur_counter != 0 && s[cur_counter] == ' ')
        {
            if (s[cur_counter-1] == ' ')
            {    
                cur_counter++;
                continue;
            }
            else
                ret_s += s[cur_counter];
        }
        else
        {
            ret_s += s[cur_counter];
        }
        cur_counter++;
    }

    /* Clean the final character */
    if (ret_s[ret_s.length() - 1] == ' ')
    {
        ret_s = ret_s.substr(0, ret_s.length() - 1);
    }

    return ret_s;
}
