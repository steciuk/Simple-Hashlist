#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Hashlist
{
    char nazwa[50];
    char populacja[12];
    struct Hashlist * next;
}Hashlist;


Hashlist * dodajEl(Hashlist * glowa, char nazwa[], char populacja[])
{
    Hashlist * nowy = malloc(sizeof (Hashlist));
    strcpy(nowy->nazwa, nazwa);
    strcpy(nowy->populacja, populacja);
    nowy->next = glowa;
    return nowy;
}

int koduj(char nazwa[])
{
    int k = 0;
    for(int i=0; i<strlen(nazwa); i++)
    {
        k += (int)nazwa[i];
    }
    k += strlen(nazwa);
    return k % 512;
}

void wypiszPodlisty(Hashlist * podlista)
{
    Hashlist * temp = podlista;
    while(temp->next != NULL)
    {
        printf(" %s %s\n", temp->nazwa, temp->populacja);
        temp = temp->next;
    }
}

void wypiszHashListe(Hashlist ** lista)
{
    for(int i=0; i<512; i++)
    {
        printf("%d \n", i);
        wypiszPodlisty(lista[i]);
    }
}


void zwolnijPodlisty(Hashlist * podlista)
{
    Hashlist * temp = NULL;
    while(podlista != NULL)
    {
        temp = podlista;
        podlista = podlista->next;
        free(temp);
    }
}

int ilePustych(Hashlist ** lista)
{
    int k = 0;
    for(int i=0; i<512; i++)
    {
        if(lista[i]->next == NULL)
        {
            k += 1;
        }
    }

    return k;
}

void gdzieKolizje(Hashlist ** lista)
{
    int k = 0;
    int w = 0;
    Hashlist * temp = NULL;
    for(int i=0; i<512; i++)
    {
        k = 0;
        temp = lista[i];
        while (temp->next != NULL)
        {
            k += 1;
            temp = temp->next;
        }

        if(k>1)
        {
            printf("kolizja w: %d, wielkosci: %d\n", i, k);
            w += 1;
        }
    }

    printf("liczba kolizji: %d\n", w);
}

Hashlist * wyszukajWPodliscie(Hashlist * lista, char tab[])
{
    Hashlist * temp = lista;
    while(temp->next != NULL)
    {
        if(strcmp(temp->nazwa, tab) == 0)
        {
            return temp;
        }
        temp = temp->next;
    }

    return NULL;
}

Hashlist * wyszukaj(Hashlist ** lista, char tab[])
{
    if((lista[koduj(tab)]->next) == NULL)
    {
        return NULL;
    }
    else
    {
        Hashlist * temp = wyszukajWPodliscie(lista[koduj(tab)], tab);

        if(temp == NULL)
        {
            return NULL;
        }
        else
        {
            return temp;
        }
    }

    return NULL;
}

Hashlist * usunWPodliscie(Hashlist * lista, char tab[])
{
    Hashlist * temp = lista;
    Hashlist * temp2 = lista;
    int k=0;

    while(temp->next != NULL)
    {
        if(strcmp(temp->nazwa, tab) == 0)
        {
            if(k == 0)
            {
                lista = temp->next;
                free(temp);
                printf("panstwo usuniente\n");
                return lista;
            }
            else
            {
                temp2->next = temp->next;
                free(temp);
                printf("panstwo usuniente\n");
                return NULL;
            }
        }
        k+=1;
        temp2 = temp;
        temp = temp->next;
    }

    printf("brak takiego panstwa\n");
    return NULL;
}

Hashlist * usun(Hashlist ** lista, char tab[])
{


    if((lista[koduj(tab)]->next) == NULL)
    {
        printf("brak takiego panstwa\n");
    }
    else
    {
        return usunWPodliscie(lista[koduj(tab)], tab);
    }
    return NULL;
}

void wyswietl(Hashlist ** lista)
{
    printf("podaj nazwe panstwa: \n");
    char tab[50];
    fgets(tab, sizeof(tab), stdin);
    tab[strlen(tab)-1] = '\0';
    tab[0] = toupper(tab[0]);

    Hashlist * temp = wyszukaj(lista, tab);

    if(temp == NULL)
    {
        printf("brak takiego panstwa\n");
    }
    else
    {
        printf("populacja %s: %s\n", temp->nazwa, temp->populacja);
    }
}

