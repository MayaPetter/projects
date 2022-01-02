package il.co.ilrd.networking;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;

import java.io.InputStreamReader;

public class TCPPingClient {

	public static void main(String[] args) {
		try(
				Socket socket = new Socket("localhost", 6666);
				DataOutputStream out = new DataOutputStream(socket.getOutputStream());
				BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
				)
		{
			String msg = "Ping\n";
			String fromServer;

			while (true) {
				out.writeBytes(msg + System.lineSeparator());
				fromServer = in.readLine();
				System.out.println("Server: " + fromServer);
			}
		} catch (IOException e) {
			e.printStackTrace();
		}

	}
}
