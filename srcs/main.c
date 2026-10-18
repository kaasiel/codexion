/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 07:31:05 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/18 07:31:41 by belaindr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_simulation(t_args args, t_data *data,
		t_dongles **dongles, t_coder **coders)
{
	int	i;

	data->simulation_end = 0;
	data->start_time = get_time();
	data->strategy = args.scheduler;
	data->cooldown = args.dongle_cooldown;
	data->ticket = 0;
	pthread_mutex_init(&data->sched_mutex, NULL);
	pthread_cond_init(&data->sched_cond, NULL);
	pthread_mutex_init(&data->print_mutex, NULL);
	*dongles = calloc((size_t)args.number_of_coders, sizeof(**dongles));
	*coders = calloc((size_t)args.number_of_coders, sizeof(**coders));
	if (*dongles == NULL || *coders == NULL)
		return (free(*dongles), free(*coders), 1);
	if (init_fork(args, *dongles) != 0
		|| init_coder(*coders, *dongles, args, data) != 0)
		return (free(*dongles), free(*coders), 1);
	data->start_time = get_time();
	i = 0;
	while (i < args.number_of_coders)
		(*coders)[i++].last_compile_start = data->start_time;
	return (0);
}

static void	destroy_simulation(t_args args, t_data *data,
		t_dongles *dongles, t_coder *coders)
{
	int	i;

	i = 0;
	while (i < args.number_of_coders)
		pthread_mutex_destroy(&coders[i++].mutex);
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->sched_mutex);
	pthread_cond_destroy(&data->sched_cond);
	free(dongles);
	free(coders);
}

int	main(int argc, char **argv)
{
	t_args		args;
	t_dongles	*dongles;
	t_coder		*coders;
	t_data		data;

	if (parse_args(argc, argv, &args) != 0)
		return (1);
	if (init_simulation(args, &data, &dongles, &coders) != 0)
		return (1);
	launcher(coders, args.number_of_coders);
	destroy_simulation(args, &data, dongles, coders);
	return (0);
}
