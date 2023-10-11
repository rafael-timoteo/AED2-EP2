#include "headers.h"

int main()
{

	puts("Por favor, entre com um dos comandos abaixo:");
	puts("I  --> Insere livro atualizando o indice");
	puts("B  --> Busca livro a partir de seu codigo utilizando o indice");
	puts("R  --> Remove livro a partir do codigo atualizando o indice");
	puts("P1 --> Imprimir o indice");
	puts("P2 --> Imprimir o conteudo do arquivo indice.dat");
	puts("P3 --> Imprimir o conteudo do arquivo data.dat");
	puts("F  --> Finaliza a execucao fechando o arquivo de indice e o arquivo de dados\n\n");

	bTree *tree = createTree("indice.dat", "data.dat", false);
	if (tree == NULL)
	{
		puts("erro ao criar a arvore");
	}

	// Dados livro (recordNode)
	char titulo[30], autor[30];
	int codigo, anoPublicacao;

	// Comando recebido via CL
	char *comando = (char *)malloc(sizeof(char) * 3);
	char *opcao = (char *)malloc(sizeof(char) * 3);

	do
	{
		printf("Digite um comando: ");
		scanf("%s", comando);

		if (strcmp(comando, "I") == 0)
		{
			scanf("%i;%[^;];%[^;];%i", &codigo, &titulo, &autor, &anoPublicacao);
			int res = createRecord(tree, codigo, titulo, autor, anoPublicacao);
			insert(tree, codigo, res);
		}
		else if (strcmp(comando, "R") == 0)
		{
			int key;
			scanf("%i", &key);

			bool res = removeFromTree(tree, key);

			if (!res)
				printf("Erro ao apagar o livro com chave %i. O livro nao existe na biblioteca\n", key);
		}
		else if (strcmp(comando, "B") == 0)
		{
			int key;
			scanf("%d", &key);
			recordNode *res = search(tree, key);

			if (res != NULL)
			{
				printNo(res);
				free(res);
			}
			else
				printf("O livro com codigo %d nao existe na biblioteca\n", key);
		}
		else if (strcmp(comando, "P1") == 0)
		{
			traverse(tree, tree->root);
		}
		else if (strcmp(comando, "P2") == 0)
		{
			printArqIndice(tree);
		}
		else if (strcmp(comando, "P3") == 0)
		{
			printArqData(tree);
		}
		else
			continue;
	} while ((strcmp(comando, "F") != 0));

	fclose(tree->fp);
	fclose(tree->fData);
	free(tree);

	return 0;
}
