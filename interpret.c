#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE_LENGTH 1024

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
	fgets(line, MAX_LINE_LENGTH, stdin);
	line[strlen(line) - 1] = '\0';
}

/**
 * execute_command - Execute a shell command.
 * @command: The command to execute.
 * Return: The exit status of the command.
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

	if (child_pid == 0)
	{
		char *argv[2];

		argv[0] = command;
		argv[1] = NULL;

		if (execvp(command, argv) == -1)
		{
			perror("hsh");
			exit(1);
		}
	}
	else
	{
		wait(&status);
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
 * Return: Always returns 0.
 */
int main(void)
{
	char line[MAX_LINE_LENGTH];

	while (1)
	{
		print_prompt();
		read_command(line);

		if (strcmp(line, "") == 0)
		{
			handle_eof();
		}
		else
		{
			execute_command(line);
		}
	}

	return (0);
}

