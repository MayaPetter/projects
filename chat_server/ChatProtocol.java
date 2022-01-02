package il.co.ilrd.chat_server;

public class ChatProtocol {
	private final static String DELIMITER = "@-#";
	private final static String INVALID_ID = "Invalid";
	
	public static String newRegistration(String ID) {
		return createMsg("Register", ID, null);
	}

	public static String unregister(String ID) {
		return createMsg("Unregister", ID, null);
	}

	public static String newMsg(String ID, String data) {
		return createMsg("message", ID, data);
	}
	
	public static String newInvalidMsg(String ID, String data) {
		return createMsg(INVALID_ID, ID, data);
	}
	
	private static String createMsg(String msgKey, String ID, String data) {
		return new String(msgKey + DELIMITER + ID + DELIMITER + data + DELIMITER );
	}
	
	public static MsgFormat decodeMsg(String toDecode) {
		String[] parsed = toDecode.split(DELIMITER);
		return new MsgFormat(parsed[0], parsed[1], parsed[2]);
	}

	protected static class MsgFormat{

		private final String msgKey;
		private final String ID;
		private String data;
		
		private MsgFormat(String msgKey, String ID, String data) {
			this.msgKey = msgKey;
			this.ID = ID;
			this.data = data;
		}
		
		public boolean isMsg() {
			return this.msgKey.equalsIgnoreCase("message");
		}
		
		public boolean isRegister() {
			return this.msgKey.equalsIgnoreCase("Register");
		}
		
		public boolean isUnregister() {
			return this.msgKey.equalsIgnoreCase("Unregister");
		}
		
		public boolean isInvalid() {
			return this.msgKey.equalsIgnoreCase("Invalid");
		}
		
		public String getInvalidID() {
			return INVALID_ID;
		}
		
		public String getID() {
			return ID;
		}

		public String getData() {
			return data;
		}

		public void setData(String msg) {
			this.data = msg;
		}
	}
}