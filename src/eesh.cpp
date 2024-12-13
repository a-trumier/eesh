#include <stdio.h>
#include <input.hpp>
#include <vector>
#include <launcher.hpp>
#include <environment.hpp>

int main()
{
    Environment env = Environment();
    printf(env.get_variable("PS1").value.c_str());
    std::string haha = read_input();
    std::vector<std::string> lol = tokenize_input(haha); 

    launch_command(lol);
    return 0;
}
