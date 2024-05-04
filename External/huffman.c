#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "compressed.h"
#include "data.h"

#define xBit 32
#define yBit 24

#define M (xBit/8)
#define N yBit

#define CHAR_RANGE 234
char characters[] = {
0x01,
0xfc,
0x80,
0xfd,
0xe1,
0xde,
0xfa,
0x97,
0x47,
0xdc,
0xec,
0x09,
0xf7,
0xe8,
0x27,
0x61,
0x3b,
0x10,
0x06,
0x40,
0xd8,
0x99,
0x9b,
0xb3,
0xbb,
0x49,
0x75,
0x59,
0x89,
0xdd,
0xb5,
0x6d,
0x50,
0xc2,
0x6b,
0xa7,
0x16,
0xab,
0xd4,
0xda,
0xd3,
0xb2,
0x7a,
0x53,
0x35,
0x0a,
0xa6,
0xa4,
0xb6,
0x29,
0x46,
0x94,
0x93,
0x69,
0x3a,
0x14,
0x92,
0xac,
0x6c,
0xf1,
0x02,
0xc1,
0xe3,
0x05,
0x9c,
0x84,
0xf5,
0x7d,
0x39,
0x3d,
0xf6,
0x83,
0x81,
0x20,
0xfe,
0xf8,
0xc0,
0x1f,
0xf0,
0xe0,
0x7e,
0x73,
0x48,
0xc9,
0x4f,
0xcd,
0x72,
0x66,
0xed,
0xe9,
0x9d,
0x65,
0x88,
0x13,
0x19,
0xee,
0xe7,
0x1e,
0xc3,
0xfb,
0x8e,
0xce,
0xb0,
0x17,
0x82,
0x8c,
0x67,
0xe4,
0xef,
0x0e,
0x08,
0x1b,
0x62,
0x44,
0xd9,
0xb8,
0x23,
0x33,
0xa0,
0xf4,
0xaf,
0x2d,
0xb7,
0x36,
0xc7,
0x30,
0xf9,
0xcf,
0x78,
0xd0,
0x28,
0xa1,
0xd2,
0xa5,
0x22,
0x5c,
0x1a,
0xc4,
0x0d,
0xc8,
0x98,
0xd7,
0x68,
0x12,
0xe2,
0xcb,
0x24,
0x03,
0x0f,
0x07,
0x3c,
0x60,
0x9f,
0x0c,
0x38,
0x18,
0x31,
0x21,
0x43,
0xc6,
0x5f,
0xf2,
0x42,
0x4e,
0x85,
0x25,
0xa3,
0x8d,
0x9a,
0xb1,
0x64,
0x6e,
0x1d,
0x26,
0x5e,
0x74,
0xc5,
0xdb,
0xae,
0x34,
0xd6,
0x70,
0x87,
0xdf,
0xbe,
0x9e,
0xe6,
0x37,
0x90,
0xf3,
0xcc,
0x58,
0xb9,
0xeb,
0xa8,
0xd1,
0x4b,
0x11,
0x2f,
0xbf,
0x77,
0xbc,
0x41,
0x0b,
0x79,
0xb4,
0x91,
0x2e,
0x04,
0x7c,
0x1c,
0x8f,
0x76,
0xad,
0xa9,
0xbd,
0x96,
0x95,
0x63,
0x5b,
0xe5,
0x86,
0x7b,
0x2c,
0x8b,
0x4c,
0x32,
0x71,
0x6f,
0x3e,
0x7f,
0x3f,
0xff,
0x00
};

