package il.co.ilrd.thread_pool;

import java.util.ArrayList;
import java.util.ConcurrentModificationException;
import java.util.List;
import java.util.Objects;
import java.util.concurrent.Callable;
import java.util.concurrent.CancellationException;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.RejectedExecutionException;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;


import il.co.ilrd.waitable_queue.*;

public class ThreadPool implements Executor{
	private WaitableQueue<Task<?>> taskQueue = new WaitableQueueSema<>();
	private List<WorkerThreads> threadPool; 
	private volatile boolean isPause = false;
	private volatile boolean isShutdown = false;
	private int numThreads;
	private static final int MIN_PRIORITY = 0;
	private static final int MAX_PRIORITY = 100;
	private Semaphore pauseSem = new Semaphore(0);
	private Semaphore awaitTerminationSem = new Semaphore(0);

	public ThreadPool(int numThreads) { 
		if (1 > numThreads) {
			throw new IllegalArgumentException("need at least one thread");
		}
		this.numThreads = numThreads;
		threadPool = new ArrayList<>();
		addThreads(numThreads);
	}

	@Override
	public void execute(Runnable task) {
		submit(Executors.callable(task));
	}

	public  Future<?> submit (Runnable task, TaskPriority priority){
		return submit(Executors.callable(task), priority);
	}

	public <T> Future<T> submit (Runnable task, TaskPriority priority, T result){
		return submit(Executors.callable(task, result), priority);
	}

	public <T> Future<T> submit (Callable<T> task){
		return submit(task, TaskPriority.MEDIUM);
	}

	public <T> Future<T> submit (Callable<T> task, TaskPriority priority){

		Objects.requireNonNull(task, "need to submit a task");
		if (isShutdown) {
			throw new RejectedExecutionException("can't add new task now");
		}
		return submitInternal(task, priority);
	}
	
	public <T> Future<T> submitInternal (Callable<T> task, TaskPriority priority){

		Future<T> result = null;
		Task<T> newTask = new Task<>(task, priority);

		if (taskQueue.enqueue(newTask)) {
			result = newTask.getFuture();
		}
		return result;
	}

	public void setNumThreads(int numThreads) {
		int threadDiff = numThreads - this.numThreads;
		this.numThreads = numThreads;

		if (0 < threadDiff) {
			if (isPause) {
				pauseWorkerThreads(threadDiff);
			}

			addThreads(threadDiff);
		} else if (0 > threadDiff) { 
			addBadApple(Math.abs(threadDiff), setTaskSpecialPriority(MAX_PRIORITY));
			pauseSem.release(Math.abs(threadDiff));
		}
	}

	public void pause() {
		pauseSem.drainPermits();
		isPause = true;
		pauseWorkerThreads(numThreads);
	}

	public void resume() {
		isPause = false;
		pauseSem.release(numThreads); 
	}

	public boolean awaitTermination(long timeout, TimeUnit unit) throws InterruptedException {
		boolean aqcired = awaitTerminationSem.tryAcquire(numThreads, timeout, unit);
		if(aqcired) {
			try{
				for(WorkerThreads worker : threadPool) {
					if (null != worker) {
						worker.join();
					}
				}
			} catch (ConcurrentModificationException e) {}
		}
		return aqcired;
	}

	public void shutdown() { 
		if (isPause) {
			resume();
		}
		awaitTerminationSem.drainPermits();
		addBadApple(numThreads, setTaskSpecialPriority(MIN_PRIORITY));
		numThreads = 0;
		isShutdown = true;
	}

	public List<Runnable> shutdownNow() { 
		isShutdown = true;
		stopAllThreads();
		return convertQueueToList();
	}

	private List<Runnable> convertQueueToList(){
		List<Runnable> tasksList = new ArrayList<>();
		try {
			for(Task<?> task = taskQueue.dequeue(1, TimeUnit.NANOSECONDS) ; 
					null != task; 
					task = taskQueue.dequeue(1, TimeUnit.NANOSECONDS)) {
				tasksList.add(task);
			}
		} catch (InterruptedException e) {}

		return tasksList;
	}

	private void addThreads(int numThreads) {
		for(int i = 0; i < numThreads; ++i) {
			WorkerThreads worker = new WorkerThreads(); 
			threadPool.add(worker);
			worker.start(); 
		}
	}

	private void addBadApple(int numThreads, TaskPriority priority) {
		for(int i = 0; i < numThreads; ++i) {
			submitInternal(badApple, priority);
		}
	}

