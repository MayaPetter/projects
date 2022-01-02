package il.co.ilrd.networking;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class UDPClientPing {

	public static void main(String[] args) {

		try(
				DatagramSocket clientSocket = new DatagramSocket();
				)
		{
			InetAddress IPAddress = InetAddress.getByName("localhost");

			byte[] sendData = new byte[1024];
			byte[] receiveData = new byte[1024];

			sendData = "Ping".getBytes();

			while(true) {
				DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, 6666);
				clientSocket.send(sendPacket);

				DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
				clientSocket.receive(receivePacket);
				System.out.println("Server: " + new String( receivePacket.getData()));
			}
		} catch (IOException e) {
			System.err.println(" " + e);
		}
	}

}
