# communicate_with_pipes
Main process create a child process and communicates with its child over pipes.
Main process accepts command line arguments and passes each argument its child. Then child process reverses message and send back its parent.

## build
gcc two-way-communication.c -lpthread -o outputName

## run
./outputName arg1 arg2 ...
