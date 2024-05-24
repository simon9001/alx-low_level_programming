#include "main.h"

/**
 * read_error - prints an error msg if cannot read from file
 * @str: the file to read from
 */
void read_error(char *str)
{
	dprintf(STDERR_FILENO, "Error: Can't read from file %s\n", str);
	exit(98);
}

/**
 * write_error - prints an error msg if cannot write to file
 * @str: the file to write to
 */
void write_error(char *str)
{
	dprintf(STDERR_FILENO, "Error: Can't write to %s\n", str);
	exit(99);
}

/**
 * close_error - prints an error msg if cannot close file
 * @file: the file which failed to close
 */
void close_error(int file)
{
	dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", file);
	exit(100);
}

/**
 * main - where execution begins
 * @argc: the number of arguments
 * @argv: string of arguments
 *
 * Description: copies contents of a file to another file
 * if file to be copied to doesn't exist it is created
 *
 * Return: 0 on success
 */
int main(int argc, char **argv)
{
	int file_read, file_write;
	ssize_t char_read, char_write;
	char buffer[1024];

	if (argc != 3)
	{
		dprintf(STDERR_FILENO, "Usage: cp file_from file_to\n");
		exit(97);
	}
	/* open file to read from as read only */
	file_read = open(argv[1], O_RDONLY);
	if (file_read < 0)
		read_error(argv[1]);
	/* open file to write to, create and set permissions if not present */
	file_write = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, 0664);
	if (file_write < 0)
		write_error(argv[2]);

	char_read = 1024; /* initialized to 1024 to begin loop */
	while (char_read == 1024)
	{

		/* read the characters and store them in a buffer */
		/* 1024 bytes at a time */
		char_read = read(file_read, buffer, 1024);
		if (char_read < 0)
			read_error(argv[1]);
		/* write the characters read from file_read */
		char_write = write(file_write, buffer, char_read);
		if (char_write < 0)
			write_error(argv[2]);
	}

	/* close files and check for error when closing */
	if (close(file_read) < 0)
		close_error(file_read);
	if (close(file_write) < 0)
		close_error(file_write);

	return (0);
}
