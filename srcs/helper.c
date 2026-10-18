/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 06:59:40 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/18 07:01:16 by belaindr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	printlog(t_coder *coder, char *msg)
{
	long	timestamp;

	pthread_mutex_lock(&coder->data->print_mutex);
	if (!coder->data->simulation_end)
	{
		timestamp = get_time() - coder->data->start_time;
		printf("%ld %d %s\n", timestamp, coder->id, msg);
	}
	pthread_mutex_unlock(&coder->data->print_mutex);
}
