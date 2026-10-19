/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 10:00:00 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/19 10:00:00 by belaindr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_coder(t_coder *coders, t_dongles *dongles, t_args args, t_data *data)
{
	int	i;
	int	nb;

	i = 0;
	nb = args.number_of_coders;
	while (i < nb)
	{
		coders[i].id = i + 1;
		coders[i].data = data;
		coders[i].last_compile_start = data->start_time;
		coders[i].max_compile = args.number_of_compiles_required;
		coders[i].time_to_burntout = args.time_to_burnout;
		coders[i].time_to_compile = args.time_to_compile;
		coders[i].time_to_debug = args.time_to_debug;
		coders[i].time_to_refactor = args.time_to_refactor;
		coders[i].left = &dongles[i];
		coders[i].right = &dongles[(i + 1) % nb];
		if (pthread_mutex_init(&coders[i].mutex, NULL) != 0)
			return (-1);
		i++;
	}
	return (0);
}

int	init_fork(t_args args, t_dongles *dongles)
{
	int	i;

	i = 0;
	while (i < args.number_of_coders)
	{
		dongles[i].id = i;
		dongles[i].is_taken = 0;
		dongles[i].availabilitytime = 0;
		dongles[i].heap_size = 0;
		dongles[i].heap = malloc(sizeof(t_coder *) * HEAP_CAP);
		if (dongles[i].heap == NULL
			|| pthread_mutex_init(&dongles[i].mutex, NULL) != 0
			|| pthread_cond_init(&dongles[i].cond, NULL) != 0)
			return (-1);
		i++;
	}
	return (0);
}

static void	destroy_dongles(t_dongles *dongles, int nb)
{
	int	i;

	i = 0;
	while (i < nb)
	{
		pthread_mutex_destroy(&dongles[i].mutex);
		pthread_cond_destroy(&dongles[i].cond);
		free(dongles[i].heap);
		i++;
	}
}

void	destroy_simulation(t_args args, t_data *data, t_dongles *dongles,
		t_coder *coders)
{
	int	i;

	destroy_dongles(dongles, args.number_of_coders);
	i = 0;
	while (i < args.number_of_coders)
		pthread_mutex_destroy(&coders[i++].mutex);
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->ticket_mutex);
	free(dongles);
	free(coders);
}

void	reset_clock(t_coder *coders, t_data *data, int nb)
{
	int	i;

	data->start_time = get_time();
	i = 0;
	while (i < nb)
		coders[i++].last_compile_start = data->start_time;
}
