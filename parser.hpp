#ifndef __EESH_PARSER__
#define __EESH_PARSER__

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
vector<vector<string>> generate_parsed_tokens(vector<string> input);

#endif
