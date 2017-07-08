#include <stdio.h>
#include <stdlib.h>

/*--------------------------------------------------------------------------*/
/* 			     Universidade Federal de São João del - Rei 
			        Algoritmos e Estrutura de Dados III
		        	Ciência da Computação - 3º período
      
					Isabella Vieira Ferreira
					 Mônica Neli de Resende
						Março / 2012                */
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
#ifndef OPLISTA_H
#define OPLISTA_H

/*--------------------------------------------------------------------------*/
//					TAD - Lista
/*--------------------------------------------------------------------------*/
		typedef struct celula *Apontador;
		typedef struct celula {
			int itens;
			struct celula * prox;
		} celula;

		//Tipo abstrato Lista simplesmente encadeada
		typedef struct {
			Apontador primeiro, ultimo;
		} Lista;
  
		/* Structure of arguments to pass to client thread */
		typedef struct {
			celula *primeiro;
			celula *ultimo;
		}ThreadArgs, *ponteiro;  
		  
  
/*--------------------------------------------------------------------------*/

void criarLista(Lista *L1);
void desalocarLista (Lista *L1);
void mostrarLista (Lista *L1);
void inserirElemento(int n, Lista *L1);
Apontador buscarElemento(int x, Lista *L1);
int Vazia(Lista *L1);
void remover(Lista *L1, int item);
int getFirst(Lista *L1);
int getFirst2(ponteiro L1);
#endif
