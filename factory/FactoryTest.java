package il.co.ilrd.factory;

import il.co.ilrd.tree.*;
import il.co.ilrd.strings.*;

import static org.junit.Assert.assertTrue;

import java.nio.file.FileSystemException;
import java.util.ArrayList;
import java.util.List;
import java.util.function.Function;

import org.junit.Test;


public class FactoryTest {

	@Test //using factory with lambda expression
	public void lambdaTest() {
		Factory<String,Integer,String> stringFactory = new Factory<>(); 
		
		stringFactory.add(1, (obj)->{return new String(obj);});
		
		assertTrue(stringFactory.create(1, "Dilbert").contentEquals("Dilbert"));
	}
	
	@Test //using factory with lambda expression to create Tree
	public void treeTestLambda() {
		Factory<Tree,Integer,String> factory = new Factory<>(); 
		factory.add(1, (path )-> {try {
				return new Tree(path);
			} catch (FileSystemException e) {
				System.out.println("IOException");
				return null;
			}
		});
		Tree tree = factory.create(1, "/home/student/maya-petter");
//		tree.print();
	}
	
	
	@Test //using factory with anonymous class
	public void anonymusTest() {
		Factory<String,Integer,String> annoFactory = new Factory<>();
		
		annoFactory.add(69, new Function<String, String>() {
		    public String apply(String s) {
			      return new String(s);
			    }
			});
		
		assertTrue(annoFactory.create(69, "Dilbert").contentEquals("Dilbert"));
	}
	
	@Test //using factory with static method reference
	public void staticTest() {
		Factory<Boolean,Integer,String> staticRef = new Factory<>();
		
		staticRef.add(3, Strings::isPalindrome);
		staticRef.add(6, (str)->Strings.isPalindrome(str));
		
		assertTrue(staticRef.create(3, "Dilbert").equals(staticRef.create(6, "Dilbert")));
		assertTrue(staticRef.create(3, "AbcooocbA").equals(staticRef.create(6, "AbcooocbA")));
	}
	
	@Test //using factory with non-static method reference for existing object
	public void nonStaticExistTest() {
		Factory<String,Integer,Integer> nonStaticRef = new Factory<>();
		NonStaticExist obj = new NonStaticExist();
		nonStaticRef.add(9, obj::play);
		
		assertTrue(nonStaticRef.create(9, 6).contentEquals("playing"));
	}
	
	@Test //using factory with non-static method reference for not existing object
	public void nonStaticNotExistTest(){
		Factory<Integer,Integer,List<Integer>> nonStaticRef = new Factory<>();
		nonStaticRef.add(6, List<Integer>::size);
		
		List<Integer> list = new ArrayList<>();
		list.add(1);
		list.add(8);
		list.add(40);
		list.add(20);
		assertTrue(nonStaticRef.create(6, list).equals(list.size()));

	}
	
	public class NonStaticExist{
		public String play(Integer num) {
			return "playing";
		}
	}
}
