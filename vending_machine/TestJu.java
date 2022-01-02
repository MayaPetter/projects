package il.co.ilrd.vending_machine;


import java.util.ArrayList;
import java.util.List;

import org.junit.jupiter.api.Test;

class TestJu {

	@Test
	void insertAndCancel() {
		List<VendingMachine.Product> items = new ArrayList<>();
		
		items.add(new VendingMachine.Product("Jack", 25));

		VendingMachine newVM = new VendingMachine(items, System.out::println);

		newVM.start();
		newVM.insertCoin(5);
		newVM.cancel();
		newVM.shutDown();
	}
	
	@Test
	void insertExact() {
		List<VendingMachine.Product> items = new ArrayList<>();
		
		items.add(new VendingMachine.Product("Jack", 25));
		items.add(new VendingMachine.Product("Sprite", 5));

		VendingMachine newVM = new VendingMachine(items, System.out::println);

		newVM.start();
		newVM.insertCoin(25);
		newVM.chooseProduct(1);
		try {
			Thread.sleep(10000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		newVM.insertCoin(5);
		newVM.chooseProduct(0);
		newVM.shutDown();
	}
	
	@Test
	void testTimeout() {
		List<VendingMachine.Product> items = new ArrayList<>();
		
		items.add(new VendingMachine.Product("Jack", 25));

		VendingMachine newVM = new VendingMachine(items, System.out::println);

		newVM.start();
		newVM.insertCoin(25);
		try {
			Thread.sleep(20000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		newVM.chooseProduct(0);
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		newVM.shutDown();
	}
	
	@Test
	void payLess() {
		List<VendingMachine.Product> items = new ArrayList<>();
		
		items.add(new VendingMachine.Product("Jack", 25));

		VendingMachine newVM = new VendingMachine(items, System.out::println);

		newVM.start();
		newVM.insertCoin(5);
		newVM.chooseProduct(0);
		try {
			Thread.sleep(20000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	@Test
	void payMore() {
		List<VendingMachine.Product> items = new ArrayList<>();
		
		items.add(new VendingMachine.Product("Jack", 25));

		VendingMachine newVM = new VendingMachine(items, System.out::println);

		newVM.start();
		newVM.insertCoin(50);
		newVM.chooseProduct(0);
		try {
			Thread.sleep(20000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
}
