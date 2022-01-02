 package il.co.ilrd.thread_pool;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.io.IOException;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import il.co.ilrd.thread_pool.ThreadPool.TaskPriority;

class TreadPoolTest {

	private class TaskPrint implements Runnable{
		
		@Override
		public void run() {
			System.err.println(Thread.currentThread().getName() + " from total " + Thread.activeCount() + " threads");
		}
		
	}
//	@Test
//	void test() {
//		ThreadPool threadPool = new ThreadPool(10);
//		threadPool.submit(new TaskPrint(), TaskPriority.HIGH);
//		threadPool.submit(new TaskPrint(), TaskPriority.MEDIUM);
//		threadPool.execute(new TaskPrint());
//		threadPool.execute(new Runnable() {
//			@Override
//			public void run() {
//				System.err.println(Thread.currentThread().getName() + " runnable");
//			}
//		});
//		threadPool.setNumThreads(2);
//		threadPool.submit(new TaskPrint(), TaskPriority.LOW);
//		threadPool.submit(new TaskPrint(), TaskPriority.LOW);
//		threadPool.pause();
//		threadPool.submit(new Runnable() {
//			@Override
//			public void run() {
//				System.err.println(Thread.currentThread().getName() + " 2nd runnable");
//			}
//		}, TaskPriority.HIGH); 
//		threadPool.submit(new TaskPrint(), TaskPriority.HIGH);
//	}
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
            System.err.println(pool.awaitTermination(11, TimeUnit.SECONDS));
        }catch(InterruptedException exc){}

        pool.shutdown();
        assertTrue(pool.awaitTermination(11, TimeUnit.SECONDS));
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
	            assertFalse(pool.awaitTermination(1, TimeUnit.NANOSECONDS));
	        }catch(InterruptedException exc){}

	        pool.shutdown();
	        pool.awaitTermination(5, TimeUnit.SECONDS);
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

//	    @Test
//	    public void testExecutionException() throws ExecutionException{
//	        ThreadPool pool = new ThreadPool(4);
//	        int times = 10000;
//	        int[] arr = new int[times];
//
//	        for (int i = 0; i < times; ++i){
//	            int finalI = i;
//	            pool.submit(()-> {
//	                        arr[finalI]++;
//	                    }, TaskPriority.HIGH
//	            );
//	        }
//	       
//	        Assertions.assertThrows(ExecutionException.class, () -> {});
//	        Future<?> result = pool.submit(
//
//	                ()->    {
//	                    throw new IOException("Haha test");
//	                },
//	                TaskPriority.LOW);
//
//	       try{
//	           result.get(5, TimeUnit.SECONDS);
//	       }catch(ExecutionException except){
//	            throw new ExecutionException("haha", except);
//	       }
//	       catch(Exception except){
//	           //something else
//	       }
//	    }
}
