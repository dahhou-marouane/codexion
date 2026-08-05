/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahhou <mdahhou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 15:55:45 by mdahhou           #+#    #+#             */
/*   Updated: 2026/08/05 23:28:46 by mdahhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include "struct.h"
# include <limits.h> // max_int
# include <stdio.h>
# include <stdlib.h>
# include <string.h>   //strcmp
# include <sys/time.h> // gettimeofday
# include <unistd.h>   // usleep

// parsing.c
int				ft_parsing(char **av, t_arguments *args);

// garbage_collector.c
void			ft_free_garbage(t_grbmanager *grbmanager);
void			*ft_malloc(size_t size, t_ressources *ressources);

// thread_mutex_cond.c
pthread_mutex_t	*ft_mutex_create(t_ressources *ressources);
pthread_cond_t	*ft_cond_create(t_ressources *ressources);
pthread_t		*ft_pthread_create(t_ressources *ressources, void *(*f)(void *),
					void *arg);

// free_all.c
void			ft_free_all(t_ressources *ressources);

// init.c
int				ft_init_codexion(t_codexion *codexion);
int				ft_init_dongles(t_codexion *codexion);
int				ft_init_coders(t_codexion *codexion);

// main.c
int				ft_sim_is_stoped(t_codexion *codexion);

void			ft_set_sim_stop(t_codexion *codexion);
int				ft_start_sim(t_codexion *codexion);

// routine.c
void				ft_debug(t_coder *coder);
void				ft_compile(t_coder *coder);
void				ft_refactor(t_coder *coder);

// print.c
void			ft_print_logs(t_codexion *codexion, int id, int msg);
void			ft_print_err(char *msg);

// heap.c
int				peek(t_heap *heap);
void			push(t_heap *heap, t_request *req);
void			pop(t_heap *heap);

// time.c
long			ft_get_curr_t_mic(void);
void			ft_usleep(long time, t_codexion *codexion);
// sim.c
void			*coder_routine(void *arg);

void			*monitor_routine(void *arg);
#endif
