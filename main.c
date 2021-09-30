#include <iostream>
#include <stdio.h>
#include <dirent.h>
#include <cstring>

typedef struct TrieNode {
    struct TrieNode *chars[26]; //26 characters to represent letters a-z
    char character;
    bool lastWordCharacter;
} TrieNode;

TrieNode* createNewNode()
{
    TrieNode *node = (TrieNode *)(malloc(sizeof(TrieNode)));
    node->lastWordCharacter = false;

    for (int i = 0; i < 26; i++) {
        node->chars[i] = NULL;
    }

    return node;
}

// Inserts all the file names into the trie
void insert(TrieNode *root, char *str)
{
    TrieNode *currentNode = root;

    if ((strlen(str)) != 0) {
        char currentChar = *str;
        if (currentNode->chars[currentChar - 'a'] == NULL) {
            struct TrieNode *newNode = createNewNode();
            newNode->character = currentChar;
            currentNode->chars[currentChar- 'a'] = newNode;
        }
        // Goes to next char
        str++;
        insert(currentNode->chars[currentChar - 'a'], str);
    } else {
        currentNode->lastWordCharacter = true;
    }

    return;
}

// Using the prefix given, searches for any completed words.
void printResults(char *result, int depth, TrieNode *root) {

    int i;

    if (root->lastWordCharacter) {
        result[depth] = 0;
        printf("%s \n", result);
    }

    for (i=0; i< 26; i++) {
        if (root->chars[i]!=NULL && (strstr(result, result) != NULL)) {
            result[depth] = i + 'a';
                printResults(result, depth + 1, root->chars[i]);
            }
        }
}

// Creates a prefix base to send to the print function. This sets a minimum depth. Ex: if the prefix is "zip", the depth is 3.
void generatePrefix(TrieNode* root, char str[], int depth, char *prefix) {
    if (root == NULL) {
        return;
    }

    int i;

    for (i=0; i< 26; i++) {
        if (root->chars[i]!=NULL) {
            char currentChar = *str;
            if (root->chars[i]->character == currentChar) {
                prefix[depth] = i + 'a';
                str++;
                generatePrefix(root->chars[i], str, depth + 1, prefix);
            } else if (currentChar == NULL && !(depth < strlen(prefix)) ) {
                printResults(prefix, depth, root);
                return;
            }
        }
    }
}

void to_lowercase(char *c) {
    int length = strlen(c);

    for (int i = 0; i < length; i++) {
        if (isupper(c[i])) {
            c[i] = tolower(c[i]);
        }
    }
}

int main(void)
{
    struct dirent *de;
    char folderName[100];
    printf("Enter a folder name: ");
    fgets(folderName, 100, stdin);
    // Removes newline character
    folderName[strcspn(folderName, "\n")] = 0;

    DIR *dr = opendir(folderName);

    if (dr == NULL)
    {
        printf("Could not open current directory" );
        return 0;
    }

    TrieNode* root = createNewNode();

    // Ignores any hidden files (including parent and child directories) in the folder.
    while ((de = readdir(dr)) != NULL) {
        if (de->d_name[0] != '.') {
            to_lowercase(de->d_name);
            insert(root, de->d_name);
        }
    }


    closedir(dr);

    char searchTerm[100];

    for (;;) {
        printf("> ");
        fgets(searchTerm, 100, stdin);
        // Removes newline character
        searchTerm[strcspn(searchTerm, "\n")] = 0;

        if (searchTerm[0] == '\0') {
            printf("No search term entered. Stopping Program.");
            return 0;
        } else {
            char *prefix = NULL;
            prefix = (char*) malloc(26);

            // Making sure 'prefix' is empty to store the next search term.
            if (prefix != NULL) {
                free(prefix);
                prefix = NULL;
                prefix = (char*) malloc(26);
            }

            printf("Files starting with %s in %s \n", searchTerm, folderName);
            generatePrefix(root, searchTerm, 0, prefix);
        }
    }
}
