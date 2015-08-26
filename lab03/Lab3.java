// Mikayla Ray & Mitchell Duncan
// 25 August 2015
// CS-451 : Operating Systems Concepts
// Producer-Consumer Problem
// Java Threads

import java.util.Random;
import java.util.Arrays;

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
         
         if (counter > 0)
            counter--;
            
			return (counter+1);
		} catch (Exception e) {
			return -1;
		}
	}

	public static void main(String[] args) {
		
      if (args[0] != null && args[1] != null && args[2] != null)
		   System.out.println("\nLab 3 (Program 3, Chapter 5)\nMikayla Ray & Mitchell Duncan\nJAVA\nSleep: " + args[0] + " | Producer Threads: " + args[1] + " | Consumer Threads: " + args[2] + "\n\n");
		else
         System.exit(1);
         		
		// Retrieve command line arguments
		int sleepTime = 0, numOfProducerThreads = 0, numOfConsumerThreads = 0;
		
		if ((Integer.valueOf(args[0]) < 0) || (Integer.valueOf(args[0]) > Integer.MAX_VALUE)) {
			sleepTime = 0;
		} else {
			sleepTime = Integer.valueOf(args[0]);
		}
		
		if ((Integer.valueOf(args[1]) < 0) || (Integer.valueOf(args[1]) > Integer.MAX_VALUE)) {
			numOfProducerThreads = 0;
		} else {
			numOfProducerThreads = Integer.valueOf(args[1]);
		}
		
		if ((Integer.valueOf(args[2]) < 0) || (Integer.valueOf(args[2]) > Integer.MAX_VALUE)) {
			numOfConsumerThreads = 0;
		} else {
			numOfConsumerThreads = Integer.valueOf(args[2]);
		}
				
		// Create producer threads
      final int limit = sleepTime;
		while (numOfProducerThreads > 0) {
         Thread t = new Thread() {
            public void run() {
               // Sleep for random amount of time
      			try {
      			   Thread.sleep(new Random().nextInt(limit));
               } catch (InterruptedException e) {
                  System.out.println("EXCEPTION!");
               }
      			
      			// Attempt insert
               int num = new Random().nextInt(100);
      			if (insert(num) == 0) {
      				// Success
      				System.out.println("Producer Thread Id: " + Thread.currentThread().getId() + " inserted " + num);
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
         			   Thread.sleep(new Random().nextInt(limit));
                  } catch (InterruptedException e) {
                     System.out.println("EXCEPTION!");
                  }
         			
         			// Attempt remove
                  int num = new Random().nextInt(100);
                  int result = remove(num);
         			if (result > -1) {
         				// Success
         				System.out.println("Consumer Thread Id: " + Thread.currentThread().getId() + " removed item at position " + result);
         			} else {
         				// Failure
         				System.out.println("Consumer Thread Id: " + Thread.currentThread().getId() + " Failed!");
         			}                  
               }
         };
         t.start();
					
			numOfConsumerThreads--;
		}

      try {
         Thread.sleep(sleepTime);
         System.out.println("\n\n" + Arrays.toString(buffer));
         System.exit(0);
      } catch (InterruptedException e) {
         System.exit(1);
      }
	}
}