// A very minimal shell - basically just a wrapper

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_CMD_LENGTH 256

// Function to clear screen
void clearScreen() {
    system("cls");
}

// Function to print the current working directory
void printWorkingDirectory() {
    char buffer[MAX_PATH];
    if (GetCurrentDirectory(MAX_PATH, buffer)) {
        printf("%s\n", buffer);
    } else {
        printf("Error getting current directory\n");
    }
}

// Function to change directory
void changeDirectory(char *path) {
    if (SetCurrentDirectory(path)) {
        printf("Directory changed to: %s\n", path);
    } else {
        printf("Error: Unable to change directory\n");
    }
}

// Function to print text
void echoCommand(char *text) {
    printf("%s\n", text);
}

// Function to list all built-in commands
void listCommands() {
    printf("Available Commands:\n");
    printf("  cmds   - Show this command list\n");
    printf("  cls    - Clear the screen\n");
    printf("  pwd    - Show current directory\n");
    printf("  cd <path> - Change directory\n");
    printf("  echo <text> - Print text to screen\n");
    printf("  exit   - Exit the shell\n");
    printf("  (You can also run external commands like notepad, calc, etc.)\n");
}

// Function to execute an external command
void executeExternalCommand(char *command) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Create a process to run the command
    if (CreateProcess(NULL, command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        // Wait for the process to complete
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        printf("Error: Failed to execute command\n");
    }
}

// Function to handle input commands
void handleCommand(char *input) {
    char *command = strtok(input, " "); // Get the first word
    char *args = strtok(NULL, "");      // Get the rest of the string

    if (strcmp(command, "exit") == 0) {
        printf("Exiting shell...\n");
        exit(0);
    } else if (strcmp(command, "cls") == 0) {
        clearScreen();
    } else if (strcmp(command, "pwd") == 0) {
        printWorkingDirectory();
    } else if (strcmp(command, "cd") == 0) {
        if (args) {
            changeDirectory(args);
        } else {
            printf("Usage: cd <path>\n");
        }
    } else if (strcmp(command, "echo") == 0) {
        if (args) {
            echoCommand(args);
        } else {
            printf("\n");
        }
    } else if (strcmp(command, "cmds") == 0) {
        listCommands();
    } else {
        // If it's not a built-in command, try executing it as an external command
        executeExternalCommand(input);
    }
}

// Main function
int main() {
    char command[MAX_CMD_LENGTH];

    printf("\033[2J\033[H"); // Clears screen
    printf("MySh (c) - Custom Shell for Windows\n");
    printf("Type 'cmds' to see available commands\n");

    while (1) {
        printf("shell> ");
        if (fgets(command, MAX_CMD_LENGTH, stdin) == NULL) {
            break;
        }

        // Remove newline character
        command[strcspn(command, "\n")] = 0;

        // Ignore empty input
        if (strlen(command) == 0) {
            continue;
        }

        // Handle command
        handleCommand(command);
    }

    return 0;
}
