/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 07:02:25 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/18 07:02:54 by belaindr         ###   ########.fr       */
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
		coders[i].id = i;
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

int	init_fork(t_args args, t_dongles *dongles_list)
{
	int	i;
	int	nb;

	i = 0;
	nb = args.number_of_coders;
	while (i < nb)
	{
		dongles_list[i].id = i;
		dongles_list[i].is_taken = 0;
		i++;
	}
	return (0);
}
