#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Function Declares */
int translateNext();
int checkTLB(int);
int checkPT(int);
int pageFault(int);
signed char resolve(int frame, int offset);
void updateTLB(int page, int frame);

/* Global Vars */
int pageTable[256];
int TLB[16][2];
int framesUsed = -1;//Keeps track of next empty frame
char* mainMemory;//This is where frames live
int totalAddresses = 0;//Statistics variables
int pageFaults = 0;
int TLBHits = 0;

FILE* inFile;//Addresses file
FILE* outFile;//answer file
FILE* storeFile;//Backing store

int main(int argc, char** argv)
{
        //Initialize PT
        for (int i = 0; i < 256; i++) {
                pageTable[i] = -1;
        }
        //Initialize TLB
        for (int i = 0; i < 16; i++) {
                TLB[i][0] = -1;
                TLB[i][1] = -1;
        }
        //Initialize Main Memory
        mainMemory = (char*)malloc(65536);//Frame size * number of frames
        inFile = fopen(argv[argc - 1], "r");//Open all files
        outFile = fopen("answer.txt", "w");
        storeFile = fopen("BACKING_STORE.bin", "r");
        while (translateNext())//All work done here
                ;
        float faultRatio = pageFaults * 1.00 / totalAddresses;//Report statistics
        float TLBRatio = TLBHits * 1.00 / totalAddresses;
        fprintf(outFile, "Total Addresses handled: %d\nPage Faults: %d\nPage Fault Rate: %0.3f\nTLB Hits: %d\nTLB Hit Rate: %0.3f\n",
            totalAddresses, pageFaults, faultRatio, TLBHits, TLBRatio);
        fclose(inFile);//Clean up after ourselves
        fclose(outFile);
        fclose(storeFile);
        free(mainMemory);
        return 0;
}

int translateNext()
{
        int logAddr;//Logical address from inFile
        int frame = -1;//The frame of the physical address
        fscanf(inFile, "%d", &logAddr);
        if (feof(inFile)) {//If we read in EOF, we're done
                return 0;
        }
        int page = logAddr / 256;//Extract page and offset values
        int offset = logAddr % 256;
        frame = checkTLB(page);//Check TLB first
        if (frame == -1) {
                frame = checkPT(page);//Otherwise check page table
                if (frame == -1) {
                        frame = pageFault(page);//Otherwise need to read in page from store
                }
        }
        signed char value = resolve(frame, offset);//Find value at the location
        fprintf(outFile, "Virtual: %d Physical: %d Value: %d\n", logAddr, (frame * 256) + offset, value);//Print nicely
        totalAddresses++;//Increment stats
        return 1;
}

int checkTLB(int page)
{
        int frame = -1;//Return value
        for (int i = 0; i < 16; i++) {//Without special HW, we just check all TLB slots
                if (TLB[i][0] == page) {//If found
                        frame = TLB[i][1];//Set frame
                        TLBHits++;//Set stats
                        break;//Done
                }
        }
        return frame;
}

int checkPT(int page)
{
        int frame = -1;//Return value
        if (pageTable[page] != -1) {//If there's a value in the page table
                frame = pageTable[page];//Set it
                updateTLB(page, frame);//Put it in TLB
        }
        return frame;
}

int pageFault(int page)
{
        pageFaults++;//Increase stats
        framesUsed++;//Update next empty frame
        fseek(storeFile, (256 * page), SEEK_SET);//Go to page in backing store
        fread((mainMemory + (framesUsed * 256)), 1, 256, storeFile);//Read page into next empty frame
        pageTable[page] = framesUsed;//Set return
        updateTLB(page, framesUsed);//Put it in TLB
        return framesUsed;
}

signed char resolve(int frame, int offset)
{
        signed char result = 0;
        result = *(mainMemory + (frame * 256) + offset);//Value is in main memory at the given offset
        return result;
}

void updateTLB(int page, int frame)
{
        for (int i = 15; i > 0; i--) {//Move all TLB values down one
                TLB[i][0] = TLB[i - 1][0];
                TLB[i][1] = TLB[i - 1][1];
        }
        TLB[0][0] = page;//Put the new value into slot zero
        TLB[0][1] = frame;
        return;
}
