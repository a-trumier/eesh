#ifndef __EESH_ENVIRONMENT__
#define __EESH_ENIVORNMENT__

#include <string>
#include <vector>

class EnvironmentVariable
{
    public:
        std::string name;
        std::string value;
        EnvironmentVariable(std::string n, std::string v);
};


/*
 * Class which holds all environment variables.
 */
class Environment
{
    private:
        std::vector<EnvironmentVariable> variables;

    public:
        Environment();
        EnvironmentVariable get_variable(std::string name);
        int create_variable(std::string name, std::string value); 
};

#endif
