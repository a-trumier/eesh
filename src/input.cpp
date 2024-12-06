#include <string>
#include <cstdio>
#include :


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
