#include "oplista.h"
/*--------------------------------------------------------------------------*/
/* 			     Universidade Federal de São João del - Rei 
			        Algoritmos e Estrutura de Dados III
		        	Ciência da Computação - 3º período
      
					Isabella Vieira Ferreira
					 Mônica Neli de Resende
						Março / 2012                */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* Este código possui as seguintes funções:
 
   - Criar uma lista simplesmente encadeada
   - Destruir a lista
   - Imprimir na tela a lista 
   - Inserir elementos na lista
   - Buscar um elemento na lista e retornar sua posição.
                                                                            */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* 					TIPO ABSTRATO LISTA: 
 * LISTA: 
		CELULA PRIMEIRO, ULTIMO;
	
	 CELULA:
	  	- itens;
	  	- celula *prox
	  	
	 CELULA *Apontador;
*/

/*--------------------------------------------------------------------------*/
/* Função responsável por criar a lista simplesmente encadeada.
   Possui célula cabeça.                                                    */
/*--------------------------------------------------------------------------*/

void criarLista(Lista *L1){
	
	// L1->primeiro é a célula cabeça
	L1->primeiro = (celula  *) malloc(sizeof(celula));
	L1->ultimo = (celula  *) L1->primeiro;
	L1->primeiro->prox = NULL;
	
}


/*--------------------------------------------------------------------------*/
/* Função responsável por desalocar a lista.                                 */
/*--------------------------------------------------------------------------*/

void desalocarLista (Lista *L1) {

	Apontador aux1 = L1->primeiro;
	Apontador aux2 = NULL;

	while (aux1 != L1->ultimo) {
		//printf("Item %d ", aux1->itens);
		aux2 = aux1;
		aux1 = aux1->prox;
		//printf("Freed\n");
		free(aux2);
		//printf("Teste Item %d \n", aux2->itens);
	}
	//printf("Last Item %d \n", aux1->itens);
	free(aux1);	
}

/*--------------------------------------------------------------------------*/
/*Função responsável por imprimir a lista na tela.  */
/*--------------------------------------------------------------------------*/

void mostrarLista (Lista *L1) {  
	
	if (L1->primeiro == L1->ultimo) {
		printf("Lista vazia!\n");
		return;
	}
	
	Apontador aux = L1->primeiro;
	int i = 1;
	
	while (aux != L1->ultimo){
	
		printf("Item %d: %d\n", i, aux->itens);
		aux = aux->prox;
		i++;
	}
	
}

/*--------------------------------------------------------------------------*/
/* Função responsável por inserir um elemento na lista.                       */
/*--------------------------------------------------------------------------*/

void inserirElemento(int n, Lista *L1){

	celula *p;
	/*novoItem = malloc(sizeof(celula));
	novoItem->prox = NULL;
	novoItem->itens = n;
	L1->ultimo->prox = novoItem;
	L1->ultimo = L1->ultimo->prox;*/
	L1->ultimo->itens = n;
	
	L1->ultimo->prox = malloc(sizeof(celula));
	L1->ultimo = L1->ultimo->prox;	
	L1->ultimo->prox = NULL;
}


/*--------------------------------------------------------------------------*/
/* Função que busca um elemento na lista e retorna sua posição na lista.    */
/*--------------------------------------------------------------------------*/
Apontador buscarElemento(int x, Lista *L1){
	if (L1->primeiro == L1->ultimo) {
		printf("lista vazia\n");
		return NULL;
	}
	
	Apontador aux = L1->primeiro;
	
	while((aux != NULL) && (aux->itens != x)){
		
		aux = aux->prox;		
		
	}
	
	if (aux == NULL){
		printf("\nItem não presente na lista\n");
		return NULL;
	}
	// Retorna NULL ou o apontador para a célula com o item buscado
	return aux;
}

/*--------------------------------------------------------------------------*/
/* Função que verifica se a lista está vazia ou não. 					   */
/*--------------------------------------------------------------------------*/
int Vazia(Lista *L1) {
	
	return (L1->primeiro == L1->ultimo);
}

/*--------------------------------------------------------------------------*/
/* Função que verifica se a lista está vazia ou não. 					   */
/*--------------------------------------------------------------------------*/
void remover(Lista *L1, int item) {
	//printf("\nRemover!\n");
	if (L1->primeiro == L1->ultimo) {
		//printf("lista vazia\n");
		return;
	}
	Apontador aux1 = NULL;
	Apontador aux2 = L1->primeiro;
	
	while((aux2 != NULL) && (aux2->itens != item )){
		//printf("\nItem %d == %d\n", aux2->itens, item);
		aux1 = aux2;
		aux2 = aux2->prox;		
	}
	
	if (aux2 != NULL){
		//printf("\nItem %d == %d\n", aux2->itens, item);
		if (aux2 == L1->primeiro) {
			L1->primeiro = L1->primeiro->prox;
		} else if (aux2 == L1->ultimo){
			L1->ultimo = aux1;
		}else aux1->prox = aux2->prox;
		
		free(aux2);
	}else{
		printf("\nItem não presente na lista!\n");
	}
}

/*--------------------------------------------------------------------------*/
/* Função GetFirst e remove it.						  					    */
/*--------------------------------------------------------------------------*/
int getFirst(Lista *L1) {
	int item = -1;
	Apontador first;

	if (L1->primeiro == L1->ultimo) { 
		//printf("Vazia...\n"); 
		return (-1);
	}

	first = L1->primeiro;
	item = L1->primeiro->itens; 
	L1->primeiro = L1->primeiro->prox;
	first->prox = NULL;
	//printf("\n>>>>>>>>>>First: %d\n", item); 
	free(first);
	return (item);
}

/*--------------------------------------------------------------------------*/
/* Função GetFirst e remove it.						  					    */
/*--------------------------------------------------------------------------*/
int getFirst2(ponteiro L1) {
	int item = -1;
	celula * first;

	if (L1->primeiro == L1->ultimo) { 
		//printf("Vazia...\n"); 
		return (-1);
	}

	first = (*L1).primeiro;
	item = (*L1).primeiro->itens; 
	(*L1).primeiro = (*L1).primeiro->prox;
	first->prox = NULL;
	//printf("\n>>>>>>>>>>First: %d\n", item); 
	free(first);
	return (item);
}
