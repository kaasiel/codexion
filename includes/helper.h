/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 06:44:59 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/19 09:01:40 by belaindr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPER_H
# define HELPER_H

# include <stdio.h>
# include <sys/time.h>

typedef struct s_coder	t_coder;

void	printlog(t_coder *coder, char *msg);
long	get_time(void);
int		ft_strlen(char *c);

#endif