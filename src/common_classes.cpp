#include <common_classes.hpp>
#include <unistd.h>
#include <pwd.h>
#include <string>
#include <unordered_map>
#include <cstring>
/* ENVIRONMENT CLASS */

Environment::Environment()
{
    /* TODO: Read from config files on initialize.*/ 
    
    /* First, get the username of user. */
    /* 
     * Thanks tanpero! 
     * https://github.com/tanpero/getUserName/blob/master/src/getUserName.cc 
     * */
    uid_t userid;
    struct passwd* pwd;
    userid = getuid();
    pwd = getpwuid(userid);
    std::string user = pwd->pw_name;

    /* Set basic environment variables */
    variables["USER"] = user;
    variables["PWD"] = "/home/" + user;
    variables["HOME"] = "/home/" + user;
    variables["PS1"] = "$ ";

    /* Constructing path variable */
    std::string temp_path = "";
    /* First, construct user directories */
    /* TODO: Optimize PWD get or get rid of this garbage */
    temp_path += get_value("PWD") + "/.local/bin:" 
        + get_value("PWD") + "/bin";
    /* Now we can put system wide binary places here.*/
    temp_path += ":/usr/local/bin:/usr/local/sbin:/usr/bin:/usr/sbin";
    variables["PATH"] = temp_path;
}

std::string Environment::get_value(std::string name)
{
    return variables[name];
}

void Environment::set_variable(std::string name, std::string val)
{
    variables[name] = val;
}

/* COMMAND CLASS */

Command::Command(std::vector<std::string> t, std::string i, std::string o, 
        std::string e, Command* p)
{
    tokens = t;
    in = i;
    out = o;
    err = e;
    piped_to = p;
}

char** Command::get_tokens()
{
    char** ret_val = (char**) malloc(sizeof(char*)*(tokens.size() + 1));
    for (long unsigned int i = 0; i < tokens.size(); i++)
    {
        ret_val[i] = (char*) malloc(sizeof(char) * (tokens[i].length() + 1));
        strcpy(ret_val[i], tokens[i].c_str());
    }
    ret_val[tokens.size()] = NULL;
    return ret_val;
}

void Command::free_tokens(char** free_vals, long unsigned int size)
{
    for (long unsigned int i = 0; i < size; i++)
    {
        free(free_vals[i]);
    }
    free(free_vals);
}

void Command::add_token(std::string tok)
{
    tokens.push_back(tok);
}

std::string Command::get_command()
{
    return tokens[0];
}

void Command::set_command(std::string new_cmd)
{
    tokens[0] = new_cmd;
}

std::vector<std::string> Command::get_tokens_vec()
{
    return tokens;
}
