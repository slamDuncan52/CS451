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
int framesUsed = -1;
char* mainMemory;
int totalAddresses = 0;
int pageFaults = 0;
int TLBHits = 0;

FILE* inFile;
FILE* outFile;
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
        //Initialize Main Memory
        mainMemory = (char*)malloc(65536);
        inFile = fopen(argv[argc - 1], "r");
        outFile = fopen("answer.txt", "w");
        storeFile = fopen("BACKING_STORE.bin", "r");
        while (translateNext())
                ;
        float faultRatio = pageFaults * 1.00 / totalAddresses;
        float TLBRatio = TLBHits * 1.00 / totalAddresses;
        fprintf(outFile, "Total Addresses handled: %d\nPage Faults: %d\nPage Fault Rate: %0.3f\nTLB Hits: %d\nTLB Hit Rate: %0.3f\n",
            totalAddresses, pageFaults, faultRatio, TLBHits, TLBRatio);
        fclose(inFile);
        fclose(outFile);
        fclose(storeFile);
        free(mainMemory);
        return 0;
}

int translateNext()
{
        int logAddr;
        int frame = -1;
        fscanf(inFile, "%d", &logAddr);
        if (feof(inFile)) {
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
        signed char value = resolve(frame, offset);
        fprintf(outFile, "Virtual: %d Physical: %d Value: %d\n", logAddr, (frame * 256) + offset, value);
        totalAddresses++;
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
        framesUsed++;
        fseek(storeFile, (256 * page), SEEK_SET);
        for (int i = 0; i < 256; i++) {
                fread((mainMemory + (framesUsed * 256) + i), 1, 1, storeFile);
        }
        pageTable[page] = framesUsed;
        updateTLB(page, framesUsed);
        return framesUsed;
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
