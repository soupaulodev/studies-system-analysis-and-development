#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

char tabuleiro[8][8];

void inicializar_tabuleiro()
{
    // Peças pretas (linhas 0 e 1)
    tabuleiro[0][0] = 'r';
    tabuleiro[0][1] = 'n';
    tabuleiro[0][2] = 'b';
    tabuleiro[0][3] = 'q';
    tabuleiro[0][4] = 'k';
    tabuleiro[0][5] = 'b';
    tabuleiro[0][6] = 'n';
    tabuleiro[0][7] = 'r';
    for (int i = 0; i < 8; i++)
        tabuleiro[1][i] = 'p';

    // Peças brancas (linhas 6 e 7)
    for (int i = 0; i < 8; i++)
        tabuleiro[6][i] = 'P';
    tabuleiro[7][0] = 'R';
    tabuleiro[7][1] = 'N';
    tabuleiro[7][2] = 'B';
    tabuleiro[7][3] = 'Q';
    tabuleiro[7][4] = 'K';
    tabuleiro[7][5] = 'B';
    tabuleiro[7][6] = 'N';
    tabuleiro[7][7] = 'R';

    // Espaços vazios
    for (int i = 2; i < 6; i++)
        for (int j = 0; j < 8; j++)
            tabuleiro[i][j] = '.';
}

void exibir_tabuleiro()
{
    printf("\n  a b c d e f g h\n");
    for (int i = 0; i < 8; i++)
    {
        printf("%d ", 8 - i);
        for (int j = 0; j < 8; j++)
        {
            printf("%c ", tabuleiro[i][j]);
        }
        printf("%d\n", 8 - i);
    }
    printf("  a b c d e f g h\n\n");
}

int converter_posicao(char *pos, int *x, int *y)
{
    if (pos[0] < 'a' || pos[0] > 'h' || pos[1] < '1' || pos[1] > '8')
        return 0;

    *x = pos[0] - 'a';
    *y = 8 - (pos[1] - '0');
    return (*x >= 0 && *x < 8 && *y >= 0 && *y < 8);
}

int validar_movimento_peao(int from_x, int from_y, int to_x, int to_y, int jogador)
{
    int direcao = (jogador == 0) ? -1 : 1;
    int linha_inicial = (jogador == 0) ? 6 : 1;

    // Movimento vertical
    if (from_x == to_x)
    {
        if ((to_y == from_y + direcao) && tabuleiro[to_y][to_x] == '.')
            return 1;
        if ((to_y == from_y + 2 * direcao) && (from_y == linha_inicial) &&
            tabuleiro[from_y + direcao][from_x] == '.' && tabuleiro[to_y][to_x] == '.')
            return 1;
    }
    // Captura
    else if ((to_x - from_x > 0 ? to_x - from_x : -(to_x - from_x)) == 1 && (to_y == from_y + direcao) &&
             tabuleiro[to_y][to_x] != '.' && ((jogador == 0 && islower(tabuleiro[to_y][to_x])) || (jogador == 1 && isupper(tabuleiro[to_y][to_x]))))
        return 1;

    return 0;
}

int validar_movimento_cavalo(int dx, int dy)
{
    return (abs(dx) == 2 && abs(dy) == 1) || (abs(dx) == 1 && abs(dy) == 2);
}

int validar_movimento_bispo(int from_x, int from_y, int to_x, int to_y)
{
    if (abs(to_x - from_x) != abs(to_y - from_y))
        return 0;

    int passo_x = (to_x > from_x) ? 1 : -1;
    int passo_y = (to_y > from_y) ? 1 : -1;

    for (int i = 1; i < abs(to_x - from_x); i++)
        if (tabuleiro[from_y + i * passo_y][from_x + i * passo_x] != '.')
            return 0;

    return 1;
}

int validar_movimento_torre(int from_x, int from_y, int to_x, int to_y)
{
    if (from_x != to_x && from_y != to_y)
        return 0;

    if (from_x == to_x)
    {
        int passo = (to_y > from_y) ? 1 : -1;
        for (int y = from_y + passo; y != to_y; y += passo)
            if (tabuleiro[y][from_x] != '.')
                return 0;
    }
    else
    {
        int passo = (to_x > from_x) ? 1 : -1;
        for (int x = from_x + passo; x != to_x; x += passo)
            if (tabuleiro[from_y][x] != '.')
                return 0;
    }
    return 1;
}

int validar_movimento(int from_x, int from_y, int to_x, int to_y, int jogador)
{
    if (from_x < 0 || from_x > 7 || from_y < 0 || from_y > 7)
        return 0;
    if (to_x < 0 || to_x > 7 || to_y < 0 || to_y > 7)
        return 0;

    char peca = tabuleiro[from_y][from_x];
    if (peca == '.' || (jogador == 0 && islower(peca)) || (jogador == 1 && isupper(peca)))
        return 0;

    char destino = tabuleiro[to_y][to_x];
    if ((isupper(peca) && isupper(destino)) || (islower(peca) && islower(destino)))
        return 0;

    int dx = to_x - from_x;
    int dy = to_y - from_y;
    peca = tolower(peca);

    switch (peca)
    {
    case 'p':
        return validar_movimento_peao(from_x, from_y, to_x, to_y, jogador);
    case 'n':
        return validar_movimento_cavalo(dx, dy);
    case 'b':
        return validar_movimento_bispo(from_x, from_y, to_x, to_y);
    case 'r':
        return validar_movimento_torre(from_x, from_y, to_x, to_y);
    case 'q':
        return validar_movimento_bispo(from_x, from_y, to_x, to_y) ||
               validar_movimento_torre(from_x, from_y, to_x, to_y);
    case 'k':
        return abs(dx) <= 1 && abs(dy) <= 1;
    default:
        return 0;
    }
}

int main()
{
    inicializar_tabuleiro();
    int jogador = 0;
    char entrada[10];

    while (1)
    {
        exibir_tabuleiro();
        printf("Jogador %s - Insira seu movimento (ex: e2e4): ", jogador ? "Preto" : "Branco");
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = '\0';

        if (strlen(entrada) != 4)
        {
            printf("Formato inválido! Use 4 caracteres (ex: a1a2)\n");
            continue;
        }

        int x_origem, y_origem, x_destino, y_destino;
        if (!converter_posicao(entrada, &x_origem, &y_origem) ||
            !converter_posicao(entrada + 2, &x_destino, &y_destino))
        {
            printf("Posição inválida!\n");
            continue;
        }

        if (!validar_movimento(x_origem, y_origem, x_destino, y_destino, jogador))
        {
            printf("Movimento inválido!\n");
            continue;
        }

        // Realizar movimento
        char peca = tabuleiro[y_origem][x_origem];
        tabuleiro[y_destino][x_destino] = peca;
        tabuleiro[y_origem][x_origem] = '.';

        // Promoção de peão
        if (tolower(peca) == 'p' && ((jogador == 0 && y_destino == 0) || (jogador == 1 && y_destino == 7)))
            tabuleiro[y_destino][x_destino] = jogador ? 'q' : 'Q';

        jogador = !jogador;
    }
    return 0;
}