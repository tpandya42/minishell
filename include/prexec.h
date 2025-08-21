/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prexec.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:10:46 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/21 11:38:32 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PREXEC_H
# define PREXEC_H

# include "minishell.h"


// -----------------------------------------//
//                PROTOTYPES                //
// -----------------------------------------//

// void	pre_execution(t_node *node, char **envp);
void	pre_execution(t_program *program, t_node *node);

#endif