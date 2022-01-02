package il.co.ilrd.linked_list;

public class TestLinkedList {
	
	public static void main(String[] args) {
		
	LinkedList newList = new LinkedList();
		
		if (0 != newList.size()) {
			System.out.println("Error create or size");
		}
	
		if (!newList.isEmpty()) {
			System.out.println("Error is empty");
		}
	
		int a = 1, b = 2, c = 3, d = 4, e = 5, not = 6;
		int size = 0;
		
		newList.pushFront(a);
		++size;
		
		if (size != newList.size()) {
			System.out.println("Error push " + size);
		}
		
		newList.pushFront(b);
		++size;
		
		if (size != newList.size()) {
			System.out.println("Error push " + size);
		}
		
		newList.pushFront(c);
		++size;
		
		if (size != newList.size()) {
			System.out.println("Error push " + size);
		}
		
		newList.pushFront(d);
		++size;
		
		if (size != newList.size()) {
			System.out.println("Error push " + size);
		}
		
		newList.pushFront(e);
		++size;
		
		if (size != newList.size()) {
			System.out.println("Error push " + size);
		}
		
		System.out.println("begin " +newList.begin());
		
		if (null != newList.find(not)) {
			System.out.println("Error not find");
		}
		
		if (null == newList.find(a)) {
			System.out.println("Error find a");
		}
		
		if (null == newList.find(e)) {
			System.out.println("Error find e");
		}
		
		newList.popFront();
		--size;
		
		if (size != newList.size()) {
			System.out.println("Error pop " + size);
		}
		
		newList.popFront();
		--size;
		
		if (size != newList.size()) {
			System.out.println("Error pop " + size);
		}
		newList.popFront();
		--size;
		
		if (size != newList.size()) {
			System.out.println("Error pop " + size);
		}
		newList.popFront();
		--size;
		
		if (size != newList.size()) {
			System.out.println("Error pop " + size);
		}
		
		newList.popFront();
		--size;
		
		if (size != newList.size()) {
			System.out.println("Error pop " + size);
		}
		
		if (!newList.isEmpty()) {
			System.out.println("Error is empty");
		}
	}

}
