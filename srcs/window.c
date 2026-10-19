/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 10:00:00 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/19 10:00:00 by belaindr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	sort_ids(int *ids)
{
	int	i;
	int	j;
	int	tmp;

	i = 1;
	while (i < 4)
	{
		j = i;
		while (j > 0 && ids[j - 1] > ids[j])
		{
			tmp = ids[j];
			ids[j] = ids[j - 1];
			ids[j - 1] = tmp;
			j--;
		}
		i++;
	}
}

int	window_ids(t_coder *c, int *ids)
{
	int	nb;
	int	i;
	int	n;

	nb = c->data->nb;
	ids[0] = (c->left->id + nb - 1) % nb;
	ids[1] = c->left->id;
	ids[2] = c->right->id;
	ids[3] = (c->right->id + 1) % nb;
	sort_ids(ids);
	n = 1;
	i = 1;
	while (i < 4)
	{
		if (ids[i] != ids[n - 1])
			ids[n++] = ids[i];
		i++;
	}
	return (n);
}

void	window_lock(t_data *d, int *ids, int n)
{
	int	i;

	i = 0;
	while (i < n)
		pthread_mutex_lock(&d->dongles[ids[i++]].mutex);
}

void	window_unlock(t_data *d, int *ids, int n)
{
	while (n > 0)
		pthread_mutex_unlock(&d->dongles[ids[--n]].mutex);
}

void	window_unlock_but(t_data *d, int *ids, int n, int keep)
{
	while (n > 0)
	{
		n--;
		if (ids[n] != keep)
			pthread_mutex_unlock(&d->dongles[ids[n]].mutex);
	}
}
