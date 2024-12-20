#include <path.hpp>
#include <common_classes.hpp>
#include <string>
#include <vector>
#include <stdio.h>


/*
 * Thanks to clearn for this
 *
 * https://www.learnc.net/c-tutorial/c-file-exists/
 */
// return true if the file specified by the filename exists
bool file_exists(std::string filename)
{
    FILE *fp = fopen(filename.c_str(), "r");
    bool is_exist = false;
    if (fp != NULL)
    {
        is_exist = true;
        fclose(fp); // close the file
    }
    return is_exist;
}

/*
 * Take in the PATH variable and parse it into multiple paths.
 */
std::vector<std::string> parse_path_env(std::string p)
{
    std::vector<std::string> return_val;

    std::string cur_str;
    for (unsigned int i = 0; i < p.length(); i++)
    {
        if (p[i] == ':')
        {
            return_val.push_back(cur_str);
            cur_str = "";
        }
        else
        {
            cur_str += p[i];
        }
    }
    
    return return_val;
}

/*
 * Given a path, return the previous directory in that path.
 */
std::string get_prior_directory(std::string p)
{
    for (unsigned long int i = (p.length() - 1); i != 0; i--)
    {
        if (i != p.length() - 1 && p[i] == '/')
        {
            return p.substr(0, i);
        }
    }
    return "/"; /* Catch case */
}


std::string parse_relative_path(std::string p, Environment* env)
{
    /*
     * A relative path is effectively any string which has a forward slash in
     * it that DOES NOT have a forward slash in it. So, a correct relative path 
     * can only begin with a dot, two dots, a tilde, and a name of a directory 
     * or file in there. So, if we do not enounter any dots or tildes, we cant 
     * just return the PWD + the path we have been given.
     */

    std::string ret_path = env->get_value("PWD") + "/";
    std::string temp_working_directory = env->get_value("PWD");
    for (unsigned int i = 0; i < p.length(); i++)
    {
        if (p[i] == '.')
        {
            /* First, ensure that the next character is valid and a dot. */
            if (i+1 < p.length())
                if (p[i+1] == '.')
                {
                    /* This is the case where we just want the prior directory*/
                    ret_path = 
                        get_prior_directory(ret_path);
                    i++; /* Funky but should work? */
                }
            /* 
            * Otherwise, we have a dot that indicates the current working 
            * directory. So do nothing.
            */
        }
        else if (p[i] == '~')
        {
            if (i != 0)
                return ""; /* Invalid expression */
        }
        else
        {
            if (p[i] == '/' && ret_path[ret_path.length() - 1] == '/')
                /* skip adding in this case*/
                ret_path += ""; /* This is really bad case detection TODO fix*/
            else
                ret_path += p[i];
        }

    }
    return ret_path;
}

std::string find_command_path(std::string command, Environment* env)
{
    /* 
     * If we encounter a shell builtin, just pass the command through 
     * and don't worry about it. TODO: Optimize this, perhaps a member in env
     * class or an enum or something. This just causes headache.
     */
    if (command == "exit" || command == "cd" || command == "export"
            || command == "history")
    {
        return command;
    }
    std::string p = env->get_value("PATH");

    if (p.compare("") == 0)
    {
        return "";
    }

    std::vector<std::string> parsed_path = parse_path_env(p);

    for (unsigned int i = 0; i < parsed_path.size(); i++)
    {
        std::string full_path = parsed_path[i] + "/" + command;
        
        if (file_exists(full_path))
        {
            return full_path;
        }
    }
    return "";
}
