#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 256

void reverse_case(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (islower(str[i])) {
            str[i] = toupper(str[i]);
        } else if (isupper(str[i])) {
            str[i] = tolower(str[i]);
        }
    }
}

int main() {
    int pipe1[2], pipe2[2];
    pid_t pid;
    char write_msg[BUFFER_SIZE];
    char read_msg[BUFFER_SIZE];

    printf("Enter a message: ");
    fgets(write_msg, BUFFER_SIZE, stdin);
    write_msg[strcspn(write_msg, "\n")] = '\0';  


    if (pipe(pipe1) == -1) {
        fprintf(stderr, "pipe1 failure");
        return 1;
    }
    if (pipe(pipe2) == -1) {
        fprintf(stderr, "pipe2 failure");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "fork failure");
        return 1;
    }

    if (pid > 0) { // parent
        close(pipe1[0]); // close pipe1 read end
        close(pipe2[1]); // close pipe2 write end

   
        write(pipe1[1], write_msg, strlen(write_msg) + 1);
        close(pipe1[1]); 

        read(pipe2[0], read_msg, BUFFER_SIZE);
        printf("parent read msg: %s\n", read_msg);
        close(pipe2[0]); 

    } else { // child
        close(pipe1[1]); 
        close(pipe2[0]); 

        read(pipe1[0], read_msg, BUFFER_SIZE);
        close(pipe1[0]); 

        reverse_case(read_msg);

        write(pipe2[1], read_msg, strlen(read_msg) + 1);
        close(pipe2[1]); 
    }

    return 0;
}
