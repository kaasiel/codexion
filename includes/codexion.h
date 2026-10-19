/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 10:00:00 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/19 11:20:22 by belaindr         ###   ########.fr       */
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

# define HEAP_CAP 2

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
	pthread_mutex_t	ticket_mutex;
	t_dongles		*dongles;
	int				nb;
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
	int				id;
	int				is_taken;
	long			availabilitytime;
	t_coder			**heap;
	int				heap_size;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
}	t_dongles;

typedef struct monitor
{
	t_coder	*coders;
	int		nb;
}	t_moni;

int		parse_args(int argc, char **argv, t_args *args);
int		parse_field(const char *s, const char *name, long min, long *dest);
int		init_coder(t_coder *coders, t_dongles *dongles, t_args args,
			t_data *data);
int		init_fork(t_args args, t_dongles *dongles);
void	destroy_simulation(t_args args, t_data *data, t_dongles *dongles,
			t_coder *coders);
void	reset_clock(t_coder *coders, t_data *data, int nb);
int		launcher(t_coder *coders, int nb);
void	*routine(void *coder);
void	*monitoring(void *arg);
void	compile(t_coder *coder);
void	debug(t_coder *coder);
void	refactoring(t_coder *coder);
int		acquire_dongles(t_coder *c);
void	release_dongles(t_coder *c);
void	set_priority(t_coder *c);
int		can_take(t_coder *c);
int		sim_end(t_data *d);
int		value_verifier(t_args args);
void	wait_dongle(t_dongles *d);
int		heap_before(t_coder *a, t_coder *b);
void	heap_swap(t_coder **a, t_coder **b);
void	heap_push(t_dongles *d, t_coder *c);
void	heap_fix(t_dongles *d, int i);
void	heap_remove(t_dongles *d, t_coder *c);
t_coder	*heap_top(t_dongles *d);
void	ft_usleep(long nbr, t_data *data);
void	error_printer(char *message);
int		window_ids(t_coder *c, int *ids);
void	window_lock(t_data *d, int *ids, int n);
void	window_unlock(t_data *d, int *ids, int n);
void	window_unlock_but(t_data *d, int *ids, int n, int keep);

#endif
