package il.co.ilrd.selector;

import java.io.File;
import java.io.IOException;
import java.nio.file.FileSystems;
import java.nio.file.Path;
import java.nio.file.StandardWatchEventKinds;
import java.nio.file.WatchEvent;
import java.nio.file.WatchKey;
import java.nio.file.WatchService;
import java.util.concurrent.atomic.AtomicBoolean;

import il.co.ilrd.observer.CallBack;
import il.co.ilrd.observer.Dispatcher;

public class FileMonitor implements Runnable{
	private AtomicBoolean isStop = new AtomicBoolean(false);
    private Dispatcher<Void> changeDispatcher = new Dispatcher<>();
    private WatchService watcher;
    private Path fileToMonitor;
    private Thread runningThread;
    
    public FileMonitor(String pathToMonitor) throws IOException{
    	File monitoredFile = new File(pathToMonitor); 
    	if(!monitoredFile.isFile()) {
    		throw new IOException();
    	}
    	watcherInit(monitoredFile);
    }
    
    private void watcherInit(File monitoredFile) throws IOException {
    	fileToMonitor = monitoredFile.toPath();
    	watcher = FileSystems.getDefault().newWatchService();
    	fileToMonitor.getParent().register(watcher, StandardWatchEventKinds.ENTRY_MODIFY, StandardWatchEventKinds.ENTRY_DELETE);
    }
    
    @Override
    public void run(){
    	runningThread = Thread.currentThread();
    	while(!isStop.get()) {
    		WatchKey watchKey = null;
    		try {
    			watchKey = watcher.take();
			} catch (InterruptedException e) {
				continue;
			} 
    		notifyIfEvent(watchKey);
    		watchKey.reset();
    	}
    }
    
    public void register(CallBack<Void> callback){
    	changeDispatcher.register(callback);
    }

    public void unregister(CallBack<Void> callback){
    	changeDispatcher.unregister(callback);
    }

    public void stopMonitoring() throws IOException{
    	isStop.compareAndSet(false, true);
    	runningThread.interrupt();
    	watcher.close();
    }

    private void notifyIfEvent(WatchKey watchKey) {
    	for (WatchEvent<?> event: watchKey.pollEvents()) {
    		Path fileName = (Path)event.context();
    		if (StandardWatchEventKinds.OVERFLOW == event.kind()) {
                continue;
            }

    		if(fileToMonitor.endsWith(fileName)) {
    			changeDispatcher.notifyAll(null);
    		}
    	}
    }
    
}