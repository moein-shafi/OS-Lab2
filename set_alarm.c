#include "user.h"


int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf(1, "command must have 1 argument, %d given.\n", argc - 1);
        exit();
    }
    int seconds = atoi(argv[1]);
    set_alarm(seconds);
    exit();
}