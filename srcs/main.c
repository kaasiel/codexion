/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 10:00:00 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/19 09:08:17 by belaindr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "helper.h"

static int	init_data(t_args args, t_data *data)
{
	data->simulation_end = 0;
	data->strategy = args.scheduler;
	data->cooldown = args.dongle_cooldown;
	data->ticket = 0;
	data->nb = args.number_of_coders;
	data->start_time = get_time();
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->ticket_mutex, NULL) != 0)
		return (pthread_mutex_destroy(&data->print_mutex), 1);
	return (0);
}

static int	alloc_all(t_args args, t_dongles **dongles, t_coder **coders)
{
	size_t	n;

	n = (size_t)args.number_of_coders;
	*dongles = malloc(sizeof(**dongles) * n);
	*coders = malloc(sizeof(**coders) * n);
	if (*dongles == NULL || *coders == NULL)
		return (free(*dongles), free(*coders), 1);
	memset(*dongles, 0, sizeof(**dongles) * n);
	memset(*coders, 0, sizeof(**coders) * n);
	return (0);
}

static int	init_simulation(t_args args, t_data *data,
		t_dongles **dongles, t_coder **coders)
{
	if (init_data(args, data) != 0)
		return (1);
	if (alloc_all(args, dongles, coders) != 0)
	{
		pthread_mutex_destroy(&data->print_mutex);
		pthread_mutex_destroy(&data->ticket_mutex);
		return (1);
	}
	data->dongles = *dongles;
	if (init_fork(args, *dongles) != 0
		|| init_coder(*coders, *dongles, args, data) != 0)
	{
		destroy_simulation(args, data, *dongles, *coders);
		return (1);
	}
	return (0);
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
	if (value_verifier(args) != 0)
		return (1);
	reset_clock(coders, &data, args.number_of_coders);
	launcher(coders, args.number_of_coders);
	destroy_simulation(args, &data, dongles, coders);
	return (0);
}
