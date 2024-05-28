/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 15:12:59 by zfiros-a          #+#    #+#             */
/*   Updated: 2024/05/28 19:16:50 by zfiros-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_usleep(long long wait_t, t_philos *philo)
{
	long long	current_time;

	current_time = timestamp();
	pthread_mutex_lock(&philo->data->gen_lock);
	if (philo->data->end_game != 0)
	{
		pthread_mutex_unlock(&philo->data->gen_lock);
		return ;
	}
	pthread_mutex_unlock(&philo->data->gen_lock);
	while (timestamp() - current_time < wait_t)
		usleep(100);
}

long long	timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

long long	real_time(t_philos *philo)
{
	return (timestamp() - philo->data->start_time);
}

int	printing(t_philos *philo, char *str, long long time, int i)
{
	pthread_mutex_lock(&philo->data->gen_lock);
	if (philo->data->end_game == 0)
		printf("🕰️ time : %lld ms | id : %d | %s\n", time, philo->id, str);
	else if (i)
		printf("🕰️ time : %lld ms | id : %d | %s\n", time, philo->id, str);
	pthread_mutex_unlock(&philo->data->gen_lock);
	return (1);
}

int	valid_args(char **args)
{
	int	i;
	int	n;

	i = 1;
	while (args[i])
	{
		n = 0;
		while (args[i][n])
		{
			if (ft_isdigit(args[i][n]))
				n++;
			else
				return (0);
		}
		i++;
	}
	return (1);
}
