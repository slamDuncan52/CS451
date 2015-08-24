// Lab 3 (Program 3 Chapter 5)
// Producer - Consumer Problem
// Mikayla Ray & Mitchell Duncan

import java.util.Random;

public class Lab3 {

   // Initialize the buffer
   static final int BUFFER_SIZE = 5;
	static int[] buffer = new int[BUFFER_SIZE];
	static int counter = 0;
   
	public static synchronized int insert(int value) {
		try {
			buffer[counter] = value;
			counter++;
			return 0;
		} catch (Exception e) {
			return -1;
		}
	}
	
	public static synchronized int remove(int value) {
		try {
			buffer[counter] = 0;
			counter--;
			return 0;
		} catch (Exception e) {
			return -1;
		}
	}

	public static void main(String[] args) {
		
		System.out.println("\nLab 3 (Program 3, Chapter 5)\nMikayla Ray & Mitchell Duncan\nJAVA\n> " + args + "\n");
				
		// Retrieve command line arguments
		int sleepTime = 0, numOfProducerThreads = 0, numOfConsumerThreads = 0;
		
		if ((args[0] == null) || (Integer.valueOf(args[0]) < 0) || (Integer.valueOf(args[0]) > Integer.MAX_VALUE)) {
			sleepTime = 0;
		} else {
			sleepTime = Integer.valueOf(args[0]);
		}
		
		if ((args[1] == null) || (Integer.valueOf(args[1]) < 0) || (Integer.valueOf(args[1]) > Integer.MAX_VALUE)) {
			numOfProducerThreads = 0;
		} else {
			numOfProducerThreads = Integer.valueOf(args[1]);
		}
		
		if ((args[2] == null) || (Integer.valueOf(args[2]) < 0) || (Integer.valueOf(args[2]) > Integer.MAX_VALUE)) {
			numOfProducerThreads = 0;
		} else {
			numOfProducerThreads = Integer.valueOf(args[2]);
		}
				
		// Create producer threads
		while (numOfProducerThreads > 0) {
         Thread t = new Thread() {
            public void run() {
               // Sleep for random amount of time
      			try {
      			   Thread.sleep(new Random().nextInt(Integer.MAX_VALUE));
               } catch (InterruptedException e) {
                  System.out.println("EXCEPTION!");
               }
      			
      			// Attempt insert
      			if (insert(new Random().nextInt(Integer.MAX_VALUE)) == 0) {
      				// Success
      				System.out.println("Producer Thread Id: " + Thread.currentThread().getId() + " Success!");
      			} else {
      				// Failure
      				System.out.println("Producer Thread Id: " + Thread.currentThread().getId() + " Failed!");
      			}
            }
         };
         t.start();
					
			numOfProducerThreads--;
		}
		
		// Create consumer threads
		while (numOfConsumerThreads > 0) {
   		Thread t = new Thread() {
               public void run() {   
                  // Sleep for random amount of time
                  try {
         			   Thread.sleep(new Random().nextInt(Integer.MAX_VALUE));
                  } catch (InterruptedException e) {
                     System.out.println("EXCEPTION!");
                  }
         			
         			// Attempt remove
         			if (remove(new Random().nextInt(Integer.MAX_VALUE)) == 0) {
         				// Success
         				System.out.println("Consumer Thread Id: " + Thread.currentThread().getId() + " Success!");
         			} else {
         				// Failure
         				System.out.println("Consumer Thread Id: " + Thread.currentThread().getId() + " Failed!");
         			}                  
               }
         };
         t.start();
					
			numOfConsumerThreads--;
		}
		
		// "Sleep" -> Exit
		while(sleepTime > 0) {
			sleepTime--;
		}
		
		if (sleepTime == 0) {
			System.exit(0);
		}
	}
}