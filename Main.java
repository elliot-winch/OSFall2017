package lab4;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Random;

public class Main {
	
	static Frame[] frameTable;
	static Process[] processes;
	static BufferedReader randomReader;

	public static void main(String[] args) throws NumberFormatException, IOException {

		File randomFile = new File("src/lab4/random-numbers");
		randomReader = new BufferedReader(new FileReader(randomFile));
		
		if(randomFile.exists() == false){
			System.err.println("Random number file not found");
			System.exit(1);
		}
		
		int machineSize = Integer.parseInt(args[0]);
		int pageSize = Integer.parseInt(args[1]);
		int processSize =  Integer.parseInt(args[2]);
		
		frameTable = new Frame[machineSize / pageSize];
				
		int jobMix = Integer.parseInt(args[3]);
		int referencesPerProcess = Integer.parseInt(args[4]);
		
		if(jobMix == 1){
			processes = new Process[] {
					new Process(referencesPerProcess, (111 + processSize) % processSize, 1, 0, 0)};
		} else if(jobMix == 2) {
			processes = new Process[4];
			for(int i = 1; i <= 4; i++) {
				processes[i - 1] = new Process(referencesPerProcess, ((111 * i) + processSize) % processSize, 1, 0, 0);
			}
		} else if(jobMix == 3) {
			for(int i = 1; i <= 4; i++) {
				processes[i - 1] = new Process(referencesPerProcess, ((111 * i) + processSize) % processSize, 0, 0, 0);
			}
		} else if(jobMix == 4) {
			processes = new Process[] {
				(new Process(referencesPerProcess, ((111 * 1) + processSize) % processSize, 0.75f, 0.25f, 0)),
				(new Process(referencesPerProcess, ((111 * 2) + processSize) % processSize, 0.75f, 0, 0.25f)),
				(new Process(referencesPerProcess, ((111 * 3) + processSize) % processSize, 0.75f, 0.125f, 0.125f)),
				(new Process(referencesPerProcess, ((111 * 4) + processSize) % processSize, 0.5f, 0.125f, 0.125f)),
			};
		} else {
			System.err.println("JobMix must be an integer 1, 2, 3 or 4");
			System.exit(1);
		}
		
		Function<Frame[], Process, Integer, Integer> algorithm = null;
		String PRAstring = args[5];
		
		if(PRAstring.equalsIgnoreCase("random")) {
			algorithm = (Frame[] frameTable, Process p, Integer size, Integer time) -> { PRA.Random(frameTable, p, size, time);
			return null; };
		} else if (PRAstring.equalsIgnoreCase("lifo")) {
			algorithm = (Frame[] frameTable, Process p, Integer size, Integer time) -> { PRA.LIFO(frameTable, p, size, time);
			return null; };
		} else if (PRAstring.equalsIgnoreCase("lru")) {
			algorithm = (Frame[] frameTable, Process p, Integer size, Integer time) -> { PRA.LRU(frameTable, p, size, time);
			return null; };
		} else {
			System.err.println("PRA must be a string 'random', 'lifo' or 'lru'");
			System.exit(1);
		}
		
		int timer = 1;
		float randomNum;
		int currentRunningIndex = -1;
		Process currentRunning = null;
		
		while(CheckProcesses()){
				
			if(timer % 3 == 1 || currentRunning.referencesRemaining <= 0){
				do {
					currentRunning = processes[(++currentRunningIndex + processes.length) % processes.length];
				
				} while (currentRunning.referencesRemaining <= 0);
			} 
			
			if(referenceInFrameTable(currentRunning, timer) == false){
				System.out.println(String.format("Page fault: process %d is looking for %d", (currentRunningIndex + 1) % processes.length, currentRunning.currentReference ));
				currentRunning.numFaults++;
				algorithm.apply(frameTable, currentRunning, pageSize, timer);
			} 
				
			System.out.println(String.format("Process %d at time %d is referencing %d", (currentRunningIndex + 1) % processes.length,  timer, currentRunning.currentReference));
				
			currentRunning.referencesRemaining--;
			
			timer++;
			
			randomNum = Float.parseFloat(randomReader.readLine()) / (Integer.MAX_VALUE);
			
			if(randomNum < currentRunning.A()){
				currentRunning.currentReference = (currentRunning.currentReference + 1 + processSize) % processSize;
			} else if(randomNum < currentRunning.A() + currentRunning.B()){
				currentRunning.currentReference = (currentRunning.currentReference - 5 + processSize) % processSize;
			} else if(randomNum < currentRunning.A() + currentRunning.B() + currentRunning.C()){
				currentRunning.currentReference = (currentRunning.currentReference + 4 + processSize) % processSize;
			} else {
				currentRunning.currentReference = Integer.parseInt(randomReader.readLine()) % processSize;
			}
			
		}
		
		int totalFaults = 0;
		float overallResidency = 0;
		for(int i = 0; i < processes.length; i++) {
			int pagesStillInTable = 0;
			for(Frame f : frameTable) {
				if(f!= null && f.process() == processes[i]) {
					pagesStillInTable++;
				}
			}
			
			System.out.print(String.format("Process %d had %d faults", i + 1, processes[i].numFaults));
			
			totalFaults += processes[i].numFaults;
			
			if(processes[i].numFaults - pagesStillInTable > 0) {
				System.out.println(String.format(" with an average residency of %f.", processes[i].averageResidency / (float)(processes[i].numFaults - pagesStillInTable)));
				overallResidency += processes[i].averageResidency / (float)(processes[i].numFaults - pagesStillInTable);
			} else {
				System.out.println(String.format(". None of its pages were evicted, so its average residency is undefined."));
				
			}
		}
		
		System.out.print(String.format("\nThe total number of faults is %d", totalFaults));
		
		if(overallResidency > 0) {
			System.out.println(String.format(" and the overall average residency is %f.", overallResidency / processes.length));
		} else {
			System.out.println(String.format(". No pages were evicted so the overall average residency is undefined."));
			
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
	
	static boolean referenceInFrameTable(Process p, int time) {
		for(Frame f : frameTable) {			
			 if(f != null && f.process() == p && p.currentReference < f.topReference() + f.size() && p.currentReference >= f.topReference()){
				f.lastUsed = time;
				return true;
			 }
		}
		
		return false;
	}
}

class Frame {
	public int lastUsed;
	
	private int size;
	private int topReference;
	private Process process;
	private int birthTime;
	
	public Frame(int size, int topReference, Process p, int time) {
		this.size = size;
		this.topReference = topReference;
		this.process = p;
		this.birthTime = time;
		this.lastUsed = time;
	}
	
	public int size() {
		return this.size;
	}
	
	public int topReference() {
		return this.topReference;
	}
	
	public Process process() {
		return this.process;
	}

	public int birthTime() {
		return this.birthTime;
	}
}

class Process {
	public int referencesRemaining;
	public int currentReference;
	public Frame currentFrame;
	public int numFaults;
	public int averageResidency;
	
	int startingReference;
	float A;
	float B;
	float C;
	
	public Process(int referencesPerProcess, int startingReference, float A, float B, float C){
		this.referencesRemaining = referencesPerProcess;
		this.startingReference = startingReference;
		this.currentReference = this.startingReference;
		this.A = A;
		this.B = B;
		this.C = C;
		this.numFaults = 0;
		this.averageResidency = 0;
	}
	
	public int startingReference(){
		return this.startingReference;
	}
	
	public float A() {
		return this.A;
	}
	
	public float B() {
		return this.B;
	}
	
	public float C() {
		return this.C;
	}
}

class PRA {
	static void Random(Frame[] frameTable, Process p, int size, int time) {
		for(int i = frameTable.length - 1; i >= 0; i--) {
			if(frameTable[i] == null) {
				frameTable[i] = new Frame(size, p.currentReference - p.currentReference % size, p, time);
				System.out.println(p.currentReference - p.currentReference % size);

				return;
			}
		}
		
		try {
			int randomIndex = Integer.parseInt(Main.randomReader.readLine()) % frameTable.length;
			frameTable[randomIndex].process().averageResidency += time - frameTable[randomIndex].birthTime();
			frameTable[randomIndex] = new Frame(size, p.currentReference - p.currentReference % size, p, time);
			System.out.println(p.currentReference - p.currentReference % size);

		} catch (Exception e) {
			System.err.println(e.getMessage());
			System.exit(1);
		}
	}
	
	static void LIFO(Frame[] frameTable, Process p, int size, int time) {
		int max = 0;
		int maxIndex = -1;
		
		for(int i = frameTable.length - 1; i >= 0; i--) {
			if(frameTable[i] == null) {
				frameTable[i] = new Frame(size, p.currentReference - p.currentReference % size, p, time);
				return;
			} else {
				if(frameTable[i].birthTime() >= max) {
					max = frameTable[i].birthTime();
					maxIndex = i;
				}
			}
		}
		
		System.out.println("Evicting frame: " + maxIndex);
		frameTable[maxIndex].process().averageResidency += time - frameTable[maxIndex].birthTime();
		frameTable[maxIndex] = new Frame(size, p.currentReference - p.currentReference % 10, p, time);
	}
	
	static void LRU(Frame[] frameTable, Process p, int size, int time) {
		int min = Integer.MAX_VALUE;
		int minIndex = -1;
		
		for(int i = frameTable.length - 1; i >= 0; i--) {
			if(frameTable[i] == null) {
				frameTable[i] = new Frame(size, p.currentReference - p.currentReference % 10, p, time);
				return;
			} else {
				if(frameTable[i].lastUsed < min) {
					minIndex = i;
					min = frameTable[i].lastUsed;
				}
			}
		}
		System.out.println("Evicting frame: " + minIndex + " with " + frameTable[minIndex].lastUsed);

		frameTable[minIndex].process().averageResidency += time - frameTable[minIndex].birthTime();
		frameTable[minIndex] = new Frame(size, p.currentReference - p.currentReference % 10, p, time);
	}
}

@FunctionalInterface
interface Function<T, R, S, U> {
	public R apply(T t, R r, S s, U u);
}
