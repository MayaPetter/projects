package il.co.ilrd.vending_machine;

import java.util.ArrayList;
import java.util.List;

public class Test {

	public static void main(String[] args) {
	
		List<VendingMachine.Product> items = new ArrayList<>();
		
		items.add(new VendingMachine.Product("Cola", 3));
		items.add(new VendingMachine.Product("Jack", 25));
		items.add(new VendingMachine.Product("Jin", 20));
		items.add(new VendingMachine.Product("Sprite", 5));

		VendingMachine newVM = new VendingMachine(items, System.out::println);
		newVM.start();
		
		Test test = new Test();

		test.InsertAndCancel(newVM, 3, 5);
		test.InsertExact(newVM, 2, 20);
		test.InsertLess(newVM, 2, 15);
		test.InsertMore(newVM, 2, 40);
		newVM.shutDown();
		
		newVM.start();

		newVM.insertCoin(2);
		newVM.insertCoin(5);
		try {
			Thread.sleep(20000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		newVM.chooseProduct(1);
		newVM.insertCoin(20);
		newVM.insertCoin(10);
		newVM.chooseProduct(1);
		newVM.shutDown();
		System.out.println("Finished");
	}
	
	private void InsertAndCancel (VendingMachine newVM, int index, int coin){
		System.out.println("InsertAndCancel");
		newVM.insertCoin(coin);
		newVM.cancel();
	}
	
	private void InsertExact (VendingMachine newVM, int index, int coin){
		System.out.println("InsertExact");
		newVM.insertCoin(coin);
		newVM.chooseProduct(index);
	}
	
	private void InsertLess (VendingMachine newVM, int index, int coin){
		newVM.insertCoin(coin);
		newVM.chooseProduct(index);
		newVM.insertCoin(5);
		try {
			Thread.sleep(25000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		newVM.chooseProduct(index);
		System.out.println("InsertLess");
	}
	
	private void InsertMore (VendingMachine newVM, int index, int coin){
		System.out.println("InsertMore");
		newVM.insertCoin(coin);
		newVM.chooseProduct(index);
	}

}
