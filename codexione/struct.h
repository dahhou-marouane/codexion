/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahhou <mdahhou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 16:45:17 by mdahhou           #+#    #+#             */
/*   Updated: 2026/08/25 04:05:54 by mdahhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H
# include <pthread.h>

typedef struct s_request
{
	int							coder_id;
	long						key;
	long						req_time;
}								t_request;

typedef struct s_heap
{
	t_request					*items;
	int							size;
}								t_heap;

typedef struct s_dongle
{
	int							id;
	int							taken;
	t_heap						heap;
	pthread_mutex_t				*lock;
	long						cooldown_end;
}								t_dongle;

typedef struct s_grbmanager
{
	void						*p;
	struct s_grbmanager			*next;
}								t_grbmanager;

typedef struct s_mutexes_manager
{
	pthread_mutex_t				lock;
	struct s_mutexes_manager	*next;
}								t_mutexes_manager;

typedef struct s_cond_manager
{
	pthread_cond_t				cond;
	struct s_cond_manager		*next;
}								t_cond_manager;

typedef struct s_thread_manager
{
	pthread_t					thread;
	struct s_thread_manager		*next;
}								t_thread_manager;

typedef struct s_ressources
{
	t_grbmanager				*allocs;
	t_thread_manager			*threads;
	t_mutexes_manager			*mutexes;
	t_cond_manager				*conds;
}								t_ressources;

typedef struct s_coder
{
	int							id;
	long						l_com_start;
	int							compile_count;
	t_dongle					*left;
	t_dongle					*right;
	pthread_mutex_t				*lock;
	struct s_codexion			*codexion;
}								t_coder;

typedef struct s_arguments
{
	int							nb_of_coders;
	long						t_to_burnout;
	long						t_to_compile;
	long						t_to_debug;
	long						t_to_refactor;
	int							nb_of_compiles_req;
	long						dongle_cooldown;
	int							fifo;
}								t_arguments;

typedef struct s_codexion
{
	t_ressources				ressources;
	t_arguments					*args;
	t_coder						*coders;
	t_dongle					*dongles;

	long						start_time_mic;

	pthread_mutex_t				*log_lock;

	int							f_routine;
	pthread_mutex_t				*f_routine_lock;

	int							start;
	pthread_mutex_t				*start_lock;
	pthread_cond_t				*start_cond;

	int							stop;
	pthread_mutex_t				*stop_lock;

	pthread_mutex_t				*seq_lock;
	int							seq;

}								t_codexion;

#endif
