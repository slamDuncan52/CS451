// Lab 3 (Program 3 Chapter 5)
// Producer - Consumer Problem
// Mikayla Ray & Mitchell Duncan

#import java.util.Random;
public Lab3 {
	public static void main(String[] args) {
		
		System.out.println("\nLab 3 (Program 3, Chapter 5)\nMikayla Ray & Mitchell Duncan\nJAVA\n> " + args + "\n");
		
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
				buffer[counter] = null;
				counter--;
				return 0;
			} catch (Exception e) {
				return -1;
			}
		}
		
		// Create producer threads
		while (numOfProducerThreads > 0) {
			Thread t = new Thread(new Producer());
			t.start();
					
			numOfProducerThreads--;
		}
		
		// Create consumer threads
		while (numOfConsumerThreads > 0) {
			Thread t = new Thread(new Consumer());
			t.start();
					
			numOfConsumerThreads--;
		}
		
		// "Sleep" -> Exit
		while(sleepTime > 0) {
			sleepTime--;
		}
		
		if (sleepTime == 0) {
			exit(0);
		}
	}
	
	private static class Producer implements Runnable {
		public void run() {
			
			// Sleep for random amount of time
			Thread.sleep(Random.randInt(0, Integer.MAX_VALUE));
			
			// Attempt insert
			if (insert(Random.randInt(0, Integer.MAX_VALUE)) == 0) {
				// Success
				System.out.println("Producer Thread Id: " + Thread.currentThread().getId() + " Success!");
			} else {
				// Failure
				System.out.println("Producer Thread Id: " + Thread.currentThread().getId() + " Failed!");
			}
		};
	}
	
	private static class Consumer implements Runnable {
		public void run() {
			
			// Sleep for random amount of time
			Thread.sleep(Random.randInt(0, Integer.MAX_VALUE));
			
			// Attempt remove
			if (remove(Random.randInt(0, Integer.MAX_VALUE)) == 0) {
				// Success
				System.out.println("Consumer Thread Id: " + Thread.currentThread().getId() + " Success!");
			} else {
				// Failure
				System.out.println("Producer Thread Id: " + Thread.currentThread().getId() + " Failed!");
			}
		};
	}
}