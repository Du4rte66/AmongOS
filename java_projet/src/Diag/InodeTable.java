package Diag;


public class InodeTable extends Utils {
	Inode[] tableInode;
	private byte[] data;
	private int size;
	
	public InodeTable(byte[] data,int size){
		super(data);
		this.data = data;
		this.size = size;
		tableInode = new Inode[size];
	}
	
	public void readInodeTable() {
		for(int i = 0; i<size;i++) {
			Inode in = new Inode(data);
			in.readInode(i);
			tableInode[i] = in;
		}
	}
	
	public String printInodeTable() {
		String texte = "";
		for(int i = 0; i<size; i++) {
			System.out.println("Inode["+ i + "]= "+tableInode[i].toString());
			texte = texte + tableInode[i].toString() + "\n";
		}
		
		return texte;
	}

	public Inode[] getTableInode() {
		return tableInode;
	}

	public int getSize() {
		return size;
	}
	
	
}
