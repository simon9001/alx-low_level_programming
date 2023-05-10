#include "main.h"

/**
 * close_file - closes the opened file
 * @file: the file to close
 *
 * Description: displays an error message if closing fails
 */
void close_file(int file)
{
	if (close(file) < 0)
	{
		dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", file);
		exit(98);
	}
}

/**
 * check_file - checks if file is an ELF file
 * @if_elf: ELF file to check
 */
void check_file(unsigned char *if_elf)
{
	int count = 0;

	while (count < 4)
	{
		if (if_elf[count] != 127 && if_elf[count] != 'E' &&
		    if_elf[count] != 'L' && if_elf[count] != 'F')
		{
			dprintf(STDERR_FILENO, "Error: Not ELF file\n");
			exit(98);
		}
		count++;
	}
}

/**
 * print_magic - prints the magic numbers of the ELF file
 * @magic: the ELF file class to check
 */
void print_magic(unsigned char *magic)
{
	int count;

	printf("ELF Header:\n");
	printf("  Magic:   ");
	for (count = 0; count < EI_NIDENT; count++)
	{
		printf("%02x", magic[count]);

		if (count == EI_NIDENT - 1)
			printf("\n");
		else
			printf(" ");
	}
}

/**
 * print_class - prints the class of an ELF file
 * @class: the ELF file to check for the class
 *
 * Description: prints unknown if no class is found
 */
void print_class(unsigned char *class)
{
	printf("  Class:                             ");

	switch (class[EI_CLASS])
	{
	case ELFCLASSNONE:
		printf("none\n");
		break;
	case ELFCLASS32:
		printf("ELF32\n");
		break;
	case ELFCLASS64:
		printf("ELF64\n");
		break;
	default:
		printf("<unknown: %x>\n", class[EI_CLASS]);
	}
}

/**
 * print_data - prints the data format of an ELF file
 * @data: the ELF file class to check
 *
 * Description: prints unknown in case of an error
 */
void print_data(unsigned char *data)
{
	printf("  Data:                              ");
	switch (data[EI_DATA])
	{
	case ELFDATANONE:
		printf("none\n");
		break;
	case ELFDATA2LSB:
		printf("2's complement, little endian\n");
		break;
	case ELFDATA2MSB:
		printf("2's complement, big endian\n");
		break;
	default:
		printf("<unknown: %x>\n", data[EI_CLASS]);
	}
}

/**
 * print_version - prints the version of the ELF file
 * @version: the ELF file to check version from
 *
 * Description: displays current if the version is the current one
 */
void print_version(unsigned char *version)
{
	printf("  Version:                           %d", version[EI_VERSION]);
	switch (version[EI_VERSION])
	{
	case EV_CURRENT:
		printf(" (current)\n");
		break;
	default:
		printf("\n");
		break;
	}
}

/**
 * print_os_abi - prints the OS/ABI
 * @os_abi: the ELF version to check for OS/ABI
 */
void print_os_abi(unsigned char *os_abi)
{
	printf("  OS/ABI:                            ");

	switch (os_abi[EI_OSABI])
	{
	case ELFOSABI_NONE:
		printf("UNIX - System V\n");
		break;
	case ELFOSABI_HPUX:
		printf("UNIX - HP-UX\n");
		break;
	case ELFOSABI_NETBSD:
		printf("UNIX - NetBSD\n");
		break;
	case ELFOSABI_LINUX:
		printf("UNIX - Linux\n");
		break;
	case ELFOSABI_SOLARIS:
		printf("UNIX - Solaris\n");
		break;
	case ELFOSABI_IRIX:
		printf("UNIX - IRIX\n");
		break;
	case ELFOSABI_FREEBSD:
		printf("UNIX - FreeBSD\n");
		break;
	case ELFOSABI_TRU64:
		printf("UNIX - TRU64\n");
		break;
	case ELFOSABI_ARM:
		printf("ARM\n");
		break;
	case ELFOSABI_STANDALONE:
		printf("Standalone App\n");
		break;
	default:
		printf("<unknown: %x>\n", os_abi[EI_OSABI]);
	}
}

/**
 * print_abi_version - prints the OS/ABI - version
 * @abi_ver: the version of the OS/ABI
 */
void print_abi_version(unsigned char *abi_ver)
{
	printf("  ABI Version:                       %d\n",
	       abi_ver[EI_ABIVERSION]);
}

/**
 * print_type - prints the type of the file
 * @type: type passed as paramenter
 * @src: the ELF file to check from
 */
void print_type(unsigned int type, unsigned char *src)
{
	/* check for endianness */
	if (src[EI_DATA] == ELFDATA2MSB)
		type = type >> 8;

	printf("  Type:                              ");
	switch (type)
	{
	case ET_NONE:
		printf("NONE (none)\n");
		break;
	case ET_REL:
		printf("REL (Relocatable file)\n");
		break;
	case ET_EXEC:
		printf("EXEC (Executable file)\n");
		break;
	case ET_DYN:
		printf("DYN (Shared object file)\n");
		break;
	case ET_CORE:
		printf("CORE (Core file)\n");
		break;
	default:
		printf("<unknown: %x>\n", type);
	}
}

/**
 * print_entry_addr - prints the entry address of the ELF file
 * @entry: passing the entry point for the file
 * @src: the ELF file to check from
 */
void print_entry_addr(unsigned long int entry, unsigned char *src)
{
	printf("  Entry point address:               ");

	/* check if it is big endian */
	if (src[EI_DATA] == ELFDATA2MSB)
	{
		entry = ((entry << 8) & 0xFF00FF00) |
			((entry >> 8) & 0xFF00FF);
		entry = (entry << 16) | (entry >> 16);
	}

	/* if it is 32 bit */
	if (src[EI_CLASS] == ELFCLASS32)
		printf("%#x\n", (unsigned int)entry);
	else
		printf("%#lx\n", entry);
}

/**
 * main - a program that works similar to readelf -h
 * @argc: the argument count
 * @argv: string of arguments
 *
 * Return: 0 on success
 */
int main(int argc, char *argv[])
{
	int f_open;
	ssize_t f_read;
	Elf64_Ehdr *header;
	/* correct number of arguments */
	if (argc != 2 || !argv[1])
	{
		dprintf(STDERR_FILENO, "Usage: %s elf_filename\n", argv[0]);
		exit(98);
	}
	f_open = open(argv[1], O_RDONLY); /* open the file as read only */
	if (f_open < 0) /* if open failed */
	{
		dprintf(STDERR_FILENO, "Error: Can't read file %s\n", argv[1]);
		exit(98);
	}
	header = malloc(sizeof(Elf64_Ehdr)); /* allocate memory space */
	if (!header) /* if allocation failed */
	{
		close_file(f_open);
		dprintf(STDERR_FILENO, "Error: Can't read file %s\n", argv[1]);
		exit(98);
	}
	f_read = read(f_open, header, sizeof(Elf64_Ehdr));
	if (f_read < 0) /* if read fails */
	{
		free(header);
		close_file(f_open);
		dprintf(STDERR_FILENO, "Error: %s: No such file\n", argv[1]);
		exit(98);
	}
	/* the elements to display */
	check_file(header->e_ident);
	print_magic(header->e_ident);
	print_class(header->e_ident);
	print_data(header->e_ident);
	print_version(header->e_ident);
	print_os_abi(header->e_ident);
	print_abi_version(header->e_ident);
	print_type(header->e_type, header->e_ident);
	print_entry_addr(header->e_entry, header->e_ident);

	free(header);
	close_file(f_open);
	return (0);
}
