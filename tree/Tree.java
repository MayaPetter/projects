package il.co.ilrd.tree;

import java.io.File;
import java.io.UncheckedIOException;
import java.nio.file.FileSystemException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.function.Function;

import il.co.ilrd.factory.Factory;
public class Tree {
	private Node root;

    /**
     * @param: path of full pathname of root of tree.
     */
    public Tree(String path) throws FileSystemException {
		root = createNode(new File(path));
    }

    private static Node createNode(File file) throws FileSystemException {
    	if (!file.canRead()) {
    		throw new FileSystemException(file.getPath() + " path does not exist");    	
    	}
    	if (file.isDirectory()) {
    		return new Dir(file.getPath());
    	}
    	return new FileNode(file.getPath());
    }

    /**
     * Prints tree and all of it's files and directories recursively.
     */
    public void print(){
    	root.print(0);
    }

    private static void printIndentation(int depth) {
    	
    	System.out.print("|");
    	for (int i = 0; i < depth; ++i) {
    		System.out.print("_");
    	}
    }
    
    private static abstract class Node{

         String path;

        /**
         *
         * @param path name for given file.
         */
        private Node(String path){
        	this.path = path;
        }

        /**
         *
         * @param depth of directories, initial value should be 0.
         */
        public abstract void print(int depth);
    }

    private static class FileNode extends Node{

        /**
         *
         * @param path of given file.
         */
        private FileNode (String path){
            super(path);
        }

        /**
         *
         * @param depth of directories, initial value should be 0.
         */
        @Override
        public void print(int depth) {
        	File file = new File(path);
    		Tree.printIndentation(depth);
        	System.out.println(file.getName());
        }
    }

    private static class Dir extends Node{

        Collection<Node> nodes = new ArrayList<Tree.Node>();

        /**
         *
         * @param path of given directory
         * @throws FileSystemException 
         */
        private Dir(String path) throws FileSystemException{
            super(path);
            
            File file = new File(path);
            
            File[] files = file.listFiles();
            
            for (File f : files) {
            	try {
					nodes.add(Tree.createNode(f));
				} catch (FileSystemException e) {
					nodes.add(Tree.createNode(new File("Access denied")));
				}
            }
        }

        /**
         *
         * @param depth of directories, initial value should be 0.
         */
        @Override
        public void print(int depth) {
        	
        	File file = new File(path);
        	Tree.printIndentation(depth);
        	System.out.println(file.getName());
        	for (Node n : nodes) {
        		n.print(depth + 1);
        	}
    }
}

    public static void main (String []args){

        try{
            Tree t = new Tree("/home/student/maya-petter");
            t.print();
        }
        catch(FileSystemException exception){
            exception.printStackTrace();
        }
        
    	Factory<Tree,Integer,String> factory =new Factory<Tree,Integer,String>(); 
		factory.add(1, (path )-> {try {
			return new Tree(path);
		} catch (FileSystemException e) {
			System.out.println("IOException");
		}
		return null;});
		Tree tree = factory.create(1, "/home/student/maya-petter");
		tree.print();
    }
}
