#include <environment.hpp>
#include <unistd.h>
#include <pwd.h>
#include <string>

EnvironmentVariable::EnvironmentVariable(std::string n, std::string v)
{
    name = n;
    value = v;
}

Environment::Environment()
{
    /* TODO: Read from config files on initialize.*/ 
    
    /* First, get the username of user. */
    /* 
     * Thanks tanpero! 
     * https://github.com/tanpero/getUserName/blob/master/src/getUserName.cc 
     * */
    uid_t userid;
    struct passwd* pwd;
    userid = getuid();
    pwd = getpwuid(userid);
    std::string user = pwd->pw_name;

    /* Now, push environment variables back. */
    variables.push_back(EnvironmentVariable("USER", user));
    variables.push_back(EnvironmentVariable("PWD", "/home/"+user));
    variables.push_back(EnvironmentVariable("PS1", 
                "[ " + get_variable("PWD").value + " ] $ "));
}

EnvironmentVariable Environment::get_variable(std::string name)
{
    for (unsigned long int i = 0; i < variables.size(); i++)
    {
        if (variables[i].name.compare(name) == 0)
        {
            return variables[i];
        }
    }
    return EnvironmentVariable(name, "");
}

int Environment::create_variable(std::string name, std::string value)
{
    variables.push_back(EnvironmentVariable(name, value));
    return 0;
}
