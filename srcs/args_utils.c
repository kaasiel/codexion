/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 07:29:28 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/19 10:35:23 by belaindr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_all_digits(const char *s)
{
	int	i;

	i = 0;
	if (!s[0])
		return (0);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	append_digit(long *result, int digit, const char *s)
{
	if (*result > (LONG_MAX - digit) / 10)
	{
		error_printer("codexion: error: value out of range:");
		printf("%s", s);
		return (-1);
	}
	*result = *result * 10 + digit;
	return (0);
}

static int	str_to_long(const char *s, long *out)
{
	long	result;
	int		i;
	int		digit;

	if (s[0] == '-')
	{
		error_printer("this program does not take negative values");
		return (-1);
	}
	if (!is_all_digits(s))
	{
		error_printer("codexion: error: not a valid integer");
		return (-1);
	}
	result = 0;
	i = 0;
	while (s[i])
	{
		digit = s[i] - '0';
		if (append_digit(&result, digit, s) < 0)
			return (-1);
		i++;
	}
	*out = result;
	return (0);
}

int	parse_field(const char *s, const char *name, long min, long *dest)
{
	if (str_to_long(s, dest) < 0)
		return (-1);
	if (*dest < min)
	{
		fprintf(stderr, "\033[31mcodexion: error:"
			"%s must be >= %ld (got %ld)\033[0m\n",
			name, min, *dest);
		return (-1);
	}
	return (0);
}
