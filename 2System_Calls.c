#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // For process and communication system calls
#include <sys/wait.h>  // For wait system call
#include <fcntl.h>  // For file-related system calls
#include <sys/stat.h>  // For stat system call
#include <string.h>

void process_system_calls();
void file_system_calls();
void communication_system_calls();
void information_system_calls();

int main() {
    int choice;
    
    while (1) {
        printf("\n--- Menu ---\n");
        printf("1. Process-related system calls (fork, exit, wait)\n");
        printf("2. File-related system calls (open, read, write, close, link, unlink, stat)\n");
        printf("3. Communication system calls (pipe, fifo)\n");
        printf("4. Information-related system calls\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                process_system_calls();
                break;
            case 2:
                file_system_calls();
                break;
            case 3:
                communication_system_calls();
                break;
            case 4:
                information_system_calls();
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

// Process-related system calls: fork, exit, wait
void process_system_calls() {
    pid_t pid = fork();  // Create a child process

    if (pid < 0) {
        printf("Fork failed!\n");
    } else if (pid == 0) {
        printf("Child process. PID = %d\n", getpid());
        printf("Exiting child process.\n");
        exit(0);  // Exit child process
    } else {
        printf("Parent process. PID = %d\n", getpid());
        printf("Waiting for child process to finish...\n");
        wait(NULL);  // Wait for child to exit
        printf("Child process finished.\n");
    }
}

// File-related system calls: open, read, write, close, link, unlink, stat
void file_system_calls() {
    int fd;
    char buffer[100];
    struct stat fileStat;

    // Create and open a file
    fd = open("example.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        printf("Error opening file!\n");
        return;
    }
    
    // Write to file
    write(fd, "Hello, this is a file system call example.\n", 42);
    printf("Data written to file.\n");
    
    // Read from file
    lseek(fd, 0, SEEK_SET);  // Set file offset to the beginning
    read(fd, buffer, 42);  // Read the written data
    buffer[42] = '\0';
    printf("Data read from file: %s\n", buffer);
    
    // Get file status
    if (stat("example.txt", &fileStat) == 0) {
        printf("File size: %ld bytes\n", fileStat.st_size);
        printf("File permissions: %o\n", fileStat.st_mode & 0777);
    }

    // Close file
    close(fd);

    // Create a link to the file
    link("example.txt", "example_link.txt");
    printf("Link created: example_link.txt\n");

    // Remove the link
    unlink("example_link.txt");
    printf("Link removed: example_link.txt\n");

    // Delete the file
    unlink("example.txt");
    printf("File deleted: example.txt\n");
}

// Communication system calls: pipe, fifo
void communication_system_calls() {
    int pipefd[2];
    char write_msg[20] = "Hello, Pipe!";
    char read_msg[20];

    if (pipe(pipefd) == -1) {
        printf("Pipe failed!\n");
        return;
    }

    // Fork a child process
    pid_t pid = fork();

    if (pid < 0) {
        printf("Fork failed!\n");
    } else if (pid == 0) {
        // In child process, write to pipe
        close(pipefd[0]);  // Close reading end
        write(pipefd[1], write_msg, strlen(write_msg) + 1);
        close(pipefd[1]);
        exit(0);
    } else {
        // In parent process, read from pipe
        wait(NULL);  // Wait for child to finish
        close(pipefd[1]);  // Close writing end
        read(pipefd[0], read_msg, sizeof(read_msg));
        printf("Parent received from pipe: %s\n", read_msg);
        close(pipefd[0]);
    }

    // Create FIFO
    mkfifo("my_fifo", 0666);
    printf("FIFO created: my_fifo\n");

    // Write and read using FIFO
    if (fork() == 0) {
        // Child process: write to FIFO
        int fd_fifo = open("my_fifo", O_WRONLY);
        write(fd_fifo, "Hello from FIFO!", 16);
        close(fd_fifo);
        exit(0);
    } else {
        // Parent process: read from FIFO
        wait(NULL);  // Wait for child
        int fd_fifo = open("my_fifo", O_RDONLY);
        read(fd_fifo, read_msg, sizeof(read_msg));
        printf("Parent received from FIFO: %s\n", read_msg);
        close(fd_fifo);
    }

    // Remove FIFO
    unlink("my_fifo");
    printf("FIFO removed: my_fifo\n");
}

// Information-related system calls (using getpid, getppid)
void information_system_calls() {
    printf("Current process ID: %d\n", getpid());
    printf("Parent process ID: %d\n", getppid());
}

