#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

int main()
{
    struct timeval  tv;

    memset(&tv, 0, sizeof(tv));
    while (1)
    {
        gettimeofday(&tv, NULL);
//      time = tv.tv_sec;
        printf("usec: %d\n", tv.tv_usec);
        usleep(10000);
    }
}