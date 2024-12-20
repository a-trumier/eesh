# eesh Main Loop

eesh uses a simplistic loop to complete user commands. In this loop there are
4 main steps:

1. Read user input and sanitize whitespace
2. Tokenize input
3. Parse tokens
4. Execute command(s)

Within this documentation we will delve into each step.

## Step One: Read user input

The first step is the most simple. We very easily read the user input character
by character into a buffer with the read_input() function. When we hit the
EOF or newline character, that is the end of the user input and we pass the
result of the function to the clean_whitespace() function. clean_whitespace()
does as advertised: it deletes all whitespace in front and behind the string,
as well as truncates all whitespace within the string to only be one whitespace
character at a time. For example the string within the braces:

```
[eesh  adsw    23  ]
```

becomes

```
[eesh adsw 23]
```

## Step Two: Tokenize input

Once the user's input has been taken, we must tokenize the input. Each token
is a part of a command, which is enclosed by whitespace. For example, the 
string

```
eesh foo bar
```

has three tokens: eesh, foo, and bar, as they all are delimited by whitespace.
To do this, we call the tokenize_input() function, which returns each of the
tokens inside a std::vector. 

tokenize_input() is relatively simple: it iterates through the input string,
adding to the current token if the current character is not a whitespace. Once
it reaches a whitespace character, it pushes the new token to the back of the 
return vector and resets the current token. It does this until the end of the
string.

## Step Three: Parse Tokens

Once each token has been parsed, and put into a std::vector, eesh calls
the generated_parsed_tokens() function. The function returns a std::vector
of std::vectors of strings, because in any given user input there may be 
multiple commands. In this step, generate_parsed_tokens expands shell variables,
expands relative paths, and finds the path of the actual command being carried
out.

This is where a core concept of eesh is presented. At a system level, 
everything that eesh gives to the operating system is in absolute paths.
This is because the main design philosophy of eesh is that it has *preemtive*
error checking. eesh ensures that commands exist and they are relatively
error free *before* forking a process and executing it.

## Step Four: Execute command(s)

Once tokens have been parsed and eesh knows how many commands to execute,
it simply creates a loop and iterates over all commands that have been giving,
passing them to launch_command(). The function is simple: it checks first
to see if it is a builtin function. If so, eesh does what is required. If the
command is not builtin, it forks a process and passes the command to the
operating system.
