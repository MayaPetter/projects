package il.co.ilrd.thread_pool;

import org.junit.Before;
import org.junit.Test;

import il.co.ilrd.thread_pool.ThreadPool.TaskPriority;

import java.io.IOException;
import java.util.concurrent.Callable;
import java.util.concurrent.CancellationException;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

import static org.junit.Assert.*;

public class GrishTest {

	@Test
	public void printNumThreads(){
		assertTrue(2 <= Thread.activeCount());
	}

	@Before
	public void createThreadPool(){
	}

	@Test
	public void testSimpleSubmit() throws InterruptedException{
		ThreadPool pool = new ThreadPool(48);
		Future<Integer> res = pool.submit(()->{
			return Integer.valueOf(3);},
				TaskPriority.LOW);
		pool.execute(()-> Thread.activeCount());
		pool.submit(()->{
		},
				TaskPriority.HIGH);

		try{
			Integer g = res.get();
			assertEquals(Integer.valueOf(3), g);
		}catch(InterruptedException exc){

		}catch(ExecutionException exc){

		}

		pool.shutdown();
		pool.awaitTermination(5, TimeUnit.SECONDS);
	}

	@Test
	public void testMultipleSubmitsWithGrowingNumberOfThreads() throws InterruptedException{

		int initNumOfThreads = Thread.activeCount();
		ThreadPool pool = new ThreadPool(1);
		int times = 100000;
		int[] arr = new int[times];
		for(int i = 0; i < times; ++i){
			int finalI = i;
			pool.execute(()-> {
				arr[finalI]++;});
		}
		pool.setNumThreads(50);
		try{
			Thread.sleep(1000);
		}catch(Exception exc){

		}
		assertTrue(40 + initNumOfThreads <= Thread.activeCount());
		for (int i = 0; i < times; ++i){
			assertEquals(1, arr[i]);
		}

		pool.shutdown();
		pool.awaitTermination(5, TimeUnit.SECONDS);
	}

	@Test
	public void testMultipleSubmitsWithShrinkingNumberOfThreads() throws InterruptedException{
		ThreadPool pool = new ThreadPool(1000);
		int times = 100000;
		int[] arr = new int[times];
		for(int i = 0; i < times; ++i){
			int finalI = i;
			pool.execute(()-> {
				arr[finalI]++;});
		}

		pool.setNumThreads(1);

		try{
			Thread.sleep(10000);
		}catch(Exception exc){

		}
		for (int i = 0; i < times; ++i){
			assertEquals(1, arr[i]);
		}

		System.err.println("%" + Thread.activeCount());

		pool.shutdown();
		assertTrue(3 <= Thread.activeCount());
		pool.awaitTermination(5, TimeUnit.SECONDS);
	}

	@Test
	public void testPause() throws InterruptedException{

		ThreadPool pool = new ThreadPool(200);

		int numOfIdxs = 20000;
		int[] arr = new int[numOfIdxs];

		pool.pause();

		for(int i = 0; i < numOfIdxs; ++i) {
			int finalI = i;
			pool.execute(() -> {
				arr[finalI]++;
			});
		}

		pool.resume();

		try{
			Thread.sleep(5000);
		}catch(Exception ex){}

		for (int i = 0; i < numOfIdxs; ++i){
			assertEquals(1, arr[i]);
		}

		pool.shutdown();
		pool.awaitTermination(5, TimeUnit.SECONDS);
	}

	@Test
	public void IntegrationSetNumThreadsAndPause() throws InterruptedException{
		ThreadPool pool = new ThreadPool(520);

		int times = 10000;
		int[] arr = new int[times];

		for(int i = 0; i < times; ++i){
			int finalI = i;
			pool.execute(()-> {
				arr[finalI]++;});
		}

		pool.pause();
		pool.setNumThreads(3);
		pool.resume();

		try{
			Thread.sleep(5000);
		}catch(Exception ex){}

		for (int i = 0; i < times; ++i){
			assertEquals(1, arr[i]);
		}

		pool.shutdown();
		System.err.println("Shutdown");
		pool.awaitTermination(5, TimeUnit.SECONDS);
	}

