#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <pthread.h>

#include "mandelbrot_set.h"

pthread_mutex_t mutex =  PTHREAD_MUTEX_INITIALIZER;
int task_counter = -1;
static int range_y = 32;
//arg to global variables
//int num_tasks;
//int x_resolution_g,y_resolution_g,max_iter_g;
//double view_x0_g,view_y1_g;
//double x_stepsize_g, y_stepsize_g;
//int palette_shift_g;
//unsigned char* image_g;
        
struct pthread_args
{
    //int id;
    int num_tasks;
    int x_resolution_g,y_resolution_g,max_iter_g;
    double view_x0_g,view_y1_g;
    double x_stepsize_g, y_stepsize_g;
    int palette_shift_g;
    unsigned char* image_g;
};

void * kernel (void * args)
{
    double y;
    double x;
    complex double Z;
    complex double C;
    int k;

    int start_y, end_y;
    int current_task;
    struct pthread_args * arg = (struct pthread_args *) args;
    
    pthread_mutex_lock (&mutex);
    task_counter++;
    current_task = task_counter;
    pthread_mutex_unlock(&mutex);

    //printf("start=%d\n",current_task);
    //int while_loop = 0;
    do
    {//	while_loop++;
	//printf("num of loop=%d",while_loop);
	start_y = current_task * range_y;
	//printf("start=%d\n",current_task);
    //printf("thread%d new task: start_y=%d ",arg->id,start_y);
    end_y = start_y + range_y;
    if( end_y > arg->y_resolution_g ) { end_y = arg->y_resolution_g; }
   	//printf("end_y=%d\n",end_y);
	for(int i = start_y; i < end_y ; i++)
    {
		for (int j = 0; j < arg->x_resolution_g; j++)
		{
			y = arg->view_y1_g - i * arg->y_stepsize_g;
			x = arg->view_x0_g + j * arg->x_stepsize_g;
			Z = 0 + 0 * I;
			C = x + y * I;
			k = 0;

			do
			{
				Z = Z * Z + C;
				k++;
			} while (cabs(Z) < 2 && k < arg->max_iter_g);

			if (k == arg->max_iter_g)
			{
				memcpy( arg->image_g + 3*i*arg->x_resolution_g+3*j, "\0\0\0", 3);
			}
			else
			{
				int index = (k + arg->palette_shift_g)
			            % (sizeof(colors) / sizeof(colors[0]));
				memcpy( arg->image_g + 3*i*arg->x_resolution_g+3*j, colors[index], 3);
			}
		}	
    }

  	pthread_mutex_lock (&mutex);
	task_counter ++;
	current_task= task_counter;
	//printf("next task=%d",current_task);
	pthread_mutex_unlock (&mutex);		
	
    }while( current_task  < num_tasks);

    return NULL;
}

void mandelbrot_draw(int x_resolution, int y_resolution, int max_iter,
	                double view_x0, double view_x1, double view_y0, double view_y1,
	                double x_stepsize, double y_stepsize,
	                int palette_shift, unsigned char (*image)[x_resolution][3], int num_threads) 
{


	pthread_t * threads = ( pthread_t *) malloc ( num_threads* sizeof ( pthread_t ) ) ;
	struct pthread_args * args = (struct pthread_args *) malloc ( num_threads* sizeof ( struct pthread_args ) ) ;

	for (int i = 0 ; i < num_threads ; ++i ) 
	{	
		//args[i].id = i;
        args[i]->num_tasks = y_resolution/range_y +1;
        args[i]->max_iter_g = max_iter;
        args[i]->view_x0_g = view_x0;
        args[i]->view_y1_g = view_y1;
        args[i]->x_stepsize_g = x_stepsize;
        args[i]->y_stepsize_g = y_stepsize;
        args[i]->image_g =(unsigned char*) image;
        args[i]->palette_shift_g = palette_shift;
        args[i]->x_resolution_g = x_resolution;
        args[i]->y_resolution_g = y_resolution;

		pthread_create (&threads[i] , NULL, kernel , args+i ) ;
		//pthread_create (&threads[i] , NULL, kernel , NULL ) ;
	}

	for (int i = 0 ; i < num_threads ; ++i ) { pthread_join (threads[i] , NULL ) ; }
	
	//pthread_mutex_destroy( &mutex );
	free(threads);
	free(args);

}
