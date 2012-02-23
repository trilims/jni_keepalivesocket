package common.net.keepalive;

public class LTSocketImpl {
	static{
		System.loadLibrary("keepalive");
	}
	
	public static interface ReadCallback{
		int onReadSuccess(int length, byte[] content);
		int onFailed(int errcode, byte[] errmsg);
	}
	
	public void callback(int notify_id, int param){
		
	}
	
	//public native void setCallback();
	
	public native int init();
	
	public native int connect(String host, int port);
	
	public native int send(byte[] content, int length);
	
	public native int readBlock(byte[] buf, int buf_length);
	
	public native int delete();
	
	public native int setKeepalive(int tcp_keepalive_probes, int tcp_keepalive_time, int tcp_keepalive_intvl);
	

}
