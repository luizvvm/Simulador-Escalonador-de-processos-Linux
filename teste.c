%%writefile programa.c
#include <stdio.h>
#include <stdlib.h>

#define QUANTUM_FILA1 30
#define QUANTUM_FILA2 30

typedef struct PCB{
  //int PPID;
  int PID;
  int prioridade;
  float tempo_servico;
  float tempo_execucao;
  float tempo_sistema;
  int []v = [-1, 0, 1]; //-1 bloqueado, 0 pronto, 1 em CPU
} PCB;

//fila de processos prontos
typedef struct no_fila_prontos{
  PCB processos;
  float quantum;
  struct no_fila1 *prox;
} no_fila_prontos;

//Primeira fila de processos
typedef struct no_fila1{
  PCB processos;
  float quantum;
  struct no_fila1 *prox;
} no_fila1;

//Segunda fila de processos
typedef struct no_fila2{
  PCB processos;
  float quantum;
  struct no_fila2 *prox;
} no_fila2;

//Terceira fila de processos
typedef struct no_fila3{
  PCB processos;
  struct no_fila3 *prox;
} no_fila3;

no_fila1* insere (no_fila1 *p, PCB *processo){
  no_fila1 *processo_novo;
  processo_novo = (no_fila1 *) malloc (sizeof(no_fila1));
  processo_novo->quantum = QUANTUM_FILA1;
  processo_novo->processos = *processo;
  p->prox = processo_novo;
  processo_novo->prox = NULL;

  return processo_novo;
}

void CPU(no_fila1 *P, no_fila1 *fim_1, no_fila1 *inicio_1){
p->processos.v = 0;
while(P->quantum > 0){
  P->quantum --;
  P->processos.tempo_execucao ++;
  P->processos.tempo_execucao ++;
}

if(P->quantum == 0){
    p->quantum = QUANTUM_FILA2;
  p->processos.v = 0;
  printf("final do tempo de CPU");
  inicio_1->prox = P->prox;
  P->prox = fim_1; //alterar aqui
  free(P);
}
}

void processos_prontos(no_fila_prontos inicio_prontos, no_fila_prontos fim_prontos){
    PCB *processo_1;
    processo_1 = (PCB *) malloc (sizeof(PCB));
    processo_1->PID = 2003;
    processo_1->prioridade = 1;
    processo_1->tempo_servico

    int prioridade;
  float tempo_servico;
  float tempo_execucao;
  float tempo_sistema;
  int []v = [-1, 0, 1]; //-1 bloqueado, 0 pronto, 1 em CPU
}

int main() {
    PCB *processo_teste;
    processo_teste = (PCB *) malloc (sizeof(PCB));
    processo_teste->PID = 4323;
    processo_teste->prioridade = 0;
    processo_teste->tempo_servico = -1;
    processo_teste->tempo_execucao = 0;
    processo_teste->tempo_sistema = 0;

no_fila1 *processo_atual;
no_fila_prontos *inicio_prontos = NULL;
no_fila_prontos *fim_prontos = NULL;
no_fila1 *inicio_1 = NULL;
no_fila1 *fim_1 = NULL;



While()


if(inicio_1 == NULL){
  inicio_1 = (no_fila1 *) malloc (sizeof(no_fila1));
  fim_1 = inicio_1;
  inicio_1->prox = NULL;
  processo_atual = insere(inicio_1, processo_teste);
}
else{
  insere(fim_1, processo_teste);
}


CPU(processo_atual, fim_1, inicio_1);

return 0;

}