#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <dcntl.h>

#include <event.h>

int main()
{
    event_init();
    return 0;
}
