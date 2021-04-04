#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct student {

    char surname[13];
    char name[10];
    char birthday[11];
    int marks[5];
    struct student* next;

} student;

void dataRead(char* fileName, student** list, student** ratingList);

void printList(student* ptr);

void newNodeBySurname(student* ptr, student** headList);

void newNodeByRating(student* ptr, student** headList);

void deleteNode(student** headList);

int compareBySurname(student* one, student* two);

int compareByRating(student* one, student* two);

double rating(student* ptr);

void printLowestRating(student* headList);

int main(void) {

    student* listBySurname = NULL;
    student* listByRating = NULL;

    dataRead("input.txt", &listBySurname, &listByRating);

    printf("List before:\n");
    printList(listBySurname);
    printf("\n");

    deleteNode(&listBySurname);

    printf("List after:\n");
    printList(listBySurname);
    printf("\n");


    printf("5 students with lowest rating:\n");
    printLowestRating(listByRating);
    printf("\n");

}

void dataRead(char* fileName, student** list, student** ratingList) {
    FILE* f = fopen_s(fileName, "r");
    do {
        student* ptrFirst = (student*)malloc(sizeof(student));
        student* ptrSecond = (student*)malloc(sizeof(student));
        fscanf_s(f, "%s %s %s", ptrFirst->surname, ptrFirst->name, ptrFirst->birthday);
        for (int i = 0; i < 5; ++i)
            fscanf_s(f, "%i", &(ptrFirst->marks[i]));

        strcpy_s(ptrSecond->surname, ptrFirst->surname);
        strcpy_s(ptrSecond->name, ptrFirst->name);
        strcpy_s(ptrSecond->birthday, ptrFirst->birthday);
        for (int i = 0; i < 5; ++i)
            ptrSecond->marks[i] = ptrFirst->marks[i];

        newNodeBySurname(ptrFirst, list);
        newNodeByRating(ptrSecond, ratingList);
    } while (fgetc(f) != EOF);
    fclose(f);
}

void printList(student* ptr) {
    printf("+---+-------------+----------+----------+--------------+\n");
    printf("| # |   Surname   |   Name   |   Date   |Marks:        |\n");
    printf("+---+-------------+----------+----------+--------------+\n");
    int i = 0;
    while (ptr != NULL) {
        printf("|#%2i|%13s|%10s|%10s|", ++i, ptr->surname, ptr->name, ptr->birthday);
        for (int j = 0; j < 5; ++j)
            printf("%2i|", ptr->marks[j]);
        printf("\n+---+-------------+----------+----------+--------------+\n");
        ptr = ptr->next;
    }
}

void newNodeBySurname(student* ptr, student** headList) {
    if ((*headList) == NULL) {
        (*headList) = ptr;

        (*headList)->next = NULL;
        return;
    }
    if (compareBySurname(*headList, ptr) < 0) //cортування по прізвищу протилежного алфавіту
    {
        ptr->next = *headList; //додавання нового елемента на початок списку
        *headList = ptr;
        return;
    }
    student* current = *headList;  //щоб не затерти звязок
    while ((current->next) != NULL) {
        if (compareBySurname(current, ptr) > 0 && compareBySurname(current->next, ptr) <= 0)//сортування
        {
            ptr->next = current->next;
            current->next = ptr;
            return;
        }
        else current = current->next;
    }
    current->next = ptr;//додаєм останній елемент
    ptr->next = NULL;//присвоєння NULL кінець списку
}

void newNodeByRating(student* ptr, student** headList) {
    if ((*headList) == NULL) {
        (*headList) = ptr;

        (*headList)->next = NULL;
        return;
    }
    if (compareByRating(*headList, ptr) < 0) //cортування по прізвищу протилежного алфавіту
    {
        ptr->next = *headList; //додавання нового елемента на початок списку
        *headList = ptr;
        return;
    }
    student* current = *headList;  //щоб не затерти звязок
    while ((current->next) != NULL) {
        if (compareByRating(current, ptr) > 0 && compareByRating(current->next, ptr) <= 0)//сортування
        {
            ptr->next = current->next;
            current->next = ptr;
            return;
        }
        else current = current->next;
    }
    current->next = ptr;//додаєм останній елемент
    ptr->next = NULL;//присвоєння NULL кінець списку
}

int compareBySurname(student* one, student* two) {
    return strcmp(one->surname, two->surname); //перевірка яке прізвище скоріше за алфавітом
}

int compareByRating(student* one, student* two) {
    double rOne = rating(one);
    double rTwo = rating(two);

    return rOne == rTwo ? 0 : rOne > rTwo ? -1 : 1;
}

void deleteNode(student** headList) {
    student* prev = NULL, * current = *headList;
    while (current != NULL) {
        if ((!prev) &&
            (current->marks[0] == 2 || current->marks[1] == 2 || current->marks[2] == 2 || current->marks[3] == 2 ||
                current->marks[4] == 2)) // видалення якщо 2 в хеді
        {
            *headList = current->next;
            free(current);
            current = *headList;
            continue;
        }
        if (current->marks[0] == 2 || current->marks[1] == 2 || current->marks[2] == 2 || current->marks[3] == 2 ||
            current->marks[4] == 2) //якщо не в хеді
        {
            prev->next = current->next; //звязка через 1
            free(current);
            current = prev->next;
            continue;
        }

        prev = current;
        current = prev->next;//перехід до кінця списку
    }
}

double rating(student* ptr) {
    double sum = 0;
    for (int i = 0; i < 5; i++) sum += ptr->marks[i];
    return sum;
}

void printLowestRating(student* headList) {
    student* ptr = headList;

    printf("+---+-------------+----------+----------+--------------+\n");
    printf("|List of students with lowest rating                   |\n");
    printf("+---+-------------+----------+----------+--------------+\n");
    printf("| # |   Surname   |   Name   |   Date   |Marks:        |\n");
    printf("+---+-------------+----------+----------+--------------+\n");
    for (int i = 0; i < 5 && ptr->next; i++) {
        printf("|#%2i|%13s|%10s|%10s|", i + 1, ptr->surname, ptr->name, ptr->birthday);
        for (int j = 0; j < 5; ++j)
            printf("%2i|", ptr->marks[j]);
        printf("\n+---+-------------+----------+----------+--------------+\n");
        ptr = ptr->next;
    }

    printf("\n\n");
}
