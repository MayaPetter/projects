package il.co.ilrd.waitable_queue;

import java.util.Comparator;
import java.util.Objects;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class WaitableQueueConVar<T> implements WaitableQueue<T>{
	private Queue<T> conVarSynchQ;
	private final Lock lockQ = new ReentrantLock();
	private final Condition notEmpty = lockQ.newCondition();

	public WaitableQueueConVar() {
		this(null);
	}

	public WaitableQueueConVar(Comparator<? super T> c) {
		conVarSynchQ = new PriorityQueue<>(c);
	}

	public boolean enqueue(T data){
		boolean addStatus = false;
		Objects.requireNonNull(data);

		lockQ.lock();
		addStatus = conVarSynchQ.add(data); 
		
		try {
			if(addStatus) {
				notEmpty.signal();
			}
		} finally {
			lockQ.unlock();
		}
		return addStatus;
	}

	public T dequeue() throws InterruptedException {
		lockQ.lockInterruptibly();

		try {
			while (conVarSynchQ.isEmpty()) {
				notEmpty.await();
			}
			return conVarSynchQ.poll();

		} finally {
			lockQ.unlock();
		}
	}


	public T dequeue(long timeout, TimeUnit unit) throws InterruptedException{
		T dataToReturn = null;
		lockQ.lockInterruptibly();

		try {
			if (conVarSynchQ.isEmpty()) {
				if (notEmpty.await(timeout, unit)) {
					dataToReturn = conVarSynchQ.poll();
				}
			}
			else {
				dataToReturn = conVarSynchQ.poll();
			}

		} finally {
			lockQ.unlock();
		}
		return dataToReturn;
	}

	public boolean remove(T dataToRemove) {
		Objects.requireNonNull(dataToRemove);
		boolean removeStatus = false;

		lockQ.lock();
		try {
			removeStatus = conVarSynchQ.remove(dataToRemove);
		} finally {
			lockQ.unlock();
		}

		return removeStatus; 
	}

}