package Diag;

import java.nio.ByteBuffer;


public class Inode extends Utils {
	
	private String file;
	private int size;
	private int uid;
	private int uright;
	private int oright;
	private String ctimestamp;
	private String mtimestamp;
	private int nblock;
	private int first_byte;

	
	public Inode(byte[] data) {
		super(data);

		// TODO Auto-generated constructor stub
	}
	
	public void readInode(int x) {
		
		int next = 108; // + startInode
		int startInode = 17;
		for(int i = 0; i < x;i++) {
			startInode = startInode + next;
		}
    	
		file = new String(readData(startInode,32), java.nio.charset.StandardCharsets.UTF_8);
		startInode = startInode + 32;
		size = ByteBuffer.wrap(reverse(readData(startInode,4))).getInt();
		startInode = startInode + 4;
		uid = ByteBuffer.wrap(reverse(readData(startInode,4))).getInt();
		startInode = startInode + 4;
		uright = ByteBuffer.wrap(reverse(readData(startInode,4))).getInt();
		startInode = startInode + 4;
		oright = ByteBuffer.wrap(reverse(readData(startInode,4))).getInt();
		startInode = startInode + 4;
		ctimestamp = new String(readData(startInode,26), java.nio.charset.StandardCharsets.UTF_8);
		startInode = startInode + 26;
		mtimestamp = new String(readData(startInode,26), java.nio.charset.StandardCharsets.UTF_8);
		startInode = startInode + 26;
		nblock = ByteBuffer.wrap(reverse(readData(startInode,4))).getInt();
		startInode = startInode + 4;
		first_byte = ByteBuffer.wrap(reverse(readData(startInode,4))).getInt();
		startInode = startInode + 4;
		
	}
	
	

	@Override
	public String toString() {
		return "[file=" + file + ", size=" + size + ", uid=" + uid + ", uright=" + uright + ", oright=" + oright
				+ ", ctimestamp=" + ctimestamp + ", mtimestamp=" + mtimestamp + ", nblock=" + nblock + ", first_byte="
				+ first_byte + "]";
	}

	public String getFile() {
		return file;
	}

	public int getSize() {
		return size;
	}

	public int getUid() {
		return uid;
	}

	public int getUright() {
		return uright;
	}

	public int getOright() {
		return oright;
	}

	public String getCtimestamp() {
		return ctimestamp;
	}

	public String getMtimestamp() {
		return mtimestamp;
	}

	public int getNblock() {
		return nblock;
	}

	public int getFirst_byte() {
		return first_byte;
	}

	public void setFirst_byte(int first_byte) {
		this.first_byte = first_byte;
	}

	
	
}
