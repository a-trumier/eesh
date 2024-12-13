#include <vector>
#include <string>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

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

int launch_command(std::vector<std::string> &tokens)
{
    pid_t pid, wpid;
    int status_code;

    char** args = convert_tokens(tokens);

    pid = fork();
    /* Child case */
    if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
        {
            perror("Error: Command invalid.\n");
        }
        return -1;
    }
    else if (pid < 0)
    {
        perror("Error: Forking error.");
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