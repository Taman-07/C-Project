# **File Compression Tool**

A command-line file compression utility written completely in **C**.

The project implements the **Huffman Coding algorithm from scratch** to compress and decompress text and binary files.

---

## **Project Description**

The goal of this project is to understand and implement a real file compression system using low-level C programming concepts.

The program:

- Reads an input file using a file path
- Calculates byte frequencies
- Builds a Huffman tree
- Creates Huffman codes
- Converts the codes into binary bits
- Stores the compressed data in a custom `.mc` format
- Packages the `.mc` file into a ZIP container
- Extracts the `.mc` file during decompression
- Rebuilds the Huffman tree
- Decodes the compressed bitstream
- Recreates the original file

The project is implemented in **C only**.

---

## **Features**

- Huffman compression
- Huffman decompression
- Command-line file paths
- Text file support
- Binary file support
- Custom `.mc` compressed format
- ZIP container creation
- ZIP extraction
- CRC32 verification
- Bit-level file processing
- Original file verification

---

## **Technologies Used**

- C
- GCC
- MinGW
- Makefile
- Windows PowerShell
- Git & GitHub

---

## **Concepts Used**

- Structures
- Pointers
- Dynamic memory allocation
- Binary trees
- Priority queues
- Arrays
- Strings
- Recursion
- File handling
- Binary file handling
- Bit manipulation
- Huffman coding
- CRC32
- ZIP file structure
- Command-line arguments

---

## **Compression**

The compression process follows:

```text
Input File
    |
    v
Read File Bytes
    |
    v
Frequency Table
    |
    v
Priority Queue
    |
    v
Huffman Tree
    |
    v
Huffman Codes
    |
    v
Bit Packing
    |
    v
.mc File
    |
    v
ZIP Container
    |
    v
output.zip
```

---

## **Decompression**

Decompression performs the reverse operation:

```text
input.zip
    |
    v
Extract .mc
    |
    v
Read Header
    |
    v
Frequency Table
    |
    v
Rebuild Huffman Tree
    |
    v
Read Bits
    |
    v
Traverse Huffman Tree
    |
    v
Original File
```

---

## **Requirements**

You need:

- GCC
- MinGW
- `mingw32-make`
- Windows PowerShell

Check GCC:

```powershell
gcc --version
```

Check Make:

```powershell
mingw32-make --version
```

---

## **Compilation**

Open PowerShell inside the project directory.

Run:

```powershell
mingw32-make
```

This executes the Makefile and creates:

```text
mycompress.exe
```

To clean the compiled executable:

```powershell
mingw32-make clean
```

---

## **Decompress a File**

```powershell
.\mycompress.exe decompress <input.zip> <output-file>
```

Example:

```powershell
.\mycompress.exe decompress test.zip restored.txt
```

For an image:

```powershell
.\mycompress.exe decompress "C:\path\to\dog.zip" "C:\path\to\dog_restored.jpg"
```

---

The project combines data structures, algorithms, memory management, file handling, binary processing, and low-level bit manipulation into one practical C application.
