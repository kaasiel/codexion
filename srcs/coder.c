/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 10:00:00 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/19 08:08:06 by belaindr         ###   ########.fr       */
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
	printlog(coder, "\033[1;32mis compiling\033[0m");
	usleep(coder->time_to_compile * 1000);
	release_dongles(coder);
	pthread_mutex_lock(&coder->mutex);
	coder->max_compile--;
	pthread_mutex_unlock(&coder->mutex);
}

void	debug(t_coder *coder)
{
	printlog(coder, "mis debugging");
	usleep(coder->time_to_debug * 1000);
}

void	refactoring(t_coder *coder)
{
	printlog(coder, "is refactoring");
	usleep(coder->time_to_refactor * 1000);
}
