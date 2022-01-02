package il.co.ilrd.chat_server;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.atomic.AtomicBoolean;

import il.co.ilrd.chat_server.ChatProtocol.MsgFormat;
import il.co.ilrd.hash_map.HashMap;

public class ChatServer implements Runnable{
	private ServerSocketChannel serverSocketChannel;
	private Selector selector;
	private AtomicBoolean isRunning = new AtomicBoolean(true);
	private MsgHandler msgHandler = new MsgHandler();

	public ChatServer(String ip, int port) {
		try {
			selector = Selector.open(); 
			registerChannelToSelector(ip, port);
		} catch (IOException e) {
			e.printStackTrace(); 
		}
	}

	private void registerChannelToSelector(String ip, int port) throws IOException {
		serverSocketChannel = ServerSocketChannel.open();
		serverSocketChannel.bind(new InetSocketAddress(ip, port));
		serverSocketChannel.configureBlocking(false);
		serverSocketChannel.register(selector, SelectionKey.OP_ACCEPT);
	}

	public void stop() {
		isRunning.set(false);
		try {
			serverSocketChannel.close();
			selector.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void start() {
		this.run();
	}

	@Override
	public void run() {
		try {
			while(isRunning.get()) {

				selector.select(); /*blocking*/
				Set<SelectionKey> events = selector.selectedKeys();
				Iterator<SelectionKey> eventsIter = events.iterator();

				while(eventsIter.hasNext()) {
					SelectionKey key = eventsIter.next();
					listen(key);
					eventsIter.remove();
				}
				events .clear();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private void listen(SelectionKey key) throws IOException {
		if(key.isAcceptable()) {
			acceptNewClient(key);

		} else if (key.isReadable()) {
			readMsg(key);
		} else {
			throw new IOException();
		}
	}

	private void acceptNewClient(SelectionKey key) throws IOException {
		SocketChannel clientSocket = serverSocketChannel.accept();
		clientSocket.configureBlocking(false);
		clientSocket.register(selector, SelectionKey.OP_READ);
	}

	private void readMsg(SelectionKey key) throws IOException {
		SocketChannel clientChannel = (SocketChannel)key.channel();
		ByteBuffer buffer = ByteBuffer.allocate(1024);
		clientChannel.read(buffer);
		msgHandler.routeMsg(new String(buffer.array()).trim(), clientChannel);
		buffer.clear();
	}

	private class MsgHandler {
		private Map<String, ClientChannel> clients = new HashMap<>();
		private static final String REGISTER_MSG = " joined the group";
		private static final String UNREGISTER_MSG = " left the group";
		private static final String INVALID_MSG = "Not valid msg";

		private void routeMsg(String msg, SocketChannel clientChannel) {
			MsgFormat newMsg = ChatProtocol.decodeMsg(msg);

			if(newMsg.isMsg() && userExist(newMsg.getID())) {
				broadcastMsg(newMsg, clientChannel);

			} else if(newMsg.isUnregister() && userExist(newMsg.getID())) {
				handleUnregister(newMsg, clientChannel);
				
			} else if(newMsg.isRegister()) {
				handleRegistration(newMsg, clientChannel);
				
			} else {
				sendInvalideMsg(newMsg, clientChannel);
			}
		}

		private boolean userExist(String ID) {
			return clients.containsKey(ID);
		}

		private void broadcastMsg(MsgFormat newMsg, SocketChannel clientSocket) {
			for(ClientChannel client : clients.values()) {
				client.handleMsg(newMsg.getID(), newMsg.getData());
			}
		}
		
		private void handleUnregister(MsgFormat newMsg, SocketChannel clientChannel) {
			ClientChannel user = clients.get(newMsg.getID());
			try {
				user.clientChannel.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
			clients.remove(newMsg.getID());
			newMsg.setData(newMsg.getID() + UNREGISTER_MSG);
			broadcastMsg(newMsg, clientChannel);
		}

		private void handleRegistration(MsgFormat newMsg, SocketChannel clientChannel) {
			if (addUser(newMsg.getID(), clientChannel)) {
				newMsg.setData(newMsg.getID() + REGISTER_MSG);
				broadcastMsg(newMsg, clientChannel);
			}
		}

		private void sendInvalideMsg(MsgFormat newMsg, SocketChannel clientChannel) {
			new ClientChannel(clientChannel).handleMsg(newMsg.getID(), INVALID_MSG);
		}

		private boolean addUser(String ID, SocketChannel clientChannel) { 
			ClientChannel user = new ClientChannel(clientChannel);
			if (!userExist(ID)) {
				clients.put(ID, user);
				user.handleMsg(ID, "Welcome " + ID);
				return true;
			} else {
				user.handleInvalidMsg(ID, "invalid name");
				return false;
			}
		}
	}

	private class ClientChannel {

		private SocketChannel clientChannel;

		private ClientChannel(SocketChannel clientChannel) {
			this.clientChannel = clientChannel;
		}

		private void handleMsg(String ID, String msg) { 
			ByteBuffer buffer = ByteBuffer.wrap(ChatProtocol.newMsg(ID, msg).getBytes());
			try {
				clientChannel.write(buffer);
				buffer.clear();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
		private void handleInvalidMsg(String ID, String msg) { 
			ByteBuffer buffer = ByteBuffer.wrap(ChatProtocol.newInvalidMsg(ID, msg).getBytes());
			try {
				clientChannel.write(buffer);
				buffer.clear();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	public static void main(String[] args) throws InterruptedException {

		ChatServer server = new ChatServer("localhost", 6666);
		server.start();
	}	
}
