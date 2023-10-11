#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

struct rec
{
	bool valid;		 // indica se o registro foi eliminado ou não
	int codigoLivro; // chave
	char titulo[30];
	char nomeCompletoPrimeiroAutor[30];
	int anoPublicacao;
};
typedef struct rec recordNode;

struct bTreeNode
{
	bool valid; // indica se o nó da árvore B foi eliminado ou não
	int noOfRecs;
	bool isLeaf;
	int pos;
	int keyRecArr[2 * t - 1]; // as chaves dos registros, que são no máximo 2t-1
	int posRecArr[2 * t - 1]; // posições dos registros no arquivo data.dat, que são no máximo 2t-1
	int children[2 * t];	  // posições das páginas filhas no arquivo tree.dat, que são no máximo 2t
};
typedef struct bTreeNode bTreeNode;

struct tree
{
	char fileName[20];
	char recordFileName[20];
	FILE *fp;
	FILE *fData;
	int root;
	int nextPos;
	int nextRecPos;
};
typedef struct tree bTree;

bTree *createTree(char *fileName, char *recordFileName, bool mode);
bTreeNode *nodeInit(bTreeNode *node, bool isLeaf, bTree *tree);
void insert(bTree *tree, int codigoLivro, int pos);
void delete(bTree *tree, int key);
void traverse(bTree *tree, int root);
void dispNode(bTreeNode *node);
void writeFile(bTree *ptr_tree, bTreeNode *p, int pos);
void readFile(bTree *ptr_tree, bTreeNode *p, int pos);

void enterData(recordNode *record, bool valid, int id_num, char titulo[], char nomeCompletoPrimeiroAutor[], int anoPublicacao);
recordNode *getData(char *filepath, int len);
recordNode *search(bTree *tree, int key);
int searchRecursive(bTree *tree, int key, bTreeNode *root);
bool removeFromTree(bTree *tree, int key);
bTreeNode *merge(bTree *tree, bTreeNode *node, int idx);
void borrowFromNext(bTree *tree, bTreeNode *node, int idx);
void borrowFromPrev(bTree *tree, bTreeNode *node, int idx);
void fill(bTree *tree, bTreeNode *node, int idx);
recordNode *getSucc(bTree *tree, bTreeNode *node, int idx);
recordNode *getPred(bTree *tree, bTreeNode *node, int idx);
void removeFromNonLeaf(bTree *tree, bTreeNode *node, int idx, bool removeRecord);
void removeFromLeaf(bTree *tree, bTreeNode *node, int idx, bool removeRecord);
void removeNode(bTree *tree, bTreeNode *node, int k, bool removeRec);
int findKey(bTreeNode *node, int k);
