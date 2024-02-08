package Diag;

import java.io.IOException;
import java.nio.ByteBuffer;

public class Superblock extends Utils {
	
	  private int number_of_files; // dans super bloc
	  private int number_of_users; // idem
	  private int nb_blocks_used ; // ????????
	  private int first_free_byte; // premier octet libre

	  
	public Superblock(byte[] data) {
		super(data);
	}
	
	
	
	@Override
	public String toString() {
		return "Superblock [number_of_files=" + number_of_files + ", number_of_users=" + number_of_users
				+ ", nb_blocks_used=" + nb_blocks_used + ", first_free_byte=" + first_free_byte + "]";
	}

	public void printSuperBlock() {
		System.out.println(toString());
	}

	public void readSuperblock() throws IOException {
		
		number_of_files = ByteBuffer.wrap(reverse(readData(0,4))).getInt();
		number_of_users = ByteBuffer.wrap(reverse(readData(4,4))).getInt();	
		nb_blocks_used = ByteBuffer.wrap(reverse(readData(8,4))).getInt();		
		first_free_byte = ByteBuffer.wrap(reverse(readData(12,4))).getInt();

    	
	}



	public int getNumber_of_files() {
		return number_of_files;
	}



	public int getNumber_of_users() {
		return number_of_users;
	}



	public int getNb_blocks_used() {
		return nb_blocks_used;
	}



	public int getFirst_free_byte() {
		return first_free_byte;
	}



	public void setFirst_free_byte(int first_free_byte) {
		this.first_free_byte = first_free_byte;
	}
	 
	
	  
		  
}
