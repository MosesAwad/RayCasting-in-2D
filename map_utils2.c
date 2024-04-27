/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 18:46:24 by mawad             #+#    #+#             */
/*   Updated: 2024/04/24 23:28:34 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_map_width(char **map)
{
	int	j;

	j = 0;
	while (map[0][j])
		j++;
	return (j);
}

int	get_map_height(char **map)
{
	int	i;

	i = 0;
	while (map[i])
		i++;
	return (i);
}
