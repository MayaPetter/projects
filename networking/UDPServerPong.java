package il.co.ilrd.networking;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class UDPServerPong {

	public static void main(String[] args) {

		try(
				DatagramSocket serverSocket = new DatagramSocket(6666);
				)
		{
			byte[] receiveData = new byte[1024];
			byte[] sendData = new byte[1024];

			while(true)
			{
				DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
				serverSocket.receive(receivePacket);
				System.out.println("Client: " + new String(receivePacket.getData()));

				InetAddress IPAddress = receivePacket.getAddress();
				int port = receivePacket.getPort();
				sendData = "Pong".getBytes();
				DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, port);
				serverSocket.send(sendPacket);
			}

		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
