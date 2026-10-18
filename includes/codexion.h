/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 06:29:50 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/18 06:44:14 by belaindr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include "helper.h"
# include <stdio.h>
# include <string.h>
# include <limits.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_dongles	t_dongles;
typedef struct s_coder		t_coder;

typedef enum e_scheduler
{
	AL_FIFO,
	AL_EDF
}	t_scheduler;

typedef struct s_args
{
	int			number_of_coders;
	long		time_to_burnout;
	long		time_to_compile;
	long		time_to_debug;
	long		time_to_refactor;
	int			number_of_compiles_required;
	long		dongle_cooldown;
	t_scheduler	scheduler;
}	t_args;

typedef struct helper
{
	long			start_time;
	pthread_mutex_t	print_mutex;
	int				simulation_end;
	t_scheduler		strategy;
	long			cooldown;
	long			ticket;
	pthread_mutex_t	sched_mutex;
	pthread_cond_t	sched_cond;
}	t_data;

typedef struct s_coder
{
	int				id;
	int				max_compile;
	long			key;
	long			ticket;
	long			last_compile_start;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	long			time_to_burntout;
	t_dongles		*left;
	t_dongles		*right;
	pthread_mutex_t	mutex;
	t_data			*data;
}	t_coder;

typedef struct s_dongles
{
	int			id;
	int			is_taken;
	long		availabilitytime;
	t_coder		*queue[2];
	int			queue_size;
}	t_dongles;

typedef struct monitor
{
	t_coder	*coders;
	int		nb;
}	t_moni;

int		acquire_dongles(t_coder *c);
int		can_take(t_coder *c);
void	queue_dongles(t_dongles *d, t_coder *c, int add);
int		sim_end(t_data *d);
void	wait_ms(t_data *d, long ms);
int		parse_args(int argc, char **argv, t_args *args);
int		parse_field(const char *s, const char *name, long min, long *dest);
int		init_coder(t_coder *coders, t_dongles *dongles, t_args args,
			t_data *data);
int		init_fork(t_args args, t_dongles *dongles_list);
int		launcher(t_coder *coders, int nb);
void	*routine(void *coder);
void	release_dongles(t_coder *c);
void	compile(t_coder *coder);
void	debug(t_coder *coder);
void	refrcatinng(t_coder *coder);
void	burnout(t_coder *coder);
void	*monitoring(void *arg);

#endif