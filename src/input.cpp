#include <string>
#include <cstdio>
#include <vector>


std::string read_input(void)
{
    /*
     * Code adapted from Brennan's code, thanks!
     *
     * brennan.io/2015/01/16/write-a-shell-in-c/
     * */
    int cur_char;
    std::string buf;

    while (1)
    {
        cur_char = getchar();

        /* 
         * Case where user wants us to actually do a command. This will
         * be passed to the tokenizer.
         * */

        if (cur_char == EOF || cur_char == '\n')
        {
            return buf;
        }

        /* Otherwise, just add to the buffer and iterate */
        else
        {
            buf += cur_char;
        }
    }
}

std::vector<std::string> tokenize_input(std::string &inp)
{
    std::vector<std::string> output;

    long unsigned int i = 0;

    std::string cur_token = "";
    while (i < inp.length())
    {
        /* White space delimting a token. */
        if (inp[i] == ' ')
        {

            if (cur_token == "")
            {
                perror("Error: Command cannot begin with a whitespace.\n");
                return output;
            }
            else
            {
                output.push_back(cur_token);
                cur_token = "";
            }
        }
        else
        {
            cur_token += inp[i];
        }
        i++;
    }
    return output;
}
