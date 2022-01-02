package il.co.ilrd.networking;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.UnknownHostException;

public class BroadcastClientPing {
	public static void main(String[] args) throws UnknownHostException {

		try(
				DatagramSocket serverSocket = new DatagramSocket(6667);
				){

			serverSocket.setBroadcast(true);
			byte[] receiveData = new byte[1024];
			byte[] sendData = "Ping".getBytes();

			DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
			InetAddress group = InetAddress.getByName("255.255.255.255");
			DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, group, 6666);

			while(true)
			{
				serverSocket.send(sendPacket);
				serverSocket.receive(receivePacket);
				Thread.sleep((long)(Math.random() * 1000));
				System.out.println("Server: " + new String(receivePacket.getData()));
			}
		} catch (IOException e) {
			e.printStackTrace();
		} catch (InterruptedException e1) {
			e1.printStackTrace();
		}
	}
}
