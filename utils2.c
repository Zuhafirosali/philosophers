/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 08:35:10 by zfiros-a          #+#    #+#             */
/*   Updated: 2024/05/28 19:08:52 by zfiros-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	pick_fork2(t_philos *philo)
{
	pthread_mutex_lock(&philo->data->fork_locker[philo->right_fork]);
	pthread_mutex_lock(&philo->data->fork_locker[philo->left_fork]);
	while (philo->data->fork[philo->left_fork] == -1
		|| philo->data->fork[philo->right_fork] == -1
		|| philo->data->fork[philo->right_fork] == philo->id
		|| philo->data->fork[philo->left_fork] == philo->id)
	{
		pthread_mutex_unlock(&philo->data->fork_locker[philo->right_fork]);
		pthread_mutex_unlock(&philo->data->fork_locker[philo->left_fork]);
		usleep(1500);
		pthread_mutex_lock(&philo->data->fork_locker[philo->right_fork]);
		pthread_mutex_lock(&philo->data->fork_locker[philo->left_fork]);
	}
	philo->data->fork[philo->left_fork] = -1;
	philo->data->fork[philo->right_fork] = -1;
	pthread_mutex_unlock(&philo->data->fork_locker[philo->right_fork]);
	pthread_mutex_unlock(&philo->data->fork_locker[philo->left_fork]);
	printing(philo, "\033[1;94m has taken a fork 🥄 \033[0m", real_time(philo), 0);
	printing(philo, "\033[1;94m has taken a fork 🥄 \033[0m", real_time(philo), 0);
}

void	pick_fork(t_philos *philo)
{
	if (philo->id % 2 != 0)
	{
		usleep(100);
		pthread_mutex_lock(&philo->data->fork_locker[philo->left_fork]);
		pthread_mutex_lock(&philo->data->fork_locker[philo->right_fork]);
		while (philo->data->fork[philo->left_fork] == -1
			|| philo->data->fork[philo->right_fork] == -1
			|| philo->data->fork[philo->right_fork] == philo->id
			|| philo->data->fork[philo->left_fork] == philo->id)
		{
			pthread_mutex_unlock(&philo->data->fork_locker[philo->left_fork]);
			pthread_mutex_unlock(&philo->data->fork_locker[philo->right_fork]);
			usleep(100);
			pthread_mutex_lock(&philo->data->fork_locker[philo->left_fork]);
			pthread_mutex_lock(&philo->data->fork_locker[philo->right_fork]);
		}
		philo->data->fork[philo->left_fork] = -1;
		philo->data->fork[philo->right_fork] = -1;
		pthread_mutex_unlock(&philo->data->fork_locker[philo->left_fork]);
		pthread_mutex_unlock(&philo->data->fork_locker[philo->right_fork]);
		printing(philo, "\033[1;94m has taken a fork 🥄", real_time(philo), 0);
		printing(philo, " has taken a fork 🥄 \033[0m", real_time(philo), 0);
	}
	else
		pick_fork2(philo);
}

void	ft_eat(t_philos *philo)
{
	pthread_mutex_lock(&philo->data->gen_lock);
	if (philo->data->end_game == 0)
	{
		pthread_mutex_unlock(&philo->data->gen_lock);
		pick_fork(philo);
		printing(philo, "\033[1;35m is eating 🍰 \033[0m", real_time(philo), 0);
		pthread_mutex_lock(&philo->data->last_meal_locker);
		philo->last_meal = timestamp();
		pthread_mutex_unlock(&philo->data->last_meal_locker);
		pthread_mutex_lock(&philo->data->num_meals_locker);
		philo->data->total_num_meals++;
		pthread_mutex_unlock(&philo->data->num_meals_locker);
		ft_usleep(philo->data->eat_t, philo);
		pthread_mutex_lock(&philo->data->fork_locker[philo->left_fork]);
		philo->data->fork[philo->left_fork] = philo->id;
		pthread_mutex_unlock(&philo->data->fork_locker[philo->left_fork]);
		pthread_mutex_lock(&philo->data->fork_locker[philo->right_fork]);
		philo->data->fork[philo->right_fork] = philo->id;
		pthread_mutex_unlock(&philo->data->fork_locker[philo->right_fork]);
		return ;
	}
	pthread_mutex_unlock(&philo->data->gen_lock);
}

void	one_philo(t_philos *philo)
{
	pthread_mutex_lock(&philo->data->fork_locker[philo->left_fork]);
	printing(philo, "\033[1;94m has taken a fork 🥄 \033[0m", real_time(philo), 0);
	ft_usleep(philo->data->die_t, philo);
	printing(philo, "\033[1;31m died ❌ \033[0m", real_time(philo), 1);
	pthread_mutex_lock(&philo->data->gen_lock);
	philo->data->end_game = 1;
	pthread_mutex_unlock(&philo->data->gen_lock);
	pthread_mutex_unlock(&philo->data->fork_locker[philo->left_fork]);
}

void	ft_destroy(t_data *main)
{
	int	i;

	i = -1;
	while (++i < main->num_philos)
		if (pthread_join(main->t_philos[i].thread, NULL) != 0)
			return ;
	i = -1;
	while (++i < main->num_philos)
		pthread_mutex_destroy(&main->fork_locker[i]);
	pthread_mutex_destroy(&main->num_meals_locker);
	pthread_mutex_destroy(&main->last_meal_locker);
	pthread_mutex_destroy(&main->gen_lock);
	free(main->t_philos);
	free(main->fork);
	free(main->fork_locker);
	free(main);
}
