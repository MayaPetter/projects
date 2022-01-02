#include <stdio.h> /*printf*/
#include <assert.h> /*assert*/

#include "scheduler.h"

#define REPEATS 5
#define UNUSED(x) (void)x

/******************************************************************************/
void TestCreateAndDestroy();
void TestAddAndRemove();
void TestRunAndStop();

/******************************************************************************/
static task_uid_t uid_for_remove = {0,0};
static task_uid_t uid_for_self_remove = {0,0};

task_status TestFunc(void *param);
task_status TestCallStop(void *param);
task_status TestAddTask(void *param);
task_status TestRemoveTask(void *param);
task_status TestClearQue(void *param);
task_status TestCallIsEmpty(void *param);
task_status TestSelfDestroy(void *param);
/******************************************************************************/

int main ()
{
	TestCreateAndDestroy();
	TestAddAndRemove();
	TestRunAndStop();
	
	return 0;
}

/******************************************************************************/
void TestCreateAndDestroy()
{
	scheduler_t *scheduler = SchedulerCreate(); 
	
	assert(NULL != scheduler);
	assert(1 == SchedulerIsEmpty(scheduler));
	assert(0 == SchedulerSize(scheduler));
	
	SchedulerDestroy(scheduler); scheduler = NULL; 
}

void TestAddAndRemove()
{
	task_uid_t tuid1 = {0,0};
	task_uid_t tuid2 = {0,0};
	task_uid_t tuid3 = {0,0};
	size_t counter = 0;
	scheduler_t *scheduler = SchedulerCreate(); 
	assert(NULL != scheduler);
	assert(1 == SchedulerIsEmpty(scheduler));
	assert(0 == SchedulerSize(scheduler));
	
	tuid1 = SchedulerAddTask(scheduler, &counter, TestFunc, time(NULL)+10, 5);
	assert(!UIDIsSame(tuid1, BAD_UID));
	tuid2 = SchedulerAddTask(scheduler, &counter, TestFunc, time(NULL), 5);
	assert(!UIDIsSame(tuid2, BAD_UID));
	tuid3 = SchedulerAddTask(scheduler, &counter, TestFunc, time(NULL)+5, 5);
	assert(!UIDIsSame(tuid3, BAD_UID));
	
	assert(0 == SchedulerIsEmpty(scheduler));
	assert(3 == SchedulerSize(scheduler));
	
	
	/*remove exicting uid*/
	assert(0 == SchedulerRemoveTask(scheduler, tuid2)); 
	assert(2 == SchedulerSize(scheduler));
	/*remove non-exicting uid*/
	assert(1 == SchedulerRemoveTask(scheduler, tuid2)); 
	assert(2 == SchedulerSize(scheduler));
	/*remove BAD_UID - from full*/
	assert(1 == SchedulerRemoveTask(scheduler, BAD_UID));
	
	/*clear*/
	SchedulerClear(scheduler);
	assert(1 == SchedulerIsEmpty(scheduler));
	assert(0 == SchedulerSize(scheduler));
	
	/*remove from empty*/
	assert(1 == SchedulerRemoveTask(scheduler, tuid1)); 
	/*remove BAD_UID - from empty*/
	assert(1 == SchedulerRemoveTask(scheduler, BAD_UID));
	
	SchedulerDestroy(scheduler); scheduler = NULL;	
}

void TestRunAndStop()
{
	size_t i = 0; 
	task_uid_t temp_uid = {0,0};
	scheduler_t *scheduler = SchedulerCreate();
	assert(NULL != scheduler);
	assert(1 == SchedulerIsEmpty(scheduler));
	assert(0 == SchedulerSize(scheduler));
	
	/*filler tasks, do nothing*/
	for(i = 0; i < 3; ++i)
	{
		time_t time_to_execute = time(NULL)+i;
		time_t interval = 3;
		
		temp_uid = SchedulerAddTask(scheduler, scheduler, TestFunc,
									   time_to_execute, interval);
		assert(!UIDIsSame(temp_uid, BAD_UID));
	}
	
	/*filler task to remove*/
	uid_for_remove = SchedulerAddTask(scheduler, scheduler, TestFunc,
									   time(NULL)+2, 1);
	assert(!UIDIsSame(uid_for_remove, BAD_UID));
	
	/*self destructing task*/
	uid_for_self_remove = SchedulerAddTask(scheduler, scheduler, TestSelfDestroy,
									   time(NULL)+2, 1);
	assert(!UIDIsSame(uid_for_self_remove, BAD_UID));
	
	/*calls stop*/
	temp_uid = SchedulerAddTask(scheduler, scheduler, TestCallStop,
									   time(NULL)+5, 3);
	assert(!UIDIsSame(temp_uid, BAD_UID));
	
	/*adds task to queue*/
	temp_uid = SchedulerAddTask(scheduler, scheduler, TestAddTask,
									   time(NULL)+4, 0);
	assert(!UIDIsSame(temp_uid, BAD_UID));
	
	/*removes task from queue*/
	temp_uid = SchedulerAddTask(scheduler, scheduler, TestRemoveTask,
									   time(NULL)+3, 0);
	assert(!UIDIsSame(temp_uid, BAD_UID));
	
	/*clears*/
	temp_uid = SchedulerAddTask(scheduler, scheduler, TestClearQue,
									   time(NULL)+7, 0);
	assert(!UIDIsSame(temp_uid, BAD_UID));
	
	/*calls IsEmpty*/
	temp_uid = SchedulerAddTask(scheduler, scheduler, TestCallIsEmpty,
									   time(NULL)+4, 0);
	assert(!UIDIsSame(temp_uid, BAD_UID));
	
	/*run scheuler*/
	SchedulerRun(scheduler);
	puts("stopped");
	SchedulerRun(scheduler);
	
	
	assert(1 == SchedulerIsEmpty(scheduler));
	assert(0 == SchedulerSize(scheduler));
	
	SchedulerDestroy(scheduler); scheduler = NULL;	
}
/******************************************************************************/


task_status TestFunc(void *param)
{
	static size_t counter = 0; 
	assert(NULL != param);
	
	printf("filler task - %lu\n", (SchedulerSize((scheduler_t *)param)));
	++counter;
	
	if(30 < counter)
	{
		return SCHED_NO_REPEAT;
	}
	
	return SCHED_REPEAT;
}

task_status TestCallStop(void *param)
{
	puts("stoping");
	SchedulerStop((scheduler_t *)param);
	
	return SCHED_REPEAT;
}

task_status TestAddTask(void *param)
{
	task_uid_t tuid = SchedulerAddTask((scheduler_t *)param, 
									   (scheduler_t *)param, 
									   TestFunc, time(NULL)+1, 1);
	assert(!UIDIsSame(tuid, BAD_UID));
	puts("adding task");
	
	return SCHED_NO_REPEAT;
}

task_status TestRemoveTask(void *param)
{
	puts("removing task");
	assert(0 == SchedulerRemoveTask((scheduler_t *)param, uid_for_remove)); 
	
	return SCHED_NO_REPEAT;
}

task_status TestClearQue(void *param)
{
	puts("clearing queue");
	SchedulerClear((scheduler_t *)param);
	return SCHED_REPEAT;
}

task_status TestCallIsEmpty(void *param)
{
	printf("is empty? %d\n", SchedulerIsEmpty((scheduler_t *)param));
	
	return SCHED_NO_REPEAT;
}

task_status TestSelfDestroy(void *param)
{
	puts("self destroying task");
	SchedulerRemoveTask(((scheduler_t *)param), uid_for_self_remove);
	return SCHED_REPEAT;
}
