package il.co.ilrd.chat_server;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.util.Scanner;
import java.util.concurrent.atomic.AtomicBoolean;

import il.co.ilrd.chat_server.ChatProtocol.MsgFormat;

public class TCPClient implements Runnable {
	private SocketChannel socketChannel;
	private UserInterface ui = new UserInterface();

	public TCPClient(String IP, int port) {
		try {
			InetSocketAddress address = new InetSocketAddress(IP, port);
			socketChannel = SocketChannel.open(address);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void run() {
		ByteBuffer buffer = ByteBuffer.allocate(1024);
		int byteRead = 0;
		try {
			while(-1 != byteRead) {
				byteRead = socketChannel.read(buffer);
				ui.displayMsg(buffer);
				buffer.clear();
			}
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				System.err.println("Goodbye!");
				socketChannel.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	public void start() {
		Thread userInputThread = new Thread(ui);
		userInputThread.start();

		Thread listenToServerThread = new Thread(this);
		listenToServerThread.start();

	}

	private class UserInterface implements Runnable{
		private Scanner inputScenner = new Scanner(System.in);
		private String fromUser;
		private String userName;
		private AtomicBoolean isExit = new AtomicBoolean(false);

		@Override
		public void run() {
			displayMenue();
			while(!isExit.get()) {
				String input = inputScenner.nextLine();
				fromUser = handleUserInput(input);
				if(null == fromUser) {
					continue;
				}
				try {
					socketChannel.write(ByteBuffer.wrap(fromUser.getBytes()));
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}

		private void displayMenue() {
			System.err.println("New user? please register - \"register\"");
			System.err.println("unregister - \"unregister\"");
		}

		private void exit() {
			isExit.compareAndSet(false, true);
		}

		private String handleUserInput(String input) {
			if (input.equalsIgnoreCase("register")) {
				return handleRegistration();

			} else if (input.equalsIgnoreCase("unregister")){
				return ChatProtocol.unregister(userName);

			} else if (input.equalsIgnoreCase("exit")){
				this.exit();
				return ChatProtocol.unregister(userName);
			}


			if(null == userName) {
				System.err.println("please register to group");
				return null;
			}

			return ChatProtocol.newMsg(userName, input);
		}

		private String handleRegistration() {
			String currUser = userName;
			System.err.println("Enter user name:");
			userName = inputScenner.nextLine();
			return checkUser(currUser);
		}

		private String checkUser(String currUser) {
			if (null != currUser) {
				System.err.println("Do you replace " + currUser + "? (Y/N)");
				String replace = inputScenner.nextLine();
				if (replace.equalsIgnoreCase("N")){
					userName = currUser;
					return null;
				}
			}
			return ChatProtocol.newRegistration(userName);
		}
		private void displayMsg(ByteBuffer buffer) {
			ByteBuffer dispBuffer = buffer;
			buffer.clear();
			MsgFormat msg = ChatProtocol.decodeMsg(new String(dispBuffer.array()).trim());
			if(msg.isInvalid()) {
				System.err.println(msg.getData());
				System.err.println("please try again");
				userName = null;
			} else if (msg.getID().contentEquals(userName)) {
				System.err.println(msg.getData());
			} else {
				System.err.println(msg.getID() + ": " + msg.getData());
			}
		}
	}

	public static void main(String[] args) throws InterruptedException {

		TCPClient client = new TCPClient("localhost", 6666);
		client.start();
	}

}