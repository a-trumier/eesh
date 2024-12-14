#include <stdio.h>
#include <input.hpp>
#include <vector>
#include <launcher.hpp>
#include <environment.hpp>
#include <parser.hpp>

int main()
{
    Environment env = Environment();
    printf(env.get_variable("PS1").value.c_str());
    std::string haha = read_input();
    std::vector<std::string> lol = tokenize_input(haha); 
    std::vector<std::vector<std::string>> full = generate_parsed_tokens(lol);

    for (unsigned long int i = 0; i < full.size(); i++)
    {
        printf("Cur command: %ld\n\n", i);
        for (unsigned long int j = 0; j < full[i].size(); j++)
        {
            printf("Cur token: %s\n", full[i][j].c_str());
        }
    }

    launch_command(full[0]);
    return 0;
}
