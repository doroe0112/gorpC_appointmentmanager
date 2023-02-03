#include <stdio.h>#include <stdlib.h>#include <utime.h>#include <time.h>typedef struct {
time_t start;               //die müssen definiert werden, wenn das Element erstellt wird    char *description;
} Appointment;
typedef struct Element {
    Appointment *appointment;           //muss mit dem zuständigen Appointment verknüpft werden -> Name == deklarationsname    struct Element *next;
} Element;
typedef struct {
    Element *head, *tail;
} List;
int validateDate();
void insertElement();
int main() {
    //Daten einlesen    printf("\n\nWelcome by DAP... Dominik's Appointment Planer.\n\nHauptmenü:\n1. \t neue Liste erstellen\n2.\t\n3.\tElement hinzufügen\n4.\tElement finden\n5.\tElement Löschen\n6.\tgebe Appointment aus\n7.\tgebe Liste aus\n8.\tProgramm Schließen\n\nbitte geben Sie die Zahl der entsprechenden Menüfunktion ein und drücken Sie auf enter.\n=============\n");
    do {
        int menuPoint=3;
        scanf("%d", &menuPoint);
        switch (menuPoint) {
            case 1:
                //Element a = createList;                break;
            case 3:
                printf("\nneues Appointment anlegen.\nBitte geben Sie die Bezeichnung des Appointments an.\n");
                char *appointmentBezeichnung[50];
                scanf("%s",appointmentBezeichnung);
                printf("Bitte legen Sie das Datum für das Appointment %s fest(DD.MM.YYYY).\n", appointmentBezeichnung);
                struct tm date;
                scanf("%d.%d.%d",&date.tm_mday,&date.tm_mon,&date.tm_year);
                printf("Bitte geben sie Uhrzeit ein.\n");
                scanf("%d:%d",&date.tm_hour,&date.tm_min);
                switch (validateDate(date))
                {
                    case 0:
                        //Datum gültig                        insertElement();
                        break;
                    case 1:
                        printf("Formfehler in Datumsformat");
                        break;
                    case 2:
                        printf("Datum liegt in der Vergangenheit");
                        break;
                }
                break;
            case 8:
                printf("\tProgramm Schließen.\nalle Daten werden gespeichert.\nFortfahren?(1=ja;2=nein)\n");
                scanf("%d", &menuPoint);
                if (menuPoint == 1) {
                    //rufe speichern der Liste auf                    printf("Daten gespeichert.\nProgramm wird beendet...");
                    exit(0);
                } else                    printf("abgebrochen.");
                break;
            default:
                fprintf(stderr, "[Warning]: Eingabe ungültig.\n");
        }
        printf("\n\n");
        // ClearInputBuffer();    } while (1);
        //struct Element Liste;    return 0;
    }
    void insertElement(Element *head, struct tm date, char* name)
    {
        Element *p = head;
        while(p->next != p->next->next)             //Script unverstädnlich!!!        p= p->next;
    }
    int validateDate(struct tm date)
    {
        time_t rawtime;
        struct tm * nowtime;
        int status = 1; //status 0: Datum gültig//status 1: datum ungültig // status 2: datum veraltet //status 4 = tage im monat gültig    time ( &rawtime );
        nowtime = localtime ( &rawtime );
        nowtime->tm_year+= 1900;
        if(date.tm_year == nowtime->tm_year)
        {
            if(date.tm_mon == nowtime->tm_mon)
            {
                if(date.tm_mday == nowtime->tm_mday)
                {
                    if(date.tm_hour>=nowtime->tm_hour)
                        status =4;
                    else                    status = 0;
                }                                           ///noch nicht fertig!!!!        }
                else            status = 0;
            }
            else    {
                if(date.tm_year>nowtime->tm_year)
                    status = 4;
                else            status = 0;
            }
            if( 0 < date.tm_mon <=12 && status == 4)//Jahr und monat in ordnung plus tag    {
                status = 1;
            if(date.tm_mon == 1 || date.tm_mon == 3 || date.tm_mon == 5 || date.tm_mon == 7 || date.tm_mon == 9 || date.tm_mon == 11)
            {
                if(0<date.tm_mday<=31)
                    status=0;
                else                status=1;
            }
            else        {
                if(date.tm_mon == 4 || date.tm_mon == 6 || date.tm_mon == 8 || date.tm_mon == 10 || date.tm_mon == 12)
                {
                    if(0<date.tm_mday<=31)
                        status=0;
                    else                    status=1;
                }
                else            {
                    if(date.tm_mon == 2)
                    {
                        if( date.tm_year%4==0 && date.tm_year%100!=0 || date.tm_year%400==0)
                        {
                            if(0<date.tm_mday<=28)
                                status=0;
                            else                            status=1;
                        }
                    }
                }
            }
        }
        else        status = 1;
        if(status == 0)
            status =1;
        if(0<=date.tm_hour<=24)
            if(0<=date.tm_min<=59)
                status=0;
        return status;
    }
    void *createList()
    {
        Element *head = malloc(sizeof(Element));
        Element *tail = malloc(sizeof(Element));
        head->next = tail->next = tail;
        return head;
    }