/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 15:24:35 by zfiros-a          #+#    #+#             */
/*   Updated: 2024/05/28 17:24:31 by zfiros-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_think(t_philos *philo)
{
	pthread_mutex_lock(&philo->data->gen_lock);
	if (philo->data->end_game == 0)
	{
		pthread_mutex_unlock(&philo->data->gen_lock);
		printing(philo, "is thinking", real_time(philo), 0);
		return ;
	}
	pthread_mutex_unlock(&philo->data->gen_lock);
}

void	ft_sleep(t_philos *philo)
{
	pthread_mutex_lock(&philo->data->gen_lock);
	if (philo->data->end_game == 0)
	{
		pthread_mutex_unlock(&philo->data->gen_lock);
		printing(philo, "\033[1;90m is sleeping 🛏️ \033[0m", real_time(philo), 0);
		ft_usleep(philo->data->sleep_t, philo);
		return ;
	}
	pthread_mutex_unlock(&philo->data->gen_lock);
}

int	main_init(int ac, char **av, t_data *main)
{
	main->num_philos = ft_atoi(av[1]);
	main->die_t = ft_atoi(av[2]);
	main->eat_t = ft_atoi(av[3]);
	main->sleep_t = ft_atoi(av[4]);
	main->end_game = 0;
	main->start_time = timestamp();
	main->total_num_meals = 0;
	main->num_to_eat = 0;
	if (ac == 6)
		main->num_to_eat = ft_atoi(av[5]);
	if (main->num_philos <= 0 || main->die_t <= 0
		|| main->eat_t <= 0 || main->sleep_t <= 0 || main->num_to_eat < 0)
	{
		printf("%s\n", "\033[1;45m  Invalid Arguments \033[0m");
		free(main);
		return (1);
	}
	main->t_philos = ft_calloc(main->num_philos, sizeof(t_philos));
	pthread_mutex_init(&main->num_meals_locker, NULL);
	pthread_mutex_init(&main->last_meal_locker, NULL);
	pthread_mutex_init(&main->gen_lock, NULL);
	main->fork = ft_calloc(main->num_philos, sizeof(int));
	main->fork_locker = ft_calloc(main->num_philos, sizeof(pthread_mutex_t));
	philos_init(main);
	return (0);
}

int	philos_init(t_data *main)
{
	int	i;

	i = -1;
	while (++i < main->num_philos)
	{
		main->t_philos[i].data = main;
		main->t_philos[i].id = i + 1;
		main->t_philos[i].last_meal = timestamp();
		main->fork[i] = 0;
		pthread_mutex_init(&main->fork_locker[i], NULL);
		main->t_philos[i].right_fork = i;
		if (main->num_philos > 1)
			main->t_philos[i].left_fork = (i + 1) % main->num_philos;
	}
	return (0);
}
