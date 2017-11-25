package lab4;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

public class Main {
	
	static Frame[] frameTable;
	static ArrayList<Process> processes;
	static int machineSize;
	static int pageSize;
	static int processSize;
	static int jobMix;
	static int A;
	static int B;
	static int C;
	static int numProcesses;
	static int referencesPerProcess;

	public static void main(String[] args) throws NumberFormatException, IOException {

		File randomFile = new File("src/lab4/random-numbers");
		System.out.println(randomFile.getCanonicalPath());
		FileReader r = new FileReader(randomFile);
		BufferedReader randomReader = new BufferedReader(r);
		
		if(randomFile.exists() == false){
			System.err.println("Random number file not found");
			System.exit(1);
		}
		
		machineSize = Integer.parseInt(args[0]);
		pageSize = Integer.parseInt(args[1]);
		processSize =  Integer.parseInt(args[2]);
		
		frameTable = new Frame[machineSize / pageSize];
		
		for(int i = 0; i < frameTable.length; i++){
			frameTable[i] = new Frame(pageSize);
		}
		
		jobMix = Integer.parseInt(args[3]);
		
		if(jobMix == 1){
			processes = new ArrayList<Process>();
			A = 1;
			numProcesses = 1;
		}
		//else 2, 3, 4
		
		referencesPerProcess = Integer.parseInt(args[4]);
		
		for(int i = 1; i <= numProcesses; i++){
			Process p = new Process(referencesPerProcess, ((111 * i) + processSize) % processSize);
			
			p.currentReference = p.startingReference();
			processes.add(p);
		}
		
		
		float randomNum;	
		int quantum = 3;
		Process currentRunning = processes.get(0);
		int currentRunningIndex = 0;
		
		while(processes.isEmpty() == false){
			
			if(quantum >= 3){
				currentRunning = processes.get((currentRunningIndex + 1) % processes.size());
				currentRunningIndex++;
				quantum = 0;
			} else {
				quantum++;
			}
			
			if(currentRunning.currentFrame == null || (currentRunning.currentReference >= currentRunning.currentFrame.topReference + currentRunning.currentFrame.size && currentRunning.currentReference < currentRunning.currentFrame.topReference)){
				//page fault
				currentRunning.currentFrame = frameTable[0];
			} else {
				System.out.println(currentRunning.currentReference);
			}
				
			currentRunning.referencesRemaining--;
			
			if(currentRunning.referencesRemaining <= 0){
				processes.remove(currentRunning);
				continue;
			}
			
			randomNum = Float.parseFloat(randomReader.readLine()) / (Integer.MAX_VALUE);
			
			if(randomNum < A){
				currentRunning.currentReference = (currentRunning.currentReference + 1) % processSize;
			} else if(randomNum < A + B){
				currentRunning.currentReference = (currentRunning.currentReference - 5) % processSize;
			} else if(randomNum < A + B + C){
				currentRunning.currentReference = (currentRunning.currentReference + 4) % processSize;
			} else {
				currentRunning.currentReference = Integer.parseInt(randomReader.readLine()) % processSize;
			}
		}
	}
	
	static boolean CheckProcesses(){
		for(Process p : processes){
			if(p.referencesRemaining > 0){
				return true;
			}
		}
		
		return false;
	}
	
	///////	
}

class Frame {
	int size;
	public int topReference;
	
	public Frame(int size) {
		this.size = size;
	}

}

class Process {
	public int referencesRemaining;
	public int currentReference;
	public Frame currentFrame;
	
	int startingReference;
	
	public Process(int referencesPerProcess, int startingReference){
		this.referencesRemaining = referencesPerProcess;
		this.startingReference = startingReference;
	}
	
	public int startingReference(){
		return this.startingReference;
	}
}