char* huffmanCodes[] = {
"00000",
"00001",
"00010",
"000110000",
"000110001",
"000110010000",
"000110010001",
"0001100100100",
"0001100100101",
"000110010011",
"00011001010",
"00011001011",
"0001100110",
"000110011100",
"000110011101",
"000110011110",
"000110011111",
"000110100",
"000110101",
"000110110",
"00011011100",
"000110111010",
"00011011101100",
"000110111011010",
"000110111011011",
"000110111011100",
"0001101110111010",
"0001101110111011",
"000110111011110",
"000110111011111",
"00011011110000",
"00011011110001",
"0001101111001",
"000110111101",
"00011011111000",
"000110111110010",
"00011011111001100",
"00011011111001101",
"00011011111001110",
"00011011111001111",
"00011011111010",
"00011011111011000",
"00011011111011001",
"00011011111011010",
"00011011111011011",
"0001101111101110",
"00011011111011110",
"00011011111011111",
"0001101111110000",
"0001101111110001",
"0001101111110010",
"0001101111110011",
"000110111111010",
"000110111111011",
"000110111111100",
"000110111111101",
"0001101111111100",
"0001101111111101",
"000110111111111",
"000111000",
"000111001",
"000111010",
"000111011",
"00011110000",
"000111100010",
"000111100011",
"0001111001000",
"0001111001001",
"000111100101",
"000111100110",
"000111100111",
"000111101",
"000111110",
"000111111",
"00100",
"00101",
"00110",
"00111",
"01000",
"01001",
"010100000",
"0101000010000",
"01010000100010",
"01010000100011",
"010100001001",
"01010000101000",
"01010000101001",
"0101000010101",
"0101000010110",
"01010000101110",
"010100001011110",
"010100001011111",
"010100001100",
"010100001101",
"010100001110",
"010100001111",
"010100010",
"010100011",
"010100100",
"0101001010",
"010100101100",
"010100101101",
"010100101110",
"010100101111",
"010100110000",
"010100110001",
"010100110010",
"010100110011",
"0101001101",
"010100111",
"010101000",
"010101001000",
"01010100100100",
"010101001001010",
"010101001001011",
"0101010010011",
"010101001010",
"010101001011",
"010101001100",
"010101001101",
"010101001110",
"01010100111100",
"01010100111101",
"0101010011111",
"010101010",
"010101011",
"010101100",
"010101101",
"010101110",
"010101111000",
"010101111001000",
"010101111001001",
"01010111100101",
"010101111001100",
"010101111001101",
"010101111001110",
"010101111001111",
"010101111010",
"010101111011",
"010101111100",
"010101111101",
"01010111111000",
"01010111111001",
"0101011111101",
"0101011111110",
"01010111111110",
"01010111111111",
"01011",
"01100",
"01101",
"011100000",
"011100001",
"011100010",
"011100011",
"011100100",
"011100101",
"011100110000",
"011100110001",
"0111001100100",
"0111001100101",
"011100110011",
"011100110100",
"011100110101000",
"0111001101010010",
"0111001101010011",
"0111001101010100",
"0111001101010101",
"0111001101010110",
"01110011010101110",
"01110011010101111",
"01110011010110",
"01110011010111",
"0111001101100",
"0111001101101",
"01110011011100",
"011100110111010",
"011100110111011",
"011100110111100",
"0111001101111010",
"0111001101111011",
"01110011011111",
"011100111",
"011101000",
"0111010010",
"0111010011000",
"0111010011001",
"011101001101",
"011101001110",
"011101001111",
"0111010100",
"01110101010",
"011101010110000",
"0111010101100010",
"01110101011000110",
"011101010110001110",
"011101010110001111",
"01110101011001",
"0111010101101",
"011101010111",
"0111010110",
"011101011100",
"0111010111010",
"0111010111011",
"011101011110",
"0111010111110",
"01110101111110",
"011101011111110",
"011101011111111",
"0111011000",
"0111011001",
"011101101",
"011101110",
"011101111000000",
"011101111000001",
"011101111000010",
"011101111000011",
"011101111000100",
"011101111000101",
"01110111100011",
"011101111001000",
"011101111001001",
"01110111100101",
"0111011110011",
"011101111010000",
"011101111010001",
"011101111010010",
"011101111010011",
"0111011110101",
"011101111011",
"0111011111",
"011110",
"011111",
"10",
"11"
};



typedef struct Node {
    unsigned char data;
    struct Node *left;
    struct Node *right;
} Node;


Node* createNode(unsigned char data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}


