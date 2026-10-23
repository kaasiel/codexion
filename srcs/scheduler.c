/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 07:17:37 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/21 22:01:02 by belaindr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	launcher(t_coder *coders, int nb)
{
	int			i;
	t_moni		m;
	pthread_t	*threads;
	pthread_t	mon;

	m.nb = nb;
	m.coders = coders;
	threads = malloc(sizeof(pthread_t) * nb);
	if (!threads)
		return (1);
	i = 0;
	while (i < nb)
	{
		if (pthread_create(&threads[i], NULL, routine, &coders[i]) != 0)
			return (free(threads), 1);
		i++;
	}
	pthread_create(&mon, NULL, monitoring, &m);
	i = 0;
	while (i < nb)
		pthread_join(threads[i++], NULL);
	pthread_join(mon, NULL);
	free(threads);
	return (0);
}

static int	should_stop(t_coder *c)
{
	int	stop;

	pthread_mutex_lock(&c->data->print_mutex);
	stop = c->data->simulation_end;
	pthread_mutex_unlock(&c->data->print_mutex);
	if (stop)
		return (1);
	pthread_mutex_lock(&c->mutex);
	stop = (c->max_compile <= 0);
	pthread_mutex_unlock(&c->mutex);
	return (stop);
}

void	*routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->left == coder->right)
	{
		printlog(coder, "has taken a dongle");
		while (!should_stop(coder))
			usleep(1000);
		return (NULL);
	}
	while (!should_stop(coder))
	{
		compile(coder);
		debug(coder);
		refactoring(coder);
		if (should_stop(coder))
			break ;
	}
	return (NULL);
}
