package il.co.ilrd.networking;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.*;

public class DownloadPage {

	public static void main(String[] args) {
		URL url;
	    InputStream is = null;
	    BufferedReader br;
	    String line;

	    try {
	        url = new URL("https://www.wikipedia.org/");
	        is = url.openStream();  // throws an IOException
	        br = new BufferedReader(new InputStreamReader(is));

	        while (null != (line = br.readLine())) {
	            System.out.println(line);
	        }
	    } catch (MalformedURLException mue) {
	         mue.printStackTrace();
	    } catch (IOException ioe) {
	         ioe.printStackTrace();
	    } finally {
	        try {
	            if (null != is) {
	            	is.close();
	            }
	        } catch (IOException ioe) {}
	    }
	}
}