void decompressHuffman(Node* root, char* bitstream, int bitstreamSize, const char* outputFileName) {
    unsigned char fstring[M*N] = {0};


    int count = 0;
    int frame = 0;

    int x = 0;
    int y = 0;

    FILE* outputFile = fopen(outputFileName, "wb");
    if (outputFile == NULL) {
        printf("Error opening output file.\n");
        return;
    }

    Node* current = root;
    for (int i = 0; i < bitstreamSize; i++) {
        unsigned char currentChar = (unsigned char)bitstream[i];


        for (int j = 7; j >= 0; j--) {
            int bit = (currentChar >> j) & 1; 
            if (bit == 0) {
                current = current->left;
            } else {
                current = current->right;
            }

            if (current->left == NULL && current->right == NULL) {
  

                unsigned char WATCHDOG = current->data;
                fputc(WATCHDOG, outputFile); 
                //printf("WATCHDOG - 0x%x\n",WATCHDOG);
                fstring[count] = (unsigned char)current->data;
                count++;
                if(count == 96){

                    printf("Frame %d\n -- BITSHIFT:%d  -- COUNT:%d",frame,j,i);
                    frame++;
                    count = 0;

                    for(int k = 0;k<N*M;k++){
                            printf(" |0x%02x| ",(unsigned char)fstring[k]);
                            if((k+1)%M == 0){
                                printf("\n");
                            }
                        
                        
                        }
                    }

                    //DISPLAY HERE
                current = root;
                }



                    
                //DISPLAY FRAME HERE
                // OR RETURN FROM FUNCTION AND KEEP PARAMETERS
                


            }
        }
    fclose(outputFile);
    }

//Returns a decompressed frame, pass the last iteration bitshift and current decompression count
char* decompressHuffmanFrame(Node* root, char* bitstream, int bitstreamSize,int *shift, int *deCount) {
   char* fstring = calloc(M*N,1);


    int count = 0;
    int frame = 0;

    int x = 0;
    int y = 0;

    //need to start from the parameters
    char isFirstIt = 0xff;



    Node* current = root;
    for (int i = (*deCount); i < bitstreamSize; i++) {

        unsigned char currentChar = (unsigned char)bitstream[i];
        //silly check
        if(isFirstIt == 0x00){
            (*shift) = 7;
        }


        for (int j = (*shift); j >= 0; j--) {
            isFirstIt = 0x00;
            int bit = (currentChar >> j) & 1; 
            if (bit == 0) {
                current = current->left;
            } else {
                current = current->right;
            }

            if (current->left == NULL && current->right == NULL) {
  

                unsigned char WATCHDOG = current->data;
                //printf("WATCHDOG - 0x%x\n",WATCHDOG);
                fstring[count] = (unsigned char)current->data;
                count++;
                if(count == 96){

                   printf("BITSHIFT:%d  -- COUNT:%d\n",j,i); 
                    //we finished a whole byte, so we can return the next in the decount
                    if(j == 0){
                        *shift = 7;
                        *deCount = i+1;

                    }
                    else{
                        //if not, then we still have some bits in the next iteration, but we are past this one
                        (*shift) = j-1;
                        (*deCount) = i;
                    }

                    //

                    //printf("Frame %d\n -- BITSHIFT:%d  -- COUNT:%d",frame,j,i);
                    frame++;
                    count = 0;

                    for(int k = 0;k<N*M;k++){
                            printf(" |0x%02x| ",(unsigned char)fstring[k]);
                            if((k+1)%M == 0){
                                printf("\n");
                            }
                        
                        
                        }

                        return fstring;
                        


                    }



                    //DISPLAY HERE
                current = root;
                }



            
                


            }
        }

        return fstring;

    }



Node* buildHuffmanTree(char* huffmanCodes[]) {
    Node* root = createNode('\0'); 

    for (int i = 0; i < CHAR_RANGE; i++) {
        if (huffmanCodes[i] != NULL) {
            Node* current = root;
            for (size_t j = 0; j < strlen(huffmanCodes[i]); j++) {
                if (huffmanCodes[i][j] == '0') {
                    if (current->left == NULL) {
                        current->left = createNode('\0');
                    }
                    current = current->left;
                } else if (huffmanCodes[i][j] == '1') {
                    if (current->right == NULL) {
                        current->right = createNode('\0');
                    }
                    current = current->right;
                } else {
                    fprintf(stderr, "Invalid Huffman code.\n");
                    exit(EXIT_FAILURE);
                }
            }
            current->data = (unsigned char)characters[i]; 
        }
    }

    return root;
}


// Example usage
int main() {

    Node* root = buildHuffmanTree(huffmanCodes);
    int c = 0;

    int shift = 7;
    int _fCount = 0;
    while(c != 20){

        printf("Frame %d \n",c);



        char(* frame)[4] = decompressHuffmanFrame(root,data,data_size,&shift,&_fCount);

        free(frame);

        c++;

    }
    //decompressHuffman(root, data, data_size,"decompressed2.bin");



    return 0;
}
