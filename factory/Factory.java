package il.co.ilrd.factory;

import java.util.HashMap;
import java.util.Map;
import java.util.function.Function;

public class Factory<T,K,D> {
	private Map<K, Function<D,? extends T>> funcMap;
	
	public Factory(){
		funcMap = new HashMap<K, Function< D,? extends T>>();
	}
	/**
	 * 
	 * @param key
	 * @return new object from type T
	 */
	public T create(K key) {
		return create(key, null); 
	}
	
	/**
	 * 
	 * @param key
	 * @param data
	 * @return new object from type T
	 */
	public T create(K key, D data) {
		return funcMap.get(key).apply(data);
	}

	/**
	 * 
	 * @param key
	 * @param funcMap
	 */
	public void add(K key, Function<D,? extends T> func) {
		funcMap.put(key, func);
	}
}
