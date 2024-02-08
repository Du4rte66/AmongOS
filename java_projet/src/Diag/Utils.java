package Diag;

public class Utils {
	private byte[] data;
	  
	public Utils(byte[] data) {
		super();
		this.data = data;
	}

	protected byte[] reverse(byte[] ret) {
		for(int i=0; i< ret.length/2; i++){
	        byte tmp = ret[i];
	        ret[i] = ret[ret.length-i-1];
	        ret[ret.length-i-1] = tmp;
	    }
		
		return ret;
	}
	
	protected byte[] readData(int start,int nbBytes) {
		byte[] value = new byte[nbBytes];
		int curs=0;
		int i = start;
		while(curs < nbBytes) {
			value[curs] = data[i];
			curs++;
			i++;
		}
		return value;
	}
	
}
