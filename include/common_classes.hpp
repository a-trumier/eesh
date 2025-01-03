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
 * The command class that contains tokenized input.
 */
class Command
{
    private:

        std::vector<std::string> tokens;
        
        /* The three output streams */
        std::string in;
        std::string out;
        std::string err;

        /* Whether or not we should append to the stdout and stderr. */
        bool out_app;
        bool err_app;

        Command* piped_to;

    public:

        /*
         * Constructs command with tokens t.
         */
        Command(std::vector<std::string> t);

        /* 
         * Returns the pointer array that contains the tokens of the array,
         * followed by NULL. Used to pass into execvp.
         */
        char** get_tokens();

        std::vector<std::string> get_tokens_vec();
        
        /*
         * Frees a reference to tokens generated by get_tokens()
         */
        void free_tokens(char** free_vals, long unsigned int size);

        void add_token(std::string tok);

        /*
         * Returns the actual command name with no flags, i.e. the first
         * item in the vector.
         */
        std::string get_command();

        void set_command(std::string new_cmd);

        /*
         * Attempts to find output redirections. Returs true if it worked
         * correctly, false if not.
         */
        bool redirect_streams();

        void expand_variables(Environment* env);

        std::string get_in();
        std::string get_out();
        std::string get_err();

        bool get_out_app();
        bool get_err_app();
};

#endif
