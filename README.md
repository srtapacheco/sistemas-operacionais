# Trabalho - Uso de Locks e Variáveis de Condição
## EEL/770 - DEL/Poli/UFRJ | 2024/2

Este repositório contém a implementação do problema **"The Unisex Bathroom Problem"**, baseado no livro [*The Little Book of Semaphores*](http://greenteapress.com/semaphores/LittleBookOfSemaphores.pdf). O objetivo é substituir a solução baseada em semáforos por **locks** e **variáveis de condição**, utilizando a API **pthreads** para sincronização de threads.

## 📌 Objetivos
- Implementar o problema utilizando **locks** e **variáveis de condição**.
- Documentar o **processo de raciocínio**, incluindo os prompts utilizados no ChatGPT e sua contribuição para a solução.
- Explorar como a **IA generativa** pode auxiliar na compreensão de problemas de concorrência.

## 🛠 Tecnologias Utilizadas
- Linguagem: **C**
- API: **pthreads**
- Plataforma: **Linux/MacOS/WSL**
- Compilação: **Makefile**

## 📄 Estrutura do Repositório
```
📂 grupoXX/
 ├── 📂 src/                  # Código-fonte do trabalho
 │   ├── main.c              # Implementação principal
 │   ├── unisex_bathroom.c   # Lógica de sincronização
 │   ├── unisex_bathroom.h   # Arquivo de cabeçalho
 │   └── utils.c             # Funções auxiliares
 ├── 📂 docs/                 # Documentação
 │   ├── relatorio.pdf       # Processo de raciocínio e prompts do ChatGPT
 │   ├── README.md           # Explicação geral do projeto
 ├── Makefile                 # Script para compilação
 ├── .gitignore               # Arquivos ignorados no repositório
└── grupoXX.zip               # Arquivo compactado para submissão
```

## 🔧 Como Compilar e Executar
```sh
make
./unisex_bathroom
```
A execução do programa permitirá configurar o **número de threads** de forma interativa via terminal.

## 📜 Regras e Restrições
✔ O código deve ser parametrizável, sem valores hard-coded.  
✔ Uso de **semáforos não é permitido**.  
✔ O programa deve ser **livre de deadlocks**.  
✔ O tempo entre ações das threads deve ser aleatório (**rand()**, **sleep()**).  
✔ O critério de parada do programa deve ser bem definido.  
✔ O código será testado em um ambiente **Linux**.  


**Autores:** Grupo 3 - Karen, Gabriel e Felipe 

**Disciplina:** EEL/770 - Sistemas Operacionais  

**Instituição:** DEL/Poli/UFRJ  
