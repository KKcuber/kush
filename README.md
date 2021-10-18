# Kush
A shell made in C which simulates the bash/zsh shell and supports all shell commands, piping, redirection. It also has a variety of user-built commands along with extensive signal and error handling.

## Running the Shell

1. Clone the repository.
2. cd into the directory `kush`.
3. Run the command `make` (if you don't have make installed, install it using `sudo apt install make`).
4. Run the command `./kush` to start the shell.
5. Enter `quit` when you wish to exit the shell.

## File Structure
1. main.c - contains the code for the main function.
2. inputLoop.c - contains the loop which runs the shell including logic for piping and redirection.
3. prompt.c - contains code to print the prompt akin to the bash shell and also initialise the shell.
4. pwd.c - contains code for the pwd command.
5. cd.c - contains code for the cd command.
6. ls.c - contains code for the ls command.
7. echo.c - contains code for the echo command.
8. pinfo.c contains code for the pinfo command.
9. foreground.c - contains code for the executing foreground processes.
10. background.c - contains code for the executing background processes.
11. childHandler.c - contains code for handling the signal when the child process exits.
12. headers.h - contains all the function declarations, global variables, macros and headers required by the shell.
13. jobs.c - contains all the code for the job command.
14. sig.c - contains the code for fg, bg, sig.
15. signalhandlers.c - contains code for handling signals such as ctrl + c, ctrl + z.
16. replay.c - contains code for the bonus command replay which gives the user functionality to repeatedly execute a command after specific intervals of time.
17. piping.c - contains code for executing commands using execvp which have pipes.
18. makefile - contains code for the makefile.
