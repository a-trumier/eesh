#include "../include/input.hpp"

#include <cstdio>
#include <string>
using namespace std;

int main()
{
    string inp = read_input();

    fprintf(stdout, inp.c_str());
    
    return 0;
}
