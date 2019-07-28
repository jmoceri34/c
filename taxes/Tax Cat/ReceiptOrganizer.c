// Joe Moceri
// 12/22/2011
// Tax Receipts Program

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define RECSIZE 50

// Constants
const int TAXSIZE = 1500;
const int PATHSIZE = 100;
const int NAMESIZE = 50;
const int SIZE = 10;

// Structure for Receipt
struct receipt {

    char name[RECSIZE];
    int month;
    int day;
    double total;
    int num;

};

// Structure for Receipt temporary
struct temporary {

    char name[RECSIZE];
    int month;
    int day;
    double total;
    int num;

};

//Functions
void getFileNames(char nameIn[], char nameOut[]);
void storeInfo(struct receipt tax_read[], FILE *ifp, int *count, double *overall);
void sortInfo(struct receipt tax[], struct temporary temp[], int count);
void printInfo(struct receipt tax[], int count);
void Swap(struct receipt *tax, struct receipt *inst, struct temporary *temp);
void getYear(FILE *ifp, int *year);
void printFile(struct receipt tax, FILE *ofp);
void printStart(int year, FILE *ofp, char catName[]);
void printEnd(int i, int year, double overall, FILE *ofp);
void getMainCat(FILE *ifp, char catName[]);
void getSubInfo(FILE *ifp, int *amt, int num[], char name[][NAMESIZE]);
void printSubStart(FILE *ofp, char name[NAMESIZE]);
void printSubEnd(FILE *ofp, int count, double overall, char name[NAMESIZE]);
void printSub(FILE *ofp, int subAmt, char subName[][NAMESIZE], int subNum[], int tally, struct receipt tax[]);

int main(){

    // Define structures to use
    struct receipt tax[TAXSIZE];
    struct temporary temp[TAXSIZE];

    // String variables 
    char mainCat[NAMESIZE];
    char fileIn[NAMESIZE];
    char fileOut[NAMESIZE];
    char pathIn[PATHSIZE];
    char pathOut[PATHSIZE];

    // Copies file paths into variables
    strcpy(pathIn, "../Input/");
    strcpy(pathOut, "../Output/");

    // Declares file pointers
    FILE* ifp;
    FILE* ofp;

    // Calls the function getFileNames to retrieve the file names
    getFileNames(fileIn, fileOut);

    // Copies the names onto the ends of the file paths
    strcat(pathIn, fileIn);
    strcat(pathOut, fileOut);

    // Opens the input file
    ifp=fopen(pathIn, "r");

    int year;
    // Gets the year from the file
    getYear(ifp, &year);

    // Retrieves the main category name
    getMainCat(ifp, mainCat);

    // Initializes the subAmt variable, an array to store the numbers for each
    // sub category, and a 2D array to store the names of those sub categories
    int subAmt;
    int subNum[SIZE];
    char subName[SIZE][NAMESIZE];

    // Function call to gather the sub category information
    getSubInfo(ifp, &subAmt, subNum, subName);

    // Variables
    int rec_amount=0; double overall=0;

    // Reads in information from file into array of structures
    storeInfo(tax, ifp, &rec_amount, &overall);

    // Closes the input file
    fclose(ifp);

    // Sorts first by month, then day, then name, then total
    sortInfo(tax, temp, rec_amount);

    // Opens the output file
    ofp=fopen(pathOut, "w");

    // Prints to the file the beginning information
    printStart(year, ofp, mainCat);

    // Prints to the file the sorted array of structures
    // seperated into sub-categories.
    printSub(ofp, subAmt, subName, subNum, rec_amount, tax);

    // Prints to the screen the sorted array of structures
    printInfo(tax, rec_amount);

    // Prints to the file the end information
    printEnd(rec_amount, year, overall, ofp);

    // Closes the output file
    fclose(ofp);
    // Pauses the program so you can see the contents when
    // ran from the executable
    system("PAUSE");

    return 0;
}

void printInfo(struct receipt tax[], int count){

    // For loop to print the sorted information to the screen
    int i;
    for(i=0;i<count;i++){
        printf("Name: %s\nDate: %d/%d\n                -Receipt Total: %.2lf\n", tax[i].name, tax[i].month, tax[i].day, tax[i].total);
    }

}

void getFileNames(char nameIn[], char nameOut[]){

    // Variables
    char tempIn[NAMESIZE];
    char tempOut[NAMESIZE];
    char pathIn[PATHSIZE];
    char pathOut[PATHSIZE];

    int ans;

    // While loop to take in the name of the input file and check if that file name
    // exists and makes sure it has .txt within it. If all checks out good,
    // break out
    while(1){
        printf("Please enter the name of the file (in file.txt format):\n");
        scanf("%s", &tempIn);
        strcpy(pathIn, "../Input/");
        strcat(pathIn, tempIn);
        if(access(pathIn, F_OK) == -1){
            printf("Sorry, that file doesn't exist.\n\n");
            continue;
        }
        else if(strstr(tempIn, ".txt") == NULL){
            printf("Sorry, you must put .txt at the end of the filename.\n\n");
            continue;
        }
        else
            break;
    }

    // While loop to take in the name of the output file. Checks to make sure 
    // the input and output files are not the same, and also checks it has 
    // the .txt extension. Last, checks to see if the file already exists, and
    // if it does, prompts the user whether or not they want to overwrite it. 
    // If all checks out, then break out.
    while(1){
        printf("Please enter a file name to save the information to (in save.txt format):\n");
        scanf("%s", &tempOut);
        strcpy(pathOut, "../Output/");
        strcat(pathOut, tempOut);
        if(strcmp(tempIn, tempOut)==0){
            printf("Sorry, you can't use the same file name to both read from and write to.\n\n");
            continue;
        }
        else if(strstr(tempOut, ".txt") == NULL){
            printf("Sorry, you must put .txt at the end of the filename.\n\n");
            continue;

        }
        else if(access(pathOut, F_OK) == 0){
            printf("This file already exists, do you want to overwrite it?(yes=1 no=0)\n");
            scanf("%d", &ans);
            if(ans == 1)
                break;
        }
        else
            break;
    }

    // Copies the names into the variables passed in so they can be accessed
    // in main
    strcpy(nameIn, tempIn);
    strcpy(nameOut, tempOut);

}

