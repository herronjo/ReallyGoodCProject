#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

// Inline 64-bit x86 assembly version of hello function
void hello(char* name, int len) {
	asm volatile (
		"movq $1, %%rax\n" // syscall number for sys_write
		"movq $1, %%rdi\n" // file descriptor 1 is stdout
		"movq %0, %%rsi\n" // address of the string
		"movq %1, %%rdx\n" // length of the string
		"syscall\n"
		:
		: "r"((unsigned long)name), "r"((unsigned long)len)
		: "%rax", "%rdi", "%rsi", "%rdx"
	);
}

int main() {
	// Get the length of the name
	int numOfChars = 0;
	printf("Enter the length of your name: ");
	scanf("%d", &numOfChars);

	// Consume newline
	getchar();

	// Allocate memory for the name and hello function
	char* nameAndHello = mmap(NULL, (numOfChars + 1) * sizeof(char) + 64, PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0);
	// Extract the sayHello function pointer
	void (*sayHello)(char* name, int len) = (void (*)(char*, int)) (nameAndHello + ((numOfChars + 1) * sizeof(char)));

	// Copy hello function to sayHello
	memcpy(sayHello, (void *)hello, 64);

	// Get the name from the user
	printf("Enter your name: ");
	gets(nameAndHello);

	// Call the sayHello function
	(*sayHello)(nameAndHello, numOfChars);

	// Free the memory
	munmap(nameAndHello, (numOfChars + 1) * sizeof(char) + 64);
}