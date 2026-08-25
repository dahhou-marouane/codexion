/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahhou <mdahhou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 15:55:45 by mdahhou           #+#    #+#             */
/*   Updated: 2026/08/25 04:21:41 by mdahhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include "struct.h"
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

int				ft_parsing(char **av, t_arguments *args);
int				ft_nb_is_valid(char *s);

void			ft_free_garbage(t_grbmanager *grbmanager);
void			*ft_malloc(size_t size, t_ressources *ressources);

pthread_mutex_t	*ft_mutex_create(t_ressources *ressources);
pthread_cond_t	*ft_cond_create(t_ressources *ressources);
pthread_t		*ft_pthread_create(t_ressources *ressources, void *(*f)(void *),
					void *arg);

void			ft_free_all(t_ressources *ressources);
int				single_coder(t_coder *coder);
int				ft_init_codexion(t_codexion *codexion);
int				ft_init_dongles(t_codexion *codexion);
int				ft_init_coders(t_codexion *codexion);

void			ft_compile(t_coder *coder);
void			*coder_routine(void *arg);
long			ft_get_key(t_coder *coder);

int				ft_aquire_dongles(t_coder *coder);
void			ft_lock_dongles(t_coder *coder);
void			ft_unlock_dongles(t_coder *coder);
void			ft_release_dongles(t_coder *coder);

void			ft_print_logs(t_codexion *codexion, int id, int msg);
void			ft_print_err(char *msg);
void			ft_print_err_parsing(int msg);

int				peek(t_heap *heap);
void			push(t_heap *heap, t_request *req);
int				heap_compare(t_heap *heap, int child, int parent);

long			ft_get_curr_t_mic(void);
void			ft_usleep(long time, t_codexion *codexion);

int				ft_sim_is_stoped(t_codexion *codexion);
void			ft_set_sim_stop(t_codexion *codexion);
int				ft_start_sim(t_codexion *codexion);
int				ft_start_monitor(t_codexion *codexion);
int				ft_start_coders(t_codexion *codexion);

void			*monitor_routine(void *arg);

void			heap_remove_id(t_heap *heap, int coder_id);
#endif
