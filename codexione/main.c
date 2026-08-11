/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahhou <mdahhou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 13:26:13 by mdahhou           #+#    #+#             */
/*   Updated: 2026/08/07 16:47:22 by mdahhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_set_env(t_codexion *codexion, int ac, char **av)
{
	if (ac != 9)
	{
		ft_print_err("The program takes 8 arguments.");
		return (0);
	}
	if (!ft_init_codexion(codexion))
		return (0);
	if (!ft_parsing(av, codexion->args))
	{
		ft_free_all(&codexion->ressources);
		return (0);
	}
	if (!ft_init_dongles(codexion) || !ft_init_coders(codexion))
		return (0);
	return (1);
}

int	main(int ac, char **av)
{
	t_codexion	codexion;

	if (!ft_set_env(&codexion, ac, av))
		return (1);
	if (codexion.args->nb_of_compiles_req == 0)
	{
		ft_free_all(&codexion.ressources);
		return (0);
	}
	if (!ft_start_sim(&codexion))
	{
		ft_free_all(&codexion.ressources);
		return (1);
	}
	ft_free_all(&codexion.ressources);
	return (0);
}
