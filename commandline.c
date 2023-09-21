#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ARGS 64

void print_prompt(void);
void handle_eof(void);
char **create_argv(char *command);

/**
 * print_prompt - Display the shell prompt.
 */
void print_prompt(void)
{
	printf("$ ");
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
 * create_argv - Create an argument vector from a command string.
 * @command: The command string.
 * Return: An array of strings (arguments).
 */
char **create_argv(char *command)
{
	char **argv = malloc((MAX_ARGS + 1) * sizeof(char *));
	int i;
	char *token;
	int argc = 0;

	if (argv == NULL)
	{
		perror("malloc");
		exit(1);
	}

	token = strtok(command, " ");
	while (token != NULL)
	{
		argv[argc] = strdup(token);
		if (argv[argc] == NULL)
		{
			perror("strdup");
			exit(1);
		}
		argc++;

		if (argc >= MAX_ARGS)
		{
			fprintf(stderr, "Too many arguments\n");
			exit(1);
		}

		token = strtok(NULL, " ");
	}

	argv[argc] = NULL;

	for (i = 0; i < argc; i++)
	{
		free(argv[i]);
	}
	free(argv);

	return (argv);
}

/**
 * execute_and_wait - Execute a shell command and wait for it.
 * @command: The command to execute.
 */
void execute_and_wait(char *command)
{
	pid_t child_pid;
	int status;

	/* Fork a child process */
	child_pid = fork();
	if (child_pid == -1)
	{
		perror("fork");
		exit(1);
	}
	else if (child_pid == 0)
	{
		char **argv = create_argv(command);

		if (execvp(command, argv) == -1)
		{
			perror("hsh");
			free(argv);
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
		if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL)
		{
			/* Handle end of file (Ctrl+D) */
			handle_eof();
		}

		/* Remove the newline character from the end of the line */
		line[strlen(line) - 1] = '\0';

		execute_and_wait(line);
	}

	return (0);
}
