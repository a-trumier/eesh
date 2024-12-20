#ifndef __EESH_COMMON_CLASSES__
#define __EESH_COMMON_CLASSES__

#include <string>
#include <vector>
#include <unordered_map>

/*
 * Class which holds all environment variables.
 */
class Environment
{
    private:
        std::unordered_map<std::string, std::string> variables;

    public:
        Environment();
        
        /*
         * Returns the value of the environment variable with given name,
         * returns empty string if not found. 
         */
        std::string get_value(std::string name);

        /*
         * Sets the variable with string name to the value val.
         */
        void set_variable(std::string name, std::string new_val);
};

/*
 * Holds the history of the user in a buffer.
 */
class History
{
    private:
        std::string dotfile_path;
        FILE* dotfile;
        std::vector<std::string> history_list;
        int max_history;

        /*
         * Writes changes to history to file. 
         */
        int write_history_to_file();

        /*
         * Pops the backmost item from history_list into the file.
         */
        int pop_back_to_file();
    public:
        /*
         * Constructor
         */
        History(std::string path_to_file, int max_size_history);

        /*
         * Adds a given command to the history list.
         */
        void add_to_history(std::string command);

        /*
         * Print history list to standard out, with numbers.
         */
        void print_history();

        /*
         * Get a command in history at index i.
         */
        std::string get_history_at_index(int i);

        /*
         * Clears history in buffer.
         */
        void clear_history();

        int get_max_history();
};

#endif
