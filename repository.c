#include "headers.h"

void writeFile(bTree *tree, bTreeNode *p, int pos)
{
    if (pos == -1)
    {
        pos = tree->nextPos++;
    }

    fseek(tree->fp, pos * sizeof(bTreeNode), 0);
    fwrite(p, sizeof(bTreeNode), 1, tree->fp);
}

void writeFileRecord(bTree *tree, recordNode *p, int pos)
{
    if (pos == -1)
    {
        pos = tree->nextRecPos++;
    }

    fseek(tree->fData, pos * sizeof(recordNode), 0);
    fwrite(p, sizeof(recordNode), 1, tree->fData);
}

void readFile(bTree *tree, bTreeNode *p, int pos)
{
    p->valid = true;
    fseek(tree->fp, pos * sizeof(bTreeNode), SEEK_SET);
    fread(p, sizeof(bTreeNode), 1, tree->fp);
}

void readRecordFile(bTree *tree, recordNode *reg, int pos)
{

    fseek(tree->fData, pos * sizeof(recordNode), SEEK_SET);
    fread(reg, sizeof(recordNode), 1, tree->fData);
}

bTree *createTree(char *fileName, char *recordFileName, bool mode)
{
    bTree *tree = (bTree *)malloc(sizeof(bTree));

    if (!mode) // new file
    {
        strcpy(tree->fileName, fileName);
        tree->fp = fopen(fileName, "w");
        fclose(tree->fp);

        strcpy(tree->recordFileName, recordFileName);
        tree->fData = fopen(recordFileName, "w");
        fclose(tree->fData);

        tree->root = 0;
        tree->nextPos = 0;
        tree->nextRecPos = 0;
    }
    else
    {
        FILE *existingFile = fopen(fileName, "r");
        fread(tree, sizeof(bTree), 1, existingFile);
        fclose(existingFile);
    }

    tree->fp = fopen(fileName, "r+");
    tree->fData = fopen(recordFileName, "r+");

    return tree;
}
