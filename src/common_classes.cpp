#include <common_classes.hpp>
#include <unistd.h>
#include <pwd.h>
#include <string>
#include <unordered_map>

/* ENVIRONMENT CLASS */

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

    /* Set basic environment variables */
    variables["USER"] = user;
    variables["PWD"] = "/home/" + user;
    variables["HOME"] = "/home/" + user;
    variables["PS1"] = "$ ";

    /* Constructing path variable */
    std::string temp_path = "";
    /* First, construct user directories */
    /* TODO: Optimize PWD get or get rid of this garbage */
    temp_path += get_value("PWD") + "/.local/bin:" 
        + get_value("PWD") + "/bin";
    /* Now we can put system wide binary places here.*/
    temp_path += ":/usr/local/bin:/usr/local/sbin:/usr/bin:/usr/sbin";
    variables["PATH"] = temp_path;
}

std::string Environment::get_value(std::string name)
{
    return variables[name];
}

void Environment::set_variable(std::string name, std::string val)
{
    variables[name] = val;
}
