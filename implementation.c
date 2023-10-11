#include "headers.h"

void insert(bTree *tree, int codigoLivro, int pos)
{
    if (tree->nextPos == 0) // árvore vazia, primeiro elemento.
    {
        tree->root = tree->nextPos;

        bTreeNode *firstNode = malloc(sizeof(bTreeNode));
        nodeInit(firstNode, true, tree);
        firstNode->valid = true;
        firstNode->keyRecArr[0] = codigoLivro;
        firstNode->posRecArr[0] = pos;
        (firstNode->noOfRecs)++;

        writeFile(tree, firstNode, firstNode->pos);

        free(firstNode);
        return;
    }
    else
    {
        bTreeNode *rootCopy = malloc(sizeof(bTreeNode));
        readFile(tree, rootCopy, tree->root);

        if (rootCopy->noOfRecs == (2 * t - 1))
        {
            bTreeNode *registro = malloc(sizeof(bTreeNode));
            nodeInit(registro, false, tree);
            registro->children[0] = tree->root;

            splitChild(tree, registro, 0, rootCopy);

            int i = 0;
            if (registro->keyRecArr[0] < codigoLivro)
            {
                i++;
            }

            bTreeNode *childAtPosi = malloc(sizeof(bTreeNode));
            readFile(tree, childAtPosi, registro->children[i]);
            insertNonFull(tree, childAtPosi, codigoLivro, pos);

            tree->root = registro->pos;

            free(registro);
            free(childAtPosi);
        }
        else
        {
            insertNonFull(tree, rootCopy, codigoLivro, pos);
        }
        free(rootCopy);
    }
}

bool removeFromTree(bTree *tree, int codigoLivro)
{
    bTreeNode *raiz = malloc(sizeof(bTreeNode));
    readFile(tree, raiz, tree->root);

    if (search(tree, codigoLivro) != NULL)
        removeNode(tree, raiz, codigoLivro, true);
    else
        return false;

    free(raiz);
    return true;
}

recordNode *search(bTree *tree, int codigoLivro)
{
    bTreeNode *raiz = malloc(sizeof(bTreeNode));
    readFile(tree, raiz, tree->root);

    int posFound = searchRecursive(tree, codigoLivro, raiz);

    free(raiz);

    if (posFound == -1)
        return NULL;

    recordNode *dados = malloc(sizeof(recordNode));
    readRecordFile(tree, dados, posFound);

    return dados;
}

void removeNode(bTree *tree, bTreeNode *node, int k, bool removeRecord)
{

    int indice = findKey(node, k);
    if (indice < node->noOfRecs && node->keyRecArr[indice] == k)
    {
        if (node->isLeaf)
        {
            removeFromLeaf(tree, node, indice, removeRecord);
        }
        else
        {
            removeFromNonLeaf(tree, node, indice, removeRecord);
        }

        node->valid = false;
        writeFile(tree, node, node->pos);
    }
    else
    {
        if (node->isLeaf)
        {
            return;
        }
        bool flag = indice == node->noOfRecs;

        bTreeNode *childAtPosi = malloc(sizeof(bTreeNode));
        readFile(tree, childAtPosi, node->children[indice]);

        if (childAtPosi->noOfRecs < t)
        {
            fill(tree, node, indice);
            readFile(tree, childAtPosi, node->children[indice]);
        }

        if (flag && indice > node->noOfRecs)
        {
            bTreeNode *sibling = malloc(sizeof(bTreeNode));
            readFile(tree, sibling, node->children[indice - 1]);
            removeNode(tree, sibling, k, removeRecord);

            writeFile(tree, sibling, sibling->pos);
            free(sibling);
        }
        else
        {
            removeNode(tree, childAtPosi, k, removeRecord);
        }

        childAtPosi->valid = false;
        writeFile(tree, childAtPosi, childAtPosi->pos);
        free(childAtPosi);
    }
}

