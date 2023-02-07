#include "philosopher.h"

int	generate_philo(t_philo *shared)
{
	int		i;

	i = 0;
	shared->start_point = get_time();
	while (i < shared->conditions->philo_number)
	{
		if (pthread_create(&shared->philos[i], NULL, philosopher_do_something, shared) != 0)
			return (ERROR);
		i++;
	}
	return (0);
}

void	wait_philos(t_philo *shared)
{
	int	num;
	int	i;

	num = shared->conditions->philo_number;
	i = 0;
	while (i < num)
	{
		pthread_join(shared->philos[i], NULL);
		i++;
	}
}
