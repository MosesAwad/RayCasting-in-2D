/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:29:39 by mawad             #+#    #+#             */
/*   Updated: 2024/04/26 19:36:52 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

// #include "mlx-linux/mlx.h"
# include "mlx/mlx.h"
# include "Libft/libft.h"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <fcntl.h>

# define MAP_WIDTH 24
# define MAP_HEIGHT 24

# define DIM	64

# define W_KEY 13
# define A_KEY 0
# define S_KEY 1
# define D_KEY 2
# define ESC_KEY 53
# define L_ARROW 123
# define R_ARROW 124

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

typedef enum s_bool
{
	FALSE = 0,
	TRUE = 1
}	t_bool;

typedef struct s_img {
	void	*img;
	char	*img_pixels_ptr;
	int		bits_per_pixel;
	int		line_len;
	int		endian;
}				t_img;

typedef struct s_var {
	void	*mlx_ptr;
	void	*win_ptr;
	t_img	img;
}	t_var;

typedef struct s_vector {
	double	x;
	double	y;
}	t_vector;

typedef struct s_game {
	char		**map;
	int			map_height;
	int			map_width;
	double		player_posx;
	double		player_posy;
	double		player_angle;
	t_vector	dir;
	t_vector	cam_plane;
	t_var		data;
}	t_game;

//get_next_line.c
char	*get_next_line(int fd);

//map_utils.c
char	**get_map(int fd);

//map_utils2.c
int		get_map_width(char **map);
int		get_map_height(char **map);

//player_utils.c
void	set_player_pos(t_game *game);

#endif