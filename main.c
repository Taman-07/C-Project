#include <stdio.h>
#include <stdlib.h>

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


Node *createNode(unsigned char data, int frequency) {

    Node *newNode = malloc(sizeof(Node));

    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    newNode->data = data;
    newNode->frequency = frequency;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}


PriorityQueue *createPriorityQueue(int capacity) {

    PriorityQueue *queue = malloc(sizeof(PriorityQueue));

    if (queue == NULL) {
        return NULL;
    }

    queue->nodes = malloc(capacity * sizeof(Node *));

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
           queue->nodes[i]->frequency > node->frequency) {

        queue->nodes[i + 1] = queue->nodes[i];
        i--;
    }

    queue->nodes[i + 1] = node;

    queue->size++;
}


Node *extractMin(PriorityQueue *queue) {

    if (queue->size == 0) {
        return NULL;
    }

    Node *minNode = queue->nodes[0];

    for (int i = 1; i < queue->size; i++) {
        queue->nodes[i - 1] = queue->nodes[i];
    }

    queue->size--;

    return minNode;
}


int main() {

    FILE *file = fopen("test.txt", "rb");

    if (file == NULL) {
        printf("Could not open file\n");
        return 1;
    }

    int frequency[256] = {0};
    int byte;

    // Count frequency
    while ((byte = fgetc(file)) != EOF) {
        frequency[byte]++;
    }

    fclose(file);


    // Count unique bytes
    int nodeCount = 0;

    for (int i = 0; i < 256; i++) {
        if (frequency[i] > 0) {
            nodeCount++;
        }
    }


    // Create priority queue
    PriorityQueue *queue = createPriorityQueue(256);

    if (queue == NULL) {
        printf("Could not create priority queue\n");
        return 1;
    }


    // Create nodes and insert them into queue
    for (int i = 0; i < 256; i++) {

        if (frequency[i] > 0) {

            Node *node = createNode(
                (unsigned char)i,
                frequency[i]
            );

            if (node == NULL) {
                return 1;
            }

            insert(queue, node);
        }
    }


    // Display priority queue
    printf("Priority Queue:\n");
    printf("----------------\n");

    for (int i = 0; i < queue->size; i++) {

        printf("Node %d: ", i);
        printf("Byte = %d, ", queue->nodes[i]->data);
        printf("Frequency = %d",
               queue->nodes[i]->frequency);

        if (queue->nodes[i]->data >= 32 &&
            queue->nodes[i]->data <= 126) {

            printf(", Character = %c",
                   queue->nodes[i]->data);
        }

        printf("\n");
    }


    // Test extractMin()
    Node *minNode = extractMin(queue);

    if (minNode != NULL) {

        printf("\nExtracted Minimum:\n");
        printf("-------------------\n");

        printf("Byte = %d\n", minNode->data);
        printf("Frequency = %d\n", minNode->frequency);

        if (minNode->data >= 32 &&
            minNode->data <= 126) {

            printf("Character = %c\n",
                   minNode->data);
        }

        // We are not freeing minNode yet because
        // we will need these nodes for the Huffman tree.
    }


    // Display queue after extraction
    printf("\nPriority Queue After Extraction:\n");
    printf("---------------------------------\n");

    for (int i = 0; i < queue->size; i++) {

        printf("Node %d: Byte = %d, Frequency = %d\n",
               i,
               queue->nodes[i]->data,
               queue->nodes[i]->frequency);
    }


    // Cleanup
    if (minNode != NULL) {
        free(minNode);
    }

    for (int i = 0; i < queue->size; i++) {
        free(queue->nodes[i]);
    }

    free(queue->nodes);
    free(queue);

    return 0;
}