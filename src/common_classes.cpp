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
     * Thanks tanpero! Note that this is also under the same license as that
     * project
     * https://github.com/tanpero/getUserName 
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

/* HISTORY CLASS */

History::History(std::string path_to_file, int max_size_history)
{
    max_history = max_size_history;
    dotfile_path = path_to_file;

    /* First, open dotfile up for reading */
    dotfile = fopen(dotfile_path.c_str(), "r");
    if (dotfile != NULL)
    {
        /* 
         * Case where we actually have to read in data from file. General idea
         * is to get the file pointer to the end, reverse it until you hit
         * max_history newline chars, then read line by line and put the string
         * in the vector.
         *
         * Thanks David Ranieri!
         * https://stackoverflow.com/questions/17877025/reading-last-n-lines-from-file-in-c-c
         */


        printf("File has opened.\n");
        fseek(dotfile, 0, SEEK_END);
        int pos = ftell(dotfile);
        int num_newlines = 0;
        char cur_char;
        while (pos != -1)
        {
            printf("Cur pos: %i\n", pos);
            fseek(dotfile, pos--, SEEK_SET);
            cur_char = fgetc(dotfile);
            printf("Cur char: %c\n", cur_char);
            if (cur_char == '\n')
            {
                if (num_newlines++ == max_history)
                {
                    break;
                }
            }
        }
        if (pos == -1)
        {
            fseek(dotfile, 0, SEEK_SET);
        }

        /* Now read the file and load the strings into the history list */
        
        char buf[1024]; /* 
                         * TODO: This is probably memory unsafe. Find a way 
                         * to make this not scary, arbitrary buf length.
                         */
        while (fgets(buf, sizeof(buf), dotfile) != NULL)
        {
            std::string temp(buf);
            history_list.push_back(temp);
        }
        fclose(dotfile);
    }
    /* 
     * In the case where the file doesn't exist, a write will happen to make it 
     * exist so no worries right now.
     */

    printf("History has loaded.\n");
    for (unsigned long int i = 0; i < history_list.size(); i++)
    {
        printf("History index %ld\nItem: %s\n\n", i, history_list[i].c_str());
    }
}

int History::pop_back_to_file()
{
    /*
     * This is only called when a command needs to be added to the list but
     * the list is already at max capacity. So, we just pop the back of the
     * list, write that command to file, then add the new file. 
     */

    dotfile = fopen(dotfile_path.c_str(), "a");
    if (dotfile == NULL)
    {
        fprintf(stderr, "Error: Cannot open history file.\n");
        return -1;
    }

    std::string back_command = history_list[history_list.size() - 1];
    
    history_list.pop_back();
    fprintf(dotfile, back_command.c_str());
    fclose(dotfile);
    return 0;
}

int History::write_history_to_file()
{
    dotfile = fopen(dotfile_path.c_str(), "a");

    if (dotfile == NULL)
    {
        fprintf(stderr, "Error: Cannot open history file.\n");
        return -1;
    }

    for (long int i = history_list.size(); i != -1; i--)
    {
        std::string temp = history_list[i];
        fprintf(dotfile, temp.c_str());
    }

    fclose(dotfile);
    return 0;
}

void History::add_to_history(std::string command)
{
    if (history_list.size() == (long unsigned int)max_history)
    {
        pop_back_to_file();
    }

    history_list.insert(history_list.begin(), command + "\n");
}

void History::print_history()
{
    for (unsigned long int i = 0; i < history_list.size(); i++)
    {
        fprintf(stdout, history_list[i].c_str());
    }
}

std::string History::get_history_at_index(int i)
{
    return history_list[i];
}

void History::clear_history()
{
    history_list.clear();
}

int History::get_max_history()
{
    return max_history;
}
