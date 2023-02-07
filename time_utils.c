#include "philosopher.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	split_usleep(useconds_t ms)
{
	long	standard;

	standard = get_time() + ms;
	while (1)
	{
		if (get_time() > standard)
			break ;
		usleep(216);
	}
}
