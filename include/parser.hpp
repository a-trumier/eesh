#ifndef __EESH_PARSER__
#define __EESH_PARSER__

#include <common_classes.hpp>

#include <vector>
#include <string>
using namespace std;
/* I dislike this namespace, but it makes the below code more readable. */

/*
 * Takes in tokenized input and outputs multiple ready-to-process commands.
 *
 * Note: This does shell expansion as well as parses special characters
 * into multiple command line arguments. 
 */
vector<vector<string>> generate_parsed_tokens(vector<string> input, 
        Environment* env);

/*
 * Given a string s, ensure that there is no whitespace prior to or after the
 * end of the last character, and ensure all whitespace inbetween characters
 * is a single whitespace.
 */
string clean_whitespace(string s);

#endif
