#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE 1024 /* The maximum length command */
#define MAX_ARGS 128 /* The maximum number of arguments */
static char line[MAX_LINE];


int main(void) {
    char *args[MAX_ARGS]; /* command line arguments */
    int should_run = 1; /* flag to determine when to exit program */

    int line_size;
    char *token;
    const char delimiter[] = " \n&";
    int ampersand = 0;          // flag for &
    int pos_token = 0;          //index of token

    pid_t pid;

    char hist[10][MAX_LINE];    // array where history is going to be saved
    int hist_count = 0;
    int i, j;        

    while (should_run) {
        printf("lldm>");
        fflush(stdout);

        /**************************************
        ******* Read a command line *********
        ***************************************/
        if (!fgets(line, 1024, stdin))
            return 0;
        //for(int pos = 0; pos<strlen(line); printf("line %c at index %i",line[pos], pos), pos++);
        
        /**************************************
        ********** Special entries ************
        ***************************************/

        /* type exit to exit the shell */
        if (strcmp(line, "exit\n") == 0) {
            printf("processing the exit...\n");
            //break;
            should_run = 0;
        }

        /* check for ampersand */
        line_size = (int) strlen(line);
        if(line[line_size-2] == '&'){
            //printf("ampersand is true\n");
            ampersand = 1;
        }

        /* type history to show the history */
        if(strcmp(line, "history\n") == 0){
            if(hist_count == 0){      // if the only command was history, then exit 
                printf("History is empty\n");
            }
            // printing the last 10 command lines 
            if(hist_count < 10){
                for(i = hist_count; i>0; i--){
                    printf("%d\t%s", i, hist[i%10]);
                }
            }else{
                for(i = hist_count, j = 10; i > 0 && j > 0; i--, j--){  // double variables for the numbering to be right (i) and to only printf it 10 times (j)
                    printf("%d\t%s", i, hist[i%10]);
                    //printf("index i: %d\t index j: %d\t hist count: %d\n", i, j, hist_count);
                }
            }
        }

        /* copy input into history */
        hist_count++;
        strcpy(hist[hist_count%10],line);
        //printf("copy is %s at index %d\n", hist[hist_count%10], hist_count);

        /**************************************
        ******* Parse the command line ********
        ***************************************/
        token = strtok(line, delimiter);    // breaks line into a series of tokens
        while(token != NULL){               // the process is repeated until no more token is found
            args[pos_token] = token;
            //printf("%s at index %d and token %s\n", args[pos_token], pos_token, token);
            pos_token++;
            
            // check if exceeding max_args
            if(pos_token>=MAX_ARGS){
                printf("Error not enough allocate memory space\n");
                exit(EXIT_FAILURE);
            }
            token = strtok(NULL, delimiter);
        }
        pos_token = 0;      // to reset all the arguments read for a new command line
        //printf("AT THE END OF THE LOOP: %s at index %d and token %s\n", args[pos_token], pos_token, token);
        
        
        /**************************************
        ******* Execution of the program ******
        ***************************************/
        pid = fork();
        if(pid < 0){ 
            printf("Error fork!\n");
            exit(EXIT_FAILURE);
        }
        if(pid == 0){ //child process
            //printf("child is being executed\n");
            if(execvp(args[0], args) == -1){
                if(strcmp(line, "history") || strcmp(line,"exit")){ // this condition is added because it was printing something went wrong every time
                    exit(0);
                }
                else{
                    printf("\n Something went wrong ... \n");
                    exit(EXIT_FAILURE);
                }
            }
        }
        else{   // parent process
            //printf("parent is executed\n");
            sleep(1);

            /* if an & was included by user, parent process has to wait */
            if(ampersand){
                ampersand = 0;
                printf("parent should be waiting\n");
                wait(NULL);
            }
        }
    }

    return 0;
}