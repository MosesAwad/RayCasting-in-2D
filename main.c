/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:29:51 by mawad             #+#    #+#             */
/*   Updated: 2024/04/27 21:39:06 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	my_pixel_put(t_img *img, int x, int y, int color)
{
	int	offset;

	//🚨 Line len is in bytes. WIDTH 800 len_line ~3200 (can differ for alignment)
	offset = (img->line_len * y) + (x * (img->bits_per_pixel / 8));	

	*((unsigned int *)(offset + img->img_pixels_ptr)) = color;
}

//Draws a square of dimensions (DIM * DIM) starting from the top-left corner
//of the point specified by co-ordinates(x, y)
void	draw_square(t_img *img, int x, int y, int color)
{
	int	ylim;
	int	xlim;
	int	x_og;

	ylim = y + DIM - 2;
	xlim = x + DIM - 2;
	x_og = x + 2;

	while (y + 2 < ylim)
	{
		x = x_og;
		while (x < xlim)
		{
			my_pixel_put(img, x, y, color);
			x++;
		}
		y++;
	}
}

void	draw_circle(t_img *img, int x, int y, int r)
{
	int	x_inc;
	int	y_inc;
	int	decision;

	x_inc = r;
	y_inc = 0;
	decision = 1 - r;
	while (x_inc >= y_inc)
	{
		my_pixel_put(img, x + x_inc, y + y_inc, 0xFF0000);
		my_pixel_put(img, x + y_inc, y + x_inc, 0xFF0000);
		my_pixel_put(img, x - y_inc, y + x_inc, 0xFF0000);
		my_pixel_put(img, x - x_inc, y + y_inc, 0xFF0000);
		my_pixel_put(img, x - x_inc, y - y_inc, 0xFF0000);
		my_pixel_put(img, x - y_inc, y - x_inc, 0xFF0000);
		my_pixel_put(img, x + y_inc, y - x_inc, 0xFF0000);
		my_pixel_put(img, x + x_inc, y - y_inc, 0xFF0000);

		y_inc++;
		if (decision <= 0)
		{
			decision += 2 * y_inc + 1;
		}
		else
		{
			x_inc--;
			decision += 2 * (y_inc - x_inc) + 1;
		}
	}
}

void draw_line(t_img *img, t_vector start, t_vector end)
{
	// end = (t_vector){(start.x + end.x) * DIM + DIM / 2, (start.y + end.y) * DIM + DIM /2};
	// start = (t_vector){start.x * DIM + DIM /2, start.y * DIM + DIM /2};
    
	end = (t_vector){(start.x + end.x) * DIM, (start.y + end.y) * DIM};
	start = (t_vector){start.x * DIM, start.y * DIM};

	// printf("start.x is %f and start.y is %f\n", start.x, start.y);
	// printf("end.x is %f and end.y is %f\n", end.x, end.y);
	start.x = (int) start.x;
	start.y = (int) start.y;
	end.x = (int) end.x;
	end.y = (int) end.y;
	// printf("start.x is %f and start.y is %f\n", start.x, start.y);
	// printf("end.x is %f and end.y is %f\n", end.x, end.y);

	int dx = fabs(end.x - start.x);
    int dy = fabs(end.y - start.y);
    int sx = start.x < end.x ? 1 : -1;
    int sy = start.y < end.y ? 1 : -1;
    int err = dx - dy;
    int e2;

    while (!(start.x == end.x && start.y == end.y))
    {
		// printf("-----------------------------------\n");
		// printf("start.x is %f and start.y is %f\n", start.x, start.y);
		// printf("end.x is %f and end.y is %f\n", end.x, end.y);
        my_pixel_put(img, start.x, start.y, 0x4169E1);
        e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            start.x += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            start.y += sy;
        }
    }
    my_pixel_put(img, start.x, start.y, 0x4169E1); // Ensure end point is drawn
}

void	draw_walls(t_game game, t_img *img)
{
	int	x;
	int	y;

	y = 0;
	while (game.map[y])
	{
		x = 0;
		while (game.map[y][x])
		{
			if (game.map[y][x] == '1')
				draw_square(img, x * DIM, y * DIM, 0xff00);
			else
				draw_square(img, x * DIM, y * DIM, 0xD3D3D3);
			x++;
		}
		y++;
	}
}

void	flush(t_game game)
{
	int	x;
	int	y;

	y = 0;
	while (y < game.map_height * DIM)
	{
		x = 0;
		while (x < game.map_width * DIM)
		{
			my_pixel_put(&(game.data.img), x , y, 0x000000);
			x++;
		}
		y++;
	}
}

void	rotate_angle(t_game *game, t_vector *vec, int flag)
{
	double	angle;
	double	vec_x_save;

	(void) game;
	vec_x_save = vec->x;
	// angle = game->player_angle;
	if (flag == 1)
		angle = -0.035;
	else
		angle = 0.035;
	vec->x = cos(angle) * vec->x - sin(angle) * vec->y;
	vec->y = sin(angle) * vec_x_save + cos(angle) * vec->y;
}

void	ray_cast(t_game *game)
{
	printf("player_posx %f and player_posy %f\n", game->player_posx, game->player_posy);
	//These lines + DIM / 2 is to get the circle's center to be in the
	//center of the tile, not at the top left corner of the tile because
	//that's where (x * DIM, y * DIM) go. So in addition to x * DIM, I
	//want DIM / 2 and in addition to y * DIM, I also want DIM / 2.
	// draw_circle(&(data.img), game.player_posx * DIM + DIM / 2, game.player_posy * DIM + DIM / 2, 9);

	t_var		data;
	t_vector	pos;
	// t_vector	dir;
	// t_vector	cam_plane;
	double		cam_plane_mag;
	double		screen_width;
	double		camera_x; 
	t_vector 	ray_dir;

	data = game->data;

	flush(*game);
	draw_walls(*game, &(data.img));

	draw_circle(&(data.img), game->player_posx * DIM, game->player_posy * DIM, 9);
	(void) camera_x;
	(void) screen_width;
	pos = (t_vector){game->player_posx, game->player_posy};

	// rotate_angle(game, &(game->dir));
	// rotate_angle(game, &(game->cam_plane));
	
	cam_plane_mag = sqrt(pow(game->cam_plane.x, 2) + pow(game->cam_plane.y, 2));

	// for (int i = 0; game->map[6][i] != '\0'; i++)
	// {
	// 	printf("map(%d, 6) is %c\n", i, game->map[6][i]);
	// }
	// printf("\n");

	// screen_width = game.map_width * DIM;

	for (double x = 0; x <= cam_plane_mag; x += 0.066)
	{
		ray_dir.x = game->dir.x + game->cam_plane.x * x;
		ray_dir.y = game->dir.y + game->cam_plane.y * x;
		// printf("---------------------------\n");
		
		// printf("dir is (%f, %f)\n", game->dir.x, game->dir.y);
		// printf("cam_plane is (%f, %f)\n", game->cam_plane.x, game->cam_plane.y);
		
		// printf("WTF.ray_dir.x is %f and ray_dir.y is %f\n", ray_dir.x, ray_dir.y);

		t_vector	map;
		double		side_dist_x;
		double		side_dist_y;
		double		delta_dist_x;
		double		delta_dist_y;
		int			step_x;
		int			step_y;
		t_bool		hit;
		int			side;

		hit = FALSE;
		side = 0;
		map.x = (int) game->player_posx;
		map.y = (int) game->player_posy;

		if (ray_dir.x == 0.00)
			delta_dist_x = 1e30;
		else
			delta_dist_x = sqrt(1 + pow(ray_dir.y / ray_dir.x, 2));
		
		if (ray_dir.y == 0.00)
			delta_dist_y = 1e30;
		else
			delta_dist_y = sqrt(1 + pow(ray_dir.x / ray_dir.y, 2));

		// printf("delta dis x is %f\n", delta_dist_x);
		// printf("delta dis y is %f\n", delta_dist_y);

		if (ray_dir.x < 0)
		{
			step_x = -1;
			side_dist_x = (pos.x - map.x) * delta_dist_x;
		}
		else
		{
			step_x = 1;
			side_dist_x = ((1 - (pos.x - map.x)) * delta_dist_x);
		}
		if (ray_dir.y < 0)
		{
			step_y = -1;
			side_dist_y = (pos.y - map.y) * delta_dist_y;
		}
		else
		{
			step_y = 1;
			side_dist_y = (1 - (pos.y - map.y)) * delta_dist_y;
		}
		
		while (hit == FALSE)
		{
			// printf("\nside_dist_x at the beginning is %f\n", side_dist_x);
			// printf("side_dist_y at the beginning is %f\n", side_dist_y);
			if (side_dist_x < side_dist_y)
			{
				side_dist_x += delta_dist_x;
				map.x += step_x;
				side = 0;
			}
			else
			{
				side_dist_y += delta_dist_y;
				map.y += step_y;
				side = 1;
			}
			// printf("map (x, y) is (%d, %d)\n", (int) map.x, (int) map.y);
			// printf("map (%d, %d) is in fact %c\n", (int) map.x, (int) map.y, game->map[(int) map.y][(int) map.x]);
			if (game->map[(int) map.y][(int) map.x] == '1')
				hit = TRUE;
		}
		if (side == 0)
		{
			side_dist_x -= delta_dist_x;
			double	save_ray_dir_x = ray_dir.x;
			ray_dir.x = ray_dir.x * (side_dist_x / sqrt(pow(ray_dir.x, 2) + pow(ray_dir.y, 2)));
			ray_dir.y = ray_dir.y * (side_dist_x / sqrt(pow(save_ray_dir_x, 2) + pow(ray_dir.y, 2)));
			// printf("BY NOW: ray_dir.x is %f and ray_dir.y is %f\n", ray_dir.x, ray_dir.y);
		}
		else
		{
			side_dist_y -= delta_dist_y;
			double	save_ray_dir_x = ray_dir.x;
			ray_dir.x = ray_dir.x * (side_dist_y / sqrt(pow(ray_dir.x, 2) + pow(ray_dir.y, 2)));
			ray_dir.y = ray_dir.y * (side_dist_y / sqrt(pow(save_ray_dir_x, 2) + pow(ray_dir.y, 2)));
		}
		draw_line(&(data.img), pos, ray_dir);
		mlx_put_image_to_window(data.mlx_ptr, data.win_ptr, data.img.img, 0, 0);		
	}
	for (double x = -cam_plane_mag; x < 0; x += 0.066)
	{
		ray_dir.x = game->dir.x + game->cam_plane.x * x;
		ray_dir.y = game->dir.y + game->cam_plane.y * x;
		printf("---------------------------\n");
		// printf("WTF.ray_dir.x is %f and ray_dir.y is %f\n", ray_dir.x, ray_dir.y);
		t_vector	map;
		double		side_dist_x;
		double		side_dist_y;
		double		delta_dist_x;
		double		delta_dist_y;
		int			step_x;
		int			step_y;
		t_bool		hit;
		int			side;

		hit = FALSE;
		side = 0;
		map.x = (int) game->player_posx;
		map.y = (int) game->player_posy;

		if (ray_dir.x == 0)
			delta_dist_x = 1e30;
		else
			delta_dist_x = sqrt(1 + pow(ray_dir.y / ray_dir.x, 2));
		
		if (ray_dir.y == 0)
			delta_dist_y = 1e30;
		else
			delta_dist_y = sqrt(1 + pow(ray_dir.x / ray_dir.y, 2));

		if (ray_dir.x < 0)
		{
			step_x = -1;
			side_dist_x = (pos.x - map.x) * delta_dist_x;
		}
		else
		{
			step_x = 1;
			side_dist_x = ((1 - (pos.x - map.x)) * delta_dist_x);
		}
		if (ray_dir.y < 0)
		{
			step_y = -1;
			side_dist_y = (pos.y - map.y) * delta_dist_y;
		}
		else
		{
			step_y = 1;
			side_dist_y = (1 - (pos.y - map.y)) * delta_dist_y;
		}
		
		while (hit == FALSE)
		{
			// printf("side_dist_x at the beginning is %f\n", side_dist_x);
			// printf("side_dist_y at the beginning is %f\n", side_dist_y);
			if (side_dist_x < side_dist_y)
			{
				side_dist_x += delta_dist_x;
				map.x += step_x;
				side = 0;
			}
			else
			{
				side_dist_y += delta_dist_y;
				map.y += step_y;
				side = 1;
			}
			// printf("map (x, y) is (%d, %d)\n", (int) map.x, (int) map.y);
			// printf("map (%d, %d) is in fact %c\n", (int) map.x, (int) map.y, game->map[(int) map.y][(int) map.x]);
			if (game->map[(int) map.y][(int) map.x] == '1')
				hit = TRUE;
		}
		if (side == 0)
		{
			side_dist_x -= delta_dist_x;
			double save_ray_dir_x = ray_dir.x;
			ray_dir.x = ray_dir.x * (side_dist_x / sqrt(pow(ray_dir.x, 2) + pow(ray_dir.y, 2)));
			ray_dir.y = ray_dir.y * (side_dist_x / sqrt(pow(save_ray_dir_x, 2) + pow(ray_dir.y, 2)));
			// printf("BY NOW: ray_dir.x is %f and ray_dir.y is %f\n", ray_dir.x, ray_dir.y);
		}
		else
		{
			side_dist_y -= delta_dist_y;
			double save_ray_dir_x = ray_dir.x;
			ray_dir.x = ray_dir.x * (side_dist_y / sqrt(pow(ray_dir.x, 2) + pow(ray_dir.y, 2)));
			ray_dir.y = ray_dir.y * (side_dist_y / sqrt(pow(save_ray_dir_x, 2) + pow(ray_dir.y, 2)));
		}
		draw_line(&(data.img), pos, ray_dir);
		mlx_put_image_to_window(data.mlx_ptr, data.win_ptr, data.img.img, 0, 0);
	}
}

t_bool	check_valid_move(t_game *game, int flag)
{
	t_vector	target_v;

	target_v.x = game->player_posx;
	target_v.y = game->player_posy;
	if (flag == W_KEY)
		target_v.y = game->player_posy - 0.11;
	else if (flag == S_KEY)
		target_v.y = game->player_posy + 0.11;
	else if (flag == A_KEY)
		target_v.x = game->player_posx - 0.11;
	else if (flag == D_KEY)
		target_v.x = game->player_posx + 0.11;

	printf("retard detection is (%d, %d)\n", (int) target_v.x, (int) target_v.y);

	if (game->map[(int) target_v.y][(int) target_v.x] == '1')
	{
		return (FALSE);
	}
	else
		return (TRUE);
}

int	key_hook(int keycode, t_game *game)
{
	if (keycode == W_KEY)
	{
		if (check_valid_move(game, W_KEY) == TRUE)
			game->player_posy -= 0.10;
		// printf("player (%f, %f)\n", game->player_posx, game->player_posy);
		ray_cast(game);
	}
	else if (keycode == A_KEY)
	{
		if (check_valid_move(game, A_KEY) == TRUE)
			game->player_posx -= 0.10;
		ray_cast(game);
	}
	else if (keycode == S_KEY)
	{
		if (check_valid_move(game, S_KEY) == TRUE)
			game->player_posy += 0.10;
		ray_cast(game);
	}
	else if (keycode == D_KEY)
	{
		if (check_valid_move(game, D_KEY) == TRUE)
			game->player_posx += 0.10;
		ray_cast(game);
	}
	else if (keycode == L_ARROW)
	{
		// game->player_angle += 0.035;
		rotate_angle(game, &(game->dir), 1);
		rotate_angle(game, &(game->cam_plane), 1);
		ray_cast(game);
	}
	else if (keycode == R_ARROW)
	{
		// game->player_angle -= 0.035;
		rotate_angle(game, &(game->dir), 0);
		rotate_angle(game, &(game->cam_plane), 0);
		ray_cast(game);
	}
	// else if (keycode == ESC_KEY)
	// 	;
	//printf("key pressed is %d and player pos is (%f, %f)\n", keycode, game->player_posx, game->player_posy);
	return (0);
}

int main()
{
	t_game	game;	
	int		fd;

	fd = open("map.cub", O_RDONLY, 0777);
	if (fd == -1)
	{
		printf("error opening file\n");
		return (1);
	}

	game.map = get_map(fd);
	game.map_height = get_map_height(game.map);
	game.map_width = get_map_width(game.map);

	printf("map height %d and map width %d\n", game.map_height, game.map_width);

    game.data.mlx_ptr = mlx_init();

    game.data.win_ptr = mlx_new_window(game.data.mlx_ptr, game.map_width * DIM, game.map_height * DIM, "Yay");

	game.data.img.img = mlx_new_image(game.data.mlx_ptr, game.map_width * DIM, game.map_height * DIM);
	if (game.data.img.img == NULL)
	{
		printf("error : img ptr\n");
		return (1);
	}

	game.data.img.img_pixels_ptr = mlx_get_data_addr(game.data.img.img,
												&(game.data.img.bits_per_pixel),
												&(game.data.img.line_len),
												&(game.data.img.endian));
	if (game.data.img.img_pixels_ptr == NULL)
	{
		printf("error : img pxl ptr\n");
		return (1);
	}

	// draw_square(&(data.img), 400, 400, 0xff00);
	//fill_squares(&(data.img), 0xff00);
	draw_walls(game, &(game.data.img));
	set_player_pos(&game);
	game.player_angle = 0;

	game.dir = (t_vector){1, 0};
	game.cam_plane = (t_vector){0, 0.66};

	ray_cast(&game);
	mlx_put_image_to_window(game.data.mlx_ptr, game.data.win_ptr, game.data.img.img, 0, 0);

	// mlx_key_hook(game.data.win_ptr, key_hook, &game);
	mlx_hook(game.data.win_ptr, 2, 0, key_hook, &game);

    mlx_loop(game.data.mlx_ptr);
    return (0);
}
