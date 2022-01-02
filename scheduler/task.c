/******************************************
* Author:   Maya Petter                   *
* Reviewer:                               *
* Purpose:                                *
*******************************************/
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include <time.h>   /* time_t */

#include "task.h"   /* task functions */

#define UNUSED(x) (void)(x)

struct task
{
    task_uid_t uid;
    task_func func;
    void *param;
    time_t time; 
    time_t interval;
};

static task_t *InitTask(task_t *task, void *param, task_func func, time_t time, 
																time_t interval)
{
    task->uid = UIDCreate();
    if (1 == UIDIsSame(BAD_UID, task->uid))
    {
        return NULL; 
    }
	
    task->func = func;
    task->param = param;
    task->time = time; 
    task->interval = interval;
	
	return task;
}

task_t *TaskCreate(void *param, task_func func, time_t time, time_t interval)
{
    task_t *new_task = NULL;
    
    assert(NULL != func);
    assert(0 != time);
    
    new_task = (task_t *)malloc(sizeof(task_t));
    if (NULL == new_task)
    {
        return NULL;
    }
    
    new_task = InitTask(new_task, param, func, time, interval);
    
    return new_task;
}

task_status TaskExec(task_t *task)
{
	assert(NULL != task);
	
	return (task->func(task->param));
}

int TaskIsMatch(void *task, const void *task_uid, void *param)
{
	assert(NULL != task);
	assert(NULL != task_uid);
	
	UNUSED(param);	
	
	return UIDIsSame(TaskGetUID((task_t *)task), *((task_uid_t *)task_uid));
}

task_uid_t TaskGetUID(const task_t *task)
{
	assert(NULL != task);
	
	return task->uid;
}

time_t TaskGetTime(const task_t *task)
{
	assert(NULL != task);
	
	return task->time;
}

int TaskIsBefore(const task_t *new_task, const task_t *scheduled_task, 
				 void *param)
{
	assert(NULL != new_task);
	assert(NULL != scheduled_task);
	
	UNUSED(param);
		
	return (TaskGetTime(new_task) < TaskGetTime(scheduled_task));
}				 

void TaskUpdateTime(task_t *task)
{
	assert(NULL != task);
	
	task->time += task->interval; 
}

void TaskDestroy(task_t *task)
{
	
	free(task); task = NULL;
}

