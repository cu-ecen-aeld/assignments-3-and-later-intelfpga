#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    
    Thread_Data *ptr = (Thread_Data *) thread_param;
   
    ptr->thread_complete_success = false;
    
    if(usleep((ptr->wait_to_obtain_ms) * 1000)!=0)
    	return thread_param;
    	    
    
    if (pthread_mutex_lock(ptr->mutex) != 0)
    	return thread_param;
    		
    
    if(usleep((ptr->wait_to_release_ms) * 1000)!=0)
    	return thread_param;
    		
        
    if (pthread_mutex_unlock(ptr->mutex) != 0)
    	return thread_param;
    		
    
    
    ptr->thread_complete_success = true;
    
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
     
    Thread_Data *td_ptr =(Thread_Data*) malloc(sizeof (Thread_Data));
    int ret;
    
    td_ptr->wait_to_obtain_ms = wait_to_obtain_ms;
    td_ptr->wait_to_release_ms= wait_to_release_ms;
   
    td_ptr->thread = thread;
    td_ptr->mutex = mutex;
    
    
    ret = pthread_create(td_ptr->thread, NULL, threadfunc, (void *) td_ptr);
    
    
    if(ret !=0){
    	//errno = ret;
    	free(td_ptr);
    	return false;
    } 
    
    
    return true;
}

