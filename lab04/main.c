#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define pageSize 256

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
int framesUsed = 0;
int* mainMemory;
int totalAddresses = 0;
int pageFaults = 0;
int TLBHits = 0;

FILE* logicalFile;
FILE* physicalFile;
FILE* storeFile;

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
        //Initialize Main Memory with one frame
        mainMemory = (int*)malloc(256);
        logicalFile = fopen(argv[1], "r");
        physicalFile = fopen("answer.txt", "w");
        storeFile = fopen("BACKING_STORE.bin", "r");
        while (translateNext())
                ;
        fclose(logicalFile);
        fclose(physicalFile);
        fclose(storeFile);
        free(mainMemory);
        printf("Total Addresses handled: %d\nPage Faults: %d\nPage Fault Rate: %0.3f\nTLB Hits: %d\nTLB Hit Rate: %0.3f\n", totalAddresses,
            pageFaults, pageFaults / totalAddresses, TLBHits, TLBHits / totalAddresses);
        return 0;
}

int translateNext()
{
        int logAddr;
        int frame = -1;
        fscanf(logicalFile, "%d", &logAddr);
        if (feof(logicalFile)) {
                return 0;
        }
        int page = logAddr / 256;
        int offset = logAddr % 256;
        frame = checkTLB(page);
        if (frame == -1) {
                frame = checkPT(page);
                if (frame == -1) {
                        frame = pageFault(page);
                }
        }
        signed char value = 0;// = resolve(frame, offset);
        totalAddresses++;
        printf("Log: %d Phys: %d Val: %d\n", logAddr, (frame * 256) + offset, value);
        return 1;
}

int checkTLB(int page)
{
        int frame = -1;
        for (int i = 0; i < 16; i++) {
                if (TLB[i][0] == page) {
                        frame = TLB[i][1];
                        TLBHits++;
                        break;
                }
        }
        return frame;
}

int checkPT(int page)
{
        int frame = -1;
        if (pageTable[page] != -1) {
                frame = pageTable[page];
                updateTLB(page, frame);
        }
        return frame;
}

int pageFault(int page)
{
        pageFaults++;
        int frame = -1;
        mainMemory = (int*)realloc(mainMemory, (framesUsed + 1) * 256);
        fseek(storeFile, 256 * page, SEEK_CUR);
        for (int i = 0; i < 256; i++) {
                fread((mainMemory + (framesUsed * 256) + i), 1, 1, storeFile);
        }
        frame = framesUsed;
        pageTable[page] = frame;
        framesUsed++;
        updateTLB(page, frame);
        return frame;
}

signed char resolve(int frame, int offset)
{
        signed char result = 0;
        result = *(mainMemory + (frame * 256) + offset);
        return result;
}

void updateTLB(int page, int frame)
{
        for (int i = 15; i > 0; i--) {
                TLB[i][0] = TLB[i - 1][0];
                TLB[i][1] = TLB[i - 1][1];
        }
        TLB[0][0] = page;
        TLB[0][1] = frame;
        return;
}
