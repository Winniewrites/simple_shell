#include "shell.h"
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
		/* Handle end of file (Ctrl+D) */
		handle_eof();
	}

	/* Remove the newline character from the end of the line */
	line[strlen(line) - 1] = '\0';
}

/**
 * execute_command - Execute a shell command.
 * @command: The command to execute.
 */
void execute_command(char *command)
{
	pid_t child_pid;
	int status;

	/* Check if the command is "env" */
	if (strcmp(command, "env") == 0)
	{
		/* Print the current environment */
		char **env = environ;

		for ( ; *env != NULL; env++)
		{
			printf("%s\n", *env);
		}
		return;
	}

	/* Fork a child process */
	child_pid = fork();
	if (child_pid == -1)
	{
		perror("fork");
		exit(1);
	}
	else if (child_pid == 0)
	{
		/* In the child process, allocate memory for argv */
		char **argv = malloc(2 * sizeof(char *));
if (argv == NULL)
{
perror("malloc");
exit(1);
}
		argv[0] = strdup(command);
		if (argv[0] == NULL)
		{
			perror("strdup");
			free(argv);
			exit(1);
		}
		argv[1] = NULL;

		/* Execute the command */
		if (execve(command, argv, environ) == -1)
		{
			perror("hsh");
			free(argv);
			exit(1);
		}
	}
	else
	{
		/* In the parent process */
		/* Wait for the child process to finish */
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

/**
 * main - The main entry point of the shell.
 *
 * Description: This function serves as the main entry point of the shell. It
 * reads commands from the user, executes them, and displays the shell prompt.
 *
 * Return: Always returns 0.
 */
int main(void)
{
	char line[MAX_LINE_LENGTH];

	while (1)
	{
		print_prompt();
		read_command(line);
		execute_command(line);
	}

	return (0);
}
