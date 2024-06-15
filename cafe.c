#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct participante {
    int id;
    char nome[50];
    char curso[50];
    int ano;
    struct participante *next;
} Participante;

typedef struct pagante{
  int id;
  int mes;
  int ano;
  float valor;
  struct pagante *next;
} Pagante;

Participante *front = NULL;
Participante *rear = NULL;
Pagante *frontP = NULL;
Pagante *rearP = NULL;
int next_id = 1;

void inserir_participante();
void editar_participante();
void ler_participantes();
void ler_contribuintes();
void salvar_participantes();
void limpar_buffer();
void inserir_contribuinte();
void salvar_contribuintes_por_curso();
void liberar_memoria();

int main() {
    char escolha;

    do {
        printf("\nEscolha uma opção:\n");
        printf("1- Inserir novo participante\n");
        printf("2- Editar participante\n");
        printf("3- Ler todos os participantes\n");
        printf("4- Inserir contribuintes\n");
        printf("5- Ler contribuintes\n");
        printf("6- Sair\n");
        printf("Opção: ");
        
        scanf(" %c", &escolha);

        switch (escolha) {
            case '1':
                inserir_participante();
                break;
            case '2':
                editar_participante();
                break;
            case '3':
                ler_participantes();
                break;
            case '4':
                inserir_contribuinte();
                break;
            case '5':
                ler_contribuintes();
                break;
            case '6':
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (escolha != '6');

    liberar_memoria();
    return 0;
}

void salvar_participantes()
{
    FILE *participantes_file = fopen("participantes.txt", "wt");
    if (participantes_file == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }

    Participante *curr = front;
    while (curr != NULL) {
        fprintf(participantes_file, "%d,%s,%s,%d\n", curr->id, curr->nome, curr->curso, curr->ano);
        curr = curr->next;
    }

    fclose(participantes_file);
    printf("Dados salvos com sucesso\n");
}

void salvar_contribuintes()
{
    FILE *participantes_file = fopen("contribuintes.txt", "wt");
    if (participantes_file == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }

    Pagante *curr = frontP;
    while (curr != NULL) {
        fprintf(participantes_file, "%d,%0.2f,%d,%d\n", curr->id, curr->valor, curr->mes, curr->ano);
        curr = curr->next;
    }

    fclose(participantes_file);
    printf("Dados salvos com sucesso\n");
}

void salvar_contribuintes_por_curso()
{
    Pagante *curr = frontP;
    
    FILE *fileDSM = fopen("contribuintes_DSM.txt", "wt");
    FILE *fileSI = fopen("contribuintes_SI.txt", "wt");
    FILE *fileGE = fopen("contribuintes_GE.txt", "wt");
   
    if (fileDSM == NULL || fileSI == NULL || fileGE == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    while (curr != NULL) {
     
        Participante *participante = front;
        while (participante != NULL) {
            if (participante->id == curr->id) {
                switch (participante->curso[0]) {
                    case 'D':
                        fprintf(fileDSM, "%d,%0.2f,%d,%d\n", curr->id, curr->valor, curr->mes, curr->ano);
                        break;
                    case 'S':
                        fprintf(fileSI, "%d,%0.2f,%d,%d\n", curr->id, curr->valor, curr->mes, curr->ano);
                        break;
                    case 'G':
                        fprintf(fileGE, "%d,%0.2f,%d,%d\n", curr->id, curr->valor, curr->mes, curr->ano);
                        break;
                }
                break; 
            }
            participante = participante->next;
        }
        curr = curr->next;
    }

    fclose(fileDSM);
    fclose(fileSI);
    fclose(fileGE);

    printf("Dados dos contribuintes por curso salvos com sucesso.\n");
}

void inserir_participante() {
    char nome[50];
    char curso[50];
    int ano;
    
    Participante *this = malloc(sizeof(Participante));
    if (this == NULL) {
        printf("Erro ao alocar memória para o participante.\n");
        exit(1);
    }
    this->next = NULL;

    printf("Digite o primeiro nome do participante: ");
    scanf("%s", this->nome);

    printf("Escolha o curso, digite '1' para DSM, '2' para GE ou '3' para SI: ");
    char escolha;
    scanf(" %c", &escolha);
    limpar_buffer(); 

    switch (escolha) {
        case '1':
            strcpy(this->curso, "DSM1");
            break;
        case '2':
            strcpy(this->curso, "GE");
            break;
        case '3':
            strcpy(this->curso, "SI");
            break;
        default:
            printf("Opção de curso inválida.\n");
            free(this);
            return;
    }

    printf("Qual o ano em que foi cursado? ");
    scanf("%d", &ano);
    
    while(ano > 2024){
    printf("\nAno não pode ser maior que 2024. Qual o ano em que foi cursado?: ");
    scanf("%d", &ano);
  }
    limpar_buffer();

    this->ano = ano;

    if (rear == NULL) {
        front = this;
        rear = this;
        this->id = 1;
    } else {
        this->id = rear->id + 1;
        rear->next = this;
        rear = this;
    }

    salvar_participantes();
    printf("Participante inserido com sucesso!\n");
}

void inserir_contribuinte() {
    int id;
    int ano;
    int mes;
    float valor;

    printf("Digite o ID do participante que contribuiu com a turma do café: ");
    scanf("%d", &id);
    limpar_buffer();

    Participante *curr = front;
    Pagante *this = malloc(sizeof(Pagante));
    if (!this) {
        printf("Erro de alocação de memória");
        exit(1);
    }
    while (curr != NULL) {
        if (curr->id == id) {
            this->id = id;
            printf("Qual o mês da contribuição? ");
            scanf("%d", &mes);
           while(mes < 0 || mes > 12){
            printf("\nDigite um mês válido: ");
            scanf("%d", &mes);
            }

            limpar_buffer();
            this->mes = mes;

            printf("Qual o ano da contribuição? ");
            scanf("%d", &ano);
              while(ano > 2024){
                printf("\nDigite um ano menor do que o atual: ");
                scanf("%d", &ano);
              }
            limpar_buffer();
            this->ano = ano;

            printf("Qual o valor em reais da contribuição? ");
            scanf("%f", &valor);
            limpar_buffer();
            this->valor = valor;

            this->next = NULL;

            if (rearP == NULL) {
                frontP = this;
                rearP = this;
            } else {
                rearP->next = this;
                rearP = this;
            }
            salvar_contribuintes();
            salvar_contribuintes_por_curso();
            printf("Contribuição registrada com sucesso!");
            return;
        }
        curr = curr->next;
    }
    printf("ID não encontrado!");
}


void editar_participante() {
    int id;
    printf("Digite o ID do participante a ser editado: ");
    scanf("%d", &id);
    limpar_buffer();

    Participante *curr = front;
    while (curr != NULL) {
        if (curr->id == id) {
            printf("Digite o primeiro nome do participante: ");
            scanf("%s", curr->nome);
            printf("Escolha o novo curso, digite '1' para DSM, '2' para GE ou '3' para SI: ");
            char escolha;
            scanf(" %c", &escolha);
            limpar_buffer(); 

            switch (escolha) {
                case '1':
                    strcpy(curr->curso, "DSM");
                    break;
                case '2':
                    strcpy(curr->curso, "GE");
                    break;
                case '3':
                    strcpy(curr->curso, "SI");
                    break;
                default:
                    printf("Opção de curso inválida.\n");
                    return;
            }

            printf("Digite o ano em que foi cursado: ");
            scanf("%d", &curr->ano);
              while(curr->ano > 2024){
              printf("\nDigite um ano válido: ");
              scanf("%d", &curr->ano);
            }
            limpar_buffer();

            salvar_participantes();
            printf("Participante editado com sucesso!\n");
            return;
        }
        curr = curr->next;
    }

    printf("Participante com o ID %d não encontrado.\n", id);
}

void ler_participantes() {
    FILE *handler = fopen("participantes.txt", "rt");
    if (handler == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("\nLista de participantes:\n");
    int id, ano;
    char nome[50], curso[50];
    while (fscanf(handler, "%d,%[^,],%[^,],%d\n", &id, nome, curso, &ano) == 4)
  {
        printf("ID: %d, Nome: %s, Curso: %s, Ano: %d\n", id, nome, curso, ano);
    }

    fclose(handler);
}

void ler_contribuintes() {
    FILE *handler = fopen("contribuintes.txt", "rt");
    if (handler == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("\nLista de contribuintes:\n");
    int id, mes, ano;
    float valor;
    
    while (fscanf(handler, "%d,%f,%d,%d\n", &id, &valor, &mes, &ano) == 4)
  {
        printf("Membro de ID: %d,Contribuiu com valor de: R$%0.2f no Mês: %d, durante o ano de %d\n", id, valor, mes, ano);
    }

    fclose(handler);
}


void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

void liberar_memoria() {
    Participante *curr_participante = front;
    while (curr_participante != NULL) {
        Participante *temp_participante = curr_participante;
        curr_participante = curr_participante->next;
        free(temp_participante);
    }
    
    Pagante *curr_contribuinte = frontP;
    while (curr_contribuinte != NULL) {
        Pagante *temp_contribuinte = curr_contribuinte;
        curr_contribuinte = curr_contribuinte->next;
        free(temp_contribuinte);
    }

    front = NULL;
    rear = NULL;
    frontP = NULL;
    rearP = NULL;
}