	private void pauseWorkerThreads(int numThreads) {
		pauseSem.drainPermits();
		for (int i = 0; i < numThreads ; ++i) {
			submitInternal(pauseWorkers, setTaskSpecialPriority(MAX_PRIORITY));
		}
	}

	private void stopAllThreads() {
		try {
			for (WorkerThreads t : threadPool) {
				if (null != t) {
					t.setStop();
					t.interrupt();
				}
			}
		}catch (ConcurrentModificationException e) {}
	}

	private TaskPriority setTaskSpecialPriority(int priority) {
		TaskPriority specialPriority = TaskPriority.SPECIAL;
		specialPriority.setPriority(priority);
		return specialPriority;
	}

	private Callable<Void> badApple = new Callable<Void>() {
		@Override
		public Void call() throws Exception {
			WorkerThreads currThread = (WorkerThreads)Thread.currentThread();
			currThread.setStop();
			threadPool.remove(currThread);
			return null;
		}
	};

	private Callable<Void> pauseWorkers = new Callable<Void>() {
		@Override
		public Void call() throws Exception {
			try {
				pauseSem.acquire();
			} catch (InterruptedException e) {}
			return null;
		}
	};

	private class Task<T> implements Runnable, Comparable<Task<?>> {
		private TaskPriority priority; 
		private Callable<T> toRun; 
		private TaskFuture taskResult = new TaskFuture(this); 
		private Semaphore taskSema = new Semaphore(0);
		private Lock taskLock = new ReentrantLock();

		public Task (Callable<T> toRun, TaskPriority priority) {
			this.toRun = toRun;
			this.priority = priority;
		}

		private Future<T> getFuture() {
			return taskResult;
		}

		@Override
		public void run() {
			if(!taskResult.isCancelled()) {
				taskLock.lock();
				try {
					if (TaskState.WAITTING.equals(taskResult.state)) {
						taskResult.updateState(TaskState.RUNNING);
						taskResult.result = toRun.call();
						taskResult.updateState(TaskState.DONE);
						taskSema.release();
					}
				} catch (Exception e) {
					taskResult.setExceptionState(e);
				} finally {
					taskLock.unlock();
				}
			}
		}

		@Override
		public int compareTo(Task<?> otherTask) {
			return (otherTask.priority.ordinal()- priority.ordinal());
		}

		private class TaskFuture implements Future<T> {
			private volatile TaskState state = TaskState.WAITTING;
			private T result;
			private Task<T> thisTask;
			private Exception exceptionState = null;

			public TaskFuture(Task<T> thisTask) {
				this.thisTask = thisTask;
			}

			private void updateState(TaskState state) {
				this.state = state; 
			}

			public void setExceptionState(Exception exceptionState) {
				this.exceptionState = exceptionState;
			}

			@Override
			public boolean cancel(boolean isInterruptable) {
				if(!isDone()) {
					updateState(TaskState.CANCELLED);
					taskSema.release();
					return taskQueue.remove(thisTask);
				}
				return false;
			}

			@Override
			public T get() throws InterruptedException, ExecutionException {
				T result = null;
				try {
					result = get(Long.MAX_VALUE, TimeUnit.DAYS);
				} catch (TimeoutException e) {}
				return result;
			}

			@Override
			public T get(long timeout, TimeUnit unit) 
					throws InterruptedException, ExecutionException, TimeoutException {
				if (isCancelled()) {
					throw new CancellationException();
				}
				if (!taskSema.tryAcquire(timeout, unit)) {
					if (null != exceptionState) {
						throw new ExecutionException(exceptionState);
					}

					throw new TimeoutException();
				}
				return result;
			}

			@Override
			public boolean isCancelled() {
				return (TaskState.CANCELLED == state);
			}

			@Override
			public boolean isDone() {
				return (TaskState.DONE == state || TaskState.CANCELLED == state);
			}
		}
	}

	private class WorkerThreads extends Thread {
		private volatile boolean isStop = false;

		@Override
		public void run() {
			while (!isStop) {
				try {
					Task<?> task = taskQueue.dequeue();
					task.run();
				} catch (InterruptedException e){}
			}
			awaitTerminationSem.release();
		}

		private void setStop() {
			isStop = true;
		}
	}

	public enum TaskPriority { 
		HIGH(10), MEDIUM(5), LOW(1), SPECIAL(0);

		private int priority;

		TaskPriority(int priority) {
			this.priority = priority;
		}

		public int getPriority() {
			return priority;
		}

		public void setPriority(int priority) {
			this.priority = priority;
		}
	}

	private enum TaskState{ 
		WAITTING, RUNNING, CANCELLED, DONE;
	}
}
