package il.co.ilrd.networking;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class BroadcastServerPong {

	public static void main(String[] args) {	
		try(
				DatagramSocket serverSocket = new DatagramSocket(6666);
				){
			
			serverSocket.setBroadcast(true);
			byte[] receiveData = new byte[1024];
			byte[] sendData = "Pong".getBytes();
			
			DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
			InetAddress group = InetAddress.getByName("255.255.255.255");
			DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, group, 6667);

			while(true)
            {
               serverSocket.receive(receivePacket);
               System.out.println("Client: " + new String(receivePacket.getData()));
               Thread.sleep((long)(Math.random() * 1000));
               serverSocket.send(sendPacket);
            }
		} catch (IOException e) {
			e.printStackTrace();
		} catch (InterruptedException e1) {
			e1.printStackTrace();
		}
	}

}
