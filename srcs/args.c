/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 07:28:50 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/18 07:29:21 by belaindr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	parse_scheduler(const char *s, t_scheduler *out)
{
	if (!strcmp(s, "fifo"))
	{
		*out = AL_FIFO;
		return (0);
	}
	if (!strcmp(s, "edf"))
	{
		*out = AL_EDF;
		return (0);
	}
	fprintf(stderr,
		"codexion: error: scheduler must be \"fifo\" or \"edf\" (got \"%s\")\n",
		s);
	return (-1);
}

static int	print_usage(const char *prog)
{
	fprintf(stderr, "Usage: %s number_of_coders time_to_burnout "
		"time_to_compile time_to_debug time_to_refactor "
		"number_of_compiles_required dongle_cooldown scheduler\n", prog);
	fprintf(stderr, "  scheduler: fifo | edf\n");
	return (-1);
}

int	parse_args(int argc, char **argv, t_args *args)
{
	long	value;

	if (argc != 9)
		return (print_usage(argv[0]));
	if (parse_field(argv[1], "number_of_coders", 1, &value) < 0)
		return (-1);
	args->number_of_coders = (int)value;
	if (parse_field(argv[2], "time_to_burnout", 1, &args->time_to_burnout) < 0)
		return (-1);
	if (parse_field(argv[3], "time_to_compile", 1, &args->time_to_compile) < 0)
		return (-1);
	if (parse_field(argv[4], "time_to_debug", 0, &args->time_to_debug) < 0)
		return (-1);
	if (parse_field(argv[5], "time_to_refactor", 0,
			&args->time_to_refactor) < 0)
		return (-1);
	if (parse_field(argv[6], "number_of_compiles_required", 1, &value) < 0)
		return (-1);
	args->number_of_compiles_required = (int)value;
	if (parse_field(argv[7], "dongle_cooldown", 0,
			&args->dongle_cooldown) < 0)
		return (-1);
	if (parse_scheduler(argv[8], &args->scheduler) < 0)
		return (-1);
	return (0);
}
