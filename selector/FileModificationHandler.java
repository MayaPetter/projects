package il.co.ilrd.selector;

import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

import il.co.ilrd.observer.CallBack;

public class FileModificationHandler {
	private CallBack<Void> changeCallback;
	private FileMonitor monitor;
	private FileModifier backupper;
	private Thread monitoringThread;
	private Path fileToMonitor;
	private Path backupFile;


	public FileModificationHandler(String pathToMonitor, String pathTobackup) throws IOException{
		monitor = new FileMonitor(pathToMonitor);
		backupper = new FileModifier(pathTobackup);
		fileToMonitor = (new File(pathToMonitor)).toPath();
		backupFile = (new File(pathTobackup)).toPath();
	}

	public void start(){
		changeCallback = new CallBack<Void>(this::handle, ()->{}) ;
		monitor.register(changeCallback);
		monitoringThread = new Thread(monitor);
		monitoringThread.start();
	}

	public void stop(){
		try {
			monitor.stopMonitoring();
			backupper.close();
			monitor.unregister(changeCallback);
			monitoringThread.join();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void handle(Void v) {
		List<String> fileContent;
		List<String> backupFileContent;
		try {
			fileContent = new ArrayList<>(Files.readAllLines(fileToMonitor, StandardCharsets.UTF_8));
			backupFileContent = new ArrayList<>(Files.readAllLines(backupFile, StandardCharsets.UTF_8));
			updateFile(fileContent, backupFileContent);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	private void updateFile(List<String> fileContent, List<String> backupFileContent) throws IOException {
		createLines(fileContent, backupFileContent);	
		updateLines(fileContent, backupFileContent);	
		removeExtraLines(fileContent, backupFileContent);
	}
	
	private void createLines(List<String> fileContent, List<String> backupFileContent) throws IOException {
		int fileSize = fileContent.size();
		int backupSize = backupFileContent.size();

		if (backupSize < fileSize) {
			for(int i = backupSize; i < fileSize; ++i) {
				backupFileContent.add(i, fileContent.get(i));
				backupper.create(fileContent.get(i));
			}
		}
	}

	private void updateLines(List<String> fileContent, List<String> backupFileContent) throws IOException {
		int fileSize = fileContent.size();

		for(int i = 0; i < fileSize; ++i) {
			if(!fileContent.get(i).contentEquals(backupFileContent.get(i))) {
				backupFileContent.set(i, fileContent.get(i));
				backupper.update(i, fileContent.get(i));
			}
		}
	}

	private void removeExtraLines(List<String> fileContent, List<String> backupFileContent) throws IOException {
		int fileLength = fileContent.size();
		int backupLength = backupFileContent.size();

		if (backupLength > fileLength) {
			for (int i = backupLength -1 ; i > fileLength; -- i) {
				backupFileContent.remove(i);
				backupper.delete(i);
			}
		}
	}
}

