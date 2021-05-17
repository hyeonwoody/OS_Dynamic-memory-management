#include "alloc.h"

enum STATUS {INUSED, FREE};
#define NUM_BLOCK 512 //NUM_BLOCK = PAGESIZE/MINALLOC




typedef struct Block {
    char* address;
	unsigned int size:13; //number 4096 can be written with 13 bits
    unsigned int status:1; //0: in-used 1: free  
    unsigned int last:10; //index of last byte, number 512 can be written with 10 bits
}BLOCK;

char *src; 
static int allocated=0; //total allocated size

BLOCK block[NUM_BLOCK];
#if DEBUG
    int cnt=0;
    int decnt=0;
    int hello=0;
#endif

/* Function Used */
int insert(int , int , int ); //used to insert metadata
void merge (int); //used to merge blocks

int init_alloc(){

    #if DEBUG
        printf("start init \n");
    #endif
		 src= mmap(NULL, PAGESIZE, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
		if (src == MAP_FAILED) {
            #if DEBUG 
            printf("\ninit failed\n");
            #endif
            return -1;
        }
		#if DEBUG
        printf("src : %d\n",src);
        #endif
        for (int i=0; i<NUM_BLOCK; i++){
            if (i==0){
                block[i].address=src;
                block[i].size=PAGESIZE;
            }
            else{ 
                block[i].address=block[i-1].address + MINALLOC;
                block[i].size=-1;
            } 
            
            block[i].status=FREE;
        } 
        return 0;
}

int cleanup(){
    #if DEBUG
        printf("start cleanupaa");
    #endif
    if (munmap(block[0].address, PAGESIZE)==-1){
        return 1;
    }
    return 0;
}

char *alloc(int s){
    
    #if DEBUG
        cnt++;
        printf("\nstart alloc %d\n", cnt);
        print_block();
    #endif
    int i,j; //index for FOR-LOOP, i indicates first bytes of block
    if (s%8!=0){
        #if DEBUG
            printf("not 8bytes units");
        #endif
            return NULL;
    }
    if (PAGESIZE-allocated<s){
        #if DEBUG
            printf("not enough space");
        #endif
        return NULL;
    }
    i=0; 
    while(i<NUM_BLOCK){
        if (block[i].status==FREE)
        {   
            if (block[i].size<s){
                #if DEBUG
                    hello++;
                    printf ("%d, %d, %d, %d\n",hello, i ,cnt,s);

                    printf ("size : %d address : %d\n", block[i].size, block[i].address);
                    printf ("aalast size : %d address : %d\n", block[block[i].last+1].size, block[block[i].last+1].address);
                #endif
                if (block[block[i].last+1].status==FREE){
                    merge(i);
                }
                else {
                    if (block[i].last+1==512)
                        return NULL;       
                    i=block[i].last+1;
                }
                continue;
            }
            else if (block[i].size>s){

                j=insert(i, i+1, s); //insert meta data

                /* insert extra meta data */
                block[i].last=j-1;
                block[j].size=block[i].size-s;
                block[j].address=block[i].address+s;
                block[i].status=INUSED;
                block[i].size=s;
                break;
            }
            else {
                
                j=insert(i, i, s); //insert meta data

                /* insert extra meta data */
                block[i].last=j-1;
                block[j].address=block[i].address+s;
                break;
            }
        }
        i++;
    }
    allocated+=(int)s;
    #if DEBUG
        printf("size : %d  addreess : %d\n",s, block[i].address);
        print_block();
    #endif
    
	return block[i].address;
}


void dealloc(char *ptr){

    #if DEBUG
        
        decnt++;
        printf("\nstart dealloc %d\n", decnt);
        print_block();
    #endif

    int total=0;
    int i=0;
    int j=0;
    for (i=0; i<NUM_BLOCK; i++){
        if (block[i].address==ptr){
        #if DEBUG
            //printf("%d.address-> %p\n",i, ptr);
        #endif
            block[i].status=FREE;
            for (j=1; j<block[i].size/MINALLOC; j++){
                block[i+j].status=FREE;
                block[i+j].size=0;
            }
            break;
        }
    }
    total=block[i].size;
    
     /* if (block[i].last!=NUM_BLOCK-1){
        if (block[block[i].last+1].status==FREE){
            for (j=i+1; (j<NUM_BLOCK) && (block[j].status==FREE); j++){
                total+=8;
            }
            block[i].size+=total; // this parttttt
        }
    }
        
    if (i!=0){
        if (block[i-1].status==FREE){
            for (j=i-1; j>=0 && block[j].status==FREE  ; j--){
                total+=8;
            }
        
            block[j].size+=total;
        }
    }  */
    allocated-=(int)block[i].size;
    #if DEBUG
        printf("size : %d address : %d\n", block[i].size, block[i].address);
        print_block();
    #endif
}

 void print_block (){
    int i=0;
    int prev=0;
    if (DEBUG){
     while (i<NUM_BLOCK){
        if (block[i].status==FREE){}
        else if (block[i].size>0){
            printf ("%d(%d)->", block[i].address, block[i].size);
            i=block[i].last;
            if (block[i].address==block[prev].address+block[prev].size){
                printf("correct");
            }
            prev=i;

        }
        i++;
    }
    printf("\n");
    printf("allocated : %d\n",allocated);  
    }
    
} 

int insert(int i, int j, int s){ 

    while (j<i+s/MINALLOC){
        block[j].status=INUSED;
        block[j].size=s;
        block[j].address=block[i].address;  
        j++;
    }
    return j;
}

void merge (int i){
    #if DEBUG
        printf("\nstart merge\n");
    #endif
    int j=0;
    int total=0;
    for (j=block[i].last+1; (j<NUM_BLOCK) && (block[j].status==FREE); j++){
                total+=8;
    }
    block[i].size+=total; // this parttttt
}
