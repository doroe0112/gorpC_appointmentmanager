#include <stdio.h>
#include <stdlib.h>
#include <utime.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

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

int validateDate(struct tm, struct tm);

List *insertElement();

List *createList();

Element *findElement();

List *clearList(List *list);

void deleteElement();

void printList();

List *readFile();

void printAppointment(const Element *e);

List *list;


int main() {
    //perror("dies ist ein Fehlertext");


    //Daten einlesen
    setbuf(stdout, 0);

    bool listerstellt = false;
    struct tm *nowtime;
    time_t rawtime;
    time(&rawtime);
    nowtime = localtime(&rawtime);
    nowtime->tm_year += 1900;

    list = createList();
    // list = readFile(list, rawtime);


    printf("Welcome to DAP\nDominiks Appointment Manager\n=============================\n\noptions:\n\t->showToday - list all appointments today\n\t->show - list appointments on special day\n\t->list - show all appointments\n\t->new - create new appointment\n\t->search - searching for an appointment\n\t->delete - delete appointment\n\t->deleteList - Delete all Appointments\n\t->quit - save appointments and quit program\n\t->help\n");

    char buf[50];

    do {
        printf("==============\n");
        gets(buf);


        if (strcmp("showToday", buf) == 0) {
            printList(list, 0, 0, 0);     //überarbeiten!!!!
        } else {


            if (strcmp("show", buf) == 0) {
                int day;
                int month;
                int year;
                scanf("%d.%d.%d", &day, &month, &year);
                printList(list, day, month, year);
            } else {


                if (strcmp("list", buf) == 0) {
                    Element *e = list->head->next;
                    while (e != e->next)
                        printAppointment(e);
                } else {


                    if (strcmp("new", buf) == 0) {
                        struct tm *date;
                        char *appointmentBezeichnung;
                        gets(appointmentBezeichnung);
                        switch (validateDate(*date, *nowtime)) {
                            case 0:
                                list = insertElement(list, date, &appointmentBezeichnung);
                                break;
                            case 1:
                                printf("Formfehler in Datumsformat");
                                break;
                            case 2:
                                printf("Datum liegt in der Vergangenheit");
                                break;
                        }
                    } else {


                        if (strcmp("search", buf) == 0) {
                            char *appointmentBezeichnung;
                            gets(appointmentBezeichnung);
                            printAppointment(findElement(list, appointmentBezeichnung));
                        } else {


                            if (strcmp("delete", buf) == 0) {
                                char *appointmentBezeichnung;
                                gets(appointmentBezeichnung);
                                deleteElement(list, appointmentBezeichnung);
                            } else {


                                if (strcmp("deleteList", buf) == 0) {
                                    list = clearList(list);
                                } else {


                                    if (strcmp("quit", buf) == 0) {

                                        //Liste speichern
                                        exit(0);
                                    } else {


                                        if (strcmp("help", buf) == 0) {
                                            printf("\noptions:\n\t->showToday - list all appointments today\n\t->show - list appointments on special day\n\t->list - show all appointments\n\t->new - create new appointment\n\t->search - searching for an appointment\n\t->delete - delete appointment\n\t->deleteList - Delete all Appointments\n\t->quit - save appointments and quit program\n\t->help\n");
                                        }else{
                                            perror("Input Invalid.");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (buf == 'test')
            printf("\npasst\n");
    } while (true);








    /* printf("\n\nWelcome by DAP... Dominik's Appointment Plane"
            "r.\n\nHauptmenü:\n1. \t neue Liste erstellen\n2.\tListe Leeren\n3.\tElement hinzufügen\n4.\tElement finden\n5.\tElement Löschen\n6.\tgebe Appointment aus\n7.\tgebe Liste aus\n8.\tProgramm Schließen\n\nbitte geben Sie die Zahl der entsprechenden Menüfunktion ein und drücken Sie auf enter.\n=============\n");
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
                 list = clearList(list);
                 printf("Liste gelöscht!");
                 break;

             case 3:
                 printf("\nneues Appointment anlegen.\nBitte geben Sie die Bezeichnung des Appointments an.\n");
                 char *appointmentBezeichnung[50];
                 scanf("%s", appointmentBezeichnung);


                 printf("Bitte legen Sie das Datum für das Appointment %s fest(DD.MM.YYYY).\n", appointmentBezeichnung);
                 struct tm *date;
                 date = nowtime;
                 scanf("%d.%d.%d", &date->tm_mday, &date->tm_mon, &date->tm_year);
                 printf("Bitte geben sie Uhrzeit ein(hh:mm).\n");
                 scanf("%d:%d", &date->tm_hour, &date->tm_min);

             time_t time1;
             time1= mktime(&date);


                 switch (validateDate(*date, *nowtime)) {
                     case 0:

                         if (listerstellt == false) {
                             list = createList();
                             listerstellt = true;
                         }

                         list = insertElement(list, time1, &appointmentBezeichnung);



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

             case 4:                                                                             //ACHTUNG! UNVOLLSTÄNDIG -> möglichkeit mehrere gleichnamige appointments zu haben + datumsdarstellung nicht gut!

                 printf("Element suchen\nBitte geben Sie den Titel ein:\n ");
                 char *bezeichnung[50];
                 scanf("%s", bezeichnung);
                 Element *e = findElement(list, bezeichnung);

                 //char str[26];
                 //ctime_s(str, sizeof str, &e->appointment->start);
                 //printf("%s", str);

                 if (e == NULL)
                     printf("kein Element mit dieser Beschreibung vorhanden.\n");
                 else
                     printAppointment(e);
                 break;//schauen, obs funktioniert! (C99 fehler?)


             case 5:
                 printf("Element löschen\nBitte geben Sie den Titel ein:\n ");
                 char *bezeichnung5[50];
                 scanf("%s", bezeichnung5);
                 deleteElement(list, bezeichnung5);             //testen
                 break;

             case 6:


                 break;


             case 7:
                 printf("Bitte legen Sie das Datum für das Appointment %s fest(DD.MM.YYYY).\n", appointmentBezeichnung);
                 int day;
                 int mon;
                 int year;
                 scanf("%d.%d.%d", &day, &mon, &year);
                 printList(list, day, mon, year, *nowtime);


                 break;

             case 8:
                 printf("\tProgramm Schließen.\nalle Daten werden gespeichert.\nFortfahren?(1=ja;2=nein)\n");
                 scanf("%d", &menuPoint);
                 if (menuPoint == 1) {
                     //rufe speichern der Liste auf                    printf("Daten gespeichert.\nProgramm wird beendet...");
                     exit(0);
                 } else printf("abgebrochen.");
                 break;

                 fprintf(stderr, "[Warning]: Eingabe ungültig.\n");

             default:
                 fprintf(stderr, "[Warning]: Eingabe ungültig.\n");
                 break;

                 printf("_____________\n\n\n");
                 //ClearInputBuffer();    } //while (1);
                 //struct Element Liste;    return 0;
         }
     } while (1 == 1);*/
}

void printAppointment(const Element *e) {
    printf("\n===================================\nElement:\nBeschreibung:\t %s\nZeit:\t%s\nnext Element:\t%s\n===================================\n",
           e->appointment->description,
           ctime(&e->appointment->start),
           e->next->appointment->description);
}

List *readFile(List *list, time_t rawtime) {
    FILE *file;
    char *line[300];
    struct tm date;
    Element *e;
    e = list->head->next;

    file = fopen("C:\\Users\\User\\Desktop\\datei.txt", "w+");

    if (file == NULL) {
        perror("error by reading or initialising the file.");
        return list;
    }


    while (fgets(line, 300, file) != NULL) {
        //scanf("%d;%s",date,e->appointment->description);
        //time(date);
        //if(zeitrichtig)
        // insertElement(list,)
        printf("%s", line);
    }


}

void printList(List list, int day, int mon, int year) {
    Element *e = list.head->next;

    if (day == 0 && mon == 0 && year == 0) {
        while (e != e->next)
            printAppointment(e);
    } else {


        time_t inputconverted;
        struct tm *input;
        input->tm_mday = day;
        input->tm_mon = mon;
        input->tm_year = year;
        input->tm_hour = input->tm_min = input->tm_sec = 0;
        inputconverted = mktime(&input);


        printf("ghj");
/*
        int zeitVerganngenAppointment = (day + mon * 30 + year * 365) * 24;
        if (mon == 1 || mon == 3 || mon == 5 || mon == 7 || mon == 9 || mon == 11)
            zeitVerganngenAppointment+=mon*31*24;
        if(mon==4 || mon == 6 || mon == 8 || mon == 10 || mon == 12)
            zeitVerganngenAppointment+=mon*30*24;
        if(mon==2)
            if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0)
                zeitVerganngenAppointment+=mon*12*27*/




        /* while (e != e->next) {
             if (nowtime->tm_year == date.tm_year && nowtime->tm_mon == date.tm_mon && nowtime->tm_mday == date.tm_mday)
                 printf("Appointment %s ist am %d", e->appointment->description, e->appointment->start);

         }*/

    }


}


List *insertElement(List *list, time_t time1, char *name) {
    //time_t dateconv = mktime(&date);


    Element *p = list->head;

    while (p->next != p->next->next &&
           p->next->appointment->start < time1)
        p = p->next;


    Appointment *appointment = malloc(sizeof(Appointment));
    appointment->start = time1;
    appointment->description = name;

    Element *e = malloc(sizeof(Element));
    e->appointment = appointment;
    e->next = p->next;
    p->next = e;

    return list;
}


int validateDate(struct tm date, struct tm nowtime) {

    int status = 1; //status 0: Datum gültig//status 1: datum ungültig // status 2: datum veraltet //status 4 = zwischenstatus


//überprüfen, ob das Datum/Uhrzeit stimmt
    if (date.tm_mon == 1 || date.tm_mon == 3 || date.tm_mon == 5 || date.tm_mon == 7 || date.tm_mon == 9 ||
        date.tm_mon == 11) {
        if (0 < date.tm_mday <= 31)
            status = 0;
        else status = 1;
    } else {
        if (date.tm_mon == 4 || date.tm_mon == 6 || date.tm_mon == 8 || date.tm_mon == 10 ||
            date.tm_mon == 12) {
            if (0 < date.tm_mday <= 30)
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
            } else
                status = 1;
        }
    }

    if (0 <= date.tm_hour <= 24 && status == 0) {
        if (0 <= date.tm_min <= 59) {
            status = 0;
        } else
            status = 1;
    } else
        status = 1;

//überprüfen, ob das Datum bereits vergangen ist

    int zeitVerganngenAppointment = (date.tm_mday + date.tm_mon * 30 + date.tm_year * 365) * 24 + date.tm_hour;
    int zeitVerganngenUntilNow =
            (nowtime.tm_mday + nowtime.tm_mon * 30 + nowtime.tm_year * 365) * 24 + nowtime.tm_hour;

    if (zeitVerganngenAppointment < zeitVerganngenUntilNow && status == 0)
        status = 2;


    return status;
}

List *createList() {
    Element *head = malloc(sizeof(Element));
    Element *tail = malloc(sizeof(Element));
    List *list = malloc(sizeof(List));
    head->next = tail->next = tail;
    list->head = head;
    list->tail = tail;

    //tail->appointment->start=malloc(sizeof(Appointment));
    //tail->appointment->start=	2147483647;
    return list;
}


List *clearList(List *list) {
    if (list != NULL) {
        Element *p = list->head->next;
        while (p != p->next) {
            Element *e = p;
            p = p->next;
            free(e);
        }
    }
    return list;
}

Element *findElement(List list, char *name) {
    Element *e = list.head->next;
    while (e != e->next) {
        if (e->appointment->description == name) return e;
        e = e->next;
    }
    return NULL;
}


void deleteElement(List list, char *name) {
    Element *p = list.head;
    while (p->next != p->next->next) {
        if (p->next->appointment->description == name) {
            Element *e = p->next;
            p->next = e->next;
            free(e);
        } else p = p->next;
    }
}