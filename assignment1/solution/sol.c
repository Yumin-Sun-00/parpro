//filename:

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <pthread.h>

#include "mandelbrot_set.h"

typedef struct
{
	int start;
	int end;
	int x_resolution;
	int y_resolution;
	int max_iter;
	double view_x0;
	double view_x1;
	double view_y0;
	double view_y1;
	double x_stepsize;
	double y_stepsize;
	int palette_shift;
	void *image;
} compute_args;

void *compute_mandelbrot(void *arguments)
{

	compute_args *args = arguments;

	int start = args->start;
	int end = args->end;
	int x_resolution1 = args->x_resolution;
	int y_resolution = args->y_resolution;
	int max_iter = args->max_iter;
	double view_x0 = args->view_x0;
	double view_x1 = args->view_x1;
	double view_y0 = args->view_y0;
	double view_y1 = args->view_y1;
	double x_stepsize = args->x_stepsize;
	double y_stepsize = args->y_stepsize;
	int palette_shift = args->palette_shift;
	unsigned char(*image)[x_resolution1][3] = (unsigned char(*)[x_resolution1][3])args->image;

	for (int i = start; i < end; i++)
	{
		for (int j = 0; j < x_resolution1; j++)
		{
			double y = view_y1 - i * y_stepsize;
			double x = view_x0 + j * x_stepsize;

			complex double Z = 0 + 0 * I;
			complex double C = x + y * I;

			int k = 0;

			do
			{
				Z = Z * Z + C;
				k++;
			} while (cabs(Z) < 2 && k < max_iter);

			if (k == max_iter)
			{
				memcpy(image[i][j], "\0\0\0", 3);
			}
			else
			{
				int index = (k + palette_shift) % (sizeof(colors) / sizeof(colors[0]));
				memcpy(image[i][j], colors[index], 3);
			}
		}
	}
}

void mandelbrot_draw(int x_resolution, int y_resolution, int max_iter,
					 double view_x0, double view_x1, double view_y0, double view_y1,
					 double x_stepsize, double y_stepsize,
					 int palette_shift, unsigned char (*image)[x_resolution][3],
					 int num_threads)
{
	// pthread initialization
	int t, taskSize;
	pthread_t *thread;
	compute_args *thread_arg;

	thread = (pthread_t *)malloc(num_threads * sizeof(*thread));
	thread_arg = (compute_args *)malloc(num_threads * sizeof(*thread_arg));

	// build image by num_threads threads
	taskSize = y_resolution / num_threads;
	for (t = 0; t < num_threads; t++)
	{
		thread_arg[t].start = t * taskSize;
		thread_arg[t].end = (t < (num_threads - 1)) ? (t + 1) * taskSize : (t + 1) * taskSize + (y_resolution % num_threads);
		thread_arg[t].x_resolution = x_resolution;
		thread_arg[t].y_resolution = y_resolution;
		thread_arg[t].max_iter = max_iter;
		thread_arg[t].view_x0 = view_x0;
		thread_arg[t].view_x1 = view_x1;
		thread_arg[t].view_y0 = view_y0;
		thread_arg[t].view_y1 = view_y1;
		thread_arg[t].x_stepsize = x_stepsize;
		thread_arg[t].y_stepsize = y_stepsize;
		thread_arg[t].palette_shift = palette_shift;
		thread_arg[t].image = (void *)image; // todo later
		pthread_create(&thread[t], NULL, compute_mandelbrot, &thread_arg[t]);
	}

	// join threads
	for (t = 0; t < num_threads; t++)
	{
		pthread_join(thread[t], NULL);
	}

	free(thread);
	free(thread_arg);
}
