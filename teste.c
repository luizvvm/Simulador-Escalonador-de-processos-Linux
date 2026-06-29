#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROCESSOS       8
#define QUANTUM_ALTA        4
#define QUANTUM_BAIXA       8
#define PROB_IO             20
#define DURACAO_DISCO       6
#define DURACAO_FITA        4
#define DURACAO_IMPRESSORA  5
#define PID_INICIAL         1000

typedef enum { PRONTO, EXECUTANDO, BLOQUEADO } Status;
typedef enum { DISCO, FITA, IMPRESSORA }       TipoIO;

typedef struct PCB {
    int    PID;
    int    PPID;
    int    prioridade;
    Status status;
    int    tempo_servico;
    int    tempo_executado;
    int    tempo_sistema;
    int    tempo_io_restante;
    TipoIO tipo_io;
} PCB;

typedef struct No {
    PCB       *processo;
    struct No *prox;
} No;

typedef struct {
    No  *inicio;
    No  *fim;
    int  tamanho;
} Fila;

Fila *fila_alta;
Fila *fila_baixa;
Fila *fila_fcfs;
Fila *fila_disco;
Fila *fila_fita;
Fila *fila_impressora;

int pid_contador = PID_INICIAL;

Fila *cria_fila() {
    Fila *f    = (Fila *) malloc(sizeof(Fila));
    f->inicio  = NULL;
    f->fim     = NULL;
    f->tamanho = 0;
    return f;
}

int fila_vazia(Fila *f) {
    return f->inicio == NULL;
}

void insere_fila(Fila *f, PCB *p) {
    No *no       = (No *) malloc(sizeof(No));
    no->processo = p;
    no->prox     = NULL;
    if (f->inicio == NULL) {
        f->inicio = no;
        f->fim    = no;
    } else {
        f->fim->prox = no;
        f->fim       = no;
    }
    f->tamanho++;
}

PCB *remove_fila(Fila *f) {
    if (f->inicio == NULL) return NULL;
    No  *no = f->inicio;
    PCB *p  = no->processo;
    f->inicio = no->prox;
    if (f->inicio == NULL) f->fim = NULL;
    free(no);
    f->tamanho--;
    return p;
}

PCB *cria_processo(int ppid) {
    PCB *p               = (PCB *) malloc(sizeof(PCB));
    p->PID               = pid_contador++;
    p->PPID              = ppid;
    p->prioridade        = 1;
    p->status            = PRONTO;
    int tempo_servico = rand() % 16 + 5;
    p->tempo_servico     = tempo_servico;
    p->tempo_executado   = 0;
    p->tempo_sistema     = 0;
    p->tempo_io_restante = 0;
    return p;
}

void avanca_io(Fila *fila_io, Fila *fila_retorno, const char *nome, int tempo) {
    No *no       = fila_io->inicio;
    No *anterior = NULL;

    while (no != NULL) {
        PCB *p    = no->processo;
        No  *prox = no->prox;

        p->tempo_io_restante--;
        p->tempo_sistema++;

        if (p->tempo_io_restante <= 0) {
            if (anterior == NULL) fila_io->inicio = prox;
            else                  anterior->prox   = prox;
            if (prox == NULL)     fila_io->fim     = anterior;
            fila_io->tamanho--;
            free(no);

            p->status = PRONTO;
            insere_fila(fila_retorno, p);
            printf("[t=%3d] [IO ] PID %d concluiu %-11s -> retorna a fila\n",
                   tempo, p->PID, nome);

            no = prox;
        } else {
            anterior = no;
            no       = prox;
        }
    }
}

void imprime_fila(Fila *f, const char *nome) {
    printf("  %-18s [%d]: ", nome, f->tamanho);
    No *no = f->inicio;
    while (no != NULL) {
        printf("PID%d(s=%d) ", no->processo->PID, no->processo->tempo_servico);
        no = no->prox;
    }
    printf("\n");
}

void imprime_estado(int tempo) {
    printf("[t=%3d] Estado das filas:\n", tempo);
    imprime_fila(fila_alta,       "Alta prio (RR)");
    imprime_fila(fila_baixa,      "Baixa prio (RR)");
    imprime_fila(fila_fcfs,       "FCFS");
    imprime_fila(fila_disco,      "I/O Disco");
    imprime_fila(fila_fita,       "I/O Fita");
    imprime_fila(fila_impressora, "I/O Impressora");
    printf("\n");
}

