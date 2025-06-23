static char* manualPages[] = {
    // man
    "MAN(1)                          User Commands                         MAN(1)\n\n"
    "NAME\n"
    "       man - display manual pages\n\n"
    "SYNOPSIS\n"
    "       man [command]\n\n"
    "DESCRIPTION\n"
    "       man displays the manual page for the given command.\n"
    "       If no command is specified, shows available commands.\n\n"
    "EXAMPLES\n"
    "       man time     Display manual for time command\n"
    "       man exit     Display manual for exit command\n\n",

    // time
    "TIME(1)                         User Commands                        TIME(1)\n\n"
    "NAME\n"
    "       time - display current time\n\n"
    "SYNOPSIS\n"
    "       time <timezone>\n\n"
    "DESCRIPTION\n"
    "       Displays the current time in HH:MM:SS format for the specified\n"
    "       timezone offset.\n\n"
    "PARAMETERS\n"
    "       timezone     Timezone offset in hours (integer)\n\n"
    "EXAMPLES\n"
    "       time 0       Display UTC time\n"
    "       time -3      Display time with -3 hours offset\n"
    "       time +5      Display time with +5 hours offset\n\n",

    // exit
    "EXIT(1)                         User Commands                        EXIT(1)\n\n"
    "NAME\n"
    "       exit - terminate the shell\n\n"
    "SYNOPSIS\n"
    "       exit\n\n"
    "DESCRIPTION\n"
    "       Terminates the current shell session and returns to the\n"
    "       operating system.\n\n"
    "EXAMPLES\n"
    "       exit         Terminate shell\n\n",

    // registers
    "REGISTERS(1)                    User Commands                   REGISTERS(1)\n\n"
    "NAME\n"
    "       registers - display system registers\n\n"
    "SYNOPSIS\n"
    "       registers\n\n"
    "DESCRIPTION\n"
    "       Displays the state of the system registers at the time of the last capture triggered with ESC\n"
    "       Useful for debugging and system monitoring.\n\n"
    "EXAMPLES\n"
    "       registers    Show last saved register values\n\n",

    // font
    "FONT(1)                         User Commands                        FONT(1)\n\n"
    "NAME\n"
    "       font - change console font size\n\n"
    "SYNOPSIS\n"
    "       font <size>\n\n"
    "DESCRIPTION\n"
    "       Changes the console font size. Valid sizes are 0 or 1.\n\n"
    "PARAMETERS\n"
    "       size         Font size (0=small, 1=large)\n\n"
    "EXAMPLES\n"
    "       font 0       Set small font\n"
    "       font 1       Set large font\n",

    // args (debug command)
    "ARGS(1)                         User Commands                        ARGS(1)\n\n"
    "NAME\n"
    "       args - display command arguments\n\n"
    "SYNOPSIS\n"
    "       args [arguments...]\n\n"
    "DESCRIPTION\n"
    "       Debug command that displays all arguments passed to it.\n"
    "       Useful for testing argument parsing.\n\n"
    "EXAMPLES\n"
    "       args hello world    Display: args, hello, world\n\n",

    // divzero
    "DIVZERO(1)                      User Commands                     DIVZERO(1)\n\n"
    "NAME\n"
    "       divzero - trigger division by zero exception\n\n"
    "SYNOPSIS\n"
    "       divzero\n\n"
    "DESCRIPTION\n"
    "       Triggers a division by zero exception to test the system's\n"
    "       exception handling mechanism. This command is used for\n"
    "       testing and debugging purposes.\n\n"
    "WARNING\n"
    "       This command intentionally causes a system exception.\n"
    "       Use only for testing exception handlers.\n\n"
    "EXAMPLES\n"
    "       divzero      Trigger division by zero exception\n\n",

    // invalidop
    "INVALIDOP(1)                    User Commands                 INVALIDOP(1)\n\n"
    "NAME\n"
    "       invalidop - trigger invalid opcode exception\n\n"
    "SYNOPSIS\n"
    "       invalidop\n\n"
    "DESCRIPTION\n"
    "       Triggers an invalid opcode exception to test the system's\n"
    "       exception handling mechanism. This command executes an\n"
    "       undefined instruction.\n\n"
    "WARNING\n"
    "       This command intentionally causes a system exception.\n"
    "       Use only for testing exception handlers.\n\n"
    "EXAMPLES\n"
    "       invalidop    Trigger invalid opcode exception\n\n",

    // clear
    "CLEAR(1)                        User Commands                       CLEAR(1)\n\n"
    "NAME\n"
    "       clear - clear the terminal screen\n\n"
    "SYNOPSIS\n"
    "       clear\n\n"
    "DESCRIPTION\n"
    "       Clears the terminal screen and positions the cursor at the\n"
    "       top-left corner. Useful for cleaning up the display.\n\n"
    "EXAMPLES\n"
    "       clear        Clear screen and reset cursor position\n\n",

    // pongis
    "PONGIS(1)                       User Commands                      PONGIS(1)\n\n"
    "NAME\n"
    "       pongis - play Pongis golf game\n\n"
    "SYNOPSIS\n"
    "       pongis\n\n"
    "DESCRIPTION\n"
    "       Starts the classic Pongis golf game. A simple golf game where\n"
    "       players control golf clubs to hit a ball back and forth.\n"
    "       Use keyboard controls to move the player.\n\n"
    "CONTROLS\n"
    "       Player 1: w/a/s/d keys to move\n"
    "       Player 2: i/j/k/l keys to move\n"
    "       ESC: Exit game\n\n"
    "EXAMPLES\n"
    "       pongis       Start Pongis golf game\n\n"
};

static char* command_names[] = {
    "man",
    "time",
    "exit",
    "registers",
    "font",
    "args",
    "divzero",
    "invalidop",
    "clear",
    "pongis"
};
