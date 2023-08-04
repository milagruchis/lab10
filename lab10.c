#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie node structure
struct TrieNode
{
    int count; // To store the count of occurrences of the word
    struct TrieNode *children[26]; // One for each lowercase letter
};

// Trie structure
struct Trie
{
    struct TrieNode *root;
};

// Creates a new Trie node
struct TrieNode *createTrieNode()
{
    struct TrieNode *newNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    newNode->count = 0;
    for (int i = 0; i < 26; i++)
    {
        newNode->children[i] = NULL;
    }
    return newNode;
}

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie *newTrie = (struct Trie *)malloc(sizeof(struct Trie));
    newTrie->root = createTrieNode();
    return newTrie;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct TrieNode *current = pTrie->root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        if (current->children[index] == NULL)
        {
            current->children[index] = createTrieNode();
        }
        current = current->children[index];
    }
    // After reaching the end of the word, increase the count
    current->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct Trie *pTrie, char *word)
{
    struct TrieNode *current = pTrie->root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        if (current->children[index] == NULL)
        {
            return 0; // The word does not exist in the trie
        }
        current = current->children[index];
    }
    return current->count;
}

// Deallocates the trie structure
struct TrieNode *deallocateTrieNode(struct TrieNode *node)
{
    if (node == NULL)
        return NULL;

    for (int i = 0; i < 26; i++)
    {
        node->children[i] = deallocateTrieNode(node->children[i]);
    }

    free(node);
    return NULL;
}

struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (pTrie == NULL)
        return NULL;

    pTrie->root = deallocateTrieNode(pTrie->root);
    free(pTrie);
    return NULL;
}

// Reads the dictionary from the file and stores it in pInWords array.
// Returns the number of words read.
int readDictionary(char *filename, char **pInWords)
{
    int numWords = 0;
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening the file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int maxWords;
    fscanf(file, "%d", &maxWords);
    char word[100]; // Assuming no word in the dictionary will exceed 100 characters
    while (numWords < maxWords && fscanf(file, "%s", word) != EOF)
    {
        pInWords[numWords] = strdup(word);
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void)
{
    char *inWords[256];

    // Read the number of the words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    // Parse line by line, and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
		printf("There is an error in this program\n");
        
    // Free memory for words read from the dictionary
    for (int i = 0; i < numWords; ++i)
    {
        free(inWords[i]);
    }

    return 0;
}

