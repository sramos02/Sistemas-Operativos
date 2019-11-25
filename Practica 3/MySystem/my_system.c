#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>


int system(const char * command){
	int status = 0;

	pid_t child_pid;
	child_pid = fork();

	if (child_pid != 0) { wait(&status); }
	else {
		int n;

		n = execl("/bin/bash","/bin/bash", "-c", command, NULL);
		status = -1;
		return n;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	if (argc!=2){
		fprintf(stderr, "Usage: %s <command>\n", argv[0]);
		exit(1);
	}

	return system(argv[1]);
}

