package il.co.ilrd.selector;

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.file.WatchEvent;

import org.junit.jupiter.api.Test;

import il.co.ilrd.observer.CallBack;

class HilaFileMonitorTest {
	String test = "/home/maya/maya-petter/fs/projects/src/il/co/ilrd/selector/monitoredTest.txt";
	String temp = "/home/maya/maya-petter/fs/projects/src/il/co/ilrd/selector/backup.txt";
	RandomAccessFile writer = null;
	boolean wasChanged = false;
	FileMonitor fm = null;
	CallBack<WatchEvent<?>> cb = new CallBack<>(this::handel);
	Thread monitorThread;
	
//	@Before
//	public void beforeTests() throws IOException {
//		fm = new FileMonitor(test);
//		monitorThread = new Thread(fm);
//		fm.register(cb);
//		monitorThread.start();
//	}
//	
//	@After
//	public void aferTests() throws IOException, InterruptedException {
//		writer.close();
//		fm.stopMonitoring();
//		monitorThread.join();
//	}
//	
//	@Test
//	void makeChangesToFile() throws InterruptedException, IOException {
//		writer = new RandomAccessFile(test, "rw");
//		for (int i = 0; i < 10; ++i) {
//			writer.writeChars("change");
//			Thread.sleep(10);
//			assertTrue(wasChanged);
//			wasChanged = false; 
//		}
//	}
//	
//	@Test
//	public void makeChangesToOtherFile() throws InterruptedException, IOException {
//		writer = new RandomAccessFile(temp, "rw");
//		for (int i = 0; i < 10; ++i) {
//			writer.writeChars("change");
//			Thread.sleep(10);
//			assertFalse(wasChanged);
//		}
//	}
//	
	@Test
	void basicTest() throws IOException, InterruptedException {
		FileModificationHandler fmh = new FileModificationHandler(test, temp);
		fmh.start();
		writeToFile();
		Thread.sleep(1000);
		assertEquals(new File(test).length() , new File(temp).length());
	}
	
	private void writeToFile() throws IOException {
		BufferedWriter writer = new BufferedWriter(new FileWriter(new File(test), true));
		for (int i = 0; i < 10; ++i) {
			writer.append("Hello World").flush();
			writer.newLine();
		}
		writer.close();
	}

	void handel(WatchEvent<?> watchkey) {
		wasChanged = true;
    }

}