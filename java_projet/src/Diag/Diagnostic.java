package Diag;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;

public class Diagnostic {
	final static int INODE_TABLE_SIZE = 10;
	final static int BLOCK_SIZE = 4;
	final static int INODDE_START = 17;
	private Superblock superblock;
	private InodeTable inodeTable;
	private UserTable userTable;
	
	public Diagnostic() {
		super();
	}
	
	/*
	 * On verfie si pour chaque inode, sont first byte + nblock * 4 = au prochain inode
	 */
	private boolean analyseInodeTable() {
		boolean verify = true;
		System.out.println("\n-| Diagnostic inode table |-");
		for(int i = 0; i < inodeTable.getSize() - 1; i++) {
			
			if(inodeTable.tableInode[i].getSize() != 0 && inodeTable.tableInode[i+1].getSize() != 0) {
				System.out.print("INODE["+i+"] = ");
				int first_byte = inodeTable.tableInode[i].getFirst_byte();
				int nblock_inode = inodeTable.tableInode[i].getNblock() * 4;
				if(nblock_inode + first_byte == inodeTable.tableInode[i+1].getFirst_byte()) {
					System.out.println("[OK]");
				}
				else {
					System.out.println("[ERROR]");
					verify = false;
				}
			}
			else {
				System.out.println("INODE["+i+"] IGNORED BCS LAST OR NULL");
			}
			
		}
		return verify;
	}
	
	/*
	 * On verifie les infos : on verifie le nb d'inode lu par le prog java, pareil pour nb users nblock et first byte, 
	 * et on le compare a ceux marqué dans le superblock
	 */
	private boolean analyseSuperblock() {
		System.out.println("-| Diagnostic superblock |-");
		int nbInode = 0;
		
		//verif nb inode
		for(int i = 0; i < inodeTable.getSize();i++) {
			if(inodeTable.tableInode[i].getSize()>0) {
				nbInode++;
			}
		}
		if(nbInode != superblock.getNumber_of_files()) {
			System.out.println("[ERROR] Number of files incorrect");
			return false;
		}
		System.out.println("Number of files [OK]");
		
		
		//verif byte
		int z=0;
		while(inodeTable.tableInode[z].getSize() > 0) {
			z++;
		}
		z--;
		if(compute_nblock(inodeTable.tableInode[z].getFirst_byte()+inodeTable.tableInode[z].getSize())*BLOCK_SIZE != superblock.getFirst_free_byte()) {
			System.out.println("[ERROR] First free byte incorrect");
			int n = inodeTable.tableInode[z].getFirst_byte() + inodeTable.tableInode[z].getNblock()*4;
			System.out.println("Lu = "+superblock.getFirst_free_byte() 
			+ " alors que ca doit etre "+n);
			return false;
		}
		System.out.println("First free byte [OK]");
		
		//users
		int users_verif = 0;
		for(int i = 0; i < userTable.getNbUsers(); i++) {
			if(userTable.getLogin(i).charAt(0) != '\0') {
				users_verif++;
			}
		}
		if(users_verif != superblock.getNumber_of_users()) {
			System.out.println("[ERROR] Number of users incorrect");
			return false;
		}
		System.out.println("Number users [OK]");
		
		//Nbloc ? 
		return true;
	}
	

	/*
	 * Appel les fonctions ci dessus : va analyser les infos et retourne un string recapitulatif global pour l'afficher dans l'interface graphique
	 */
	public String run(String sPath) throws IOException {
		Path path = Paths.get(sPath);
		String err = "";
		
		//Init superblock
		byte[] data = null;
		try {
	    	data = Files.readAllBytes(path);
		}
		catch (IOException exception) {
		    System.out.println(
		    "[ERROR] Read bytes from file impossible");
		  }
		superblock = new Superblock(data);
		superblock.readSuperblock();
		
		//Init inodeTable
		inodeTable = new InodeTable(data,10);
		inodeTable.readInodeTable();
		
		
		//Init userTable
		userTable = new UserTable(data,5);
		userTable.readUserTable();
		
		if(analyseSuperblock()) {
			err = err + "SUPERBLOCK [OK] \n";
		}
		else {
			err = err + "SUPERBLOCK [ERROR] \n";
		}
		if(analyseInodeTable()) {
			err = err + "INODETABLE [OK] \n";
		}
		else {
			err = err + "INODETABLE [ERROR] \n";
		}
		
		err = err + userTable.toString();
		System.out.println("\n"+superblock.toString());
		//String info_sb = superblock.toString();
		//String info_inode = 
		inodeTable.printInodeTable();
		
		//err = err + info_sb + info_inode;
		return err;
	}
	
