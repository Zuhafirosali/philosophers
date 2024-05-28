/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 07:17:19 by zuhafirosal       #+#    #+#             */
/*   Updated: 2024/05/28 19:08:20 by zfiros-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philos_life(void *arg)
{
	t_philos	*philo;

	philo = (t_philos *)arg;
	if (philo->data->num_philos == 1)
		one_philo(philo);
	pthread_mutex_lock(&philo->data->gen_lock);
	while (philo->data->end_game == 0)
	{
		pthread_mutex_unlock(&philo->data->gen_lock);
		ft_eat(philo);
		ft_sleep(philo);
		ft_think(philo);
		pthread_mutex_lock(&philo->data->gen_lock);
	}
	pthread_mutex_unlock(&philo->data->gen_lock);
	return (NULL);
}

void	death_checker(t_philos *philo)
{
	int	lock;

	lock = 1;
	pthread_mutex_lock(&philo->data->last_meal_locker);
	if (timestamp() - philo->last_meal >= philo->data->die_t)
	{
		pthread_mutex_unlock(&philo->data->last_meal_locker);
		lock = 0;
		pthread_mutex_lock(&philo->data->gen_lock);
		philo->data->end_game = 1;
		pthread_mutex_unlock(&philo->data->gen_lock);
		if (philo->data->num_philos != 1)
			printing(philo, "\033[1;31m died ❌ \033[0m", real_time(philo), 1);
	}
	if (lock)
		pthread_mutex_unlock(&philo->data->last_meal_locker);
}

void	philos_launch(t_data *main)
{
	int	i;

	i = -1;
	while (++i < main->num_philos)
	{
		if (pthread_create(&main->t_philos[i].thread, NULL,
				&philos_life, &(main->t_philos[i])) != 0)
			return ;
	}
	i = -1;
	while (main->end_game == 0)
	{
		death_checker(&main->t_philos[++i % main->num_philos]);
		if (main->num_to_eat > 0)
			num_meals_checker(main);
		usleep(500);
	}
}

void	num_meals_checker(t_data *main)
{
	pthread_mutex_lock(&main->num_meals_locker);
	if (main->total_num_meals >= main->num_philos * main->num_to_eat)
	{
		pthread_mutex_unlock(&main->num_meals_locker);
		pthread_mutex_lock(&main->gen_lock);
		main->end_game = 1;
		pthread_mutex_unlock(&main->gen_lock);
		return ;
	}
	pthread_mutex_unlock(&main->num_meals_locker);
}

int	main(int ac, char **av)
{
	t_data	*main;

	if (ac != 5 && ac != 6)
	{
		printf("%s\n", "\033[1;45m Wrong number of arguments: \033[0m");
		printf("%s", "\033[1;47m Ex: ./philo [num of philo]");
		printf("%s\n", "[time 2 die] [time 2 eat] [time 2 sleep] \033[0m");
		return (0);
	}
	if (!valid_args(av))
	{
		printf("%s\n", "\033[1;45mInvalid Arguments \033[0m");
		return (0);
	}
	if (ft_atoi(av[1]) == 0)
	{
		printf("%s", "\x1b[1;45mNumber of philosophers cannot be 0 \033[0m");
		return (0);
	}
	main = ft_calloc(1, sizeof(t_data));
	if (main_init(ac, av, main))
		return (0);
	philos_launch(main);
	ft_destroy(main);
	return (0);
}
