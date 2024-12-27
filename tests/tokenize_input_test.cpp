#include <input.hpp>

#include <cstdio>
#include <vector>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Error: Incorrect argc. %d\n", argc);
        return 1;
    }

    string arg = string(argv[1]);
    vector<string> tokens = tokenize_input(arg);

    fprintf(stdout, "[");
    for (unsigned long int i = 0; i < tokens.size() - 1; i++)
    {
        fprintf(stdout, (tokens[i] + " ").c_str());
    }
    fprintf(stdout, tokens[tokens.size() - 1].c_str());
    fprintf(stdout, "]");
    return 0;
}
