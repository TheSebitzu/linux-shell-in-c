#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define RL_BUFFER_SIZE 1024
#define SL_BUFFER_SIZE 64
#define TOKEN_DELIMITERS " \t\r\n\a"

void loop(void);
char *read_line(void);
char **split_line(char *line);
int launch(char **args);
int execute(char **args);
int num_built_ins();
int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);

// Built in functions
char *built_in_str[] = {
    "cd",
    "help",
    "exit"};

int (*built_in_func[])(char **) = {
    &shell_cd,
    &shell_help,
    &shell_exit};

int main(int argc, char **argv)
{
    // Run loop
    loop();

    return EXIT_SUCCESS;
}

void loop(void)
{
    char *line;
    char **args;
    int status;

    // Main loop
    do
    {
        printf("> ");
        line = read_line();
        args = split_line(line);
        status = execute(args);

        free(line);
        free(args);
    } while (status);
}

char *read_line(void)
{
    int buffer_size = RL_BUFFER_SIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * buffer_size);

    if (!buffer)
    {
        printf("Allocation error");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        // Read a char (function returns an int)
        int c = getchar();

        // When pressing ENTER or reaching EOF
        if ((int)c == EOF || c == '\n')
        {
            buffer[position] = '\0';
            return buffer;
        }
        else
        {
            // Put the char in the buffer
            buffer[position] = c;
        }

        position++;

        // Reallocate if we need more space
        if (position >= buffer_size)
        {
            buffer_size += RL_BUFFER_SIZE;
            buffer = realloc(buffer, sizeof(char) * buffer_size);

            if (!buffer)
            {
                printf("Allocation error");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **split_line(char *line)
{
    int buffer_size = SL_BUFFER_SIZE;
    int position = 0;

    char **tokens = malloc(sizeof(char *) * buffer_size);
    char *token;

    if (!tokens)
    {
        printf("Allocation error");
        exit(EXIT_FAILURE);
    }

    // First "word" (stop at any delimiter)
    token = strtok(line, TOKEN_DELIMITERS);

    while (token)
    {
        tokens[position] = token;
        position++;

        // Realloc if we need more space
        if (position >= buffer_size)
        {
            buffer_size += SL_BUFFER_SIZE;
            tokens = realloc(tokens, sizeof(char *) * buffer_size);

            if (!tokens)
            {
                printf("Allocation error");
                exit(EXIT_FAILURE);
            }
        }

        // Go to the next "word"
        token = strtok(NULL, TOKEN_DELIMITERS);
    }

    // NULL at the end
    tokens[position] = NULL;

    return tokens;
}

int launch(char **args)
{
    pid_t pid, wpid;
    int status;

    // Fork and get the process id
    pid = fork();

    if (pid == 0)
    {
        // Child process (execute)
        if (execvp(args[0], args) == -1)
        {
            perror("shell");
        }
        exit(EXIT_FAILURE);
    }
    // Error
    else if (pid < 0)
    {
        perror("shell");
    }
    else
    {
        // Parent process
        do
        {
            // Wait until child process finishes
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int execute(char **args)
{
    if (args[0] == NULL)
    {
        return 1;
    }

    // Get built in func
    for (int i = 0; i < num_built_ins(); i++)
    {
        if (strcmp(args[0], built_in_str[i]) == 0)
        {
            // Execute function
            return (*built_in_func[i])(args);
        }
    }

    // If its not built in, use the launch func
    return launch(args);
}

int num_built_ins()
{
    return sizeof(built_in_str) / sizeof(char *);
}

// CD built in func
int shell_cd(char **args)
{
    if (args[1] == NULL)
    {
        printf("Expected argument to 'cd'\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("shell");
        }
    }
    return 1;
}

// Help built in func
int shell_help(char **args)
{
    printf("This is a shell\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");
    for (int i = 0; i < num_built_ins(); i++)
    {
        printf("\t%s\n", built_in_str[i]);
    }

    printf("Use the 'man' command for info on other programs.\n");

    return 1;
}

// Exit built in func
int shell_exit(char **args)
{
    return 0;
}
