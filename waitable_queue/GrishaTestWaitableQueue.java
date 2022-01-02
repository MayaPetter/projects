package il.co.ilrd.waitable_queue;

import org.junit.Before;
import org.junit.Test;

import java.util.Arrays;
import java.util.NoSuchElementException;
import java.util.concurrent.TimeUnit;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class GrishaTestWaitableQueue {

    WaitableQueue<Integer> waitableQueue;
    Integer x;
    boolean bool = false;

   @Before
   public void createQueue(){
       waitableQueue = new WaitableQueueConVar<>();
   }

   @Test
   public void testSingleThreadedInsertRemove(){
		assertTrue(waitableQueue.enqueue(4));
       try {
           assertEquals(Integer.valueOf(4), waitableQueue.dequeue());
           waitableQueue.dequeue(1, TimeUnit.SECONDS);
       }
       catch(InterruptedException exc){}
   }

   @Test
    public void testingMultiThreadedInsert() throws InterruptedException{

       for(int i = 0; i < 100; ++i){
           testMulti();
           assertEquals(Integer.valueOf(3), x);
//           assertTrue(bool);
       }
   }

   public void testMulti() throws InterruptedException{
       Thread prod = new Thread(()->{
			waitableQueue.enqueue(3);
	});
       Thread prod1 = new Thread(()->{
			waitableQueue.enqueue(3);
	});

       Thread consumer = new Thread(()-> {
           try {
               x = waitableQueue.dequeue();
           }catch(InterruptedException exc){
               System.out.println("throws");
           }
       });
       Thread consumer2 = new Thread(()->bool =
               waitableQueue.remove(3));

       consumer.start();

       Thread.sleep(3);

       prod.start();
       prod1.start();
       consumer2.start();

       prod.join();
       consumer.join();
       prod1.join();
       consumer2.join();
   }

   @Test
    public void testingMultiThreadedMoreThanCPU(){

       for (int i = 0; i < 50;++i) {
           testMoreThreadsThanCPU();
       }
   }

   public void testMoreThreadsThanCPU(){

       for (int i = 0; i < 200; ++i) {
           final int finalI = i;
           Thread prod12 = new Thread(() ->{ 
			waitableQueue.enqueue(finalI);});
           prod12.start();
       }

       for (int i1 = 0; i1 < 100; ++i1) {
               Thread consumer = new Thread(() -> {
                   try {int x = waitableQueue.dequeue();
           }catch(InterruptedException exc){}});

           consumer.start();
       }

       for (int i1 = 0; i1 < 50; ++i1){
           Thread consumer = new Thread(()->
                   waitableQueue.remove(Integer.valueOf(3)));
           consumer.start();
       }

       for (int i1 = 0; i1 < 50; ++i1){
           Thread consumer = new Thread(()->{
               try {
                   waitableQueue.dequeue(2, TimeUnit.SECONDS);
                }catch(InterruptedException exc) {
                   System.out.println("throws at dequeue");
                   }
           });
           consumer.start();
       }

       try{
           Thread.sleep(555);
       }catch (InterruptedException exc){}}

   

   @Test
   public void testInterruptingABlockedThread(){

       Thread consumer = new Thread(() ->{
           try{
               waitableQueue.dequeue();
           }catch(InterruptedException exc)
           {
               System.out.println("Thread was interrupted");
           }
       });

       consumer.start();

       try{
           Thread.sleep(20);
       }
       catch (InterruptedException g ){
       }

       consumer.interrupt();
   }

}