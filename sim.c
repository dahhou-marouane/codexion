/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahhou <mdahhou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 18:20:45 by mdahhou           #+#    #+#             */
/*   Updated: 2026/08/04 03:35:34 by mdahhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_sim_is_stoped(t_codexion *codexion)
{
	int	stoped;

	pthread_mutex_lock(codexion->stop_lock);
	stoped = codexion->stop;
	pthread_mutex_unlock(codexion->stop_lock);
	return (stoped);
}
int	ft_sim_is_started(t_codexion *codexion)
{
	int	started;

	pthread_mutex_lock(codexion->start_lock);
	started = codexion->start;
	pthread_mutex_unlock(codexion->start_lock);
	return (started);
}

void	ft_set_sim_stop(t_codexion *codexion)
{
	pthread_mutex_lock(codexion->stop_lock);
	codexion->stop = 1;
	pthread_mutex_unlock(codexion->stop_lock);
}

int	ft_start_coders(t_codexion *codexion)
{
	int		i;
	long	time;

	i = 0;
	time = ft_get_curr_t_mic();
	while (i < codexion->args->nb_of_coders)
	{
		codexion->coders[i].l_com_start = time;
		i++;
	}
	i = 0;
	while (i < codexion->args->nb_of_coders)
	{
		if (!ft_pthread_create(&codexion->ressources, coder_routine,
				&codexion->coders[i]))
		{
			ft_set_sim_stop(codexion);
			ft_free_all(&codexion->ressources);
			return (0);
		}
		i++;
	}
	return (1);
}

int	ft_start_monitor(t_codexion *codexion)
{
	if (!ft_pthread_create(&codexion->ressources, monitor_routine, codexion))
	{
		ft_set_sim_stop(codexion);
		ft_free_all(&codexion->ressources);
		return (0);
	}
	return (1);
}

int	ft_start_sim(t_codexion *codexion)
{
	codexion->start_time_mic = ft_get_curr_t_mic();
	if (!ft_start_coders(codexion))
		return (0);
	if (!ft_start_monitor(codexion))
		return (0);
	pthread_mutex_lock(codexion->start_lock);
	codexion->start = 1;
	pthread_cond_broadcast(codexion->start_cond);
	pthread_mutex_unlock(codexion->start_lock);
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_codexion	*codexion;
	int			i;
	long		last;
	int			f_routine;
	int			done;

	codexion = (t_codexion *)arg;
	pthread_mutex_lock(codexion->start_lock);
	while (!codexion->start)
		pthread_cond_wait(codexion->start_cond, codexion->start_lock);
	pthread_mutex_unlock(codexion->start_lock);
	while (!ft_sim_is_stoped(codexion))
	{
		i = 0;
		ft_usleep(50, codexion);
		pthread_mutex_lock(codexion->f_routine_lock);
		f_routine = codexion->f_routine;
		pthread_mutex_unlock(codexion->f_routine_lock);
		while (i < codexion->args->nb_of_coders
			&& codexion->args->nb_of_coders != f_routine)
		{
			pthread_mutex_lock(codexion->coders[i].lock);
			last = codexion->coders[i].l_com_start;
			done = codexion->coders[i].compile_count
				>= codexion->args->nb_of_compiles_req;
			pthread_mutex_unlock(codexion->coders[i].lock);
			if (!done && ft_get_curr_t_mic() - last > codexion->args->t_to_burnout)
			{
				ft_set_sim_stop(codexion);
				ft_print_logs(codexion, i + 1, 4);
				return (NULL);
			}
			i++;
		}
		pthread_mutex_lock(codexion->f_routine_lock);
		if (codexion->f_routine == codexion->args->nb_of_coders)
		{
			ft_set_sim_stop(codexion);
			pthread_mutex_unlock(codexion->f_routine_lock);
			return (NULL);
		}
		pthread_mutex_unlock(codexion->f_routine_lock);
	}
	return (NULL);
}

long	ft_get_key(t_coder *coder)
{
	long	key;

	if (coder->codexion->args->fifo)
	{
		pthread_mutex_lock(coder->codexion->seq_lock);
		key = coder->codexion->seq++;
		pthread_mutex_unlock(coder->codexion->seq_lock);
	}
	else
	{
		pthread_mutex_lock(coder->lock);
		key = coder->l_com_start + coder->codexion->args->t_to_burnout;
		pthread_mutex_unlock(coder->lock);
	}
	return (key);
}

void	ft_lock_dongles(t_coder *coder)
{
	if (coder->left->id < coder->right->id)
	{
		pthread_mutex_lock(coder->left->lock);
		pthread_mutex_lock(coder->right->lock);
	}
	else
	{
		pthread_mutex_lock(coder->right->lock);
		pthread_mutex_lock(coder->left->lock);
	}
}

void	ft_unlock_dongles(t_coder *coder)
{
	pthread_mutex_unlock(coder->right->lock);
	pthread_mutex_unlock(coder->left->lock);
}

void	ft_aquire_dongles(t_coder *coder)
{
	t_request	req;
	long		now;

	req.coder_id = coder->id;
	req.key = ft_get_key(coder);
	req.req_time = ft_get_curr_t_mic();
	ft_lock_dongles(coder);
	push(&coder->left->heap, &req);
	push(&coder->right->heap, &req);
	while (!ft_sim_is_stoped(coder->codexion))
	{
		now = ft_get_curr_t_mic();
		if (!coder->left->taken && !coder->right->taken
			&& now >= coder->left->cooldown_end
			&& now >= coder->right->cooldown_end
			&& peek(&coder->left->heap) == coder->id
			&& peek(&coder->right->heap) == coder->id)
		{
			coder->left->taken = 1;
			coder->right->taken = 1;
			ft_print_logs(coder->codexion, coder->id, 0);
			ft_compile(coder);
			pop(&coder->left->heap);
			pop(&coder->right->heap);
			break ;
		}
		ft_unlock_dongles(coder);
		ft_usleep(800, coder->codexion);
		ft_lock_dongles(coder);
	}
	ft_unlock_dongles(coder);
}

void	ft_release_dongles(t_coder *coder)
{
	ft_lock_dongles(coder);
	coder->left->taken = 0;
	coder->right->taken = 0;
	coder->left->cooldown_end = coder->codexion->args->dongle_cooldown
		+ ft_get_curr_t_mic();
	coder->right->cooldown_end = coder->codexion->args->dongle_cooldown
		+ ft_get_curr_t_mic();
	ft_unlock_dongles(coder);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	pthread_mutex_lock(coder->codexion->start_lock);
	while (!coder->codexion->start)
		pthread_cond_wait(coder->codexion->start_cond,
			coder->codexion->start_lock);
	pthread_mutex_unlock(coder->codexion->start_lock);
	if (coder->left == coder->right)
	{
		while (!ft_sim_is_stoped(coder->codexion))
			usleep(1000);
		return (NULL);
	}
	if (coder->id %2 == 0)
		usleep(coder->codexion->args->t_to_compile/2);
	while (!ft_sim_is_stoped(coder->codexion)
		&& coder->compile_count < coder->codexion->args->nb_of_compiles_req)
	{
		ft_aquire_dongles(coder);
		ft_release_dongles(coder);
		ft_debug(coder);
		ft_refactor(coder);
	}
	return (NULL);
}
