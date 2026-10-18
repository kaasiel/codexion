/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 07:29:28 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/18 07:29:28 by belaindr         ###   ########.fr       */
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
		fprintf(stderr, "codexion: error: value out of range: %s\n", s);
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
		return (-1);
	if (!is_all_digits(s))
	{
		fprintf(stderr, "codexion: error: not a valid integer: %s\n", s);
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
		fprintf(stderr, "codexion: error: %s must be >= %ld (got %ld)\n",
			name, min, *dest);
		return (-1);
	}
	return (0);
}
