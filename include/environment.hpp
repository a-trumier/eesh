#ifndef __EESH_ENVIRONMENT__
#define __EESH_ENVIRONMENT__

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

        /*
         * Returns 0 if the new value is set, returns 1 if it cannot find
         * the variable.
         */
        int set_variable(std::string name, std::string new_val);
        int create_variable(std::string name, std::string value); 
};

#endif
