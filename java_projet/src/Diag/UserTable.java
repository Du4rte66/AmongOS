package Diag;


import java.io.IOException;
import java.util.Arrays;


public class UserTable extends Utils {

	private String[] login;
	private String[] mdp;
	private int nbUsers;
	
	private byte[] data;
	
	public UserTable(byte[] data,int nbUsers) {
		super(data);
		this.data = data;
		this.nbUsers = nbUsers;
		login = new String[nbUsers];
		mdp = new String[nbUsers];
		
	}
	
	
	public void readUserTable() throws IOException {
    	int val = 1097;
    	for(int j=0; j < nbUsers; j++) {
    		
        	//On met le login dans le tab des login
        	login[j] = new String(readData(val,32), java.nio.charset.StandardCharsets.UTF_8);

        	//System.out.println(login[j]);
        	
        	//Lecture du mdp
    		val = val + 32;
        	mdp[j] = new String(readData(val,65), java.nio.charset.StandardCharsets.UTF_8);
        	val= val + 65;
    	}

	}
	
	@Override
	public String toString() {
		return "LISTE USERS : " + Arrays.toString(login);
	}
	
	public void printUserTable() {
		System.out.println(toString());
	}
	
	/*
	 * Marche pas car des espaces manquant
	 */
	public boolean verifyUser(String name) {
		/*int taille = name.length();
		taille = 32 - taille;
		for(int j = 0; j < taille;j++) {
			name = name + " ";
		}*/
		System.out.println(name);
		for(int i = 0; i < nbUsers;i++) {
			System.out.println(login[i]);
			if(login[i].equals(name)){
				System.out.println("[FOUND]");
				return true;
			}
		}
		return false;
	}


	public String getLogin(int i) {
		return login[i];
	}


	public String getMdp(int i) {
		return mdp[i];
	}


	public int getNbUsers() {
		return nbUsers;
	}
	
	
	
	
}
