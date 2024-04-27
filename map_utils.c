/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:44:12 by mawad             #+#    #+#             */
/*   Updated: 2024/04/24 18:43:57 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	ft_map_alloc_clean(char **map, int y)
{
	int	i;

	i = 0;
	while (i < y)
		free(map[i++]);
	free(map);
}

static char	*first_row(int fd, char **map, int *y)
{
	char	*row;

	if (map == NULL)
		return (NULL);
	row = get_next_line(fd);
	if (!row)
		return (free(map), NULL);
	*y = 0;
	map[*y] = (char *)malloc(sizeof(char) * (ft_strlen(row) + 1));
	if (!map[*y])
		return (free(map), NULL);
	ft_strlcpy(map[(*y)++], row, ft_strlen(row) + 1);
	return (row);
}

static char	**get_map_nl(int fd)
{
	char	**map;
	char	*row;
	int		y;

	y = 0;
	map = (char **)malloc(sizeof(char *) * (9999));
	row = first_row(fd, map, &y);
	if (!row)
		return (NULL);
	free(row);
	while (row)
	{
		row = get_next_line(fd);
		if (!row)
			break ;
		map[y] = (char *)malloc(sizeof(char) * (ft_strlen(row) + 1));
		if (!map[y])
			return (ft_map_alloc_clean(map, y), NULL);
		ft_strlcpy(map[y++], row, ft_strlen(row) + 1);
		free(row);
	}
	return (map[y] = NULL, map);
}

static char	*extract_nl(char *line)
{
	int		i;
	char	*buffer;

	i = 0;
	while (line[i] != '\n' && line[i] != '\0')
		i++;
	if (line[i] == '\0')
		return (NULL);
	buffer = malloc(sizeof(*buffer) * (i + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (line[i] != '\n')
	{
		buffer[i] = line[i];
		i++;
	}
	buffer[i] = '\0';
	free(line);
	return (buffer);
}

//if ft_strchr(map[i], \n) is used so that we keep the
//last line in the map if the user hadn't hit enter after
//the last line. So if the map[i] line where i represents the
//last line in the map ends with a '\0' instead of a '\n'
//(indicating that the user did not hit enter upon reaching
//the last line as mentioned earlier), then we keep the line
//as it is.
char	**get_map(int fd)
{
	char	**map;
	int		i;

	map = get_map_nl(fd);
	if (map == NULL)
		return (NULL);
	i = 0;
	while (map[i])
	{
		if (ft_strchr(map[i], '\n'))
			map[i] = extract_nl(map[i]);
		i++;
	}
	return (map);
}

// void	fill_map(char **map)
// {
// 	map = {'1', '1', '1', '1', '1', '1', '1', '1'};
// }

// int main()
// {
// 	t_game	game;	
// 	int		fd;

// 	fd = open("map.cub", O_RDONLY, 0777);
// 	if (fd == -1)
// 	{
// 		printf("error opening file\n");
// 		return (1);
// 	}
// 	game.map = get_map(fd);
// 	for (int i = 0; game.map[i] != NULL; i++)
// 	{
// 		printf("%s\n", game.map[i]);
// 	}
// 	// draw_square(&(data.img), 400, 400, 0xff00);
// 	//fill_squares(&(data.img), 0xff00);
//     return (0);
// }
