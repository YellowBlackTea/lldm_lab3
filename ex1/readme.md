# Exercice 1

## What was used

From what was given in `lab3.c` file:
- **Read a command line** was already written.
- **Special entries**: when user input a special command line (NULL, exit, &), how the program will react to it.
- **Parse the command line** uses the function `strtok` in `string.h` library. The process is the same, I have put an array of delimiters (whitespaces,\n, &), which is going to separate the command line into tokens. 
- **Executing of the program** is basically just a `fork()`. The `sleep(1)` is here merely to show it nicely in the shell. It can be removed.
```bash
if(execvp(args[0], args) == -1){
    if(strcmp(line,"exit") || strcmp(line,"")){    // this condition is added because it was printing something went wrong every time
        exit(0);
    }
    else{
        printf("\n Something went wrong ... \n");
        exit(EXIT_FAILURE);
    }
}
```
A modification was done here, because when exit was input, "Something went wrong" were printed. It actually printed for every command line that does not exist (ww, yy, help) and for command line I create (like exit). The same will be done in Exercice 2 for the history command line.

## Running the code

Open the terminal and run this code normally (can only be run in the terminal shell and NOT in an IDE).

```bash
gcc -o ex1 ex1.c
./ex1
```


