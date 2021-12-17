/* Pearl Iyayi
** 1001771611
** HW1 ~ Maverick Shell
*/
#define _GNU_SOURCE

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

#define WHITESPACE " \t\n"

#define MAX_COMMAND_SIZE 255    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 10

int main(void)
{
    char * cmd_str = (char*) malloc( MAX_COMMAND_SIZE );
    char *history[15];
    int count_h,count_p = 0;
    pid_t pids[15];
    pid_t pid;
    int c;

    //allocating memory to history
    for(c=0;c<15;c++)
    {
        history[c] = (char*) malloc(255);
    }


    while(1)
    {
        //prints the prompt
        printf("msh> ");

        int user_history = -1;
        // Read the command from the commandline.  The
        // maximum command that will be read is MAX_COMMAND_SIZE
        // This while command will wait here until the user
        // inputs something since fgets returns NULL when there
        // is no input
        while( !fgets (cmd_str, MAX_COMMAND_SIZE, stdin) );

        //saves the first char in cmd_str to c
        char c = cmd_str[0];
        //handling the !n command
        if(c =='!')
        {
            //saves the user input from 1-15
            user_history = atoi(cmd_str+1);

            //checks if the command is in history
            if(strlen(history[user_history-1])==0)
            {
            printf("Command not in history\n");
            continue;
            }
            //replaces the !n in cmd_str with history
            strncpy(cmd_str,history[user_history-1],255);
        }

        /* Parse input */
        char *token[MAX_NUM_ARGUMENTS];

        int   token_count = 0;

        // Pointer to point to the token
        // parsed by strsep:
        char *argument_ptr;

        char *working_str  = strdup( cmd_str );

        // we are going to move the working_str pointer so
        // keep track of its original value so we can deallocate
        // the correct amount at the end
        char *working_root = working_str;

        //checks if the user entered a blank line
        if( cmd_str[0] == ' ' || cmd_str[0] == '\n')
        {
            continue;
        }

        //saves the command entered in history
        strncpy(history[count_h],working_root,255);
        count_h++;

        //checks if the counter for history is over the limit
        if ( count_h > 14 ) count_h = 0;


        // Tokenize the input strings with whitespace used as the delimiter
        while ( ( (argument_ptr = strsep(&working_str, WHITESPACE ) ) != NULL) &&
            (token_count<MAX_NUM_ARGUMENTS))
        {
            token[token_count] = strndup( argument_ptr, MAX_COMMAND_SIZE );
            if( strlen( token[token_count] ) == 0 )
            {
                token[token_count] = NULL;
            }
            token_count++;
        }

        //exit sequence
        if(strcmp("exit",token[0])==0||strcmp("quit",token[0])==0)
        {
            //free sequence
            free(cmd_str);
            for(c=0;c<15;c++)
            {
            free(history[c]);
            }
            exit(0);
        }
        //change directory
        else if(strcmp("cd",token[0])==0)
        {
            chdir(token[1]);
        }
        //should print pids to the screen
        else if(strcmp("showpids",token[0])==0)
        {
            int i = 0;
            for(i;i<count_p;i++)
            {
                printf("%d: %d\n",i,pids[i]);
            }
        }
        //should print the history to screen
        else if(strcmp("history",token[0])==0)
        {
            int j = 0;
            for(j;j<count_h;j++)
            {
                printf("%d: %s",j,history[j]);
            }
        }
        //for commands in the bin/ directory
        else
        {
            pid = fork();
            //save pids
            pids[count_p]=pid;
            count_p++;

            //checks if the pid counter is over the limit
            if ( count_p > 14 ) count_p = 0;

            if(pid==0) //child
            {
                //exec call
                int ret = execvp(token[0],token);

                //exec fail handler
                if(ret==-1)
                {
                    printf("%s: command not found\n",token[0]);
                    exit(0);
                }
            }
            else //parent
            {
                int status;
                wait(&status);
            }//end nested else
        }//end outter else
    }//end while
}//end main
