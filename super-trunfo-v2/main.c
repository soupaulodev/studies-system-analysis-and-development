#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    char state;
    char code[4];
    char name[30];
    int population;
    float areakm2;
    float PIB;
    int touristicPoints;
} Card;

int isValidCityCode(char state, const char *code)
{
    return (code[0] == state && isdigit(code[1]) && isdigit(code[2]) &&
            ((code[1] - '0') * 10 + (code[2] - '0') >= 1 && (code[1] - '0') * 10 + (code[2] - '0') <= 4));
}

void registerCard(Card *card)
{
    do
    {
        printf("Digite a letra do estado (A-H): ");
        scanf(" %c", &card->state);
        card->state = toupper(card->state);
    } while (card->state < 'A' || card->state > 'H');

    do
    {
        printf("Digite o codigo da cidade (ex: A01, B02): ");
        scanf("%s", card->code);
    } while (!isValidCityCode(card->state, card->code));

    getchar();

    printf("Digite o nome da cidade: ");
    scanf(" %[^\n]", card->name);

    printf("Digite a populacao da cidade: ");
    scanf("%d", &card->population);

    printf("Digite a area da cidade em km2: ");
    scanf("%f", &card->areakm2);

    printf("Digite o PIB da cidade: ");
    scanf("%f", &card->PIB);

    printf("Digite a quantidade de pontos turisticos: ");
    scanf("%d", &card->touristicPoints);
}

void showCard(const Card *card)
{
    printf("\n--- Informacoes da Cidade ---\n");
    printf("Estado: %c\n", card->state);
    printf("Codigo: %s\n", card->code);
    printf("Nome: %s\n", card->name);
    printf("Populacao: %d habitantes\n", card->population);
    printf("Area: %.2f km2\n", card->areakm2);
    printf("PIB: R$ %.2f bilhoes\n", card->PIB);
    printf("Pontos turisticos: %d\n", card->touristicPoints);
}

float calculatePopulationalDensity(const Card *card)
{
    return (card->areakm2 == 0) ? 0 : (card->population / card->areakm2);
}

float getValue(const Card *c, int ch, int *rev)
{
    switch (ch)
    {
    case 1:
        return c->population;
    case 2:
        return c->areakm2;
    case 3:
        return c->PIB;
    case 4:
        return c->touristicPoints;
    case 5:
        *rev = 1;
        return calculatePopulationalDensity(c);
    default:
        return 0;
    }
}

void compareCards(const Card *card1, const Card *card2)
{
    int choice1, choice2, reverse1 = 0, reverse2 = 0;
    float value1_1, value1_2, value2_1, value2_2;

    printf("\nEscolha dois criterios de comparacao:\n");
    printf("1 - Populacao\n2 - Area\n3 - PIB\n4 - Pontos turisticos\n5 - Densidade populacional\n");
    scanf("%d %d", &choice1, &choice2);

    value1_1 = getValue(card1, choice1, &reverse1);
    value2_1 = getValue(card2, choice1, &reverse1);
    value1_2 = getValue(card1, choice2, &reverse2);
    value2_2 = getValue(card2, choice2, &reverse2);

    int result1 = (value1_1 > value2_1) ? (!reverse1) : (value1_1 < value2_1) ? reverse1
                                                                              : 0;
    int result2 = (value1_2 > value2_2) ? (!reverse2) : (value1_2 < value2_2) ? reverse2
                                                                              : 0;

    printf("\n--- Resultado da Comparacao ---\n");
    if (result1 && result2)
        printf("Cidade vencedora: %s\n", card1->name);
    else if (!result1 && !result2)
        printf("Cidade vencedora: %s\n", card2->name);
    else
        printf("Empate! Cada cidade venceu em um criterio.\n");
}

void initSuperTrunfo()
{
    Card city1, city2;
    printf("Registro da primeira cidade:\n");
    registerCard(&city1);
    showCard(&city1);

    printf("\nRegistro da segunda cidade:\n");
    registerCard(&city2);
    showCard(&city2);

    compareCards(&city1, &city2);
}

int main()
{
    char running[10] = "sim";

    while (strcmp(running, "sim") == 0)
    {
        initSuperTrunfo();
        printf("\nDeseja continuar? (sim/nao)\n");
        scanf("%9s", running);
    }

    return 0;
}
