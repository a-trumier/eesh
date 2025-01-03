#include <input.hpp>

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
        if (cur_char >= 32 && cur_char <= 255 && cur_char != 160 &&
                cur_char != 127)
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
    bool quotation = false;
    while (i < inp.length())
    {

        /* White space delimting a token. */
        if ((inp[i] == ' ' && (i + 1) < inp.length() && !quotation)
                || (inp[i] == '\n' && (i + 1) < inp.length()) 
                || (inp[i] == EOF && (i + 1) < inp.length()))
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
            /*
             * This part is really bad. TODO: Make this statement not look
             * terrible
             */
            if (inp[i] == '"' && !quotation)
            {
                quotation = true;
                cur_token += inp[i];
            }
            else if (inp[i] == '"' && quotation)
            {
                quotation = false;
                cur_token += inp[i];
            }
            else
                cur_token += inp[i];
        }
        i++;
    }

    if (cur_token.back() == ' ')
        cur_token.pop_back();
    output.push_back(cur_token);

    return output;
}
