/*
virmem.c

Student Name : Gurtej Dhami
Student ID # : 0969863

Virtual Memory Manager
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct Memoryblocks
{
	int memory[65536];//physical memory as 1D array
	int page[256][2];//page table as 2D array index 0 for column: page number, index 1 for column: frame number
	int TLB[16][2];//TLB as 2D array index 0 for column: page number, index 1 for column: frame number
};

int main(int argc, char *argv[]){

	if (argc != 2) {
        	fprintf(stderr,"Usage: %s filenameoflogicaladdresses\n", argv[0]);
        	return -1;
	}

	double numDoneAddresses = 0;
	FILE *userFile;
	FILE *backingStore;   
	int pageFaults = 0;   
	int TLBhits = 0;      
	int checkFrame = 0;  
	int checkPageTableNumber = 0;  
	int TLBentries = 0; 
	char origAddress[10];
	int logicalAddress;
	char holdBackStr[256];
	char memVal;
	int i = 0;
	int pageNumber = 0;
	int offset = 0;
	int frameNumber = 0;
	int doesFrameNumberExist = 0;
	int physAddress = 0;
	int howMany = 0;
	int num = 0;

	userFile = fopen(argv[1], "r");
	if (userFile == NULL) {
        	fprintf(stderr, "Error opening addresses.txt you put : %s\n",argv[1]);
        	return -1;
	}
	backingStore = fopen("BACKING_STORE.bin", "rb");
	if (backingStore == NULL) {
        	fprintf(stderr, "Error opening BACKING_STORE.bin\n");
        	return -1;
	}
	struct Memoryblocks m;
	while ( fgets(origAddress, 15, userFile) != NULL && origAddress[0] != '\n') {

	        logicalAddress = atoi(origAddress);
        
	        //Calculate page number of logical address
	        pageNumber = logicalAddress/256;
		//calculate offset of logical address
    		offset = logicalAddress - (256 * pageNumber);
    		
		//set to -1 initially but changed to 0 if the frame number is found
		doesFrameNumberExist = -1;
	    	//loop to see if the page number for the current address exists in the TLB
		for(i = 0; i < 16; i++){
		        if(m.TLB[i][0] == pageNumber){
			    TLBhits++;    
		            frameNumber = m.TLB[i][1];
			    doesFrameNumberExist = 0;				
		                               
		        }
		}
    		
    		//if the frame number is not found in the TLB the Page Table is searched
    		if(doesFrameNumberExist != 0){
        		for(i = 0; i < checkPageTableNumber; i++){
            			if(m.page[i][0] == pageNumber){     
                			frameNumber = m.page[i][1];
					doesFrameNumberExist = 0;          
            			}
        		}
        		
    		}
		//if the frame number has still not been found the BACKING_STORE file is checked
    		if(doesFrameNumberExist != 0){                    
         		//page number converted back for the search
			if (fseek(backingStore, pageNumber * 256, SEEK_SET) != 0) {
	        		fprintf(stderr, "Error seeking in backing store\n");
	    		}
    
	    		//error checking for fread()		
	    		if (fread(holdBackStr, sizeof(char), 256, backingStore) == 0) {
	        		fprintf(stderr, "Error reading from backing store\n");        
	    		}
    
    			//the index for the start of a frame is found and holdBackStr is stored in the following 256 index's	
	    		for(i = 0; i < 256; i++){
				num = howMany*256;
	        		m.memory[i + num] = holdBackStr[i];
	    		}
			//index for the start of the next frame increments
			howMany++;
    			//the page table is filled with the frame and page number
	    		m.page[checkPageTableNumber][1] = checkFrame;
			m.page[checkPageTableNumber][0] = pageNumber;
	    
	            	pageFaults++;
			frameNumber = checkFrame;                         
			checkFrame++;  
			checkPageTableNumber++;
	        }
		//FIFO algorithm
		//if TLB finds the page number of the current address then the loop breaks
		//otherwise the TLB must be modified
		for(i = 0; i < TLBentries; i++){
			if(m.TLB[i][0] == pageNumber){
    				break;
    			}
			//each page and frame value is shifted back one to eliminate the value at index 0 which is the first in 
			//the new page and frame value is placed at the end
			if(i == TLBentries-1){
			  
				for(i = 0; i < 16 - 1; i++){
					m.TLB[i][1] = m.TLB[i + 1][1];
                			m.TLB[i][0] = m.TLB[i + 1][0];
            			}
            			m.TLB[TLBentries-1][1] = frameNumber;
				m.TLB[TLBentries-1][0] = pageNumber;       
    			}
    		}
		//if the current page number has the same value as that of an index in the TLB
		//every index from that point gets the value from the index that is next to it
    		if(m.TLB[i][0] == pageNumber){
        		for(i = i; i < TLBentries - 1; i++){     
            			m.TLB[i][1] = m.TLB[i + 1][1];
				m.TLB[i][0] = m.TLB[i + 1][0];      
       			}
		}
		//if the TLB is full the frame and page number at the last index are replaced with the current frame and page number
       		if(TLBentries >= 16){ 
           		m.TLB[TLBentries-1][1] = frameNumber;              
			m.TLB[TLBentries-1][0] = pageNumber;
        	}
		//a new page and frame number has been added to an open slot and the amount of entries in the TLB will be incremented
        	else{                                          
            		m.TLB[TLBentries][1] = frameNumber;
			m.TLB[TLBentries][0] = pageNumber;
			TLBentries++;
        	}
		//retreive the value from memory by adding the offset to the current frame number that is indexed to its correct location
		memVal = m.memory[256*frameNumber + offset];  
		//the physical address is equal to the frame number multiplied by 256 and a bitwise orr with the offset
		physAddress = (frameNumber * 256) | offset;
		fprintf(stdout,"Virtual address: %d Physical address: %d Value: %d\n", logicalAddress, physAddress, memVal);
		//the number of complete addresses is incremented
		numDoneAddresses++;        
	}
    	fclose(userFile);
    	fclose(backingStore);
	//calculate and print error rates
    	printf("Number of translated addresses = %.0f\n", numDoneAddresses);
	TLBhits--;
    	double pagefaultRate = pageFaults / numDoneAddresses;
    	double TLBrate = TLBhits / numDoneAddresses;
    
	fprintf(stdout, "Page Faults = %d\n", pageFaults);
	fprintf(stderr, "Page Fault Rate = %.3f\n",pagefaultRate);
	fprintf(stdout, "TLB Hits = %d\n", TLBhits);
	fprintf(stderr, "TLB Hit Rate = %.3f\n", TLBrate);
    
    return 0;

}
