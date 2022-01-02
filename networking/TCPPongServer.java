package il.co.ilrd.networking;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;

public class TCPPongServer {

	public static void main(String[] args) {

		try (
				ServerSocket serverSocket = new ServerSocket(6666);
				Socket clientSocket = serverSocket.accept();
				DataOutputStream out = new DataOutputStream(clientSocket.getOutputStream());
				BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
				)
		{
			String serverMsg = "Pong\n";
			String inputMsg;

			while(true) {
				inputMsg = in.readLine();
				System.out.println("Client: " + inputMsg);
				out.writeBytes(serverMsg);
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
