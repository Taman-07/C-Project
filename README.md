# VIRTUAL OS 


## Commands ->

mingw32-make
.\mycompress.exe
del mycompress.exe

## Workflow 

Frequency Table
      ↓
Create Nodes
      ↓
Put Nodes in Priority Queue
      ↓
Extract minimum
      ↓
Extract minimum
      ↓
Combine them
      ↓
Put new node back
      ↓
Repeat until ONE node remains



Compression                     Decompression

Original file                   .mc file
     ↓                              ↓
Frequency table                 Read metadata
     ↓                              ↓
Huffman tree        ↔           Reconstruct tree
     ↓                              ↓
Codes                           Read bits
     ↓                              ↓
Encode                          Traverse tree
     ↓                              ↓
Compressed bits                Reach leaf
     ↓                              ↓
.mc file                       Original byte




                 INPUT FILE
                     │
                     ▼
              Huffman compression
                     │
                     ▼
                  file.mc
                     │
                     ▼
              ZIP packaging
                     │
                     ▼
                  file.zip



                  file.zip
                     │
                     ▼
                unzip .mc
                     │
                     ▼
                  file.mc
                     │
                     ▼
             Rebuild Huffman tree
                     │
                     ▼
               Original file
               