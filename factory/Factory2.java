package il.co.ilrd.factory;

import java.util.HashMap;
import java.util.Map;
import java.util.function.Function;
 
public class Factory2 {
	
	private Map<String, Function<String,String>> commands = new HashMap<>();
//
//	commands.put("get", (String item) -> get(item));
	
	public void add(String type, Function<String,String> func) {
		this.commands.put(type, func);
	}
	
	public String create(String type) {
		 return this.commands.get(type).apply(null);
	}
	
	
	public static void main (String[]args) {
		Factory2 factory = new Factory2();
		factory.add("maya", (String type)-> new String());
		factory.add("cezar", (String type)-> "run " +type);
		factory.add("George", (String type)-> "run " +type);
		
		System.out.println(factory.create("maya"));
		System.out.println(factory.create("cezar"));
		System.out.println(factory.create("George"));
	}
	
	    
}



class Maya{}
