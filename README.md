<div align="center">

File Compression Tool

Fast, low-level file compression built from scratch in C

<p>
  <img src="https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white" alt="C">
  <img src="https://img.shields.io/badge/GCC-323330?style=for-the-badge&logo=gnu&logoColor=white" alt="GCC">
  <img src="https://img.shields.io/badge/Makefile-427819?style=for-the-badge&logo=gnu&logoColor=white" alt="Makefile">
  <img src="https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white" alt="Windows">
</p>

<p>
  <strong>Huffman Coding • Bit Manipulation • Binary Files • CRC32 • ZIP Containers</strong>
</p>

</div>

📌 About

Huffman File Compression Tool is a command-line compression utility written completely in C.

It implements the Huffman Coding algorithm from scratch to compress and decompress both text and binary files. The generated custom .mc file is packaged inside a ZIP container.

🎯 Core idea: understand how real compression works at the byte, bit, tree, and file-format levels — without relying on a ready-made compression library for the Huffman algorithm.

✨ Features

Feature

Description

🧠 Huffman Coding

Builds a frequency-based Huffman tree and generates prefix codes

📦 Compression

Converts input bytes into a compressed bitstream

🔄 Decompression

Rebuilds the tree and restores the original file

📁 Text Files

Supports ordinary text files

🖼️ Binary Files

Supports files such as images and other binary data

🧩 Custom .mc Format

Stores the information required for decompression

🗜️ ZIP Container

Packages the .mc file into a ZIP archive

🔐 CRC32

Verifies the integrity of the extracted .mc file

⚙️ Bit-Level Processing

Performs compression and storage at the bit level

💻 CLI

Simple command-line interface using file paths

🛠️ Tech Stack

<p>
  <img src="https://img.shields.io/badge/C-Programming%20Language-00599C?style=flat-square&logo=c&logoColor=white">
  <img src="https://img.shields.io/badge/GCC-Compiler-323330?style=flat-square&logo=gnu&logoColor=white">
  <img src="https://img.shields.io/badge/MinGW-Toolchain-8A2BE2?style=flat-square">
  <img src="https://img.shields.io/badge/Make-Build%20System-427819?style=flat-square&logo=gnu&logoColor=white">
  <img src="https://img.shields.io/badge/Git-GitHub-F05032?style=flat-square&logo=git&logoColor=white">
</p>

🧠 Concepts Used

Structures and pointers

Dynamic memory allocation

Arrays and strings

Binary trees

Priority queues

Recursion

File handling

Binary file handling

Bit manipulation

Huffman coding

CRC32

ZIP file structure

Command-line arguments

🔄 How It Works

Compression Pipeline

                    ┌──────────────┐
                    │  Input File  │
                    └──────┬───────┘
                           ↓
                  ┌─────────────────┐
                  │  Read File Bytes│
                  └────────┬────────┘
                           ↓
                  ┌─────────────────┐
                  │ Frequency Table │
                  └────────┬────────┘
                           ↓
                  ┌─────────────────┐
                  │  Priority Queue │
                  └────────┬────────┘
                           ↓
                  ┌─────────────────┐
                  │  Huffman Tree   │
                  └────────┬────────┘
                           ↓
                  ┌─────────────────┐
                  │  Huffman Codes  │
                  └────────┬────────┘
                           ↓
                  ┌─────────────────┐
                  │   Bit Packing   │
                  └────────┬────────┘
                           ↓
                  ┌─────────────────┐
                  │    .mc File     │
                  └────────┬────────┘
                           ↓
                  ┌─────────────────┐
                  │  ZIP Container  │
                  └────────┬────────┘
                           ↓
                    ┌────────────┐
                    │ output.zip │
                    └────────────┘

Decompression Pipeline

 input.zip
    │
    ▼
Extract .mc
    │
    ▼
Read Header
    │
    ▼
Frequency Table
    │
    ▼
Rebuild Huffman Tree
    │
    ▼
Read Compressed Bits
    │
    ▼
Traverse Huffman Tree
    │
    ▼
Original File

🌳 Huffman Coding

Huffman coding is a variable-length prefix coding algorithm.

Bytes that occur more frequently receive shorter codes, while less frequent bytes receive longer codes.

Example:

Character    Frequency    Code
--------------------------------
m               2         0000
p               2         0001
r               2         0010
t               2         0011
o               5         011
e               4         1100

The exact codes depend on the frequency distribution of the input file.

Huffman Tree

The tree is constructed using a priority queue:

Find the two nodes with the smallest frequencies.

Combine them into a new internal node.

Insert the new node back into the queue.

Repeat until one root node remains.

During decoding:

0 → move left
1 → move right

When a leaf node is reached, its byte is decoded.

📦 Custom .mc Format

The custom compressed file is identified by:

MC01

The format contains information required for decompression:

Magic identifier

Original file size

Number of unique bytes

Byte-frequency information

Number of valid bits

Huffman-compressed bitstream

This information allows the Huffman tree to be rebuilt during decompression.

🗜️ ZIP Integration

The custom Huffman-compressed .mc file is packaged inside a ZIP container:

Original File
      │
      ▼
Huffman Compression
      │
      ▼
compressed.mc
      │
      ▼
ZIP Container
      │
      ▼
output.zip

Important: Huffman coding is the actual compression algorithm implemented by this project. The ZIP layer is used as a container for the custom .mc file.

🔐 CRC32 Verification

CRC32 is used to verify the integrity of the .mc file stored inside the ZIP container.

ZIP
 │
 ▼
Extract .mc
 │
 ▼
Calculate CRC32
 │
 ▼
Compare with stored CRC32
 │
 ├── ✓ Valid
 └── ✗ Invalid

This helps detect corrupted ZIP contents.

📂 Project Structure

C project/
│
├── main.c
├── Makefile
├── README.md
└── test.txt

After compilation:

C project/
│
├── main.c
├── Makefile
├── README.md
├── test.txt
└── mycompress.exe

Generated ZIP and restored files are created when the program is executed.

⚙️ Requirements

You need:

GCC

MinGW

mingw32-make

Windows PowerShell

Check GCC

gcc --version

Check Make

mingw32-make --version

🚀 Build

Open PowerShell inside the project directory and run:

mingw32-make

This creates:

mycompress.exe

Clean Build

mingw32-make clean

💻 Usage

Show Help

.\mycompress.exe -h

Compress a File

.\mycompress.exe compress <input-file> <output.zip>

Example:

.\mycompress.exe compress test.txt test.zip

Decompress a File

.\mycompress.exe decompress <input.zip> <output-file>

Example:

.\mycompress.exe decompress test.zip restored.txt

🖼️ Binary File Example

The program can also process binary files such as images.

Compress

.\mycompress.exe compress "C:\path\to\dog.jpg" "C:\path\to\dog.zip"

Decompress

.\mycompress.exe decompress "C:\path\to\dog.zip" "C:\path\to\dog_restored.jpg"

🧪 Testing & Verification

After decompression, the original and restored files can be compared byte-by-byte.

Text File

fc.exe /b test.txt restored.txt

Expected result:

FC: no differences encountered

Binary File

fc.exe /b dog.jpg dog_restored.jpg

This verifies that the restored binary file matches the original byte-for-byte.

🔁 Complete Workflow

1. Build
   │
   └── mingw32-make

2. Compress
   │
   └── .\mycompress.exe compress test.txt test.zip

3. Decompress
   │
   └── .\mycompress.exe decompress test.zip restored.txt

4. Verify
   │
   └── fc.exe /b test.txt restored.txt


<div align="center">

Built with C • Huffman Coding • Curiosity

A hands-on exploration of how file compression works under the hood.

</div>