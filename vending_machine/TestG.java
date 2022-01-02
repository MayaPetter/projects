package il.co.ilrd.vending_machine;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;


public class TestG {

    public static void main(String args[]) throws IOException {

        VendingMachine vm = new VendingMachine(createProdList(), System.out::println);
        vm.start();

        BufferedReader inputBuff = new BufferedReader
                                    (new InputStreamReader(System.in));
        String input = "";

        while (true) {

            input = inputBuff.readLine();

            if (input.equals("pay")) {
                vm.insertCoin(5);
            }

            else if (input.equals("stop")){
                break;
            }

            else if (input.equals("Cola")){
                vm.chooseProduct(0);
            }
            else if (input.equals("Click")){
                vm.chooseProduct(3);
            }
            else if (input.equals("two")){
                vm.insertCoin(2);
            }
            else if (input.equals("cancel")) {
                vm.cancel();
            }

        }

        vm.shutDown();
    }

    public static List<VendingMachine.Product> createProdList(){

    	List<VendingMachine.Product> itemList =
                new ArrayList<VendingMachine.Product>();

        itemList.add(new VendingMachine.Product("Cola", 3));
        itemList.add(new VendingMachine.Product("Sprite", 2));
        itemList.add(new VendingMachine.Product("Fanta", 4));

        return itemList;
    }
}

