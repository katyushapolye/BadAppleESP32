#ifndef FRAME_H
#define FRAME_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern char clear[8][128];
extern char font[128][8];

extern char frame1[64][16];

void SWAP(int *a, int *b);
void SWAP_BUFFER(char *a, char *b, size_t s);

// bitwise operations

char reverse(char b);
unsigned short int interleave(char x, char y);

// Huffman decompression
#define CHAR_RANGE 234
extern char characters[];
extern char *huffmanCodes[];
typedef struct
{
    unsigned char data;
    struct Node *left;
    struct Node *right;
} Node;

Node *createNode(unsigned char data);
Node *buildHuffmanTree(char *huffmanCodes[]);
char *decompressHuffmanFrame(Node *root, unsigned char *bitstream, int bitstreamSize, int *shift, int *deCount);
#endif