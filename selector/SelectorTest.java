package il.co.ilrd.selector;

import static org.junit.Assert.*;
import static org.junit.jupiter.api.Assertions.assertFalse;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.WatchEvent;
import java.util.ArrayList;
import java.util.List;

import org.junit.Test;

import il.co.ilrd.observer.CallBack;

public class SelectorTest {
	boolean isHandeled = false;
	boolean isstopped = false;
	String test = "/home/maya/maya-petter/fs/projects/src/il/co/ilrd/selector/monitoredTest.txt";
	String temp = "/home/maya/maya-petter/fs/projects/src/il/co/ilrd/selector/backup.txt";
	File testFile = new File(test);
	File backupFile = new File(temp);
	FileModificationHandler fileHandler;

	public void handler() {
		isHandeled = true;
	}

	public void update(WatchEvent<?> key) {
		isstopped = false;
	}

	//	@Test
	//	public void monitorTest() throws IOException{
	//		FileMonitor monitor = new FileMonitor(test);
	//		assertTrue(null != monitor);
	//		CallBack<WatchEvent<?>> changeCallback = new CallBack<WatchEvent<?>>(this::update, this::handler);
	//		monitor.register(changeCallback);
	//
	//		Thread fileChanger = new Thread(monitor);
	//		fileChanger.start();
	//		FileModifier writer = new FileModifier(test);
	//		try {
	//			Thread.sleep(1000);
	//		} catch (InterruptedException e) {}
	//
	//
	//		try {
	//			assertTrue(1 == writer.create("hello world"));
	//			writer.update(0, "Maya");
	//		} catch (IOException e) {
	//			e.printStackTrace();
	//		}
	//		try {
	//			Thread.sleep(1000);
	//		} catch (InterruptedException e) {}
	//
	//
	//		try {
	//			assertTrue(3 == writer.create("what's up "));
	//			writer.update(0, "yes yes again");
	//			writer.update(1, "Maya");
	//			writer.delete(3);
	//		} catch (IOException e) {
	//			e.printStackTrace();
	//		}
	//
	//		monitor.stopMonitoring();
	//		try {
	//			fileChanger.join();
	//			writer.close();
	//		} catch (InterruptedException | IOException e) {
	//		} catch (Exception e) {
	//			e.printStackTrace();
	//		}
	//	}

	@Test
	public void handlerTest() throws IOException, InterruptedException {
		fileHandler = new FileModificationHandler(test, temp);
		fileHandler.start();

		writeToFile();
		Thread.sleep(1000);
		assertEquals(testFile.length(), backupFile.length());
		deleteFromFlie();

		Thread.sleep(1000);
		assertEquals(testFile.length(), backupFile.length());
		updateInMiddele();
		Thread.sleep(1000);
		assertEquals(testFile.length(), backupFile.length());
		fileHandler.stop();

		writeToFile();
		Thread.sleep(1000);
		assertFalse(testFile.length() == backupFile.length());
	}

	private void writeToFile() {
		try (BufferedWriter writer = new BufferedWriter(new FileWriter(new File(test), true))){
			for (int i = 0; i < 100; ++i) {
				writer.append("Hello " + i).flush();
				writer.newLine();
			}
		} catch(IOException e) {} catch (Exception e1) {
			System.err.println("append");
		}
	}
	private void deleteFromFlie() {
		try (FileModifier writer = new FileModifier(test)){
			Thread.sleep(1000);
			writer.delete(1);
			for (int i = 1; i < 20; ++i) {
				writer.delete(i);
			}
		} catch(IOException e) {} 
		catch (Exception e1) {}
	}
	private void updateInMiddele() {
		try (FileModifier writer = new FileModifier(test)){
			for (int i = 2; i < 5; ++i) {
				writer.update(1, "update ");
			}
		} catch(IOException e) {} 
		catch (Exception e1) {}

	}
}
