/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 09:25:34 by albetanc          #+#    #+#             */
/*   Updated: 2025/08/29 13:37:02 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
# define DEBUG_PRINT(format, ...) do { fflush(stdout); fprintf(stderr, format, ##__VA_ARGS__); } while (0)
# define DEBUG_PRINT_AST(ast) do { fflush(stdout); print_ast(ast, 0); } while (0)
#else
# define DEBUG_PRINT(format, ...) do {} while (0)
# define DEBUG_PRINT_AST(ast) do {} while (0)
#endif

/*
//optional debug for fd
#ifdef FD_DEBUG
# define DEBUG_FD(format, ...) do { fflush(stdout); fprintf(stderr, CYAN "[FD DEBUG] " format RESET, ##__VA_ARGS__); } while(0)
#else
# define DEBUG_FD(format, ...) do {} while (0)
#endif
*/
#define DEBUG_ERROR(format, ...) fprintf(stderr, format, ##__VA_ARGS__)

#endif