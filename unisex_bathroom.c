#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Constantes
#define MAX_CAPACITY 3
#define MAX_CONSECUTIVE 6

// Locks e variáveis de condição
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_men = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_women = PTHREAD_COND_INITIALIZER;

// Variáveis globais
int men_in_bathroom = 0;
int women_in_bathroom = 0;
int total_in_bathroom = 0;
int men_access_count = 0;
int women_access_count = 0;
int fairness_policy_active = 0; // 0 = nenhum, 1 = homens, 2 = mulheres
int fairness_enforced = 0; // Flag para controlar a alternância

// Função para gerar um tempo aleatório entre 1 e 3 segundos
void random_sleep() {
    int sleep_time = rand() % 3 + 1; // Gera um número entre 1 e 3
    sleep(sleep_time);
}

void apply_fairness_policy(int current_gender) {
    if (current_gender == 1 && men_access_count >= MAX_CONSECUTIVE) {
        fairness_policy_active = 2; // Alterna para mulheres
        fairness_enforced = 1;
        printf("Política de fairness aplicada: alternância para mulheres.\n");
    } else if (current_gender == 2 && women_access_count >= MAX_CONSECUTIVE) {
        fairness_policy_active = 1; // Alterna para homens
        fairness_enforced = 1;
        printf("Política de fairness aplicada: alternância para homens.\n");
    }
}

void *man(void *arg) {
    random_sleep(); // Espera aleatória antes de tentar entrar no banheiro

    pthread_mutex_lock(&mutex);

    // Espera até que não haja mulheres, haja espaço no banheiro e a política de fairness permita
    while (women_in_bathroom > 0 || total_in_bathroom >= MAX_CAPACITY ||
           (fairness_policy_active == 2 && fairness_enforced)) {
        pthread_cond_wait(&cond_men, &mutex);
    }

    men_in_bathroom++;
    total_in_bathroom++;
    men_access_count++;
    women_access_count = 0; // Reseta o contador de acessos consecutivos de mulheres
    printf("Homem %ld entrou no banheiro. Homens no banheiro: %d, Total: %d\n", (long)arg, men_in_bathroom, total_in_bathroom);

    apply_fairness_policy(1); // Avalia a política de fairness

    pthread_mutex_unlock(&mutex);

    // Seção crítica: homem no banheiro
    random_sleep(); // Simula o uso do banheiro

    pthread_mutex_lock(&mutex);

    men_in_bathroom--;
    total_in_bathroom--;
    printf("Homem %ld saiu do banheiro. Homens no banheiro: %d, Total: %d\n", (long)arg, men_in_bathroom, total_in_bathroom);

    // Se não houver mais homens, sinaliza para mulheres; senão, sinaliza para homens
    if (men_in_bathroom == 0 && fairness_policy_active == 2 && fairness_enforced) {
        fairness_enforced = 0; // Alternância concluída
        pthread_cond_broadcast(&cond_women);
    } else if (men_in_bathroom == 0) {
        pthread_cond_broadcast(&cond_women);
    } else {
        pthread_cond_broadcast(&cond_men);
    }

    pthread_mutex_unlock(&mutex);

    return NULL;
}

void *woman(void *arg) {
    random_sleep(); // Espera aleatória antes de tentar entrar no banheiro

    pthread_mutex_lock(&mutex);

    // Espera até que não haja homens, haja espaço no banheiro e a política de fairness permita
    while (men_in_bathroom > 0 || total_in_bathroom >= MAX_CAPACITY ||
           (fairness_policy_active == 1 && fairness_enforced)) {
        pthread_cond_wait(&cond_women, &mutex);
    }

    women_in_bathroom++;
    total_in_bathroom++;
    women_access_count++;
    men_access_count = 0; // Reseta o contador de acessos consecutivos de homens
    printf("Mulher %ld entrou no banheiro. Mulheres no banheiro: %d, Total: %d\n", (long)arg, women_in_bathroom, total_in_bathroom);

    apply_fairness_policy(2); // Avalia a política de fairness

    pthread_mutex_unlock(&mutex);

    // Seção crítica: mulher no banheiro
    random_sleep(); // Simula o uso do banheiro

    pthread_mutex_lock(&mutex);

    women_in_bathroom--;
    total_in_bathroom--;
    printf("Mulher %ld saiu do banheiro. Mulheres no banheiro: %d, Total: %d\n", (long)arg, women_in_bathroom, total_in_bathroom);

    // Se não houver mais mulheres, sinaliza para homens; senão, sinaliza para mulheres
    if (women_in_bathroom == 0 && fairness_policy_active == 1 && fairness_enforced) {
        fairness_enforced = 0; // Alternância concluída
        pthread_cond_broadcast(&cond_men);
    } else if (women_in_bathroom == 0) {
        pthread_cond_broadcast(&cond_men);
    } else {
        pthread_cond_broadcast(&cond_women);
    }

    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    int men_threads, women_threads, total_threads;
    pthread_t *threads;

    // Entrada interativa para número de threads
    printf("Digite o número de threads para homens: ");
    scanf("%d", &men_threads);
    printf("Digite o número de threads para mulheres: ");
    scanf("%d", &women_threads);

    total_threads = men_threads + women_threads;
    threads = malloc(total_threads * sizeof(pthread_t));

    // Semente para geração de números aleatórios
    srand(time(NULL));

    // Criando threads para homens e mulheres
    for (int i = 0; i < men_threads; i++) {
        pthread_create(&threads[i], NULL, man, (void *)(long)i);
    }
    for (int i = 0; i < women_threads; i++) {
        pthread_create(&threads[men_threads + i], NULL, woman, (void *)(long)(i + men_threads));
    }

    // Aguardando todas as threads terminarem
    for (int i = 0; i < total_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Liberando memória
    free(threads);

    // Destruindo locks e variáveis de condição
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_men);
    pthread_cond_destroy(&cond_women);

    return 0;
}
