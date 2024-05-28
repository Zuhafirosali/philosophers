/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 07:17:52 by zuhafirosal       #+#    #+#             */
/*   Updated: 2024/05/28 16:11:25 by zfiros-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdint.h>

typedef struct s_philos
{
	int				id;
	int				right_fork;
	int				left_fork;
	long long		last_meal;
	pthread_t		thread;
	struct s_data	*data;
}	t_philos;

typedef struct s_data
{
	int				num_philos;
	long long		die_t;
	long long		eat_t;
	long long		sleep_t;
	long long		start_time;
	int				num_to_eat;
	int				total_num_meals;
	t_philos		*t_philos;
	pthread_mutex_t	num_meals_locker;
	pthread_mutex_t	last_meal_locker;
	pthread_mutex_t	gen_lock;
	pthread_mutex_t	*fork_locker;
	int				end_game;
	int				*fork;
}	t_data;

// init
int			main_init(int ac, char **av, t_data *main);
int			philos_init(t_data *main);
//libft
int			ft_isdigit(int c);
int			ft_atoi(char *str);
void		*ft_calloc(size_t count, size_t size);
//philo
int			valid_args(char **args);
void		one_philo(t_philos *philo);
void		*philos_life(void *arg);
void		philos_launch(t_data *main);
int			printing(t_philos *philo, char *str, long long time, int i);
void		death_checker(t_philos *philo);
void		num_meals_checker(t_data *main);
void		ft_think(t_philos *philo);
void		ft_sleep(t_philos *philo);
void		ft_eat(t_philos *philo);
void		pick_fork2(t_philos *philo);
void		pick_fork(t_philos *philo);
int			printing(t_philos *philo, char *str, long long time, int i);
void		one_philo(t_philos *philo);
void		ft_destroy(t_data *main);
//time
long long	timestamp(void);
void		ft_usleep(long long wait_t, t_philos *philo);
long long	timestamp(void);
long long	real_time(t_philos *philo);

#endif