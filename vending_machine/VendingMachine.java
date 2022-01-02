package il.co.ilrd.vending_machine;

import java.time.LocalTime;
import java.util.List;
import java.util.Objects;
import java.util.function.Consumer;

public class VendingMachine {

    private int currBalance;
    private List<Product> products;
    private Consumer<String> view;
    private Thread thread;
    
    private State currState = State.INIT;
    private boolean active = true;
    private LocalTime startTime = LocalTime.now();

    public VendingMachine(List<Product> items,  Consumer<String> view) {
        this.products = items;
        this.view = view;
        initThread();
    }
    
    private void initThread() {
        
        thread = new Thread ( () -> {
            while (active) {
                
            	currState.timeout(this);
                
            	try {
					Thread.sleep(1000);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
            }
        });
    }
    
    private int getCurrBalance() {
		return currBalance;
	}

	private void setCurrBalance(int currBalance) {
		this.currBalance = currBalance;
	}

	private void displayChange(){
		view.accept("your change: " + getCurrBalance());
		setCurrBalance(0);
	}
	
	private void displayMenu(){
		for (Product i : products) {
			int productIndex = products.indexOf(i);
			view.accept((productIndex + 1) + ": " + products.get(productIndex).name);
		}
	}
	
	private void displayProduct(Product item){
		view.accept("product: " + item.name);
	}
	
	/**
     * Initializes and invokes VendingMachine to begin state.
     */
    public void start() {
        currState.init(this);
    }

    /**
     * Shutdown the VendingMachine.
     */
    public void shutDown() {
    	currState.shutDown(this);
    }

    /**
     * Insert money into machine.
     * @param coinValue inserted for transaction.
     */
    public void insertCoin(int coinValue) {
    	currState.insertCoin(this, coinValue);
    }

    /**
     * Chooses product to buy.
     * @param chosen product.
     * invalid product will not have effect,
     * error message will be displayed in ViewModel.
     */
    public void chooseProduct(int productIndex) {
    
    	if (products.size() <= productIndex) {
        	view.accept("invalid choice");
    	} else {
    		currState.chooseProduct(this, products.get(productIndex));
    	}
    }

    /**
     * cancels current transaction, and flushes change
     */
    public void cancel() {
    	currState.cancel(this);
    }

    private enum State {
        INIT {
            @Override
            void init(VendingMachine vm) { 
            	vm.thread.start();
            	setState(vm, IDLE);
            }
            
            @Override
            void handleCurrentState(VendingMachine vm) {
            	vm.view.accept("welcome");
            }
            
        },

        IDLE {
        	@Override
        	void insertCoin(VendingMachine vm, int coinValue) {
        	    vm.setCurrBalance(coinValue);
        	    setState(vm, COLLECTING_MONEY);
        	}

        	@Override
        	void handleCurrentState(VendingMachine vm) {
        		vm.displayMenu();
            }
        	
        	@Override
        	void chooseProduct(VendingMachine vm, Product product) {
            	vm.view.accept(product.name + " price is " + product.price);
            	setState(vm, IDLE);
            }

        },

        COLLECTING_MONEY {
            
        	@Override
            void insertCoin(VendingMachine vm, int coinValue) {
                vm.setCurrBalance(vm.getCurrBalance() + coinValue);
                setState(vm, COLLECTING_MONEY);
            }

            @Override
            void chooseProduct(VendingMachine vm, Product product) {

            	if (product.price > vm.currBalance) {
	            		setState(vm, COLLECTING_MONEY); 
	        	} else {
	        		vm.displayProduct(product);
	        		vm.setCurrBalance(vm.getCurrBalance() - product.price);
	        		vm.displayChange();
	        		setState(vm, TRANSACTION_COMPLETE); 
	        	}
            }
	            	
            void cancel (VendingMachine vm) {
            	vm.view.accept("cancaling....");
            	vm.displayChange();
            	setState(vm, IDLE); 
            }

            @Override
            void timeout(VendingMachine vm) {
            	if (vm.startTime.plusSeconds(10).isBefore(LocalTime.now())) {
            		vm.view.accept("time is out");
            		COLLECTING_MONEY.cancel(vm);
            	}
            }
            
            @Override
            void handleCurrentState (VendingMachine vm) {
            	if (0 != vm.currBalance) {
            		vm.view.accept("balance: " + vm.currBalance);
            	}
            }
        },

        TRANSACTION_COMPLETE {
            @Override
            void timeout(VendingMachine vm) {
            	if (vm.startTime.plusSeconds(5).isBefore(LocalTime.now())) {
            		setState(vm, IDLE);
            	}
            }
            
            @Override
            void handleCurrentState (VendingMachine vm) {
            	vm.view.accept("Thank you for buying!");
            }
            
            @Override
            void chooseProduct(VendingMachine vm, Product product) {
            	vm.view.accept("please wait for transaction to complete");
            }
        };

        /*
         *  Will initialize Vending machine, change to waitForProduct state
         * @param vm - current Vending Machine
         */
        void init(VendingMachine vm) {

        }
        
        /*
         *  Will shut down Vending machine, change change active flag to false
         * @param vm - current Vending Machine
         */
        void shutDown(VendingMachine vm) {
        	vm.view.accept("system shutting down");
        	vm.active = false;
        	try {
				vm.thread.join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
        }
        /*
         * Handles payment transaction, increases current balance,
         * changes state to WAIT_FOR_PRODUCT if there is sufficient balance.
         */
        void insertCoin(VendingMachine vm, int money) {
        }

        /*
         * Changes selected product in vm, changes state to waitForMoney
         */
        void chooseProduct(VendingMachine vm, Product product) {
        	vm.view.accept(product.name + " price is " + product.price);
        }

        /*
         * @param vm cancels current transaction,
         *  changes state to waitForProduct
         */
        void cancel(VendingMachine vm){
        }

        /*
         * @param vm 
         *  handles timeout event switch state when time is out 
         */
        void timeout (VendingMachine vm) {
        }
       
        /*
         * @param vm 
         *  handles timeout event switch state when time is out 
         */
        void setState(VendingMachine vm, State state) {
        	vm.startTime = LocalTime.now();
        	vm.currState.handleCurrentState(vm);
        	vm.currState = state;
        }
        
        /*
         * @param vm 
         * sets start time to current time of state transition
         */
        abstract void handleCurrentState (VendingMachine vm);
            	
       }

    public static class Product {
        private String name;
        private int price;

        Product(String name, int price) {
            this.price = price;
            this.name = name;
        }

		@Override
		public int hashCode() {
			return  Objects.hash(name, price);
		}

		@Override
		public boolean equals(Object obj) {
			if (obj == null)
				return false;
			if (getClass() != obj.getClass())
				return false;
			Product other = (Product) obj;
			if (name == null) {
				if (other.name != null)
					return false;
			} else if (!name.equals(other.name))
				return false;
			return true;
		}

    }

}
