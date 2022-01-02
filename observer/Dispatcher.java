package il.co.ilrd.observer;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

public class Dispatcher<T> {
	
	private List<CallBack<T>> listToCallBack = new ArrayList<>();
	
	public void register(CallBack<T> callBack) {
		Objects.requireNonNull(callBack);
		callBack.setDispatcher(this);
		listToCallBack.add(callBack);
	}
	
	public void unregister(CallBack<T> callBack) {
		if (null != callBack) {
			listToCallBack.remove(callBack);
			callBack.setDispatcher(null);
		}
	}
	
	public void notifyAll(T data) {
		for (CallBack<T> callBack : listToCallBack) {
			callBack.update(data);
		}
	}
	
	public void terminate() {
		for (CallBack<T> callBack : listToCallBack) {
			callBack.stop();
			callBack.setDispatcher(null);
		}
		listToCallBack.clear();
	}
}