void dopisz(Hashlist ** lista)
{
    printf("podaj nazwe panstwa: ");

    char tab[50];
    fgets(tab, sizeof(tab), stdin);
    tab[strlen(tab)-1] = '\0';
    tab[0] = toupper(tab[0]);

    Hashlist * temp = wyszukaj(lista, tab);

    if(temp == NULL)
    {
        printf("podaj populacje panstwa: ");

        char tab2[12];
        fgets(tab2, sizeof(tab2), stdin);
        tab2[strlen(tab2)-1] = '\0';

        lista[koduj(tab)] = dodajEl(lista[koduj(tab)], tab, tab2);
    }
    else
    {
        printf("istnieje juz takie panstwo\n");
    }
}



int main()
{
	FILE * nazwyPanstwPlik;
	FILE * populacjaPanstwPlik;
	nazwyPanstwPlik = fopen("panNazwy.txt", "r");
	populacjaPanstwPlik = fopen("panPopulacja.txt", "r");

	int liczbaPanstwPlik = 230;
	int liczbaDanychPlik = 2;
	char *** listaPanstwPlik = malloc(liczbaPanstwPlik * sizeof(char**));
	for(int i = 0; i < liczbaPanstwPlik; i++)
	{
        listaPanstwPlik[i] = malloc(liczbaDanychPlik * sizeof(char*));
	}

	char temp1[50];
	char temp2[50];
    int w = 0;
	while (fgets(temp1, sizeof(temp1), nazwyPanstwPlik) != NULL && fgets(temp2, sizeof(temp2), populacjaPanstwPlik) != NULL)
	{
        if(temp1[strlen(temp1)-2] == '\r') temp1[strlen(temp1)-2] = '\0';
        if(temp2[strlen(temp2)-2] == '\r') temp2[strlen(temp2)-2] = '\0';


        listaPanstwPlik[w][0] = malloc(strlen(temp1) + 1);
        listaPanstwPlik[w][1] = malloc(strlen(temp2) + 1);
		strcpy(listaPanstwPlik[w][0], temp1);
		strcpy(listaPanstwPlik[w][1], temp2);
		++w;
	}


    Hashlist ** hashlist = malloc(512 * sizeof(Hashlist*));

    for(int i=0; i<512; i++)
    {
        hashlist[i] = malloc(sizeof(Hashlist));
        hashlist[i]->next = NULL;
    }

    for(int i=0; i<230; i++)
    {
        hashlist[koduj(listaPanstwPlik[i][0])] = dodajEl(hashlist[koduj(listaPanstwPlik[i][0])], listaPanstwPlik[i][0], listaPanstwPlik[i][1]);
    }




    int wejscie;
    do
    {
        puts("\n1 - wypisz wszystkie pozycje");
        puts("2 - wyszukaj pozycje");
        puts("3 - dodaj pozycje");
        puts("4 - usun pozycje");
        puts("5 - stopien wypelnienia");
        puts("6 - wypisz kolizje");
        puts("0 - wyjscie");

        wejscie = getchar();

        char c;
        do
        {
            c = getchar();
        }while(c != '\n' && c != EOF);

        switch (wejscie)
        {
            case '1':
                wypiszHashListe(hashlist);
                break;

            case '2':
                wyswietl(hashlist);
                break;

            case '3':
                dopisz(hashlist);
                break;

            case '4':
                printf("podaj nazwe panstwa: ");

                char tab[50];
                fgets(tab, sizeof(tab), stdin);
                tab[strlen(tab)-1] = '\0';
                tab[0] = toupper(tab[0]);

                Hashlist * temp = usun(hashlist, tab);
                if(temp != NULL)
                {
                    hashlist[koduj(tab)] = temp;
                }
                break;

            case '5':
                printf("stopien wypelnienia: %d/512\n", 512 - ilePustych(hashlist));
                break;

            case '6':
                gdzieKolizje(hashlist);
                break;

            case '0':
                break;

            default:
                printf("niepoprawne wejscie\n");
                break;
        }



    }while(wejscie != '0');


    for(int i=0; i<512; i++)
    {
        zwolnijPodlisty(hashlist[i]);
    }

    free(hashlist);

	for (int k = 0; k < 230; k++)
	{
		free(listaPanstwPlik[k][0]);
		free(listaPanstwPlik[k][1]);
	}

	for(int i = 0; i < liczbaPanstwPlik; i++)
	{
        free(listaPanstwPlik[i]);
	}

	free(listaPanstwPlik);


	fclose(nazwyPanstwPlik);
	fclose(populacjaPanstwPlik);
	return 0;
}