	private int compute_nblock(int n) {
		int a=0;
		if (n%BLOCK_SIZE!=0) a++;
		return n/BLOCK_SIZE+a;
	}
	
	private byte[] deplacer(int i, int j, byte[] data) {
		  //deplacer j vers i
		  data[i]=data[j];
		  //supprimer ancien j
		  data[j]=0;
		  return data;
	}
	

    static final public byte[] intToBytes(int integer) {
        byte[] bytes = new byte[4];
        for(int i = 0; i < 4; i ++) {
            bytes[i] = (byte) (integer % 256);
            integer = integer >> 8;
        }
        return bytes;
    }
	
	public void runDefrag(String sPath) throws IOException {
		int new_first_byte,old_first_byte,fin;
		Path path = Paths.get(sPath);
		
		
		byte[] data = null;
		try {
	    	data = Files.readAllBytes(path);
		}
		catch (IOException exception) {
		    System.out.println(
		    "[ERROR] Read bytes from file impossible");
		  }
		//Init superblock
		superblock = new Superblock(data);
		superblock.readSuperblock();
		
		//Init inodeTable
		inodeTable = new InodeTable(data,10);
		inodeTable.readInodeTable();
		
		//defrag
		for (int k=1;k<INODE_TABLE_SIZE;k++) {
			if (inodeTable.tableInode[k].getFirst_byte()-BLOCK_SIZE >=inodeTable.tableInode[k-1].getFirst_byte()+inodeTable.tableInode[k-1].getSize()) {
				
				new_first_byte = compute_nblock(inodeTable.tableInode[k-1].getFirst_byte()+inodeTable.tableInode[k-1].getSize())*BLOCK_SIZE;
				old_first_byte = inodeTable.tableInode[k].getFirst_byte();
				fin = inodeTable.tableInode[k].getFirst_byte() + inodeTable.tableInode[k].getSize();
				while (old_first_byte<fin) {
					data = deplacer(new_first_byte,old_first_byte,data);
					new_first_byte++; old_first_byte++;
				}
				inodeTable.tableInode[k].setFirst_byte(compute_nblock(inodeTable.tableInode[k-1].getFirst_byte()+inodeTable.tableInode[k-1].getSize())*BLOCK_SIZE);
				byte[] bytes_first_byte = intToBytes(compute_nblock(inodeTable.tableInode[k-1].getFirst_byte()+inodeTable.tableInode[k-1].getSize())*BLOCK_SIZE);
				
				int indice_inode =  (k * 108 + 17) + 104;
				for(int z=0; z < 4;z++) {
					data[indice_inode + z] = bytes_first_byte[z];				
				}
			}
		}
		
		//MAJ superblock
		int i=0;
		while(inodeTable.tableInode[i].getSize() != 0 && i < INODE_TABLE_SIZE) {
			i++;
		}
		i--;
		int nffb = compute_nblock(inodeTable.tableInode[i].getFirst_byte()+inodeTable.tableInode[i].getSize())*BLOCK_SIZE;
		byte[] new_first_free_byte = intToBytes(nffb);
		//On ecrit la nouvelle valeur dans le tableau data
		int start_ffb_sb = 12; 
		for(int z=0; z < 4;z++) {
			data[start_ffb_sb + z] = new_first_free_byte[z];
		}
		System.out.println("\n-| DEFRAG DONE |-\n");
		Files.write(path,data,StandardOpenOption.WRITE);
	}
	
	
}
