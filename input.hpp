#ifndef __INPUT_EESH__
#define __INPUT_EESH__

#include <string>

/*
 * Take in input from the stdin and returns a string of the input.
 *
 * Note: The input is delimited by EOF or newline.
 */
std::string read_input(void);

/*
 * Takes a string input inp, tokenizes it, and places the tokens into a vec.
 */
std::vector<std::string> tokenize_input(std::string &inp);

#endif