/*A função é responsável por realizar a mesclagem de um nó e seu irmão adjacente quando o número
de registros no nó é menor do que 't - 1'.
Recebe como parâmetros um ponteiro para a estrutura da árvore, um ponteiro para o nó atual, e o
índice do registro que será mesclado.
Retorna um ponteiro para o nó 'child', resultado da mesclagem. */
bTreeNode *merge(bTree *tree, bTreeNode *node, int indice)
{

    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));

    readFile(tree, child, node->children[indice]);
    readFile(tree, sibling, node->children[indice + 1]);

    child->keyRecArr[t - 1] = node->keyRecArr[indice];
    child->posRecArr[t - 1] = node->posRecArr[indice];

    for (int i = 0; i < sibling->noOfRecs; ++i)
    {
        child->keyRecArr[i + t] = sibling->keyRecArr[i];
        child->posRecArr[i + t] = sibling->posRecArr[i];
    }

    if (!child->isLeaf)
    {
        for (int i = 0; i <= sibling->noOfRecs; ++i)
            child->children[i + t] = sibling->children[i];
    }

    for (int i = indice + 1; i < node->noOfRecs; ++i)
    {
        node->keyRecArr[i - 1] = node->keyRecArr[i];
        node->posRecArr[i - 1] = node->posRecArr[i];
    }

    for (int i = indice + 2; i <= node->noOfRecs; ++i)
    {
        node->children[i - 1] = node->children[i];
    }

    node->children[node->noOfRecs] = -1;
    child->noOfRecs += sibling->noOfRecs + 1;
    node->noOfRecs--;

    if (node->noOfRecs == 0)
    {
        tree->root = node->children[0];
    }

    writeFile(tree, node, node->pos);
    writeFile(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);
    free(sibling);

    return child;
}

void borrowFromPrev(bTree *tree, bTreeNode *node, int indi)
{
    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));

    readFile(tree, child, node->children[indi]);
    readFile(tree, sibling, node->children[indi - 1]);

    for (int i = child->noOfRecs - 1; i >= 0; --i)
    {
        child->keyRecArr[i + 1] = child->keyRecArr[i];
        child->posRecArr[i + 1] = child->posRecArr[i];
    }

    if (!child->isLeaf)
    {
        for (int i = child->noOfRecs; i >= 0; --i)
            child->children[i + 1] = child->children[i];
    }

    child->keyRecArr[0] = node->keyRecArr[indi - 1];
    child->posRecArr[0] = node->posRecArr[indi - 1];

    if (!node->isLeaf)
    {
        child->children[0] = sibling->children[sibling->noOfRecs];
        sibling->children[sibling->noOfRecs] = -1;
    }

    node->keyRecArr[indi - 1] = sibling->keyRecArr[sibling->noOfRecs - 1];
    node->posRecArr[indi - 1] = sibling->posRecArr[sibling->noOfRecs - 1];

    child->noOfRecs += 1;
    sibling->noOfRecs -= 1;

    writeFile(tree, node, node->pos);
    writeFile(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);

    free(child);
    free(sibling);

    return;
}

// Uma função para pedir emprestada uma chave do C[indi+1] e colocá-la no C[indi]
void borrowFromNext(bTree *tree, bTreeNode *node, int indi)
{

    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));

    readFile(tree, child, node->children[indi]);
    readFile(tree, sibling, node->children[indi + 1]);

    // keys[indi] é inserida como a última chave em C[indi]
    child->keyRecArr[(child->noOfRecs)] = node->keyRecArr[indi];
    child->posRecArr[(child->noOfRecs)] = node->posRecArr[indi];

    // O primeiro filho do irmão é inserido como o último filho em C[indi]
    if (!(child->isLeaf))
        child->children[(child->noOfRecs) + 1] = sibling->children[0];

    // A primeira chave do irmão é inserida em keys[indi]
    node->keyRecArr[indi] = sibling->keyRecArr[0];
    node->posRecArr[indi] = sibling->posRecArr[0];

    // Mover todas as chaves no irmão um passo atrás
    for (int i = 1; i < sibling->noOfRecs; ++i)
    {
        sibling->keyRecArr[i - 1] = sibling->keyRecArr[i];
        sibling->posRecArr[i - 1] = sibling->posRecArr[i];
    }

    // Mover os ponteiros filhos um passo atrás
    if (!sibling->isLeaf)
    {
        for (int i = 1; i <= sibling->noOfRecs; ++i)
            sibling->children[i - 1] = sibling->children[i];

        sibling->children[sibling->noOfRecs] = -1; // RESPOSTA:
    }

    // Aumentar e diminuir o número de chaves de C[indi] e C[indi+1] respectivamente
    child->noOfRecs += 1;
    sibling->noOfRecs -= 1;

    writeFile(tree, node, node->pos);
    writeFile(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);

    free(child);
    free(sibling);

    return;
}