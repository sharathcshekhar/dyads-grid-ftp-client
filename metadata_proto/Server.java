import java.io.BufferedInputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;


public class Server {

	private static ServerSocket serverSocket;
	private static final int PORT = 4444;

	void callServer(){
		System.out.println("started Server");

		try{
			serverSocket = new ServerSocket(PORT);

			do{
				Socket client = serverSocket.accept();
				System.out.println("New connection accepted");
				ClientHandler handler = new ClientHandler(client);
				handler.start();
			}while(true);

		}
		catch(Exception ex){
			ex.printStackTrace();
		}
	}
}

class ClientHandler extends Thread{
	private Socket client;
	DataInputStream input;
	DataOutputStream output;

	public ClientHandler(Socket socket){
		client = socket;

		try{
			input = new DataInputStream(new BufferedInputStream(client.getInputStream()));

			output = new DataOutputStream(client.getOutputStream());

		}
		catch(Exception ex){
			ex.printStackTrace();
		}
	}

	public void run(){

		//byte[] messageHeadIdentifier_byte = new byte[];
		//input.read(messageIdentifierHeader_byte, 0, 4);

		try{
			String message;
			String response = "Message Received";
			message = input.readLine();
			System.out.println(message);
			
			DummyObject sampleData = new DummyObject();
			populateDummy(sampleData);
			byte[] sample_byte = convertToBytes(sampleData);
			
			output.write(sample_byte);
			client.close();
		}
		catch(Exception ex){
			ex.printStackTrace();
		}
	}
	
	private byte[] convertToBytes(DummyObject sampleData) {
		ByteBuffer byteBuffer = ByteBuffer.allocate(120);
		byteBuffer.order(ByteOrder.BIG_ENDIAN);
		byteBuffer.putLong(sampleData.getSt_dev());
		byteBuffer.putLong(sampleData.getSt_ino());
		byteBuffer.putLong(sampleData.getSt_nlink());
		byteBuffer.putInt(sampleData.getSt_mode());
		byteBuffer.putInt(sampleData.getSt_uid());
		byteBuffer.putInt(sampleData.getPad());
		byteBuffer.putInt(sampleData.getSt_gid());
		byteBuffer.putLong(sampleData.getSt_rdev());
		byteBuffer.putLong(sampleData.getSt_size());
		byteBuffer.putLong(sampleData.getSt_blksize());
		byteBuffer.putLong(sampleData.getSt_blocks());
		byteBuffer.putLong(sampleData.getSt_atime_sec());
		byteBuffer.putLong(sampleData.getSt_atime_usec());
		byteBuffer.putLong(sampleData.getSt_mtime_sec());
		byteBuffer.putLong(sampleData.getSt_mtime_usec());
		byteBuffer.putLong(sampleData.getSt_ctime_sec());
		byteBuffer.putLong(sampleData.getSt_ctime_usec());
		
		byte[] byteArray = byteBuffer.array();
		
		return byteArray;
	}

	void populateDummy(DummyObject dummy){
		dummy.setSt_dev(12345);
		dummy.setSt_ino(1527323);
		dummy.setSt_mode(755);
		dummy.setSt_nlink(2);
		dummy.setSt_uid(45253);
		dummy.setSt_gid(62764);
		dummy.setPad(0);
		dummy.setSt_rdev(0);
		dummy.setSt_size(4078);
		dummy.setSt_blksize(32);
		dummy.setSt_blocks(5);
		dummy.setSt_atime_sec(2222);
		dummy.setSt_atime_usec(44444);
		dummy.setSt_mtime_sec(11111);
		dummy.setSt_mtime_usec(43333);
		dummy.setSt_ctime_sec(55555);
		dummy.setSt_ctime_usec(66666);
	}
	
	/*int littleEndianToBigEndian_INT(int i)
	{
	    return((i&0xff)<<24)+((i&0xff00)<<8)+((i&0xff0000)>>8)+((i>>24)&0xff);
	}*/
	
	/*int littleEndianToBigEndian_LONG(long j){
		
	}*/

	
}

class DummyObject{
	private long st_dev;
	private long st_ino;
	private int st_mode;
	private long st_nlink;
	private int st_uid;
	private int pad;
	private int st_gid;
	private long st_rdev;
	private long st_size;
	private long st_blksize;
	private long st_blocks;
	private long st_atime_sec;
	private long st_atime_usec;
	private long st_mtime_sec;
	private long st_mtime_usec;
	private long st_ctime_sec;
	private long st_ctime_usec;
	
	public long getSt_dev() {
		return st_dev;
	}
	public void setSt_dev(long st_dev) {
		this.st_dev = st_dev;
	}
	public long getSt_ino() {
		return st_ino;
	}
	public void setSt_ino(long st_ino) {
		this.st_ino = st_ino;
	}
	public int getSt_mode() {
		return st_mode;
	}
	public void setSt_mode(int st_mode) {
		this.st_mode = st_mode;
	}
	public long getSt_nlink() {
		return st_nlink;
	}
	public void setSt_nlink(long st_nlink) {
		this.st_nlink = st_nlink;
	}
	public int getSt_uid() {
		return st_uid;
	}
	public void setSt_uid(int st_uid) {
		this.st_uid = st_uid;
	}
	public int getSt_gid() {
		return st_gid;
	}
	public void setSt_gid(int st_gid) {
		this.st_gid = st_gid;
	}
	public long getSt_rdev() {
		return st_rdev;
	}
	public void setSt_rdev(long st_rdev) {
		this.st_rdev = st_rdev;
	}
	public long getSt_size() {
		return st_size;
	}
	public void setSt_size(long st_size) {
		this.st_size = st_size;
	}
	public long getSt_blksize() {
		return st_blksize;
	}
	public void setSt_blksize(long st_blksize) {
		this.st_blksize = st_blksize;
	}
	public long getSt_blocks() {
		return st_blocks;
	}
	public void setSt_blocks(long st_blocks) {
		this.st_blocks = st_blocks;
	}
	public int getPad() {
		return pad;
	}
	public void setPad(int pad) {
		this.pad = pad;
	}
	public long getSt_atime_sec() {
		return st_atime_sec;
	}
	public void setSt_atime_sec(long st_atime_sec) {
		this.st_atime_sec = st_atime_sec;
	}
	public long getSt_atime_usec() {
		return st_atime_usec;
	}
	public void setSt_atime_usec(long st_atime_usec) {
		this.st_atime_usec = st_atime_usec;
	}
	public long getSt_mtime_sec() {
		return st_mtime_sec;
	}
	public void setSt_mtime_sec(long st_mtime_sec) {
		this.st_mtime_sec = st_mtime_sec;
	}
	public long getSt_mtime_usec() {
		return st_mtime_usec;
	}
	public void setSt_mtime_usec(long st_mtime_usec) {
		this.st_mtime_usec = st_mtime_usec;
	}
	public long getSt_ctime_sec() {
		return st_ctime_sec;
	}
	public void setSt_ctime_sec(long st_ctime_sec) {
		this.st_ctime_sec = st_ctime_sec;
	}
	public long getSt_ctime_usec() {
		return st_ctime_usec;
	}
	public void setSt_ctime_usec(long st_ctime_usec) {
		this.st_ctime_usec = st_ctime_usec;
	}
	
	
}