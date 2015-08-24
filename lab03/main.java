// Lab 3 (Program 3 Chapter 5)
// Producer - Consumer Problem
// Mikayla Ray & Mitchell Duncan

public static void main(String[] args) {
	
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
}