#include <stdio.h>

int main(int argc, char **argv)
{
	unsigned char byte;

	printf("unsigned char %s[] = { ", argc > 1 ? argv[1] : "bin2hex");

	while(read(0, &byte, sizeof(byte)) > 0) {
		printf("0x%02X, ", byte);
	}

	printf("};\n");

	return 0;
}
