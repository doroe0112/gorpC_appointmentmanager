#include <stdio.h>
#include <stdlib.h>
#include <utime.h>
#include <time.h>
#include <stdbool.h>

typedef struct {
    time_t start;               //die müssen definiert werden, wenn das Element erstellt wird
    char *description;
} Appointment;

typedef struct Element {
    Appointment *appointment;           //muss mit dem zuständigen Appointment verknüpft werden -> Name == deklarationsname
    struct Element *next;
} Element;

typedef struct {
    Element *head, *tail;
} List;


//typedef struct List;

int validateDate();

void insertElement();

List *createList();


void *deleteList(List *list);

List *list;
bool listerstellt = false;


int main() {
    //Daten einlesen
    setbuf(stdout, 0);
    printf("\n\nWelcome by DAP... Dominik's Appointment Planer.\n\nHauptmenü:\n1. \t neue Liste erstellen\n2.\tListe Leeren\n3.\tElement hinzufügen\n4.\tElement finden\n5.\tElement Löschen\n6.\tgebe Appointment aus\n7.\tgebe Liste aus\n8.\tProgramm Schließen\n\nbitte geben Sie die Zahl der entsprechenden Menüfunktion ein und drücken Sie auf enter.\n=============\n");
    do {
        int menuPoint = 3;
        scanf("%d", &menuPoint);
        switch (menuPoint) {

            case 1:
                if (listerstellt == false) {
                    list = createList();
                    if (list != NULL) {
                        printf("Liste wurde erstell");
                        listerstellt = true;
                    } else
                        printf("Liste konnte nicht erstellt werden.");
                } else
                    printf("Error, liste existiert bereits!");

                // List *list = createList();
                break;

            case 2:
                list=deleteList(list);
                printf("Liste gelöscht!");
                break;

            case 3:
                printf("\nneues Appointment anlegen.\nBitte geben Sie die Bezeichnung des Appointments an.\n");
                char *appointmentBezeichnung[50];
                char *appointmentBeschreibung[300];
                scanf("%s", appointmentBezeichnung);
                printf("Bitte legen Sie das Datum für das Appointment %s fest(DD.MM.YYYY).\n", appointmentBezeichnung);
                struct tm date;
                scanf("%d.%d.%d", &date.tm_mday, &date.tm_mon, &date.tm_year);
                printf("Bitte geben sie Uhrzeit ein(hh:mm).\n");
                scanf("%d:%d", &date.tm_hour, &date.tm_min);
                int x = validateDate(date);
                switch (x) {
                    case 0:
                        printf("Datum in ordnung.\n Bitte geben sie nun eine Beschreibung ein.");
                        scanf("%s", appointmentBeschreibung);

                        if (listerstellt == false)
                        {
                            list=createList();
                            listerstellt=true;
                        }

                        insertElement(&list->head, date, &appointmentBezeichnung, &appointmentBeschreibung);



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
                } else printf("abgebrochen.");
                break;
            default:
                fprintf(stderr, "[Warning]: Eingabe ungültig.\n");
        }

        printf("\n\n");
        //ClearInputBuffer();    } while (1);
        //struct Element Liste;    return 0;
    } while (1 == 1);
}


void insertElement(Element *head, struct tm date, char *name, char *description) {
    //time_t dateconv = mktime(&date);
    int zeitAppointment = (date.tm_mday + date.tm_mon * 30 + date.tm_year * 365) * 24 + date.tm_hour;



    Element *p = head;
    //p = p->next;

    while (p->next != p->next->next &&
           p->next->appointment->start < zeitAppointment)
        p = p->next;


    Appointment *appointment = malloc(sizeof(Appointment));
    appointment->start = zeitAppointment;
    appointment->description = description;

    Element *e = malloc(sizeof(Element));
    e->appointment = appointment;
    e->next = p->next;
    p->next = e;


}


int validateDate(struct tm date) {
    time_t rawtime;
    struct tm *nowtime;
    int status = 1; //status 0: Datum gültig//status 1: datum ungültig // status 2: datum veraltet //status 4 = zwischenstatus
    time(&rawtime);
    nowtime = localtime(&rawtime);
    nowtime->tm_year += 1900;




//überprüfen, ob das Datum/Uhrzeit stimmt
    if (date.tm_mon == 1 || date.tm_mon == 3 || date.tm_mon == 5 || date.tm_mon == 7 || date.tm_mon == 9 ||
        date.tm_mon == 11) {
        if (0 < date.tm_mday <= 31)
            status = 0;
        else status = 1;
    } else {
        if (date.tm_mon == 4 || date.tm_mon == 6 || date.tm_mon == 8 || date.tm_mon == 10 ||
            date.tm_mon == 12) {
            if (0 < date.tm_mday <= 31)
                status = 0;
            else status = 1;
        } else {
            if (date.tm_mon == 2) {
                if (date.tm_year % 4 == 0 && date.tm_year % 100 != 0 || date.tm_year % 400 == 0) {
                    if (0 < date.tm_mday <= 28)
                        status = 0;
                    else status = 1;
                } else {
                    if (0 < date.tm_mday <= 27)
                        status = 0;
                    else status = 1;
                }
            }
            else
                status =1;
        }
    }

    if (0 <= date.tm_hour <= 24&&status==0) {
        if (0 <= date.tm_min <= 59) {
            status = 0;
        } else
            status = 1;
    } else
        status = 1;

//überprüfen, ob das Datum bereits vergangen ist

    int zeitVerganngenAppointment = (date.tm_mday + date.tm_mon * 30 + date.tm_year * 365) * 24 + date.tm_hour;
    int zeitVerganngenUntilNow =
            (nowtime->tm_mday + nowtime->tm_mon * 30 + nowtime->tm_year * 365) * 24 + nowtime->tm_hour;

    if (zeitVerganngenAppointment < zeitVerganngenUntilNow && status == 0)
        status = 2;


    return status;
}

List *createList() {
    Element *head = malloc(sizeof(Element));
    Element *tail = malloc(sizeof(Element));
    List *list = malloc(sizeof(List));
    head->next = head->next = tail;
    list->head = head;
    list->tail = tail;

    //tail->appointment->start=malloc(sizeof(Appointment));
    //tail->appointment->start=	2147483647;
    return list;
}


void *deleteList(List *list) {
    if(list!=NULL) {
    Element *p = list->head->next;
            while (p != p->next) {
            Element *e = p;
            p = p->next;
            free(e);
        }
    }
}