#include <stdio.h>
#include "../input.hpp"
#include <vector>
#include "../launcher.hpp"

int main()
{
    std::string haha = read_input();
    std::vector<std::string> lol = tokenize_input(haha);
    printf("Input: %s\n", haha.c_str());

    for (long unsigned int i = 0; i < lol.size(); i++)
    {
        printf("Token %ld: %s\n", i, lol[i].c_str());
    }

    launch_command(lol);
    return 0;
}
