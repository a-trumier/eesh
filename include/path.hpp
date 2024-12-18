#ifndef __EESH_PATH__
#define __EESH_PATH__

#include <environment.hpp>
#include <vector>
#include <string>


/*
 * Takes in a path p and and environment env and returns the absolute path
 * of the file. Returns empty string if file path invalid.
 *
 * Note: The reason for getting the environment is to get the PWD variable.
 * Later changes may override this necessity and just pass in the PWD
 * variable
 */
std::string parse_relative_path(std::string p, Environment* env);

/*
 * Given a command, return the absolute path of the command. Return empty
 * string if no command is found.
 */
std::string find_command_path(std::string command, Environment* env);

#endif
