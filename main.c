#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>


//initialisierung Struktur
typedef struct {
    time_t start;
    char *description;
} Appointment;

typedef struct Element {
    Appointment *appointment;
    struct Element *next;
} Element;

typedef struct {
    Element *head, *tail;
} List;


//Deklaration Variablen und Methoden

typedef struct File File;

int validateDate(struct tm, struct tm);

List *insertElement(List *list, time_t time1, char *name1);

List *createList();

Element *findElement();

List *clearList(List *list);

List *deleteElement();

void printList();

void writeFile(List *list, char *datei);


List *readFile();

void printAppointment(const Element *e);

List *list;

time_t mkgmtime();


int main() {

    //Deklaration Variablen und Zuweisung Zeitvariable
    setbuf(stdout, 0);

    bool listerstellt = false;
    struct tm *nowtime;
    time_t rawtime;
    time(&rawtime);
    nowtime = localtime(&rawtime);
    nowtime->tm_year += 1900;




    printf("Welcome to DAP\nDominiks Appointment Planner\n=============================\n=============================\nPlease set up the standart-file direction. Use 'x' for standart: 'C:\\Users\\User\\Desktop\\termine.txt'");
    char datei[50];

    gets(datei);
    if(strcmp("x", datei) == 0) {
        strcpy(datei,"C:\\Users\\User\\Desktop\\termine.txt");

    }
        list = createList();
    list = readFile(list, rawtime, *nowtime,datei);
    if (list == NULL)
        list = createList();










    printf("Today's Appointments:\n");
    printList(list, nowtime->tm_mday, nowtime->tm_mon, nowtime->tm_year);
    printf("\n\noptions:\n\t->showToday - list all appointments today\n\t->show - list appointments on special day\n\t->list - show all appointments\n\t->new - create new appointment\n\t->search - searching for an appointment\n\t->delete - delete appointment\n\t->deleteList - Delete all Appointments\n\t->quit - save appointments and quit program\n\t\t-help\n");

    char buf[50];
    //Anfang Hauptmenü-Loop

    do {
        printf("==============\n");
        gets(buf);

        //Das Anzeiegn aller Termine am aktuellen Tag
        if (strcmp("showToday", buf) == 0) {
            printList(list, nowtime->tm_mday, nowtime->tm_mon, nowtime->tm_year);
        } else {

            //Das Anzeigen aller Termina an einem bestimmten Tag, den der User über die Konsole eingibt.
            if (strcmp("show", buf) == 0) {
                int day;
                int month;
                int year;
                scanf("%d.%d.%d", &day, &month, &year);
                printList(list, day, month, year);
            } else {

                //Das Anzeigen der aktuellen Liste
                if (strcmp("list", buf) == 0) {
                    Element *e = list->head;
                    while (e->next != e->next->next) {
                        e = e->next;
                        printAppointment(e);
                    }
                } else {

                    //Einfügen eines neuen Termines; daten werden vom User eingegeben.
                    if (strcmp("new", buf) == 0) {
                        time_t nothing = time(NULL);
                        struct tm *date = localtime(&nothing);
                        int day;
                        int month;
                        int year;
                        int hour;
                        int min;
                        char appointmentBezeichnung[500];
                        printf("Please Input your appointment description:  ");
                        gets(appointmentBezeichnung);
                        printf("Please Input the day(dd.mm.yyyy): ");
                        scanf("%d.%d.%d", &day, &month, &year);
                        printf("Please Input the Time(hh:mm): ");
                        scanf("%d:%d", &hour, &min);

                        date->tm_min = min;
                        date->tm_hour = hour;
                        date->tm_mday = day;
                        date->tm_mon = month;
                        date->tm_year = year;

                        switch (validateDate(*date, *nowtime)) {
                            case 0:
                                nothing = mkgmtime(date);
                                list = insertElement(list, nothing, appointmentBezeichnung);
                                break;
                            case 1:
                                printf("Formfehler in Datumsformat");
                                break;
                            case 2:
                                printf("Datum liegt in der Vergangenheit");
                                break;
                        }
                    } else {

                        //Suchen eines termins anhand eingabe vom User
                        if (strcmp("search", buf) == 0) {
                            printf("Appointment: ");
                            char appointmentBezeichnung[500];
                            gets(appointmentBezeichnung);
                            Element *e;
                            e = findElement(list, appointmentBezeichnung);

                            if (e != NULL)
                                printAppointment(e);
                            else
                                printf("Couldn't find appointment.");
                        } else {


                            //einen Termin löschen
                            if (strcmp("delete", buf) == 0) {
                                char *appointmentBezeichnung;
                                gets(appointmentBezeichnung);
                                list = deleteElement(list, appointmentBezeichnung);
                            } else {


                                //Alle termine löschen
                                if (strcmp("deleteList", buf) == 0) {
                                    list = clearList(list);
                                } else {


                                    //Programm beenden && speichern && liste freigeben
                                    if (strcmp("quit", buf) == 0) {
                                        writeFile(list,datei);
                                        //Liste speichern
                                        list = clearList(list);
                                        free(list->head);
                                        free(list->tail);
                                        exit(0);
                                    } else {


                                        //Error handling bei falscher Eingabe
                                        if (strcmp("help", buf) == 0) {
                                            printf("options:\n\t->showToday - list all appointments today\n\t->show - list appointments on special day\n\t->list - show all appointments\n\t->new - create new appointment\n\t->search - searching for an appointment\n\t->delete - delete appointment\n\t->deleteList - Delete all Appointments\n\t->quit - save appointments and quit program\n\t->help\n");
                                        } else {
                                            //printf("Invalid Input. Try 'help'.\n");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } while (true);


}

void printAppointment(const Element *e) {
    struct tm *timme;
    time_t eingangszeit;
    eingangszeit = e->appointment->start;
    time(&eingangszeit);
    timme = localtime(&eingangszeit);

    printf("\n===================================\nElement:\nBeschreibung:\t %s\nZeit:%s\t",
           e->appointment->description, ctime(&e->appointment->start));
}


List *readFile(List *list, time_t rawtime, struct tm nowtime,char *datei) {
    FILE *file;
    char *line[300];
    struct tm date;
    Element *e;
    e = list->head->next;

    file = fopen(datei, "r");

    if (file == NULL) {
        file = fopen(datei, "w");
        fclose(file);
        file = fopen(datei, "r");
        if (file == NULL) {
            perror("error by reading or initialising the file.");

            return list;
        }
    }


    char *appointmentBezeichnung;
    char *storage;

    char string[500];

    while (fgets(string, 500, file)) {

        time_t date_t;
        date_t = time(NULL);
        struct tm *date;
        date = localtime(&date_t);

        char delimiter[] = ";";
        appointmentBezeichnung = strtok(string, delimiter);
        storage = strtok(NULL, delimiter);
        date->tm_mday = atoi(storage);
        storage = strtok(NULL, delimiter);
        date->tm_mon = atoi(storage);
        storage = strtok(NULL, delimiter);
        date->tm_year = atoi(storage);
        storage = strtok(NULL, delimiter);
        date->tm_min = atoi(storage);
        storage = strtok(NULL, delimiter);
        date->tm_hour = atoi(storage);

        if (validateDate(*date, nowtime) == 0) {
            date_t = mkgmtime(date);
            list = insertElement(list, date_t, appointmentBezeichnung);

        }
    }
    fclose(file);


    return list;
}


void printList(List list, int day, int mon, int year) {
    Element *e = list.head;


    //wenn nur 0len übergeben werden wird jedes Appointment ausgegeben.
    if (day == 0 && mon == 0 && year == 0) {
        while (e->next != e->next->next) {
            e = e->next;
            printAppointment(e);

        }
    } else {

//folgendes gibt die Termine des eingegeben Tages aus.
        time_t inputconverted = time(NULL);
        struct tm *input = localtime(&inputconverted);


        input->tm_mday = day;
        input->tm_mon = mon;
        input->tm_year = year;
        input->tm_hour = 0;
        input->tm_min = 0;
        input->tm_sec = 0;

        inputconverted = mkgmtime(*input);


        while (e->next != e->next->next) {
            e = e->next;
            if (inputconverted <= e->appointment->start <= (inputconverted + 60 * 60 * 24))
                printAppointment(e);
        }

    }


}

//wandelt struct tm in time_t um da mktime probleme macht.
time_t mkgmtime(const struct tm *tm) {
    static const int month_day[12] =
            {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

    int month = tm->tm_mon % 12;
    int year = tm->tm_year + tm->tm_mon / 12;
    if (month < 0) {
        month += 12;
        --year;
    }

    const int year_for_leap = (month > 1) ? year + 1 : year;

    time_t rt = tm->tm_sec
                + 60 * (tm->tm_min
                        + 60 * (tm->tm_hour
                                + 24 * (month_day[month] + tm->tm_mday - 1
                                        + 365 * (year - 70)
                                        + (year_for_leap - 69) / 4
                                        - (year_for_leap - 1) / 100
                                        + (year_for_leap + 299) / 400)));
    return rt < 0 ? -1 : rt;
}


int validateDate(struct tm date, struct tm nowtime) {

    int status = 1; //status 0: Datum gültig//status 1: datum ungültig // status 2: datum veraltet //status 4 = zwischenstatus


//überprüfen, ob das Datum stimmt
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

    //Überprüfen ob Uhrzeit stimmt
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

//erstelle neue head/tail Elemente und allokiere den entsprechenen Speicher.
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

//Lösche alle nicht-Pseudo Elemente der Struktur.
List *clearList(List *list) {
    if (list != NULL) {
        Element *p = list->head->next;
        while (p != p->next) {
            Element *e = p;
            p = p->next;
            free(e->appointment->description);
            free(e->appointment);
            free(e);
        }
    }
    return list;
}

void writeFile(List *list,char *datei) {
    File *file;
    file = fopen(datei, "w+");
    if (file != NULL) {

        struct tm *date;
        time_t time;


        Element *e;
        e = list->head;


        while (e->next != e->next->next) {
            e = e->next;
            time = e->appointment->start;
            date = localtime(&time);
            date->tm_min, date->tm_hour;

            fprintf(file, "%s;%d;%d;%d;%d;%d\n", e->appointment->description, date->tm_mday, date->tm_mon,
                    date->tm_year, date->tm_hour, date->tm_min);
        }
        fclose(file);

    } else
        perror("Daten wurden nicht gespeichert!");
}


//Füge ein Element in die Liste ein.
List *insertElement(List *list, time_t time1, char *name1) {

    Element *p = list->head;

    while (p->next != p->next->next && p->next->appointment->start < time1) {
        p = p->next;
    }


    Appointment *appointment = malloc(sizeof(Appointment));
    appointment->start = time1;
    appointment->description = malloc(strlen(name1) + 1);
    memcpy(appointment->description, name1, strlen(name1) + 1);


    Element *e = malloc(sizeof(Element));
    e->appointment = appointment;
    e->next = p->next;
    p->next = e;

    return list;
}

//Gebe ein Element zurück, welches so heißt wie der übergabeparameter
Element *findElement(List *list, char *name) {
    Element *e = list->head;
    while (e->next != e->next->next) {
        e = e->next;
        //if (e->appointment->description == name)
        if (strcmp(name, e->appointment->description) == 0)
            return e;
        e = e->next;
    }
    return NULL;
}

//Lösche ein ELement, mit dem übergebenem Namen.
List *deleteElement(List *list, char *name) {
    Element *p = list->head;
    while (p->next != p->next->next) {
        if (strcmp(name, p->next->appointment->description) == 0) {
            Element *e = p->next;
            p->next = e->next;

            free(e->appointment->description);
            free(e->appointment);
            free(e);
        } else p = p->next;
    }
    return (list);
}