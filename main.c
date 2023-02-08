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


typedef struct List;

int validateDate();

void insertElement();

List *createList();


List *list;
bool listerstellt = false;


int main() {
    //Daten einlesen
    printf("\n\nWelcome by DAP... Dominik's Appointment Planer.\n\nHauptmenü:\n1. \t neue Liste erstellen\n2.\t\n3.\tElement hinzufügen\n4.\tElement finden\n5.\tElement Löschen\n6.\tgebe Appointment aus\n7.\tgebe Liste aus\n8.\tProgramm Schließen\n\nbitte geben Sie die Zahl der entsprechenden Menüfunktion ein und drücken Sie auf enter.\n=============\n");
    do {
        int menuPoint;
        scanf("%d", &menuPoint);
        switch (menuPoint) {
            case 1:
                if (listerstellt = false) {
                    list = createList();
                    if (list != NULL) {
                        printf("Liste wurde erstell");
                        listerstellt = true;
                    }
                } else
                    printf("Error, liste konnte nicht erstellt werden!");

                // List *list = createList();
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
                switch (validateDate(date)) {
                    case 0:
                        printf("Datum in ordnung.\n Bitte geben sie nun eine Beschreibung ein.");
                        scanf("%s", appointmentBeschreibung);


                        insertElement(list->head, date, appointmentBezeichnung, appointmentBeschreibung);



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
        // ClearInputBuffer();    } while (1);
        //struct Element Liste;    return 0;
    } while (1 == 1);
}

void insertElement(Element *head, struct tm date, char *name, char *description) {


    Element *p = head;
    while (p->next != p->next->next &&
           p->next->appointment->start < 5)        //5 muss später durch umgewandeltes date ersetzt werden.
        p = p->next;


    Appointment *dasf = malloc(sizeof(Appointment));
    //dasf->start=date;     //erst umwandlung von time_tm zu time t
    dasf->description = description;

    Element *e = malloc(sizeof(Element));
    e->appointment = dasf;
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
                }
            }
        }
    }


    if (0 <= date.tm_hour <= 24) {
        if (0 <= date.tm_min <= 59) {
            status = 0;
        } else
            status = 1;
    } else
        status = 1;


//überprüfen, ob das Datum bereits vergangen ist
    if (status == 0) {
        if (date.tm_year == nowtime->tm_year) {
            if (date.tm_mon == nowtime->tm_mon) {
                if (date.tm_mday == nowtime->tm_mday) {
                    if (date.tm_hour >= nowtime->tm_hour)
                        status = 0;
                    else
                        status = 4;
                }                                           ///noch nicht fertig!!!!        }
                else
                    status = 4;
            } else
                status = 4;
        } else
            status = 4;

        if (status == 4) {
            if (date.tm_year > nowtime->tm_year) {
                if (date.tm_mon > nowtime->tm_mon) {
                    if (date.tm_mday > nowtime->tm_mday) {
                        if (date.tm_hour > nowtime->tm_hour)
                            status = 0;
                    }
                }
            }
            if (status == 4)
                status = 2;
        }
    }
    return status;
}

List *createList() {
    Element *head = malloc(sizeof(Element));
    Element *tail = malloc(sizeof(Element));
    List *list = malloc(sizeof(List));
    head->next = head->next = tail;
    list->head = head;
    list->tail = tail;
    return list;
}