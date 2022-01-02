#include <time.h>   /* time */
#include <assert.h> /* assert */
#include <stdio.h>  /* puts, printf */

#include "task.h"   /* task functions */

extern const uid_t BAD_UID;

static task_status TaskFuncNoReapeat(void *param)
{
	size_t counter = 0;
	
	assert(NULL != param);
	
	counter = *(size_t *)param;
	 
	++counter;
	
	return SCHED_NO_REPEAT;
}

static task_status PrintTest()
{

	puts("MAYA");
	
	return SCHED_NO_REPEAT;

}

static task_status TaskFuncReapeat(void *param)
{
	static size_t counter = 0;
	
	assert(NULL != param);
	
	++counter;
	
	if (3 == counter)
	{	
		return SCHED_NO_REPEAT;
	}	
	else
	{
		return SCHED_REPEAT;
	}	
}

static void TaskCreateDestroy()
{	
	size_t param = 0;
	time_t task_time = time(NULL) + 900;
	task_t *interval_task = TaskCreate(&param, &TaskFuncNoReapeat,(task_time - 5), 60); 
	task_t *new_task = TaskCreate(&param, &TaskFuncReapeat, task_time, 0);

	assert(NULL != interval_task);
	assert(NULL != new_task);
	
	assert(0 == TaskIsMatch(interval_task, new_task, &param));
	
	assert(task_time == TaskGetTime(new_task));
	assert((task_time - 5) == TaskGetTime(interval_task));
	
	TaskDestroy(interval_task);
	TaskDestroy(new_task);	
}

static task_t *CreatRepeatTask()
{
	size_t param = 0;
	time_t task_time = time(NULL) + 900;
	task_t *repeat_task = TaskCreate(&param, &TaskFuncReapeat, task_time, 0);
	
	assert(NULL != repeat_task);	
		
	return repeat_task;
}

static task_t *CreatRepeatTaskWithInterval()
{
	size_t param = 0;
	time_t task_time = time(NULL) + 900;
	task_t *repeat_task = TaskCreate(&param, &TaskFuncReapeat, task_time, 10);
	
	assert(NULL != repeat_task);	
		
	return repeat_task;
}

static task_t *CreatNoRepeatTask()
{
	size_t param = 0;
	time_t task_time = time(NULL) + 50;
	task_t *no_repeat_task = TaskCreate(&param, &TaskFuncNoReapeat, task_time, 0);

	assert(NULL != no_repeat_task);
	
	return no_repeat_task;
}

static void TaskExecTest()
{
	task_t *repeat_task = CreatRepeatTask();
	task_t *no_repeat_task = CreatNoRepeatTask();
	
	assert(NULL != repeat_task);
	assert(NULL != no_repeat_task);

	assert(SCHED_NO_REPEAT == TaskExec(no_repeat_task));
	
	assert(SCHED_REPEAT == TaskExec(repeat_task));
	assert(SCHED_REPEAT == TaskExec(repeat_task));
	assert(SCHED_NO_REPEAT == TaskExec(repeat_task));
	
	TaskDestroy(no_repeat_task); 
	TaskDestroy(repeat_task);
}

static void TaskIsMatchAndGetUID()
{
	int param = 0;
	task_t *repeat_task = CreatRepeatTask();
	task_t *no_repeat_task = CreatNoRepeatTask();
	task_uid_t repeat_uid = TaskGetUID(repeat_task);
	task_uid_t no_repeat_uid = TaskGetUID(no_repeat_task);
	
	assert(NULL != repeat_task);
	assert(NULL != no_repeat_task);

	assert(0 == TaskIsMatch(repeat_task, no_repeat_task, &param));
	assert(0 == UIDIsSame(repeat_uid, no_repeat_uid));
	
	TaskDestroy(no_repeat_task); 
	TaskDestroy(repeat_task);
}

static void GetTimeIsBeforeUpdatTimeTest()
{
	int param = 0;
	task_t *repeat_task = CreatRepeatTask();
	task_t *interval_task = CreatRepeatTaskWithInterval();
	time_t time_no_interval = {0};
	time_t interval_time = {0};
	
	assert(NULL != repeat_task);
	assert(NULL != interval_task);
	
	time_no_interval = TaskGetTime(repeat_task); 
	interval_time = TaskGetTime(interval_task);
	
	assert(time_no_interval == interval_time);
	assert(0 == TaskIsBefore(repeat_task, interval_task, &param));
	assert(0 == TaskIsBefore(interval_task, repeat_task, &param)); 
	
	TaskUpdateTime(interval_task);
	TaskUpdateTime(repeat_task);
	
	assert(1 == TaskIsBefore(repeat_task, interval_task, &param));
	time_no_interval = TaskGetTime(repeat_task); 
	interval_time = TaskGetTime(interval_task);
	
	assert(time_no_interval != interval_time);
	
	TaskDestroy(interval_task); 
	TaskDestroy(repeat_task);
}

/*int main()*/
/*{*/
/*	TaskCreateDestroy();*/
/*	TaskExecTest();*/
/*	TaskIsMatchAndGetUID();*/
/*	GetTimeIsBeforeUpdatTimeTest();*/
/*			*/
/*	return 0;*/
/*}*/
