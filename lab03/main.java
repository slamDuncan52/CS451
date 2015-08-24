// Lab 3 (Program 3 Chapter 5)
// Producer - Consumer Problem
// Mikayla Ray & Mitchell Duncan

#import java.util.Random;

public static void main(String[] args) {
	
	final int BUFFER_SIZE = 5;
	
	// Retrieve command line arguments
	int sleepTime, numOfProducerThreads, numOfConsumerThreads;
	
	if (args[0] == null ||
		args[0] < 0     ||
		args[0] > Integer.MAX_VALUE) {
		sleepTime = 0;
	} else {
		sleepTime = args[0];
	}
	
	if (args[1] == null ||
		args[1] < 0     ||
		args[1] > Integer.MAX_VALUE) {
		numOfProducerThreads = 0;
	} else {
		numOfProducerThreads = args[1];
	}
	
	if (args[2] == null ||
		args[2] < 0     ||
		args[2] > Integer.MAX_VALUE) {
		numOfProducerThreads = 0;
	} else {
		numOfProducerThreads = args[2];
	}
	
	// Initialize the buffer
	public static int[] buffer = new int[BUFFER_SIZE
	int counter = 0;
	
	public synchronize int insert(int value) {
		try {
			buffer[counter] = value;
			counter++;
			return 0;
		} catch (Exception e) {
			return -1;
		}
	}
	
	public synchronize int remove(int value) {
		try {
			buffer[counter] = value;
			counter--;
			return 0;
		} catch (Exception e) {
			return -1;
		}
	}
	
	// Create producer threads
	private static class Producer implements Runnable {
		public void run() {
			
			// Sleep for random amount of time
			Thread.sleep(Random.randInt(0, Integer.MAX_VALUE));
			
			// Attempt insert
			insert(Random.randInt(0, Integer.MAX_VALUE));
		};
	}
	
	while (numOfProducerThreads > 0) {
		Thread t = new Thread(new Producer());
		t.start();
				
		numOfProducerThreads--;
	}
	
	// Create consumer threads
	private static class Consumer implements Runnable {
		public void run() {
			
			// Sleep for random amount of time
			Thread.sleep(Random.randInt(0, Integer.MAX_VALUE));
			
			// Attempt remove
			remove(Random.randInt(0, Integer.MAX_VALUE));
		};
	}
	
	while (numOfConsumerThreads > 0) {
		Thread t = new Thread(new Consumer());
		t.start();
				
		numOfConsumerThreads--;
	}
	
	// Sleep -> Exit
	
}