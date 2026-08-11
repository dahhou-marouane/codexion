/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahhou <mdahhou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 13:26:23 by mdahhou           #+#    #+#             */
/*   Updated: 2026/08/07 16:46:36 by mdahhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	ft_save_args(int *arr, t_arguments *args, char *scheduler)
{
	args->nb_of_coders = arr[0];
	args->t_to_burnout = arr[1] * 1000;
	args->t_to_compile = arr[2] * 1000;
	args->t_to_debug = arr[3] * 1000;
	args->t_to_refactor = arr[4] * 1000;
	args->nb_of_compiles_req = arr[5];
	args->dongle_cooldown = arr[6] * 1000;
	if (strcmp("fifo", scheduler) == 0)
		args->fifo = 1;
	else
		args->fifo = 0;
}

static int	ft_fifo_or_edf(char *s, int *arr, t_arguments *args)
{
	if (strcmp("fifo", s) != 0 && strcmp("edf", s) != 0)
	{
		ft_print_err_parsing(3);
		return (0);
	}
	ft_save_args(arr, args, s);
	return (1);
}

static int	ft_atoi(char *str)
{
	int			i;
	long long	res;

	i = 0;
	res = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (res > INT_MAX / 10 || (res == INT_MAX / 10 && (str[i]
					- '0') > INT_MAX % 10))
			return (-1);
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return ((int)res);
}

int	ft_parsing(char **av, t_arguments *args)
{
	int	i;
	int	arr[7];

	i = 1;
	while (i < 8)
	{
		if (!ft_nb_is_valid(av[i]))
			return (0);
		arr[i - 1] = ft_atoi(av[i]);
		if (arr[i - 1] == -1)
		{
			ft_print_err_parsing(1);
			return (0);
		}
		i++;
	}
	if (arr[0] == 0)
	{
		ft_print_err_parsing(2);
		return (0);
	}
	if (!ft_fifo_or_edf(av[8], arr, args))
		return (0);
	return (1);
}
