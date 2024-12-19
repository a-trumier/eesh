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
    variables.push_back(EnvironmentVariable("HOME", "/home/"+user));
    variables.push_back(EnvironmentVariable("PS1", 
                "[ " + get_variable("PWD").value + " ] $ "));

    /* Constructing path variable */
    std::string temp_path = "";
    /* First, construct user directories */
    /* TODO: Optimize PWD get or get rid of this garbage */
    temp_path += get_variable("PWD").value + "/.local/bin:" 
        + get_variable("PWD").value + "/bin";
    /* Now we can put system wide binary places here.*/
    temp_path += "bin:/usr/local/bin:/usr/local/sbin:/usr/bin:/usr/sbin";
    variables.push_back(EnvironmentVariable("PATH", temp_path));
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

int Environment::set_variable(std::string name, std::string new_val)
{
   for (unsigned int i = 0; i < variables.size(); i++)
   {
       if (variables[i].name.compare(name) == 0)
       {
            variables[i].value = new_val;
            return 0;
       }
   }
   return 1;
}


int Environment::create_variable(std::string name, std::string value)
{
    variables.push_back(EnvironmentVariable(name, value));
    return 0;
}
