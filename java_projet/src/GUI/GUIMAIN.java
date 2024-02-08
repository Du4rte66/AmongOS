package GUI;

import Diag.*;

import java.awt.BorderLayout;
import java.awt.EventQueue;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;
import javax.swing.JFileChooser;
import javax.swing.JButton;
import javax.swing.JSplitPane;


import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Clip;
import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.UnsupportedAudioFileException;
import javax.swing.ImageIcon;


import java.io.File;
import java.io.IOException;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import javax.swing.JLabel;


public class GUIMAIN extends JFrame {

	private JPanel contentPane;
	private Diagnostic diag = new Diagnostic();
	private File f;
	private ImageIcon img = new ImageIcon("image/logo.png");

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					
					GUIMAIN frame = new GUIMAIN();
					frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}


	/**
	 * 
	 * @param soundFile (path of the file)
	 * @throws LineUnavailableException
	 * @throws IOException
	 * @throws UnsupportedAudioFileException
	 */
	private void playSound(String soundFile) throws LineUnavailableException, IOException, UnsupportedAudioFileException {
	    File f = new File("./" + soundFile);
	    AudioInputStream audioIn = AudioSystem.getAudioInputStream(f.toURI().toURL());  
	    Clip clip = AudioSystem.getClip();
	    clip.open(audioIn);
	    clip.start();
	}
	
	/**
	 * Create the frame.
	 * @throws LineUnavailableException 
	 * @throws IOException 
	 * @throws UnsupportedAudioFileException 
	 */
	public GUIMAIN() throws LineUnavailableException, IOException, UnsupportedAudioFileException {
		
		//FRAME PRINCIPALE
		setTitle("AmongOS tool management");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setBounds(100, 100, 795, 533);
		contentPane = new JPanel();
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		contentPane.setLayout(new BorderLayout(0, 0));
		setContentPane(contentPane);
		setIconImage(img.getImage());
		
		
		//COMPOSANTS
		JSplitPane splitPane = new JSplitPane();
		splitPane.setContinuousLayout(true);
		splitPane.setOneTouchExpandable(true);
		splitPane.setResizeWeight(0.5);
		splitPane.setOrientation(JSplitPane.VERTICAL_SPLIT);
		contentPane.add(splitPane, BorderLayout.CENTER);
		
		JPanel button_tool_management = new JPanel();
		splitPane.setRightComponent(button_tool_management);
		JLabel Retour = new JLabel("");
		
		JButton buttonAnalyse = new JButton("Analyse");

		
		JPanel panel_text = new JPanel();
		button_tool_management.add(panel_text);
		

		panel_text.add(Retour);
		buttonAnalyse.setEnabled(false);
		button_tool_management.add(buttonAnalyse);
		
		JButton buttonDefrag = new JButton("Defragmentation");
		buttonDefrag.setEnabled(false);
		
		button_tool_management.add(buttonDefrag);
		
		JPanel selection_disk = new JPanel();
		splitPane.setLeftComponent(selection_disk);
		
		JSplitPane splitPane_1 = new JSplitPane();
		splitPane_1.setOrientation(JSplitPane.VERTICAL_SPLIT);
		splitPane_1.setContinuousLayout(true);
		selection_disk.add(splitPane_1);
		
		JFileChooser fileChooser = new JFileChooser();
		splitPane_1.setRightComponent(fileChooser);
		JLabel path_disk = new JLabel("WAITING THE FILE DISK \"d0\"");
		splitPane_1.setLeftComponent(path_disk);
		
		/*
		 * Action de l'interface : slection du fichier, analyse et defragmentation
		 */
		
		//SELECTION FICHIER
		fileChooser.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				f = fileChooser.getSelectedFile().getAbsoluteFile();
				if(f.isFile() && f.getName().equals("d0")) {
					System.out.println(f.getPath());
					buttonDefrag.setEnabled(true);
					buttonAnalyse.setEnabled(true);
					path_disk.setText("FILE SELECTED : "+ f.getPath());
				}
				else {
					path_disk.setText("WRONG FILE");
				}
			}
		});
		
		//ANALAYSE
		buttonAnalyse.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					String texte = diag.run(f.getPath());
					Retour.setText(texte);
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		});
		
		//DEFRAGMENTATION
		buttonDefrag.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					diag.runDefrag(f.getPath());
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				Retour.setText("DEGFRAGMENTATION [DONE]");
			}
		});
		
		//Sound opening app
		playSound("sound/amongOS_launch.wav");
		

	}

}