void simular() {
    int tempo = 0;

    printf("Criando %d processos:\n", MAX_PROCESSOS);
    printf("  %-6s %-6s %-12s\n", "PID", "PPID", "Servico");
    for (int i = 0; i < MAX_PROCESSOS; i++) {
        PCB *p = cria_processo(0);
        insere_fila(fila_alta, p);
        printf("  %-6d %-6d %-12d\n", p->PID, p->PPID, p->tempo_servico);
    }
    printf("\n");
    imprime_estado(tempo);

    PCB  *em_execucao  = NULL;
    Fila *fila_atual   = NULL;
    int   quantum_rest = 0;

    while (1) {
        if (!em_execucao &&
            fila_vazia(fila_alta) && fila_vazia(fila_baixa) &&
            fila_vazia(fila_fcfs) && fila_vazia(fila_disco) &&
            fila_vazia(fila_fita) && fila_vazia(fila_impressora))
            break;

        tempo++;

        avanca_io(fila_disco,      fila_baixa, "DISCO",      tempo);
        avanca_io(fila_fita,       fila_alta,  "FITA",       tempo);
        avanca_io(fila_impressora, fila_alta,  "IMPRESSORA", tempo);

        if (!em_execucao) {
            if (!fila_vazia(fila_alta)) {
                fila_atual    = fila_alta;
                quantum_rest  = QUANTUM_ALTA;
            } else if (!fila_vazia(fila_baixa)) {
                fila_atual    = fila_baixa;
                quantum_rest  = QUANTUM_BAIXA;
            } else if (!fila_vazia(fila_fcfs)) {
                fila_atual    = fila_fcfs;
                quantum_rest  = 9999;
            } else {
                continue;
            }

            em_execucao = remove_fila(fila_atual);
            em_execucao->status = EXECUTANDO;

            const char *nome_fila =
                (fila_atual == fila_alta)  ? "alta"  :
                (fila_atual == fila_baixa) ? "baixa" : "FCFS";

            printf("[t=%3d] [CPU] PID %d inicia (fila %s, servico restante=%d)\n",
                   tempo, em_execucao->PID, nome_fila, em_execucao->tempo_servico);
        }

        em_execucao->tempo_servico--;
        em_execucao->tempo_executado++;
        em_execucao->tempo_sistema++;
        quantum_rest--;

        if (em_execucao->tempo_servico == 0) {
            printf("[t=%3d] [FIM] PID %d terminou (CPU=%d, sistema=%d)\n\n",
                   tempo, em_execucao->PID,
                   em_execucao->tempo_executado, em_execucao->tempo_sistema);
            free(em_execucao);
            em_execucao = NULL;
            fila_atual  = NULL;

        } else if (rand() % 100 < PROB_IO) {
            Fila       *fila_io;
            Fila       *fila_retorno;
            const char *nome_io;
            int         duracao;

            switch (rand() % 3) {
                case 0:
                    fila_io = fila_disco; fila_retorno = fila_baixa;
                    nome_io = "DISCO";    duracao = DURACAO_DISCO;
                    em_execucao->tipo_io = DISCO;
                    break;
                case 1:
                    fila_io = fila_fita;  fila_retorno = fila_alta;
                    nome_io = "FITA";     duracao = DURACAO_FITA;
                    em_execucao->tipo_io = FITA;
                    break;
                default:
                    fila_io = fila_impressora; fila_retorno = fila_alta;
                    nome_io = "IMPRESSORA";    duracao = DURACAO_IMPRESSORA;
                    em_execucao->tipo_io = IMPRESSORA;
                    break;
            }

            em_execucao->status            = BLOQUEADO;
            em_execucao->tempo_io_restante = duracao;

            printf("[t=%3d] [IO ] PID %d -> %-11s por %d u.t. (retorna: fila %s)\n",
                   tempo, em_execucao->PID, nome_io, duracao,
                   (fila_retorno == fila_alta) ? "alta" : "baixa");

            insere_fila(fila_io, em_execucao);
            em_execucao = NULL;
            fila_atual  = NULL;

        } else if (quantum_rest == 0) {
            em_execucao->status = PRONTO;

            Fila       *proxima;
            const char *nome_proxima;
            if (fila_atual == fila_alta) {
                proxima = fila_baixa; nome_proxima = "baixa";
            } else {
                proxima = fila_fcfs;  nome_proxima = "FCFS";
            }

            printf("[t=%3d] [PRE] PID %d preemptado -> fila %s (servico restante=%d)\n",
                   tempo, em_execucao->PID, nome_proxima, em_execucao->tempo_servico);

            insere_fila(proxima, em_execucao);
            em_execucao = NULL;
            fila_atual  = NULL;
        }
    }

    printf("Simulacao concluida em t=%d\n", tempo);
}

int main() {
    srand((unsigned) time(NULL));

    fila_alta       = cria_fila();
    fila_baixa      = cria_fila();
    fila_fcfs       = cria_fila();
    fila_disco      = cria_fila();
    fila_fita       = cria_fila();
    fila_impressora = cria_fila();

    simular();
    return 0;
}