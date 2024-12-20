#Shell Builtin Commands

## exit

### General Form

```bash
exit
```

The simplest shell builtin, when executed it exits the shell. Any command line
arguments given to exit are ignored. So,

```bash
exit
```

and

```bash
exit foo bar
```

are treated the same way.

## cd

### General Form

```bash
cd [directory to change to]
```

The cd command, or change directory command, changes the current directory of
the shell to a specified directory, which is the second argument to the command.
Note that both relative and absolute paths are supported. Suppose you have a
directory /home/foo/bar, and you are currently in the home directory, and you
want to go the bar directory, you can do either of the following:

```bash
cd foo/bar
```

or

```bash
cd /home/foo/bar
```

If you supply more than one argument or no command line arguments, you will be
presented with an error.
> **_NOTE:_**  You can find where the shell currently is by using echo $PWD.

This command also works with the dot operators as well as the home directory
operator. If you want to return to your home directory (which is calculated by
/home/"your current username"), you can do the following::

```bash
cd ~
```

Also, if you would want to go to the previous directory, you can use the .. 
operator:

```bash
cd ..
```

## export

### General Form

```eesh
export [variable name] [value]
```

The export command takes in two command line arguments: the variable name and
the value that you would like the variable to have. So, if you would like to
have a shell variable named FOO with the value bar, the command would be:

```eesh
export FOO bar
```

> **_NOTE:_**: While yes, it is technically possible to have a shell variable
> with special characters in it (such as /, |, or $), it is unadvised as it
> makes it confusing to keep track of syntax.

### Aside: Shell Variable Expansion

Like most other UNIX-like shells, eesh uses the dollar sign ($) character to
perform shell variable expansion. For an example, imagine we have used the
following export command:

```eesh
export FOO /home/foo/bar
```

We could use a shell variable expansion as follows to change into the directory
of FOO:

```eesh
cd $FOO
```
