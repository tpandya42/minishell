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

// Debug prints completely disabled
#define DEBUG_PRINT(format, ...) do {} while (0)
#define DEBUG_PRINT_AST(ast) do {} while (0)
#define DEBUG_ERROR(format, ...) do {} while (0)

#endif