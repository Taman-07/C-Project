#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    unsigned char data;
    int frequency;

    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    Node **nodes;
    int size;
    int capacity;
} PriorityQueue;

unsigned int crc32Table[256];

Node *createNode(unsigned char data, int frequency) {

    Node *newNode = malloc(sizeof(Node));

    if (newNode == NULL) {
        return NULL;
    }

    newNode->data = data;
    newNode->frequency = frequency;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

PriorityQueue *createPriorityQueue(int capacity) {

    PriorityQueue *queue =
        malloc(sizeof(PriorityQueue));

    if (queue == NULL) {
        return NULL;
    }

    queue->nodes =
        malloc(capacity * sizeof(Node *));

    if (queue->nodes == NULL) {
        free(queue);
        return NULL;
    }

    queue->size = 0;
    queue->capacity = capacity;

    return queue;
}

void insert(PriorityQueue *queue, Node *node) {

    int i = queue->size - 1;

    while (i >= 0 &&
           queue->nodes[i]->frequency >
           node->frequency) {

        queue->nodes[i + 1] =
            queue->nodes[i];

        i--;
    }

    queue->nodes[i + 1] = node;

    queue->size++;
}

Node *extractMin(PriorityQueue *queue) {

    if (queue->size == 0) {
        return NULL;
    }

    Node *minNode =
        queue->nodes[0];

    for (int i = 1;
         i < queue->size;
         i++) {

        queue->nodes[i - 1] =
            queue->nodes[i];
    }

    queue->size--;

    return minNode;
}

Node *mergeNodes(
    Node *left,
    Node *right
) {

    Node *parent =
        createNode(
            0,
            left->frequency +
            right->frequency
        );

    if (parent == NULL) {
        return NULL;
    }

    parent->left = left;
    parent->right = right;

    return parent;
}

Node *buildHuffmanTree(
    PriorityQueue *queue
) {

    if (queue->size == 0) {
        return NULL;
    }

    if (queue->size == 1) {
        return extractMin(queue);
    }

    while (queue->size > 1) {

        Node *left =
            extractMin(queue);

        Node *right =
            extractMin(queue);

        Node *parent =
            mergeNodes(left, right);

        if (parent == NULL) {
            return NULL;
        }

        insert(queue, parent);
    }

    return extractMin(queue);
}

void freeTree(Node *root) {

    if (root == NULL) {
        return;
    }

    freeTree(root->left);
    freeTree(root->right);

    free(root);
}

void generateCodes(
    Node *root,
    char path[],
    int level,
    char codes[256][256]
) {

    if (root == NULL) {
        return;
    }

    if (root->left == NULL &&
        root->right == NULL) {

        if (level == 0) {

            path[0] = '0';
            path[1] = '\0';

        } else {

            path[level] = '\0';
        }

        strcpy(
            codes[root->data],
            path
        );

        return;
    }

    path[level] = '0';

    generateCodes(
        root->left,
        path,
        level + 1,
        codes
    );

    path[level] = '1';

    generateCodes(
        root->right,
        path,
        level + 1,
        codes
    );
}

unsigned long long getFileSize(
    const char *filename
) {

    FILE *file =
        fopen(filename, "rb");

    if (file == NULL) {
        return 0;
    }

    if (fseek(
        file,
        0,
        SEEK_END
    ) != 0) {

        fclose(file);
        return 0;
    }

    long size =
        ftell(file);

    fclose(file);

    if (size < 0) {
        return 0;
    }

    return (unsigned long long)size;
}

int buildFrequencyTable(
    const char *filename,
    int frequency[256]
) {

    FILE *file =
        fopen(filename, "rb");

    if (file == NULL) {
        return 0;
    }

    int byte;

    while ((byte = fgetc(file)) != EOF) {
        frequency[byte]++;
    }

    fclose(file);

    return 1;
}

int writeHeader(
    FILE *output,
    int frequency[256],
    unsigned long long originalSize
) {

    char magic[4] = {
        'M',
        'C',
        '0',
        '1'
    };

    if (fwrite(
        magic,
        1,
        4,
        output
    ) != 4) {

        return 0;
    }

    if (fwrite(
        &originalSize,
        sizeof(unsigned long long),
        1,
        output
    ) != 1) {

        return 0;
    }

    unsigned short uniqueCount = 0;

    for (int i = 0;
         i < 256;
         i++) {

        if (frequency[i] > 0) {
            uniqueCount++;
        }
    }

    if (fwrite(
        &uniqueCount,
        sizeof(unsigned short),
        1,
        output
    ) != 1) {

        return 0;
    }

    for (int i = 0;
         i < 256;
         i++) {

        if (frequency[i] > 0) {

            unsigned char byte =
                (unsigned char)i;

            if (fwrite(
                &byte,
                sizeof(unsigned char),
                1,
                output
            ) != 1) {

                return 0;
            }

            if (fwrite(
                &frequency[i],
                sizeof(int),
                1,
                output
            ) != 1) {

                return 0;
            }
        }
    }

    return 1;
}

int readHeader(
    FILE *input,
    int frequency[256],
    unsigned long long *originalSize,
    unsigned char *validBits
) {

    char magic[4];

    if (fread(
        magic,
        1,
        4,
        input
    ) != 4) {

        return 0;
    }

    if (magic[0] != 'M' ||
        magic[1] != 'C' ||
        magic[2] != '0' ||
        magic[3] != '1') {

        return 0;
    }

    if (fread(
        originalSize,
        sizeof(unsigned long long),
        1,
        input
    ) != 1) {

        return 0;
    }

    unsigned short uniqueCount;

    if (fread(
        &uniqueCount,
        sizeof(unsigned short),
        1,
        input
    ) != 1) {

        return 0;
    }

    if (uniqueCount > 256) {
        return 0;
    }

    for (int i = 0;
         i < uniqueCount;
         i++) {

        unsigned char byte;
        int count;

        if (fread(
            &byte,
            sizeof(unsigned char),
            1,
            input
        ) != 1) {

            return 0;
        }

        if (fread(
            &count,
            sizeof(int),
            1,
            input
        ) != 1) {

            return 0;
        }

        if (count <= 0) {
            return 0;
        }

        frequency[byte] = count;
    }

    if (fread(
        validBits,
        sizeof(unsigned char),
        1,
        input
    ) != 1) {

        return 0;
    }

    if (*validBits == 0 ||
        *validBits > 8) {

        return 0;
    }

    return 1;
}

void writeBit(
    FILE *output,
    unsigned char *buffer,
    int *bitCount,
    int bit
) {

    *buffer =
        (*buffer << 1) | bit;

    (*bitCount)++;

    if (*bitCount == 8) {

        fwrite(
            buffer,
            1,
            1,
            output
        );

        *buffer = 0;
        *bitCount = 0;
    }
}

void writeCode(
    FILE *output,
    const char *code,
    unsigned char *buffer,
    int *bitCount
) {

    for (int i = 0;
         code[i] != '\0';
         i++) {

        int bit =
            code[i] - '0';

        writeBit(
            output,
            buffer,
            bitCount,
            bit
        );
    }
}

int readBit(
    FILE *input,
    unsigned char *buffer,
    int *bitCount
) {

    if (*bitCount == 0) {

        int value =
            fgetc(input);

        if (value == EOF) {
            return -1;
        }

        *buffer =
            (unsigned char)value;

        *bitCount = 8;
    }

    int bit =
        (*buffer & 0x80) >> 7;

    *buffer <<= 1;

    (*bitCount)--;

    return bit;
}

int compressFile(
    const char *inputFile,
    const char *outputFile
) {

    int frequency[256] = {0};

    if (!buildFrequencyTable(
        inputFile,
        frequency
    )) {

        printf(
            "Error: Could not open input file.\n"
        );

        return 0;
    }

    unsigned long long originalSize =
        getFileSize(inputFile);

    if (originalSize == 0) {

        printf(
            "Error: Input file is empty.\n"
        );

        return 0;
    }

    PriorityQueue *queue =
        createPriorityQueue(256);

    if (queue == NULL) {

        printf(
            "Error: Memory allocation failed.\n"
        );

        return 0;
    }

    for (int i = 0;
         i < 256;
         i++) {

        if (frequency[i] > 0) {

            Node *node =
                createNode(
                    (unsigned char)i,
                    frequency[i]
                );

            if (node == NULL) {

                free(queue->nodes);
                free(queue);

                return 0;
            }

            insert(queue, node);
        }
    }

    Node *root =
        buildHuffmanTree(queue);

    if (root == NULL) {

        free(queue->nodes);
        free(queue);

        return 0;
    }

    char codes[256][256] = {0};
    char path[256];

    generateCodes(
        root,
        path,
        0,
        codes
    );

    FILE *input =
        fopen(inputFile, "rb");

    if (input == NULL) {

        freeTree(root);
        free(queue->nodes);
        free(queue);

        return 0;
    }

    FILE *output =
        fopen(outputFile, "wb");

    if (output == NULL) {

        printf(
            "Error: Could not create output file.\n"
        );

        fclose(input);

        freeTree(root);
        free(queue->nodes);
        free(queue);

        return 0;
    }

    if (!writeHeader(
        output,
        frequency,
        originalSize
    )) {

        fclose(input);
        fclose(output);

        freeTree(root);
        free(queue->nodes);
        free(queue);

        return 0;
    }

    unsigned char validBits = 0;

    long validBitsPosition =
        ftell(output);

    fwrite(
        &validBits,
        1,
        1,
        output
    );

    unsigned char buffer = 0;
    int bitCount = 0;

    int byte;

    while ((byte = fgetc(input)) != EOF) {

        writeCode(
            output,
            codes[byte],
            &buffer,
            &bitCount
        );
    }

    if (bitCount > 0) {

        buffer <<=
            (8 - bitCount);

        fwrite(
            &buffer,
            1,
            1,
            output
        );

        validBits =
            (unsigned char)bitCount;

    } else {

        validBits = 8;
    }

    fseek(
        output,
        validBitsPosition,
        SEEK_SET
    );

    fwrite(
        &validBits,
        1,
        1,
        output
    );

    fclose(input);
    fclose(output);

    freeTree(root);

    free(queue->nodes);
    free(queue);

    return 1;
}

int decompressFile(
    const char *inputFile,
    const char *outputFile
) {

    FILE *input =
        fopen(inputFile, "rb");

    if (input == NULL) {

        printf(
            "Error: Could not open .mc file.\n"
        );

        return 0;
    }

    int frequency[256] = {0};

    unsigned long long originalSize = 0;

    unsigned char validBits = 0;

    if (!readHeader(
        input,
        frequency,
        &originalSize,
        &validBits
    )) {

        printf(
            "Error: Invalid or corrupted .mc file.\n"
        );

        fclose(input);

        return 0;
    }

    PriorityQueue *queue =
        createPriorityQueue(256);

    if (queue == NULL) {

        fclose(input);

        return 0;
    }

    for (int i = 0;
         i < 256;
         i++) {

        if (frequency[i] > 0) {

            Node *node =
                createNode(
                    (unsigned char)i,
                    frequency[i]
                );

            if (node == NULL) {

                free(queue->nodes);
                free(queue);
                fclose(input);

                return 0;
            }

            insert(queue, node);
        }
    }

    Node *root =
        buildHuffmanTree(queue);

    if (root == NULL) {

        free(queue->nodes);
        free(queue);
        fclose(input);

        return 0;
    }

    FILE *output =
        fopen(outputFile, "wb");

    if (output == NULL) {

        printf(
            "Error: Could not create output file.\n"
        );

        freeTree(root);
        free(queue->nodes);
        free(queue);
        fclose(input);

        return 0;
    }

    if (root->left == NULL &&
        root->right == NULL) {

        for (unsigned long long i = 0;
             i < originalSize;
             i++) {

            fputc(
                root->data,
                output
            );
        }

    } else {

        Node *current = root;

        unsigned char buffer = 0;
        int bitCount = 0;

        unsigned long long decodedCount = 0;

        while (decodedCount <
               originalSize) {

            int bit =
                readBit(
                    input,
                    &buffer,
                    &bitCount
                );

            if (bit == -1) {

                fclose(input);
                fclose(output);

                freeTree(root);
                free(queue->nodes);
                free(queue);

                return 0;
            }

            if (bit == 0) {

                current =
                    current->left;

            } else {

                current =
                    current->right;
            }

            if (current == NULL) {

                fclose(input);
                fclose(output);

                freeTree(root);
                free(queue->nodes);
                free(queue);

                return 0;
            }

            if (current->left == NULL &&
                current->right == NULL) {

                fputc(
                    current->data,
                    output
                );

                decodedCount++;

                current = root;
            }
        }
    }

    fclose(input);
    fclose(output);

    freeTree(root);

    free(queue->nodes);
    free(queue);

    return 1;
}

void initCRC32() {

    for (unsigned int i = 0;
         i < 256;
         i++) {

        unsigned int crc = i;

        for (int j = 0;
             j < 8;
             j++) {

            if (crc & 1) {

                crc =
                    0xEDB88320 ^
                    (crc >> 1);

            } else {

                crc >>= 1;
            }
        }

        crc32Table[i] = crc;
    }
}

unsigned int calculateCRC32(
    const char *filename
) {

    FILE *file =
        fopen(filename, "rb");

    if (file == NULL) {
        return 0;
    }

    unsigned int crc =
        0xFFFFFFFF;

    int byte;

    while ((byte = fgetc(file)) != EOF) {

        crc ^=
            (unsigned char)byte;

        crc =
            crc32Table[crc & 0xFF] ^
            (crc >> 8);
    }

    fclose(file);

    return crc ^ 0xFFFFFFFF;
}

void writeUInt16(
    FILE *file,
    unsigned short value
) {

    fputc(
        value & 0xFF,
        file
    );

    fputc(
        (value >> 8) & 0xFF,
        file
    );
}

void writeUInt32(
    FILE *file,
    unsigned int value
) {

    fputc(
        value & 0xFF,
        file
    );

    fputc(
        (value >> 8) & 0xFF,
        file
    );

    fputc(
        (value >> 16) & 0xFF,
        file
    );

    fputc(
        (value >> 24) & 0xFF,
        file
    );
}

unsigned short readUInt16(
    FILE *file
) {

    unsigned short value = 0;

    value |=
        (unsigned short)fgetc(file);

    value |=
        (unsigned short)fgetc(file)
        << 8;

    return value;
}

unsigned int readUInt32(
    FILE *file
) {

    unsigned int value = 0;

    value |=
        (unsigned int)fgetc(file);

    value |=
        (unsigned int)fgetc(file)
        << 8;

    value |=
        (unsigned int)fgetc(file)
        << 16;

    value |=
        (unsigned int)fgetc(file)
        << 24;

    return value;
}

int createZip(
    const char *mcFile,
    const char *zipFile
) {

    FILE *input =
        fopen(mcFile, "rb");

    if (input == NULL) {
        return 0;
    }

    FILE *zip =
        fopen(zipFile, "wb");

    if (zip == NULL) {

        fclose(input);

        return 0;
    }

    unsigned long long fileSize =
        getFileSize(mcFile);

    if (fileSize >
        0xFFFFFFFFULL) {

        fclose(input);
        fclose(zip);

        return 0;
    }

    unsigned int size =
        (unsigned int)fileSize;

    unsigned int crc =
        calculateCRC32(mcFile);

    const char *filename =
        "compressed.mc";

    unsigned short filenameLength =
        (unsigned short)strlen(filename);

    unsigned int localHeaderOffset = 0;

    writeUInt32(
        zip,
        0x04034B50
    );

    writeUInt16(zip, 20);
    writeUInt16(zip, 0);
    writeUInt16(zip, 0);

    writeUInt16(zip, 0);
    writeUInt16(zip, 0);

    writeUInt32(zip, crc);

    writeUInt32(zip, size);
    writeUInt32(zip, size);

    writeUInt16(
        zip,
        filenameLength
    );

    writeUInt16(zip, 0);

    fwrite(
        filename,
        1,
        filenameLength,
        zip
    );

    unsigned char buffer[4096];

    size_t bytesRead;

    while ((bytesRead =
        fread(
            buffer,
            1,
            sizeof(buffer),
            input
        )) > 0) {

        fwrite(
            buffer,
            1,
            bytesRead,
            zip
        );
    }

    unsigned int centralDirectoryOffset =
        (unsigned int)ftell(zip);

    writeUInt32(
        zip,
        0x02014B50
    );

    writeUInt16(zip, 20);
    writeUInt16(zip, 20);

    writeUInt16(zip, 0);
    writeUInt16(zip, 0);

    writeUInt16(zip, 0);
    writeUInt16(zip, 0);

    writeUInt32(zip, crc);

    writeUInt32(zip, size);
    writeUInt32(zip, size);

    writeUInt16(
        zip,
        filenameLength
    );

    writeUInt16(zip, 0);
    writeUInt16(zip, 0);

    writeUInt16(zip, 0);
    writeUInt16(zip, 0);

    writeUInt32(zip, 0);

    writeUInt32(
        zip,
        localHeaderOffset
    );

    fwrite(
        filename,
        1,
        filenameLength,
        zip
    );

    unsigned int centralDirectorySize =
        (unsigned int)ftell(zip) -
        centralDirectoryOffset;

    writeUInt32(
        zip,
        0x06054B50
    );

    writeUInt16(zip, 0);
    writeUInt16(zip, 0);

    writeUInt16(zip, 1);
    writeUInt16(zip, 1);

    writeUInt32(
        zip,
        centralDirectorySize
    );

    writeUInt32(
        zip,
        centralDirectoryOffset
    );

    writeUInt16(zip, 0);

    fclose(input);
    fclose(zip);

    return 1;
}

int extractMCFromZip(
    const char *zipFile,
    const char *mcFile
) {

    FILE *zip =
        fopen(zipFile, "rb");

    if (zip == NULL) {
        return 0;
    }

    unsigned int signature =
        readUInt32(zip);

    if (signature !=
        0x04034B50) {

        fclose(zip);

        return 0;
    }

    readUInt16(zip);
    readUInt16(zip);

    unsigned short compression =
        readUInt16(zip);

    if (compression != 0) {

        fclose(zip);

        return 0;
    }

    readUInt16(zip);
    readUInt16(zip);

    unsigned int crc =
        readUInt32(zip);

    unsigned int compressedSize =
        readUInt32(zip);

    unsigned int uncompressedSize =
        readUInt32(zip);

    unsigned short filenameLength =
        readUInt16(zip);

    unsigned short extraLength =
        readUInt16(zip);

    fseek(
        zip,
        filenameLength +
        extraLength,
        SEEK_CUR
    );

    FILE *output =
        fopen(mcFile, "wb");

    if (output == NULL) {

        fclose(zip);

        return 0;
    }

    unsigned char buffer[4096];

    unsigned int remaining =
        compressedSize;

    while (remaining > 0) {

        unsigned int chunk =
            remaining >
            sizeof(buffer)
            ? sizeof(buffer)
            : remaining;

        size_t bytesRead =
            fread(
                buffer,
                1,
                chunk,
                zip
            );

        if (bytesRead == 0) {

            fclose(zip);
            fclose(output);

            return 0;
        }

        fwrite(
            buffer,
            1,
            bytesRead,
            output
        );

        remaining -=
            (unsigned int)bytesRead;
    }

    fclose(zip);
    fclose(output);

    unsigned int extractedCRC =
        calculateCRC32(mcFile);

    unsigned long long extractedSize =
        getFileSize(mcFile);

    if (extractedCRC != crc ||
        extractedSize !=
        uncompressedSize) {

        remove(mcFile);

        return 0;
    }

    return 1;
}

int compressToZip(
    const char *inputFile,
    const char *zipFile
) {

    char temporaryMC[1024];

    snprintf(
        temporaryMC,
        sizeof(temporaryMC),
        "%s.temp.mc",
        zipFile
    );

    printf(
        "\nCreating Huffman compressed file...\n"
    );

    if (!compressFile(
        inputFile,
        temporaryMC
    )) {

        return 0;
    }

    printf(
        "Creating ZIP container...\n"
    );

    if (!createZip(
        temporaryMC,
        zipFile
    )) {

        printf(
            "Error: Could not create ZIP file.\n"
        );

        remove(temporaryMC);

        return 0;
    }

    remove(temporaryMC);

    printf(
        "\nZIP creation successful!\n"
    );

    printf(
        "ZIP file : %s\n",
        zipFile
    );

    return 1;
}

int decompressFromZip(
    const char *zipFile,
    const char *outputFile
) {

    char temporaryMC[1024];

    snprintf(
        temporaryMC,
        sizeof(temporaryMC),
        "%s.temp.mc",
        zipFile
    );

    printf(
        "\nExtracting Huffman file from ZIP...\n"
    );

    if (!extractMCFromZip(
        zipFile,
        temporaryMC
    )) {

        printf(
            "Error: Invalid or unsupported ZIP file.\n"
        );

        return 0;
    }

    printf(
        "Huffman file extracted.\n"
    );

    printf(
        "Starting Huffman decompression...\n"
    );

    if (!decompressFile(
        temporaryMC,
        outputFile
    )) {

        remove(temporaryMC);

        return 0;
    }

    remove(temporaryMC);

    printf(
        "\nDecompression successful!\n"
    );

    printf(
        "Output file : %s\n",
        outputFile
    );

    return 1;
}

void printUsage() {

    printf("\n");
    printf(
        "Huffman File Compression Tool\n"
    );
    printf(
        "==============================\n\n"
    );

    printf("Compress:\n");

    printf(
        "  mycompress compress <input> <output.zip>\n\n"
    );

    printf("Decompress:\n");

    printf(
        "  mycompress decompress <input.zip> <output>\n\n"
    );

    printf("Examples:\n");

    printf(
        "  mycompress compress test.txt test.zip\n"
    );

    printf(
        "  mycompress decompress test.zip restored.txt\n\n"
    );

    printf("Help:\n");

    printf(
        "  mycompress -h\n"
    );
}

int main(
    int argc,
    char *argv[]
) {

    initCRC32();

    if (argc == 2 &&
        (strcmp(
            argv[1],
            "-h"
        ) == 0 ||

         strcmp(
            argv[1],
            "--help"
         ) == 0)) {

        printUsage();

        return 0;
    }

    if (argc == 4 &&
        strcmp(
            argv[1],
            "compress"
        ) == 0) {

        if (compressToZip(
            argv[2],
            argv[3]
        )) {

            return 0;
        }

        return 1;
    }

    if (argc == 4 &&
        strcmp(
            argv[1],
            "decompress"
        ) == 0) {

        if (decompressFromZip(
            argv[2],
            argv[3]
        )) {

            return 0;
        }

        return 1;
    }

    printUsage();

    return 1;
}
