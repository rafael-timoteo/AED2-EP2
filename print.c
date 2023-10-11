#include "headers.h"

void printArvore(bTree *arvore)
{
    printf("nome do arquivo da arvore: %s\nnome do arquivo de registros: %s\nponteiro para o arquivo da arvore: %p\nponteiro para o arquivo de registros: %p\nraiz: %i\nproxima posicao: %i\nproxima posicao do arquivo de registros: %i\n\n", arvore->fileName, arvore->recordFileName, arvore->fp, arvore->fData, arvore->root, arvore->nextPos, arvore->nextRecPos);
}

void printNo(recordNode *no)
{
    printf("%i\t%s\t%s\t%i\n\n", no->codigoLivro, no->titulo, no->nomeCompletoPrimeiroAutor, no->anoPublicacao);
}

void printNoArvore(bTreeNode *no)
{

    printf("e valido: %i\nnumero de registros: %i\neh folha: %i\nposicao: %i\n", no->valid, no->noOfRecs, no->isLeaf, no->pos);

    puts("keyRecArr:");
    for (int i = 0; i < 2 * t - 1; i++)
        printf("  %i", no->keyRecArr[i]);

    puts("");
    puts("posRecArr:");
    for (int i = 0; i < 2 * t - 1; i++)
        printf("  %i", no->posRecArr[i]);

    puts("");
    puts("children:");
    for (int i = 0; i < 2 * t; i++)
        printf("  %i", no->children[i]);

    printf("\n\n");
}

void printArqIndice(bTree *tree)
{
    if (tree != NULL)
    {
        recordNode *reg = (recordNode *)malloc(sizeof(recordNode));

        for (int i = 0; i < tree->nextRecPos; i++)
        {
            readRecordFile(tree, reg, i);
            printNo(reg);
        }

        free(reg);
    }
}

void printArqData(bTree *tree)
{
    if (tree != NULL)
    {

        bTreeNode *reg = (bTreeNode *)malloc(sizeof(bTreeNode));
        for (int i = 0; i < tree->nextPos; i++)
        {
            fseek(tree->fp, i * sizeof(bTreeNode), SEEK_SET);
            fread(reg, sizeof(bTreeNode), 1, tree->fp);

            if (reg->isLeaf <= 1)
                printNoArvore(reg);
        }

        free(reg);
    }
}

void dispNode(bTreeNode *node)
{
    printf("Position in node:%d\n", node->pos);
    printf("Number of Records:%d\n", node->noOfRecs);
    printf("Is leaf?:%d\n", node->isLeaf);
    printf("CodigoLivro:\n");
    for (int i = 0; i < node->noOfRecs; i++)
    {
        printf("%d ", node->keyRecArr[i]);
    }
    printf("\n");
    printf("Links:\n");
    for (int i = 0; i < 2 * t; ++i)
    {
        printf("%d ", node->children[i]);
    }
    printf("\n");
    printf("\n");
}

// aparentemente, imprime a árvore completa
void traverse(bTree *tree, int root)
{
    if (root == -1)
    {
        return;
    }

    bTreeNode *toPrint = malloc(sizeof(bTreeNode));
    readFile(tree, toPrint, root);
    dispNode(toPrint);

    if (!toPrint->isLeaf)
    {
        for (int i = 0; i < 2 * t; i++)
        {
            traverse(tree, toPrint->children[i]);
        }

        free(toPrint);
    }
}

// imprime o nó da árvore

void hardPrint(bTree *tree)
{
    bTreeNode *lido = (bTreeNode *)malloc(sizeof(bTreeNode));
    for (int i = 0; i < tree->nextPos; i++)
    {
        fseek(tree->fp, i * sizeof(bTreeNode), SEEK_SET);
        fread(lido, sizeof(bTreeNode), 1, tree->fp);

        if (lido->isLeaf <= 1)
            dispNode(lido);
        else
            printf("ERRO: isLeaf = %i\n\n", lido->isLeaf);
    }

    free(lido);
}

void doublePrint(bTree *tree)
{
    printf("=================");
    printf("\nTraverse\n");
    traverse(tree, tree->root);

    printf("=================");
    printf("\nHard print\n");
    hardPrint(tree);
}