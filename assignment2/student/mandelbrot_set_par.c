#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <pthread.h>

#include "mandelbrot_set.h"

pthread_mutex_t mutex =  PTHREAD_MUTEX_INITIALIZER;

struct pthread_args
{
    int* task_counter;
    int start_task;
    char dummy [64];
    int id;
    int num_tasks;
    int range_y;
    int max_iter;
    double view_x0, view_x1, view_y0, view_y1;
    double x_stepsize, y_stepsize;
    int x_resolution,y_resolution;
    int palette_shift;
    unsigned char *image;
};

void * kernel (void * args)
{
    double y;
    double x;
    complex double Z;
    complex double C;
    int k;

    int start_y, end_y;
    int current_task,next_task;
    struct pthread_args * arg = (struct pthread_args *) args;
    
   // start_y = arg-> start_task * arg->range_y;
   // printf("thread%d new task: start_y=%d\n",arg->id,start_y);
   // end_y = start_y + arg->range_y;
   // if( end_y > arg->y_resolution ) { end_y = arg->y_resolution; }
   // printf("end_y=%d\n",end_y);//	pthread_mutex_unlock (&mutex);
    current_task = arg->start_task;
    next_task = current_task;
    int while_loop = 0;
    do
    {	while_loop++;
	printf("num of loop=%d",while_loop);
	start_y = current_task * arg->range_y;
    	printf("thread%d new task: start_y=%d\n",arg->id,start_y);
    	end_y = start_y + arg->range_y;
    	if( end_y > arg->y_resolution ) { end_y = arg->y_resolution; }
   	printf("end_y=%d\n",end_y);
	for(int i = start_y; i < end_y ; i++)
    	{
		//printf("thread%d : start_y=%d\n, end_y=%d\n",arg->id,start_y,end_y);
		for (int j = 0; j < arg->x_resolution; j++)
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
		//if (k != arg->max_iter)
			{
				int index = (k + arg->palette_shift)
			            % (sizeof(colors) / sizeof(colors[0]));
				memcpy( arg->image + 3*i*arg->x_resolution+3*j, colors[index], 3);
			}
		}	
    	}

  	pthread_mutex_lock (&mutex);
  	current_task = (*arg->task_counter)+1;
	(*arg->task_counter)++;
	pthread_mutex_unlock (&mutex);		

//	start_y = next_task + arg->range_y;
//	end_y = start_y+arg->range_y;	
//	if( end_y > arg->y_resolution ) { end_y = arg->y_resolution; }
	
	//start_y = start_y + arg->range_y;
    }while( current_task  < arg->num_tasks);
	   
    //pthread_mutex_destroy( &mutex );
    return NULL;
}

void mandelbrot_draw(int x_resolution, int y_resolution, int max_iter,
	                double view_x0, double view_x1, double view_y0, double view_y1,
	                double x_stepsize, double y_stepsize,
	                int palette_shift, unsigned char (*image)[x_resolution][3], int num_threads) 
{
	printf("hello...");

        int range_y = 64;
        int task_id =-1;
        int* task_counter = &task_id;
        int num_tasks = y_resolution/range_y +1;

	pthread_t * threads = ( pthread_t *) malloc ( num_threads* sizeof ( pthread_t ) ) ;
	struct pthread_args * args = (struct pthread_args *) malloc ( num_threads* sizeof ( struct pthread_args ) ) ;
	
//	int range_y = 64;
//	int task_id =-1;
//	int* task_counter = &task_id;
//	int num_tasks = y_resolution/range_y +1;	

	for (int i = 0 ; i < num_threads ; ++i ) 
	{
		pthread_mutex_lock (&mutex);
        	(*task_counter)++;
        	args[i].task_counter = task_counter;
		args[i].start_task = (*task_counter);
		printf("thread %d,task=%d\n",i,*task_counter);
        	pthread_mutex_unlock(&mutex);
		
		args[i].id = i;
        	args[i].num_tasks = num_tasks;
        	args[i].range_y = range_y;
		args[i].max_iter = max_iter;
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
		
		//args[i].start_y =i*range_y;
		//printf("%d\n", args[i].start_y);
		//args[i].end_y =i*range_y + range_y;
		//if(args[i].end_y>=y_resolution) { args[i].end_y=y_resolution; }
		//printf("%d\n",args[i].end_y);
		
		pthread_create (&threads[i] , NULL, kernel , args+i ) ;
	}

	for (int i = 0 ; i < num_threads ; ++i ) { pthread_join (threads[i] , NULL ) ; }
	
	pthread_mutex_destroy( &mutex );
	free(threads);
	free(args);

}