void printSub(FILE *ofp, int subAmt, char subName[][NAMESIZE], int subNum[], int count, struct receipt tax[]){

    // Variables
    int i, j, tally;
    double amount;

    // For loop to print each sub category
    for(i=0;i<subAmt;i++){
        printSubStart(ofp, subName[i]);
        tally = 0;
        amount = 0;
        for(j=0;j<count;j++){
            if(tax[j].num==subNum[i]){
                tally++;
                amount+=tax[j].total;
                printFile(tax[j], ofp);
            }
        }
        printSubEnd(ofp, tally, amount, subName[i]);
    }

}

void printSubStart(FILE *ofp, char name[NAMESIZE]){

    // Prints to the file the header for each sub category
    fprintf(ofp, "==================================================================\n");
    fprintf(ofp, "%s\n", name);
    fprintf(ofp, "==================================================================\n");

}

void printSubEnd(FILE *ofp, int count, double overall, char name[NAMESIZE]){

    // Prints to the file the footer for each sub category
    fprintf(ofp, "==================================================================\n");
    fprintf(ofp, "Total for %d receipts in %s Sub-Category is $%.2lf.\n", count, name, overall);
    fprintf(ofp, "==================================================================\n");
    fprintf(ofp, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

}

void getMainCat(FILE *ifp, char catName[]){

    // Retrieves the name for the main category
    char tempName[NAMESIZE];

    fscanf(ifp, "%s", &tempName);

    strcpy(catName, tempName);

}

void getSubInfo(FILE *ifp, int *amt, int num[], char name[][NAMESIZE]){

    // Scans in the amount of sub categories
    fscanf(ifp, "%d", &*amt);

    // For loop to get each subcategory number and it's associated name
    int i;
    for(i=0;i<*amt;i++){
        fscanf(ifp, "%d %s", &num[i], &name[i]);
    }

}


void sortInfo(struct receipt tax[], struct temporary temp[], int count){

    // Bubble sort that organizes by month, then day, then name, then total,
    // if necessary
    int i, j;
    for(i=0;i<count;i++){
        for(j=0;j<count-1;j++){
            if(tax[j].month>tax[j+1].month){
                Swap(&tax[j], &tax[j+1], &temp[j]);
            }
            else if(tax[j].month==tax[j+1].month){
                if(tax[j].day>tax[j+1].day){
                    Swap(&tax[j], &tax[j+1], &temp[j]);
                }
                else if(tax[j].day==tax[j+1].day){
                    if(strcmp(tax[j].name, tax[j+1].name)>0){
                        Swap(&tax[j], &tax[j+1], &temp[j]);
                    }
                    else if(strcmp(tax[j].name, tax[j+1].name)==0){
                        if(tax[j].total>tax[j+1].total){
                            Swap(&tax[j], &tax[j+1], &temp[j]);
                        }
                    }
                }
            }
        }
    }

}

void printStart(int year, FILE *ofp, char catName[]){

    // Prints the main category at the top of the page
    fprintf(ofp, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(ofp, "\nFor %s in %d\n\n", catName, year);
    fprintf(ofp, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

}

void printFile(struct receipt tax, FILE *ofp){

    // Prints each receipt to the file
    fprintf(ofp, "Name: %s\nDate: %d/%d\n                -Receipt Total: %.2lf\n", tax.name, tax.month, tax.day, tax.total);

}

void printEnd(int i, int year, double overall, FILE *ofp){

    // Prints the overall end of the main category
    fprintf(ofp, "\nOverall for %d receipts in %d, total is: $%.2lf.\n\n", i, year, overall);
    fprintf(ofp, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    printf("\nOverall for %d receipts in %d, total is: $%.2lf.\n", i, year, overall);

}

void getYear(FILE *ifp, int *year){

    // Retrieves the year for the tax receipts from the file
    fscanf(ifp, "%d", &*year);

}

void Swap(struct receipt *tax, struct receipt *inst, struct temporary *temp){

    // If statements that sort if necessary. This function is called within 
    // the bubble sort.
    if(tax->month!=inst->month){
        temp->month = inst->month;
        inst->month = tax->month;
        tax->month = temp->month;
    }
    if(tax->day!=inst->day){
        temp->day = inst->day;
        inst->day = tax->day;
        tax->day = temp->day;
    }
    if(strcmp(tax->name, inst->name)!=0){
        strcpy(temp->name, inst->name);
        strcpy(inst->name, tax->name);
        strcpy(tax->name, temp->name);
    }
    if(tax->total!=inst->total){
        temp->total = inst->total;
        inst->total = tax->total;
        tax->total = temp->total;
    }
    if(tax->num!=inst->num){
        temp->num = inst->num;
        inst->num = tax->num;
        tax->num = temp->num;
    }

}

void storeInfo(struct receipt tax_read[], FILE *ifp, int *count, double *overall){

    // Reads in the information from the input file and stores it so it's 
    // accessible from main.
    while(1){
        fscanf(ifp, "%s%d%d%lf%d", &tax_read[*count].name, &tax_read[*count].month, &tax_read[*count].day, &tax_read[*count].total, &tax_read[*count].num);
        if(tax_read[*count].num==-1){
            break;
        }
        *overall+=tax_read[*count].total;
        *count+=1;
    }

}
