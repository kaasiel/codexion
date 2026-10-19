/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 10:00:00 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/19 12:15:43 by belaindr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compile(t_coder *coder)
{
	if (!acquire_dongles(coder))
		return ;
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile_start = get_time();
	pthread_mutex_unlock(&coder->mutex);
	printlog(coder, "is compiling");
	ft_usleep(coder->time_to_compile, coder->data);
	release_dongles(coder);
	pthread_mutex_lock(&coder->mutex);
	coder->max_compile--;
	pthread_mutex_unlock(&coder->mutex);
}

void	debug(t_coder *coder)
{
	printlog(coder, "\033[32mis debugging\033[0m");
	ft_usleep(coder->time_to_debug, coder->data);
}

void	refactoring(t_coder *coder)
{
	printlog(coder, "\033[36mis refactoring\033[0m");
	ft_usleep(coder->time_to_refactor, coder->data);
}
