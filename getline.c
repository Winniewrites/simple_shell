#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64

/**
 * split_args - Splits the input command into arguments
 * @input: The input command
 * @args: An array to store the arguments
 */
void split_args(char *input, char **args)
{
	int i = 0;

	args[i] = strtok(input, " \t\n");

	while (args[i] != NULL && i < MAX_ARGS - 1)
	{
		args[++i] = strtok(NULL, " \t\n");
	}

	args[i] = NULL;
}

/**
 * execute_command - Execute a command and wait for its completion
 * @args: An array of command arguments
 */
void execute_command(char **args)
{
	pid_t pid = fork();

	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (execvp(args[0], args) == -1)
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		int status;

		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			int exit_status = WEXITSTATUS(status);

			if (exit_status != 0)
			{
				printf("Command exited with status %d\n", exit_status);
			}
		}
		else if (WIFSIGNALED(status))
		{
			int signal_number = WTERMSIG(status);

			printf("Command terminated by signal %d\n", signal_number);
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
	char input[MAX_INPUT_SIZE];
	char *args[MAX_ARGS];

	while (1)
	{
		printf("simple_shell$ ");

		if (fgets(input, sizeof(input), stdin) == NULL)
		{
			printf("\n");
			break;
		}

		input[strcspn(input, "\n")] = '\0';

		split_args(input, args);

		if (args[0] == NULL)
		{
			continue;
		}

		execute_command(args);
	}

	return (0);
}
