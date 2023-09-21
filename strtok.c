#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE_LENGTH 1024

void print_prompt(void);
void read_command(char *line);
void execute_command(char *command);
void handle_eof(void);

/**
 * main - The main entry point of the shell.
 *
 * Description:
 * This function serves as the main entry point of the shell. It reads commands
 * from the user, executes them, and displays the shell prompt.
 *
 * Return:
 * Always returns 0.
 */
int main(void)
{
char line[MAX_LINE_LENGTH];
size_t len;

while (1)
{
print_prompt();
read_command(line);

len = strlen(line);

if (len > 0 && line[len - 1] == '\n')
{
line[len - 1] = '\0';
}

execute_command(line);
}

return (0);
}

/**
 * print_prompt - Display the shell prompt.
 */
void print_prompt(void)
{
printf("$ ");
}

/**
 * read_command - Read a command from the user.
 * @line: A buffer to store the command line.
 */
void read_command(char *line)
{
if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL)
{
handle_eof();
}
}

/**
 * execute_command - Execute a shell command.
 * @command: The command to execute.
 */
void execute_command(char *command)
{
pid_t child_pid;
int status;

child_pid = fork();
if (child_pid == -1)
{
perror("fork");
exit(1);
}
else if (child_pid == 0)
{
if (execlp(command, command, NULL) == -1)
{
perror("execvp");
exit(1);
}
}
else
{
if (waitpid(child_pid, &status, 0) == -1)
{
perror("waitpid");
exit(1);
}

if (WIFEXITED(status))
{
int exit_status = WEXITSTATUS(status);

printf("Child exited with status %d\n", exit_status);
}
}
}

/**
 * handle_eof - Handle the "end of file" condition (Ctrl+D).
 */
void handle_eof(void)
{
printf("\n");
exit(0);
}
