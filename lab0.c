// NAME: Matthew Paterno
// EMAIL: mpaterno@g.ucla.edu
// ID: Matthew Paterno

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>
#include <errno.h>

// Set Segmentation Flag
static int segFlag; // can put outside main, because it's static.

void segFault_handler(int signum)
{
	if (signum == SIGSEGV)
	{
		fprintf(stderr, "ERROR: Segmentation fault caught by signal().\n");
		exit(4);
	}
}

void segFault_subroutine()
{
	char *p = NULL;
	*p = 0;
}

int main(int argc, char **argv)
{
	char *inputFile = NULL;
	char *outputFile = NULL;

	// Set Options
	while (1)
	{

		static struct option long_options[] =
				{
						{"input", required_argument, 0, 'i'},
						{"output", required_argument, 0, 'o'},
						{"segfault", no_argument, 0, 's'},
						{"catch", no_argument, 0, 'c'},
						{0, 0, 0, 0}, // No options.
				};
		int c;
		// int option_index = 0;
		c = getopt_long(argc, argv, "i:o:sc", long_options, NULL);

		/* Detect end of options */
		if (c == -1)
			break;

		switch (c)
		{
		case 'i':
			inputFile = optarg;
			break;
		case 'o':
			outputFile = optarg;
			break;
		case 's':
			segFlag = 1;
			break;
		case 'c':
			signal(SIGSEGV, segFault_handler);
			break;
		default:
			printf("Error with Usage.");
			exit(1);
		}
	}

	// Cause a segfault by dereferencing a pointer to null pointer.
	if (segFlag == 1)
	{
		segFault_subroutine();
	}

	// Create Dynamic Buffer to Write To
	int numChar = 8;
	int numInput = 0;
	char *buffer = malloc(sizeof(char) * numChar);

	int ifd = 0;
	int ofd = 1;

	if (inputFile) // Check Permissions
	{
		// Open inputFile
		ifd = open(inputFile, O_RDONLY);
		if (ifd >= 0)
		{
			close(0);
			dup(ifd);
			close(ifd);
		}
		else
		{
			fprintf(stderr, "ERROR: Issue reading input file: %s\n", strerror(errno));
			exit(2);
		}
	}
	if (outputFile) // Check Permissions
	{
		// Create outputFile
		ofd = creat(outputFile, 0666); // What does this number mean?
		if (ofd >= 0)
		{
			close(1);
			dup(ofd);
			close(ofd);
		}
		else
		{
			fprintf(stderr, "ERROR: Issue creating output file: %s\n", strerror(errno));
			exit(3);
		}
	}
	// Continuing with Read and Write Operation

	// Read stdin
	while (read(0, &buffer[numInput], 1) > 0)
	{ // Check where read returns something.
		numInput++;
		if (numInput == numChar)
		{
			// Allocate more memory
			buffer = realloc(buffer, numChar * 2);
			numChar *= 2;
		}
	}
	// Write stdout
	int i;
	for (i = 0; i < numInput; i++)
	{
		write(1, &buffer[i], 1);
	}
	free(buffer);

	// Main Operation Completed
	return 0;
}