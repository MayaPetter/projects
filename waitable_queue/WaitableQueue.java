package il.co.ilrd.waitable_queue;

import java.util.concurrent.TimeUnit;

public interface WaitableQueue<T> {
	/**
	 * The constructor does not permit non-comparable element
	 */
	
	/**
	 * Inserts the specified element into this priority queue
	 * @param data to insert
	 * @return true if this queue changed as a result of the call
	 * @throws InterruptedException 
	 */
	public boolean enqueue(T data);
	
	/**
	 * Remove the head of this queue
	 * @return the head of this queue
	 * @throws InterruptedException
	 */
	public T dequeue() throws InterruptedException;
	
	/**
	 * remove the head of this queue
	 * @param timeout the maximum time to wait for a permit
	 * @param TimeUnit the time unit of the timeout argument
	 * @return the head of this queue, or null when timed out
	 * @throws InterruptedException
	 */
	public T dequeue(long timeout, TimeUnit unit) throws InterruptedException;
	
	/**
	 * Removes a single instance of the specified element from this queue, if it is present
	 * @param dataToRemove
	 * @return true if this queue changed as a result of the call
	 */
	public boolean remove(T dataToRemove);
}
