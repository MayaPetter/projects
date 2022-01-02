package il.co.ilrd.linked_list;

public class LinkedList {
    
	private Node head;
    
    public LinkedList() {

    	head  = new Node(null, null);
    }

    /**
     * Pushes element at the front of list.
     *
     * @param data Data to push into list.
     */
    public void pushFront(Object data) {
    	
    	head = new Node(head, data);
    }

    /**
     * Removes and returns the first element of this list.
     *
     * @return The element at the front of this list, if list is empty null is returned.
     */
    public Object popFront() {

    	
    	if (isEmpty())
    	{
    		return null;
    	}

    	Object removedData = head.data;
        
    	head = head.next;
    	
    	return removedData;
    }

    /**
     * Checks if list is empty.
     *
     * @return true if empty, otherwise false.
     */
    public boolean isEmpty() {
        
    	return (null == head.next);
    }

    /**
     * Get number of elements in the list.
     *
     * @return Number of elements in the list.
     */
    public int size() {
    	
    	int counter = 0;
    	
    	ListIterator runner = new ListIterator(head);
    	
    	while (runner.hasNext()) {
    	
    		++counter;
    		
    		runner.next();
    	}
        
    	return counter;
    }

    /**
     * Finds data in Linked List.
     *
     * @param data Data to find.
     * @return Iterator with found data, otherwise null.
     */
    public Iterator find(Object data) {
    	
    	Iterator runner = begin();
    	Iterator current = begin();
    	
    	while (runner.hasNext()) {

    		if (runner.next().equals(data)) {
    			
    			return current;
    		}
    		
    		current.next();
    	}
        
    	return null;
    }

    /**
     * @return Iterator to the first element
     */
    public Iterator begin() {
    	
        return new ListIterator(head);
    }

    private class ListIterator implements Iterator {
       
    	private Node curr;
        
        private ListIterator(Node curr) {
        	
        	this.curr = curr;
        }
        
        @Override
        public boolean hasNext() {
            
        	return (null != curr.next);
        }
        
        @Override
        public Object next() {
        	
        	Object thisData = curr.data;
        	
        	curr = curr.next;
        	
        	return thisData;
        }
    }

    private class Node {
       
        private Node next;
        private Object data;

        private Node(Node next, Object data) {
            
            this.next = next;
            this.data = data;
        }
    }
}
