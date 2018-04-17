#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <pthread.h>

#include "mandelbrot_set.h"


struct pthread_args
{
	int pthread_id;
	int max_iter;
	int start_x,start_y,end_x,end_y;
	double view_x0, view_x1, view_y0, view_y1;
	double x_stepsize, y_stepsize;
	unsigned char *image;
	int palette_shift;
	int x_resolution,y_resolution;
};

void * kernel (void * args)
{
	struct pthread_args * arg = (struct pthread_args *) args;

	double y;
	double x;

	complex double Z;
	complex double C;

	int k;



	for (int i = arg->start_y; i < arg->end_y; i++)
	{
		for (int j = arg->start_x; j < arg->end_x; j++)
		{
			y = arg->view_y1 - i * arg->y_stepsize;
			x = arg->view_x0 + j * arg->x_stepsize;

			Z = 0 + 0 * I;
			C = x + y * I;

			k = 0;

			do
			{
				Z = Z * Z + C;
				k++;
			} while (cabs(Z) < 2 && k < arg->max_iter);

			if (k == arg->max_iter)
			{
				memcpy( arg->image + 3*i*arg->x_resolution+3*j, "\0\0\0", 3);
			}
			else
			{
				int index = (k + arg->palette_shift)
				            % (sizeof(colors) / sizeof(colors[0]));

				memcpy( arg->image + 3*i*arg->x_resolution+3*j, colors[index], 3);

			}
		}
	}

	return NULL;
}

void mandelbrot_draw(int x_resolution, int y_resolution, int max_iter,
	                double view_x0, double view_x1, double view_y0, double view_y1,
	                double x_stepsize, double y_stepsize,
	                int palette_shift, unsigned char (*image)[x_resolution][3],
						 int num_threads) 
{
	pthread_t * threads = ( pthread_t *) malloc ( num_threads* sizeof ( pthread_t ) ) ;
	struct pthread_args * args = (struct pthread_args *) malloc ( num_threads* sizeof ( struct pthread_args ) ) ;

	int range_x=x_resolution/num_threads;
	int range_y=y_resolution/num_threads;

	for (int i = 0 ; i < num_threads ; ++i ) {
		args[i].pthread_id = i;
		args[i].max_iter = max_iter;

		args[i].start_x =i*range_x;
		args[i].start_y =i*range_y;
		
		args[i].end_x =i*range_x + range_x;
		args[i].end_y =i*range_y + range_y;

		if(args[i].end_x>=x_resolution)
		{
			args[i].end_x=x_resolution;
		}

		if(args[i].end_y>=y_resolution)
		{
			args[i].end_y=y_resolution;
		}

		args[i].view_x0 = view_x0;
		args[i].view_x1 = view_x1;
		args[i].view_y0 = view_y0;
		args[i].view_y1 = view_y1;

		args[i].x_stepsize = x_stepsize;
		args[i].y_stepsize = y_stepsize;

		args[i].image = (unsigned char*)image;
		args[i].palette_shift = palette_shift;

		args[i].x_resolution = x_resolution;
		args[i].y_resolution = y_resolution;

		pthread_create (&threads[i] , NULL, kernel , args+i ) ;
	}

	for (int i = 0 ; i < num_threads ; ++i ) {
		pthread_join (threads[i] , NULL ) ;
	}

	free(threads);
	free(args);

}
