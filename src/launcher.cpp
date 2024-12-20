#include <launcher.hpp>
#include <path.hpp>
#include <common_classes.hpp>
#include <input.hpp>
#include <parser.hpp>

#include <vector>
#include <string>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <filesystem>


/*
 * Takes a vector of strings and mallocs an array for passing into execvp
 *
 * Note: This is because exevp() requires a char** array to exec. Therefore,
 * we have to convert our nice vector into a char**
 */
char** convert_tokens(std::vector<std::string> &tokens)
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

/*
 * Frees an array of tokens.
 */
void free_tokens(char** free_vals, long unsigned int size)
{
    for (long unsigned int i = 0; i < size; i++)
    {
        free(free_vals[i]);
    }
    free(free_vals);
}

int builtin_exit()
{
    return 231;
}

int builtin_cd(std::vector<std::string> &tokens, Environment* env)
{
    if (tokens.size() > 2)
    {
        fprintf(stderr, "Error: Too many arguments\n");
        return -1;
    }
    if (tokens.size() == 1)
    {
        fprintf(stderr, "Error: Too few arguments\n");
        return -1;
    }

    if (tokens[1][0] == '/')
    {
        /* Absolute path. So who cares! Thats our new PWD */
        std::filesystem::path p = tokens[1];
        if (!std::filesystem::is_directory(p))
        {
            fprintf(stderr, "Error: Path given to cd is not a directory\n");
            return 1;
        };
        env->set_variable("PS1", "[ " + tokens[1] + " ] $ ");
        return 0;
    }
    else
    {
        /* Convert to absolute and then pop it in. */
        std::string correct_path = parse_relative_path(tokens[1], env);
        std::filesystem::path p = correct_path;
        if (!std::filesystem::is_directory(p))
        {
            fprintf(stderr, "Error: Path given to cd is not a directory\n");
            return 1;
        }
        env->set_variable("PWD", correct_path);
        return 0;
    }
}

int builtin_export(std::vector<std::string> &tokens, Environment* env)
{

    /* 
     * Export command. Allows creation of a new variable, so just
     * error check that we have two tokens. Export works a bit differently
     * in eesh
     */

    if (tokens.size() != 3)
    {
        perror("Error: Given export not valid.\n");
        return 1;
    }

    env->set_variable(tokens[1], tokens[2]);
    return 0;
}

int builtin_history(std::vector<std::string> &tokens, Environment* env,
        History* his)
{
    if (tokens.size() != 2 && tokens.size() != 1)
    {
        fprintf(stderr, "Error: History given incorrect amount of arguments\n");
        return 1;
    }

    if (tokens.size() == 1)
    {
        fprintf(stdout, 
        "History Help\n\nTo use history command, use history [option].");
        fprintf(stdout, 
        "\n\nOptions:\nlist - lists all items in history\nclear - clears history");
        fprintf(stdout, 
        "\n<any number> - executes command at history index given in list\n");
        return 0;
    }
    else
    {
        if (tokens[1].compare("list") == 0)
        {
            his->print_history();
            return 0;
        }
        else if (tokens[1].compare("clear") == 0)
        {
            his->clear_history();
            return 0;
        }
        else
        {
            int x = std::stoi(tokens[1]);
            x--;
            if (x < 0 || x >= his->get_max_history())
            {
                fprintf(stderr, 
                        "Error: Invalid number given to history command.\n");
                return 1;
            }
            std::string input = his->get_history_at_index(x);
            std::vector<std::vector<std::string>> true_command = 
                generate_parsed_tokens(tokenize_input(input), env);
            launch_command(true_command[0], env, his);
            return 0;
        }
    }
}

int launch_command(std::vector<std::string> &tokens, Environment* env,
        History* his)
{
    pid_t pid, wpid;
    int status_code;

    /*
     * This elif block looks awful.
     *
     * TODO: Change to switch statement.
     */
    if (tokens[0].length() == 0)
    {
        /*
         * Solves weird error with perror call, when I replace this with perror
         * it prints no such file or directory even though i'm not forking.
         * Probably an RTFM moment but I cannot be bothered at this moment.
         * TODO: Fix this garbage?
         */
        fprintf(stderr, 
                "Error: Command not found. Is it in your PATH variable?\n");
        return 1;
    }

    else if (tokens[0].compare("exit") == 0)
    {
        /* Exit code */
        return builtin_exit();
    }
    
    else if (tokens[0].compare("cd") == 0)
    {
        return builtin_cd(tokens, env);
    }

    else if (tokens[0].compare("export") == 0)
    {
        return builtin_export(tokens, env);
    }
    else if (tokens[0].compare("history") == 0)
    {
        return builtin_history(tokens, env, his);
    }
    else
    {

        char** args = convert_tokens(tokens);

        pid = fork();
        /* Child case */
        if (pid == 0)
        {
            std::string change_path = env->get_value("PWD") + "/";
            int r_val = chdir(change_path.c_str());
            if (r_val < 0)
            {
                printf("Chdir error: %d\n", r_val);
                exit(0);
            }
            if (execvp(args[0], args) == -1)
            {
                perror("Error");
                /* Need to exit here because we are in child process. */
                exit(1);
            }
            /* Should never get to this level. Failsafe */
            exit(1);
        }
        else if (pid < 0)
        {
            perror("Error: Forking error.\n");
            return -1;
        }
        else
        {
            do
            {
                wpid = waitpid(pid, &status_code, WUNTRACED);
            } 
            while (!WIFEXITED(status_code) && !WIFSIGNALED(status_code));
        }

        free_tokens(args, tokens.size() + 1);
        return 0;
    }
}
