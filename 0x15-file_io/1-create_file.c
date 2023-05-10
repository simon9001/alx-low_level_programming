#include "main.h"

/**
 * _strlen - gets length of string
 * @str: string to compute length
 *
 * Return: length of string
 */
size_t _strlen(const char *str)
{
	size_t count = 0;

	while (str[count])
		count++;
	return (count);
}

/**
 * create_file - creates a new file
 * @filename: name of the file to create
 * @text_content: the text to write in the file created
 * Description: creates a new file and sets access permission to read-write
 * if file already exists, trucates it and doesn't change access permissions
 *
 * Return: returns 1 on success, -1 otherwise
 */
int create_file(const char *filename, char *text_content)
{
	int f_open, f_close;
	ssize_t char_print;

	if (!filename)
		return (-1);
	f_open = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (f_open == -1)
		return (-1);

	if (text_content)
	{
		char_print = write(f_open, text_content, _strlen(text_content));
		if (char_print == -1)
			return (-1);
	}
	f_close = close(f_open);
	if (f_close == -1)
		return (-1);

	return (1);
}
