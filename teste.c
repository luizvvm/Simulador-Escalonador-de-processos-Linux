#include <stdio.h>
#include <stdlib.h>

#define QUANTUM_FILA1 30
#define QUANTUM_FILA2 30

typedef enum {
    PRONTO,
    EXECUTANDO,
    BLOQUEADO
} Status;

typedef struct PCB{
  int PPID;
  int PID;
  int prioridade;
  float tempo_servico;
  float tempo_execucao;
  float tempo_sistema;
  Status status;
} PCB;

//fila de processos prontos
typedef struct no_fila_prontos{
  PCB processos;
  struct no_fila_prontos *prox;
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
  if(p == NULL){
    p = processo_novo;
  } else{
    p->prox = processo_novo; //o cara que o p->prox aponta passa a apontar para o processo_novo
  } 
  processo_novo->prox = NULL;

  return processo_novo;
}

void CPU(no_fila1 *P, no_fila1 *fim_1, no_fila1 *inicio_1){
P->processos.status = EXECUTANDO;
while(P->quantum > 0){
  P->quantum --;
  P->processos.tempo_execucao ++;
  P->processos.tempo_sistema ++;
}

if(P->quantum == 0){
  P->quantum = QUANTUM_FILA2;
  P->processos.status = EXECUTANDO;
  printf("final do tempo de CPU");
  inicio_1->prox = P->prox;
  P->prox = fim_1; //alterar aqui
  free(P);
}
}

void processos_prontos(no_fila_prontos* pronto_inicio, no_fila_prontos* pronto_final){
    //processo 1
    PCB *processo_1;
    processo_1 = (PCB *) malloc (sizeof(PCB));
    processo_1->PID = 2003;
    processo_1->prioridade = 1;
    processo_1->tempo_servico = 0;
    processo_1->tempo_execucao = 0;
    processo_1->tempo_sistema = 0;
    processo_1->status = PRONTO;

    //processo 2
    PCB *processo_2;
    processo_2 = (PCB *) malloc (sizeof(PCB));
    processo_2->PID = 2003;
    processo_2->prioridade = 1;
    processo_2->tempo_servico = 0;
    processo_2->tempo_execucao = 0;
    processo_2->tempo_sistema = 0;
    processo_2->status = PRONTO;

    //processo 3
    PCB *processo_3;
    processo_3 = (PCB *) malloc (sizeof(PCB));
    processo_3->PID = 2003;
    processo_3->prioridade = 1;
    processo_3->tempo_servico = 0;
    processo_3->tempo_execucao = 0;
    processo_3->tempo_sistema = 0;
    processo_3->status = PRONTO;

    no_fila1 *processo_no_3;
    processo_no_3 = (no_fila1 *) malloc (sizeof(no_fila1));
    processo_no_3->processos = *processo_3;
    processo_no_3->prox = NULL

    pronto_final = processo_no_3;

    processos_prontos *processo_no_2;
    processo_no_2 = (processos_prontos *) malloc (sizeof(processos_prontos));
    processo_no_2->processos = *processo_2;
    processo_no_3->prox = processo_no_3;

    processos_prontos *processo_no_1
    processo_no_1 = (processos_prontos *) malloc (sizeof(processos_prontos));
    processo_no_1->processos = *processo_1;
    processo_no_1->prox = processo_no_2;

    pronto_inicio = processo_no_1;
}


no_fila_prontos insere_prontos(no_fila_prontos *inicio_prontos, no_fila_prontos *fim_prontos, PCB *processo){
}

int main() {
no_fila1 *processo_atual;
no_fila1 *processo_acabou_de_ser_inserido;
no_fila_prontos *inicio_prontos = NULL;
no_fila_prontos *fim_prontos = NULL;
no_fila1 *inicio_1 = NULL;
no_fila1 *fim_1 = NULL;

processos_prontos(inicio_prontos, fim_prontos);

While(inicio_prontos->prox != NULL){
    processo_atual = inicio_prontos->prox;
    if(inicio_1 == NULL){
        processo_acabou_de_ser_inserido = insere(inicio_1, processo_atual);
        inicio_1 = processo_acabou_de_ser_inserido;
        fim_1 = processo_acabou_de_ser_inserido;
    }
    else{
        processo_acabou_de_ser_inserido = insere(fim_1, processo_atual)
        fim_1 = processo_acabou_de_ser_inserido;
    }

}

CPU(processo_atual, fim_1, inicio_1);

return 0;

}