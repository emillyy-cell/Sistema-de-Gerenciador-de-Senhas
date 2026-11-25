# Sistema de Gerenciamento de Senhas de Atendimento Clínico

Projeto em linguagem C (Desafio – Algoritmos e Pensamento Computacional, UDF).
Cadastra pacientes, classifica por prioridade, emite senhas e controla a ordem de atendimento.

## Como compilar e executar

```bash
gcc -Wall -Wextra -o senhas main.c
./senhas
# ou, com o exemplo pronto:
./senhas < exemplo_entrada.txt
```

## Regras de classificação

| Código | Categoria | Quem entra |
|---|---|---|
| `U001` | Urgente | Idade **superior a 80** anos (80 exatos não é urgente) |
| `P001` | Preferencial | Gestante, lactante, necessidade especial, obesidade, mulher com 60+ anos, homem com 65+ anos |
| `N001` | Normal | Nenhuma condição de prioridade |

- O teste de **Urgente vem primeiro**: uma mulher de 85 anos também seria preferencial, mas a categoria mais prioritária vence.
- A numeração da senha é sequencial **por categoria** (N001, N002, ...).
- As perguntas sobre gestante e lactante são feitas para todos os pacientes (1 = sim, 0 = não).

## Regras de chamada

1. Urgentes antes de Preferenciais, e Preferenciais antes de Normais.
2. Dentro de cada categoria vale a ordem de emissão (**FIFO**, fila).
3. Senha já atendida não volta para a fila.
4. Só é possível chamar outra senha depois de finalizar a atual.
5. Aceita vários cadastros durante a execução (até 100 por fila).

> A classificação por idade é só uma regra de programação desta prova e não substitui avaliação clínica.

## Estrutura do código (modularização)

| Função | Responsabilidade |
|---|---|
| `cadastrar()` | Lê os dados, classifica, gera a senha e coloca na fila |
| `classificar()` | Retorna `'U'`, `'P'` ou `'N'` |
| `proximaFila()` | Decide qual fila é chamada primeiro (U, depois P, depois N) |
| `consultar()`, `chamar()`, `emAtendimento()`, `finalizar()` | Registro de atendimento |
| `painel()` | Painel textual |

Usa apenas `scanf`, `if/else`, `switch`, `for`, `do-while`, vetores e funções.

Estrutura de dados: uma matriz `fila[3][MAX]` (uma fila por categoria) e dois vetores, `inicio` (quantos já foram chamados) e `fim` (quantos já foram emitidos). Emitir uma senha coloca o número no final da fila; chamar lê o número da posição `inicio` e avança `inicio`. Assim vale a ordem de chegada (FIFO) e a senha atendida nunca volta.

## Menu

```
1-Cadastrar  2-Proxima  3-Chamar  4-Em atendimento
5-Finalizar  6-Painel   0-Sair
```

## Exemplo demonstrativo

Cadastros (nesta ordem): homem 30 anos → `N001`; mulher 28 gestante → `P001`; homem 82 → `U001`; mulher 62 → `P002`; homem 40 obeso → `P003`.

Painel logo após os cadastros:

```
Proximas: U001 P001 P002 P003 N001
Em espera: Urgente=1 | Preferencial=3 | Normal=1
```

Ordem de chamada resultante: **U001 → P001 → P002 → P003 → N001**.

Após chamar e finalizar `U001` e chamar `P001`:

```
Senha chamada: P001
Proximas: P002 P003 N001
Em espera: Urgente=0 | Preferencial=2 | Normal=1
```

A saída completa está em `exemplo_saida.txt`.
