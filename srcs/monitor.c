/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 07:04:00 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/19 08:05:43 by belaindr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check(t_coder *coder)
{
	long	last;
	long	now;

	pthread_mutex_lock(&coder->mutex);
	last = coder->last_compile_start;
	pthread_mutex_unlock(&coder->mutex);
	now = get_time();
	if (now - last <= coder->time_to_burntout)
		return (0);
	pthread_mutex_lock(&coder->data->print_mutex);
	if (!coder->data->simulation_end)
	{
		coder->data->simulation_end = 1;
		printf("%ld %d \033[1;31mburned out\033[0m\n",
			now - coder->data->start_time, coder->id);
	}
	pthread_mutex_unlock(&coder->data->print_mutex);
	return (1);
}

static int	ender(t_moni *monitor)
{
	int	i;
	int	done;

	i = 0;
	while (i < monitor->nb)
	{
		pthread_mutex_lock(&monitor->coders[i].mutex);
		done = (monitor->coders[i].max_compile <= 0);
		pthread_mutex_unlock(&monitor->coders[i].mutex);
		if (!done)
			return (0);
		i++;
	}
	return (1);
}

void	*monitoring(void *arg)
{
	t_moni	*m;
	int		i;

	m = (t_moni *)arg;
	while (1)
	{
		i = 0;
		while (i < m->nb)
		{
			if (check(&m->coders[i]))
				return (NULL);
			i++;
		}
		if (ender(m))
		{
			pthread_mutex_lock(&m->coders[0].data->print_mutex);
			m->coders[0].data->simulation_end = 1;
			pthread_mutex_unlock(&m->coders[0].data->print_mutex);
			return (NULL);
		}
		usleep(1000);
	}
}
