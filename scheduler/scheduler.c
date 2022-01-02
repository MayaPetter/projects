/******************************************
* Author:   Maya Petter                   *
* Reviewer:                               *
* Purpose:                                *
*******************************************/
#include <assert.h> /* assert */ 
#include <stdlib.h> /* free, malloc */
#include <unistd.h> /* sleep */

#include "pqueue.h"
#include "scheduler.h" 
#include "task.h"

#define UNUSED(x) (void)(x)

struct scheduler
{
	pqueue_t *pqueue;
	task_t *running_task;
	int is_stop;
};

static int CmpFunc(void *new_task, void *scheduled_task, const void *param)
{
	assert(NULL != new_task);
	assert(NULL != scheduled_task);
	
	return TaskIsBefore((const task_t *)new_task, (const task_t *)scheduled_task, 
																(void *)param);
}

scheduler_t *SchedulerCreate(void)
{
	scheduler_t *scheduler = (scheduler_t *)malloc(sizeof(scheduler_t));
	if (NULL == scheduler)
	{
		return NULL;
	}
	
	scheduler->pqueue = PQueueCreate(&CmpFunc, NULL); 
	if (NULL == scheduler->pqueue)
	{
		free(scheduler); scheduler = NULL;
		return NULL;
	}
	
	scheduler->running_task = NULL;
	scheduler->is_stop = 0;
	
	return scheduler;
}

void SchedulerDestroy(scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	
	SchedulerClear(scheduler);
	PQueueDestroy(scheduler->pqueue); scheduler->pqueue = NULL;
	free(scheduler); scheduler = NULL;
}

task_uid_t SchedulerAddTask(scheduler_t *scheduler, void *func_param,
                   			task_status (*task_func)(void *param), 	
                   			time_t time_in_sec,
                   			time_t interval_in_sec)
{
	task_t *new_task = NULL;
	int enqueue_status = 0 ;
	
	assert(NULL != task_func);
	assert(NULL != scheduler);
	
	new_task = TaskCreate(func_param, task_func, time_in_sec, interval_in_sec);
	if (NULL == new_task)
	{
		return BAD_UID;
	}
	
	enqueue_status = PQueueEnqueue(scheduler->pqueue, new_task);
	if (PQUEUE_FAILED == enqueue_status)
	{
		TaskDestroy(new_task); new_task = NULL;
		return BAD_UID;
	}
	
	return TaskGetUID(new_task);
}

sched_status SchedulerRemoveTask(scheduler_t *scheduler, task_uid_t task_uid)
{
	void *task_data = NULL;
	
	assert(NULL != scheduler);
	
	if (NULL != scheduler->running_task &&
		(TaskIsMatch(scheduler->running_task, &task_uid, NULL)))
	{
		task_data = scheduler->running_task;
		scheduler->running_task = NULL;
	}
	else
	{
		task_data = PQueueErase(scheduler->pqueue, &TaskIsMatch, &task_uid, NULL);
		if (NULL == task_data)
		{
			return SCHED_FAIL;
		}
	}
	
	TaskDestroy(task_data); task_data = NULL;
	
	return SCHED_SUCCESS;
}

void SchedulerStop(scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	
	scheduler->is_stop = 1;
}

sched_status SchedulerRun(scheduler_t *scheduler)
{
	
	assert(NULL != scheduler);
	
	/* run until all tasks executed or until reaching stop */
	while (!SchedulerIsEmpty(scheduler) && 1 != scheduler->is_stop)
	{
		task_status task_status = SCHED_NO_REPEAT;
		int nq_status = PQUEUE_SUCCESS;

		/* hold task info and release task */
		scheduler->running_task = PQueuePeek(scheduler->pqueue);
		PQueueDequeue(scheduler->pqueue);
		
		/* sleep until reaching execution time */
		while (TaskGetTime(scheduler->running_task) > time(NULL))
		{
			sleep(TaskGetTime(scheduler->running_task) - time(NULL));
		}
		
		task_status = TaskExec(scheduler->running_task);

		/* if task is not self destroying */
		if (NULL != scheduler->running_task)
		{
	  /* update time for next execution of repeating task and add to scheduler*/
			if (SCHED_REPEAT == task_status)
			{
				TaskUpdateTime(scheduler->running_task);
				
				nq_status = PQueueEnqueue(scheduler->pqueue, 
													scheduler->running_task);
				
				if (PQUEUE_FAILED == nq_status)
				{
					TaskDestroy(scheduler->running_task); 
					scheduler->running_task = NULL;
					scheduler->is_stop = 0;
					
					return SCHED_FAIL;
				}			
				
			}
			/* if task does not repeat - destroy task after execution */
			else
			{
				TaskDestroy(scheduler->running_task); 
			}
			
				scheduler->running_task = NULL;
		}	
	}
	
	scheduler->is_stop = 0;
	
	return SCHED_SUCCESS;
}

int SchedulerIsEmpty(const scheduler_t *scheduler)
{
	assert(NULL != scheduler);

	return (PQueueIsEmpty(scheduler->pqueue) && NULL == scheduler->running_task);
}

size_t SchedulerSize(const scheduler_t *scheduler)
{
	size_t executing_task = 0;
	
	assert(NULL != scheduler);
	
	if (NULL != scheduler->running_task)
	{
		executing_task = 1;
	}
	
	/* if there is a task executing now- add to size */ 
	return (executing_task + PQueueSize(scheduler->pqueue));
}

void SchedulerClear(scheduler_t *scheduler)
{
	task_t *task = NULL;
	
	assert(NULL != scheduler);
	
	if (NULL != scheduler->running_task)
	{
		TaskDestroy(scheduler->running_task); 
		scheduler->running_task = NULL;
	}	

	while (!SchedulerIsEmpty(scheduler))
	{
		task = PQueuePeek(scheduler->pqueue);
		TaskDestroy(task); task = NULL;
		PQueueDequeue(scheduler->pqueue);
	}

}

