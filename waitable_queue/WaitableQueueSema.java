package il.co.ilrd.waitable_queue;

import java.util.Comparator;
import java.util.Objects;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;

public class WaitableQueueSema<T> implements WaitableQueue<T>{
	private Queue<T> semaSynchronizedQ;
	private Semaphore elementSem = new Semaphore(0);
	private Object lockQ = new Object();

	public WaitableQueueSema() {
		this(null);
	}

	public WaitableQueueSema(Comparator<? super T> c) {
		semaSynchronizedQ = new PriorityQueue<>(c);
	}

	public boolean enqueue(T data) {
		Objects.requireNonNull(data);
		boolean addStatus = false;

		synchronized (lockQ) {
			addStatus = semaSynchronizedQ.add(data);
		}
		if (addStatus) {
			elementSem.release();
		}
		return addStatus;
	}

	public T dequeue() throws InterruptedException {
		elementSem.acquire();
		synchronized (lockQ) {
			return semaSynchronizedQ.poll();
		}
	}

	public T dequeue(long timeout, TimeUnit unit) throws InterruptedException{
		if (elementSem.tryAcquire(timeout, unit)) {
				return pollQueue();
		}
		return null;
	}

	public boolean remove(T dataToRemove) { 
		elementSem.tryAcquire();
		synchronized(lockQ) {
			return semaSynchronizedQ.remove(dataToRemove);
		}
	}
	
	private T pollQueue() {
		synchronized (lockQ) {
			return semaSynchronizedQ.poll(); 
		}
	}
}
