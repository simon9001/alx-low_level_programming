#include "main.h"

/**
 * read_textfile - reads a given file and prints a set number of bytes
 * @filename: name of the file to read
 * @letters: the number of characters to print
 *
 * Return: the number of characters printed
 * returns 0 if there is an error
 */
ssize_t read_textfile(const char *filename, size_t letters)
{
	ssize_t char_print, char_read;
	char *str_buf;
	int f_open, f_close;

	if (!filename) /* file name is NULL */
		return (0);
	f_open = open(filename, O_RDONLY); /* open file as read only */
	if (f_open == -1)
		return (0);

	str_buf = malloc(sizeof(char) * letters); /* memory for buffer */
	if (!str_buf) /* memory allocation failed */
	{
		close(f_open);
		return (0);
	}

	char_read = read(f_open, str_buf, letters); /* read the file */
	if (char_read == -1) /* fail to read */
	{
		free(str_buf);
		return (0);
	}
	f_close = close(f_open); /* close file */
	if (f_close == -1) /* failed to close */
	{
		free(str_buf);
		return (0);
	}
	char_print = write(STDOUT_FILENO, str_buf, char_read); /* print */
	if (char_print == -1 || char_print != char_read) /* failed to print */
	{
		free(str_buf);
		return (0);
	}
	free(str_buf);
	return (char_read);
}
