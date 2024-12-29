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
    std::string home_dir = pwd->pw_dir;


    /* Set basic environment variables */
    variables["USER"] = user;
    variables["PWD"] = home_dir;
    variables["HOME"] = home_dir;
    variables["PS1"] = "$ ";

    /* Constructing path variable */
    std::string temp_path = "";
    /* First, construct user directories */
    temp_path += home_dir + "/.local/bin:" + home_dir + "/bin";
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

Command::Command(std::vector<std::string> t)
{
    tokens = t;
    in = "";
    out = "";
    err = "";
    piped_to = nullptr;
}

bool Command::redirect_streams()
{

    /*
     * We need to check if in the
     * tokens we have been given contain an output redirection. 
     */

    std::vector<std::string> new_tokens;
    for (unsigned int i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].compare(">") == 0 || tokens[i].compare("1>") == 0)
        {
            if (i == tokens.size() - 1)
            {
                fprintf(stderr,
                        "Error: Output redirection must be given a file.\n");
                return false;
            }

            if (out.compare("") != 0)
            {
                fprintf(stderr, 
                        "Error: Command cannot have multiple redirections.\n");
                return false;
            } 

            /* 
             * Case where we can put the next token as stdout, with
             * no append.
             */
            out = tokens[i+1];
            out_app = false;
            i++;
        }
        else if (tokens[i].compare(">>") == 0|| tokens[i].compare("1>>") == 0)
        {
            if (i == tokens.size() - 1)
            {
                fprintf(stderr, 
                        "Error: Output redirection must be given a file\n");
                return false;
            }

            if (out.compare("") != 0)
            {
                fprintf(stderr, 
                        "Error: Command cannot have multiple redirections.\n");
                return false;
            } 
            /*
             * Case where we can actually do an append. 
             */
            out = tokens[i+1];
            out_app = true;
            i++;
        }
        else if (tokens[i].compare("2>") == 0)
        {
            if (i == tokens.size() - 1)
            {
                fprintf(stderr,
                        "Error: Output redirection must be given a file\n");
                return false;
            }
            
            if (err.compare("") != 0)
            {
                fprintf(stderr, 
                        "Error: Command cannot have multiple redirections.\n");
                return false;
            } 
            err = tokens[i+1];
            err_app = false;
            i++;
        }
        else if (tokens[i].compare("2>>") == 0)
        {

            if (i == tokens.size() - 1)
            {
                fprintf(stderr,
                        "Error: Output redirection must be given a file\n");
                return false;
            }
        
            if (err.compare("") != 0)
            {
                fprintf(stderr, 
                        "Error: Command cannot have multiple redirections.\n");
                return false;
            } 
            err = tokens[i+1];
            err_app = true;
            i++;
        }
        else if (tokens[i].compare("2>&1") == 0)
        {
            if (err.compare("") != 0)
            {
                fprintf(stderr, 
                        "Error: Command cannot have multiple redirections.\n");
                return false;
            }

            if (out.compare("") == 0)
            {
                fprintf(stderr,
                        "Error: Command must have prior redirection to use this operator\n");
            }
            err = out;
            err_app = out_app;
            i++;
        }
        else if (tokens[i].compare("1>&2") == 0)
        {

            if (out.compare("") != 0)
            {
                fprintf(stderr, 
                        "Error: Command cannot have multiple redirections.\n");
                return false;
            }

            if (err.compare("") == 0)
            {
                fprintf(stderr,
                        "Error: Command must have prior redirection to use this operator\n");
            }
            out = err;
            out_app = err_app;
            i++;
        }
        else if (tokens[i].compare("<") == 0)
        {

            if (i == tokens.size() - 1)
            {
                fprintf(stderr,
                        "Error: Input redirection must be given a file\n");
                return false;
            }
        
            if (in.compare("") != 0)
            {
                fprintf(stderr, 
                        "Error: Command cannot have multiple redirections.\n");
                return false;
            } 
            in = tokens[i+1];
            i++;
        }
        else
        {
            new_tokens.push_back(tokens[i]);
        }
    }
    tokens = new_tokens;
    return true;
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

std::string Command::get_in()
{
    return in;
}

std::string Command::get_out()
{
    return out;
}

std::string Command::get_err()
{
    return err;
}

bool Command::get_out_app()
{
    return out_app;
}

bool Command::get_err_app()
{
    return err_app;
}

void Command::expand_variables(Environment* env)
{
    for (unsigned long int i = 0; i < tokens.size(); i++)
    {
        if (tokens[i][0] == '$')
        {
            std::string var = tokens[i].substr(1);
            tokens[i] = env->get_value(var);
        }
    }
}
