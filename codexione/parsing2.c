/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahhou <mdahhou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 05:43:16 by mdahhou           #+#    #+#             */
/*   Updated: 2026/08/07 16:46:20 by mdahhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	ft_neg_zero_is_valid(char *s, int i)
{
	if (!s[i])
	{
		ft_print_err_parsing(0);
		return (0);
	}
	while (s[i] == '0')
		i++;
	if (s[i])
	{
		ft_print_err_parsing(0);
		return (0);
	}
	return (1);
}

static int	ft_digits_are_valid(char *s, int i)
{
	if (!s[i])
	{
		ft_print_err_parsing(0);
		return (0);
	}
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
		{
			ft_print_err_parsing(0);
			return (0);
		}
		i++;
	}
	return (1);
}

int	ft_nb_is_valid(char *s)
{
	int	i;

	i = 0;
	if (s[i] == '-')
		return (ft_neg_zero_is_valid(s, i + 1));
	if (s[i] == '+')
		i++;
	return (ft_digits_are_valid(s, i));
}
