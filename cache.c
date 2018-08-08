#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
//Memory:
unsigned char Memory[16777216*sizeof(char)];
//Implement Cache:
int cacheSize;
int associativity;
int blockSize;
int blockCap;
int numSets;
    
struct dataBlock{
    int valid;
    int tagNum;
    int hitCount;
};
    

int log2(int n) {
    int r=0;
    while (n>>=1) r++;
    return r;
}  

int main(int argc, char* argv[]) {
    FILE *file;
    
    cacheSize=atoi(argv[2]);//in KB.
    cacheSize=cacheSize*1024;//in Bytes
    
    
    associativity=atoi(argv[3]);
    
    blockSize=atoi(argv[4]);
    
    blockCap=cacheSize/blockSize;//max number of blocks cache can hold. Frame capacity.
    
    
    numSets=blockCap/associativity;
    
    struct dataBlock myCache[numSets][associativity];
    
     
    for(int i=0;i<numSets;i++){
        
        for(int j=0;j<associativity;j++){
            myCache[i][j].valid=0;
            
            
            
        }
    
    }
    
    //printf("%d\n",myCache[34][12].valid);
    
    file =fopen(argv[1],"r");
    if(file==NULL)
    {
        printf("No File found\n");
    }
    char value [64];
    int hitCtrl=0;
    int storeBool=0;
    int count=0;
    unsigned char *memPointer;
    unsigned int storeAddress=0;
    unsigned int loadAddress=0;
    unsigned int blockOffset=0;
    unsigned int setIndexBits=0;
    unsigned int setIndex=0;
    unsigned int tagBitsNum=0;
    unsigned int indexOff=0;
    unsigned int tag=0;
    unsigned int accessCount=0;
    unsigned int counterLRU=1000;

    
    char *p ;//p pointer
    
    
    
    
    while((fscanf(file,"%s",value) != EOF))
    
    {
        
        
    if((strcmp(value,"load")==0)||(strcmp(value,"store")==0))
    
    {
        if((strcmp(value,"load")==0))
        {
            hitCtrl=0;//control variable for hit or miss.
            //check if we have a load or store instruction:
            printf("load ");
            storeBool=0;
            count++;
            
        }
        
        if((strcmp(value,"store")==0))
        {
           hitCtrl=0;//control variable for hit or miss.
           //check if we have a load or store instruction:
           printf("store ");
           storeBool=1;
           count++;
           
            
        }
        
        }
        
         else//other values other than load and store.
        
        {
            
            
            
            if(storeBool==1)
            
            {
                
                switch(count){
                    case 1:
                    printf("%s ",value);   
                      
                  sscanf(value,"%x",&storeAddress);
                     
                blockOffset=log2(blockSize);  
                        
                setIndexBits=log2(numSets);//Bits repping the set #.
                        
                tagBitsNum=24-(blockOffset+setIndexBits);
                    
                tag=storeAddress >>(blockOffset+setIndexBits);
                        
                indexOff=(storeAddress) & ((1<<(blockOffset+setIndexBits))-1);//get lower bits.
                
                setIndex=indexOff>>blockOffset;//get set index of address.
         
                    count++;
                     break;
                    
                    case 2:
                     accessCount=atoi(value);//??
                     
                    // Find if Hit Or Miss:
                        for(int i=0;i<associativity;i++){
                            if(myCache[setIndex][i].tagNum==tag && myCache[setIndex][i].valid==1)
                            {
                                printf("hit ");
                                myCache[setIndex][i].hitCount+=1;
                                hitCtrl=1;
                                  
                            }
                            
                        }
                     
                    if(hitCtrl==0){
                        printf("miss ");
                    }
                     
                     count++;
                     break;
                    
                    case 3:
                    p=value;//Pointer to hex string
                        
                    //WRITE TO MEMORY:
                    memPointer=Memory;//first index of memory.
                memPointer=memPointer+storeAddress;
                        
                     for(int i=0;i<accessCount;i++){
                         
        sscanf(p,"%2hhx",memPointer);
                        p=p+2;
                        memPointer++;
                     }
                     count=0;
                    //printf("%x\n",Memory[storeAddress]);
                        
                     printf("\n");
                     break;     
                    
                }
                
            }
            
            else
            {
            
                switch(count){
                    case 1:
                    printf("%s ",value);
                    sscanf(value,"%x",&loadAddress);
                     
                blockOffset=log2(blockSize);  
                        
                setIndexBits=log2(numSets);//Bits repping the set #.
                        
                tagBitsNum=24-(blockOffset+setIndexBits);
                    
                tag=loadAddress >>(blockOffset+setIndexBits);
                        
                indexOff=(loadAddress) & ((1<<(blockOffset+setIndexBits))-1);//get lower bits.
                
                setIndex=indexOff>>blockOffset;//get set index of address.
                        
                      count++;
                      break;
                    
                    case 2:
                       accessCount=atoi(value);
                        for(int i=0;i<associativity;i++){
                            if(myCache[setIndex][i].tagNum==tag && myCache[setIndex][i].valid==1)
                            {
                                printf("hit ");
                                myCache[setIndex][i].hitCount+=1;
                                hitCtrl=1;
                        
                        
                     for(int i=0;i<accessCount;i++){
                         
                        printf("%02hhx",Memory[loadAddress]);
                        
                        loadAddress++;
                         
                     }
                                
                        printf(" ");//space after characters.
                                
                                
                                
                                 
                            }
                            
                        }
                        
                        if(hitCtrl==0){
                            printf("miss ");
                            for(int i=0;i<accessCount;i++){
                         
                                printf("%02hhx",Memory[loadAddress]);
                        
                                loadAddress++;
                         
                            }
                                
                        printf(" ");//space after characters.
                            
                        //NOW ALTER CACHE:
                        int i=0;int x=0;
                        int displaceCtrl=1;
                        int minIndex=0;
                        
                        int min=16777216;
                        while(i<associativity){
                            if(myCache[setIndex][i].valid==0){
                                myCache[setIndex][i].tagNum=tag;
                                myCache[setIndex][i].valid=1;
                                myCache[setIndex][i].hitCount=i;
                                displaceCtrl=0;
                                i=associativity;//to break out of while loop.
                                
                            }
                            i++;
                        }
                            if(displaceCtrl==1){//Displace some things from cache:LRU
                            while(x<associativity){
                                if(myCache[setIndex][x].hitCount<min){
                                    min=myCache[setIndex][x].hitCount;
                                    minIndex=x;
                                }
                                x++;
                            }
                            
                        
                         //need to replace some blocks
                            myCache[setIndex][minIndex].tagNum=tag;
                            myCache[setIndex][minIndex].valid=1;
                            counterLRU=counterLRU+1;
                            myCache[setIndex][minIndex].hitCount=counterLRU;
                                
                        }
                               
                        }
                        
                      
                      count=0;
                      printf("\n");
                      break;
                    
                    
                }
                
                  
            }
            
            
            
        }
        
       
        
        
    }
    return 0;
    
}


//potential problems:set position and tag.

//could have been removed or not not removed!
