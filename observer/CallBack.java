package il.co.ilrd.observer;

import java.util.Objects;
import java.util.function.Consumer;

public class CallBack<T> {
	
	private Dispatcher<T> dispatcher;
	private Consumer<T> updateFunction;
	private Runnable stopFunction;
	
	public CallBack(Consumer<T> update){
		this(update, null);
	}
	
	public CallBack(Consumer<T> update, Runnable stop){
		this.updateFunction = Objects.requireNonNull(update);
		this.stopFunction = stop;
	}
	
	public Dispatcher<T> getDispatcher() {
		return dispatcher;
	}

	public void setDispatcher(Dispatcher<T> dispatcher) {
		this.dispatcher = dispatcher;
	}

	public void update(T data) {
		updateFunction.accept(data);
	}
	
	public void stop() {
		stopFunction.run();
	}
	
}