	@Test
	public void shutdownTestAwaitTrue() throws InterruptedException{
		ThreadPool pool = new ThreadPool(2);

		int times = 10000;
		int[] arr = new int[times];

		for(int i = 0; i < times; ++i){
			int finalI = i;
			pool.execute(()-> {
				arr[finalI]++;});
		}

		pool.shutdown();

		try{
			assertTrue(pool.awaitTermination(5, TimeUnit.SECONDS));
		}catch(InterruptedException exc){}

		pool.shutdown();
		pool.awaitTermination(10, TimeUnit.SECONDS);
	}

	@Test
	public void shutdownTestAwaitFalse() throws InterruptedException{
		ThreadPool pool = new ThreadPool(5);

		int times = 10000;
		int[] arr = new int[times];

		for(int i = 0; i < times; ++i){
			int finalI = i;
			pool.execute(()-> {
				arr[finalI]++;});
		}

		pool.shutdown();
		try{
			System.err.println(pool.awaitTermination(1, TimeUnit.NANOSECONDS));
		}catch(InterruptedException exc){}

		pool.shutdown();
		pool.awaitTermination(5, TimeUnit.SECONDS);
	}

	@Test
	public void shutdownNowTest() {
		ThreadPool pool = new ThreadPool(5);

		int times = 10000;
		int[] arr = new int[times];

		for(int i = 0; i < times; ++i){
			int finalI = i;
			pool.execute(()-> {
				arr[finalI]++;
			}
					);
		}

		pool.shutdownNow();

		try{
			Thread.sleep(200);
		}catch(InterruptedException exc){}

		boolean res = true;
		for (int i = 0; i < times; ++i){

			if (1 != arr[i]){
				res = false;
				break;
			}
		}

		assertFalse(res);
	}

	@Test
	public void futureCancelTesting(){
		ThreadPool pool = new ThreadPool(4);
		int times = 10000;
		int[] arr = new int[times];

		for (int i = 0; i < times; ++i){
			int finalI = i;
			pool.submit(()-> {
				arr[finalI]++;
			}, TaskPriority.HIGH
					);
		}

		Callable<Void> callable = ()-> {
			try{
				Thread.sleep(Long.MAX_VALUE);
			} catch(InterruptedException exc){
				throw new Exception("Something has occured", exc);
			}
			return null;};

			Future<?> result = pool.submit(callable, TaskPriority.LOW);

			result.cancel(true);

			assertTrue(result.isCancelled());
			assertTrue(result.isDone());
	}

	@Test
	public void futureIsDoneTesting(){
		ThreadPool pool = new ThreadPool(4);
		int times = 10000;
		int[] arr = new int[times];

		for (int i = 0; i < times; ++i){
			int finalI = i;
			pool.submit(()-> {
				arr[finalI]++;
			}, TaskPriority.HIGH
					);
		}

		Future<?> result = pool.submit(

				()-> {
					for (int i = 0; i < 100000000; ++i){
					}
				},
				TaskPriority.LOW);

		while (!result.isDone()){
		}

		assertTrue(result.isDone());
		assertFalse(result.isCancelled());
	}

	@Test(expected = ExecutionException.class)
	public void testExecutionException() throws ExecutionException{
		ThreadPool pool = new ThreadPool(4);
		int times = 10000;
		int[] arr = new int[times];

		for (int i = 0; i < times; ++i){
			int finalI = i;
			pool.submit(()-> {
				arr[finalI]++;
			}, TaskPriority.HIGH
					);
		}

		Future<?> result = pool.submit(

				()->    {
					throw new IOException("Haha test");
				},
				TaskPriority.LOW);

		try{
			result.get(5, TimeUnit.SECONDS);
		}catch(ExecutionException except){
			throw new ExecutionException("haha", except);
		}catch (CancellationException c) {
			
		} catch(Exception except){
			//something else
		}
	}
}