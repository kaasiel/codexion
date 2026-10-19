/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 08:48:32 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/19 09:23:01 by belaindr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_strlen(char *c)
{
	int	i;

	i = 0;
	while (c[i])
		i++;
	return (i);
}

int	value_verifier(t_args args)
{
	if (args.number_of_coders > 40)
	{
		fprintf(stderr,
			"\033[31mThe number of coder should not exceed 40.\033[0m\n");
		return (-1);
	}
	if (args.dongle_cooldown >= INT_MAX
		|| args.time_to_refactor >= INT_MAX
		|| args.time_to_burnout >= INT_MAX
		|| args.time_to_compile >= INT_MAX
		|| args.time_to_debug >= INT_MAX)
	{
		fprintf(stderr,
			"\033[31m That number is too big to use as timing!\033[0m\n");
		return (-1);
	}
	if (args.number_of_compiles_required >= 1000)
	{
		fprintf(stderr,
			"\033[31mlower the number of compile required!\033[0m\n");
		return (-1);
	}
	return (0);
}
