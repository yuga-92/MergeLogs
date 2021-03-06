//
//  main.c
//  MergeLogs
//
//  Created by Yuga Hal on 2/1/16.
//  Copyright © 2016 Yura Halych. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#define MESSAGE_LENGTH 10000

struct list {
    unsigned long id;
    char data[MESSAGE_LENGTH];
};
int openFileAndAddToList(struct list items[], long numberOfLines);
long numberOfEntries();
void writeToFile(struct list items[],long size);
void sorting(struct list items[], long l,long u);
long partArr(struct list items[],long l,long u);

int main(int argc, const char * argv[])
{
    long listSize;
    listSize = numberOfEntries(); //calculates size of list array
    /*in future we can implement some functions that will add new elements into list array
     like in ArrayList in Java, so then we do not need to calculate the size of the array :)*/
    struct list items[listSize];
    openFileAndAddToList(items, listSize);
    printf("Merging...\n");
   // sorting(items, listSize);
    sorting(items,0 , listSize-1);
    writeToFile(items,listSize);
    printf("Done!\n");
    printf("___________________________________________________________\n");
    printf("All data written in \"all.log\" file at program root folder.\n");
    printf("___________________________________________________________\n");
    return 0;
}

long numberOfEntries()
{
    DIR* FD;
    struct dirent* oneFile;
    FILE* sourceFile;
    char  buffer[BUFSIZ];
    long numberOfLines;
    FD = opendir("./callLog");
    if (FD != NULL){
        while ((oneFile = readdir(FD))){
            if (!strcmp (oneFile->d_name, "."))
                continue;
            if (!strcmp (oneFile->d_name, ".."))
                continue;
            //next line i used for mac os in linux it is not necessary
            if (!strcmp (oneFile->d_name, ".DS_Store"))
                continue;
            char fullFileName [255];
            strcpy(fullFileName,"./callLog/");
            strcat(fullFileName, oneFile->d_name);
            sourceFile = fopen(fullFileName, "rw");
            if (sourceFile == NULL)
            {
                fprintf(stderr, "Error : Can`t open source file - %s\n", strerror(errno));
                return 1;
            }
            while (fgets(buffer, BUFSIZ, sourceFile) != NULL)
            {
                numberOfLines++;
            }
            
        }
        
    }
    fclose(sourceFile);
    return numberOfLines;
}

int openFileAndAddToList(struct list items[], long numberOfLines)
{
    DIR* FD;
    struct dirent* oneFile;
    FILE    *sourceFile;
    char    buffer[BUFSIZ];
    int k = 0;
    FD = opendir("./callLog");
    if (FD != NULL){
        while ((oneFile = readdir(FD))){
            if (!strcmp (oneFile->d_name, "."))
                continue;
            if (!strcmp (oneFile->d_name, ".."))
                continue;
            if (!strcmp (oneFile->d_name, ".DS_Store"))
                continue;
            char fullFileName [255];
            strcpy(fullFileName,"./callLog/");
            strcat(fullFileName, oneFile->d_name);
            printf("Opening file: %s \n",fullFileName);
            sourceFile = fopen(fullFileName, "rw");
            if (sourceFile == NULL)
            {
                fprintf(stderr, "Error : Can`t open source file - %s\n", strerror(errno));
                return 1;
            }
            while (fgets(buffer, BUFSIZ, sourceFile) != NULL)
            {
                sscanf(buffer, "%lu",&items[k].id);
                size_t numbers_end = strspn(buffer, "1234567890. \t");
                strcpy(items[k].data, buffer+numbers_end);
                k++;
            }
            fclose(sourceFile);
        }
    }
    return 0;
}


void writeToFile(struct list items[],long size)
{
    FILE    *sourceFile;
    sourceFile = fopen("all.log", "w");
    if (sourceFile == NULL)
    {
        fprintf(stderr, "Error : Failed to open destination file - %s\n", strerror(errno));
        
    }
    for (int i=0; i<size; i++)
    {
        fprintf(sourceFile,"%lu %s", items[i].id, items[i].data);
    }
    fclose(sourceFile);
}

void sorting(struct list items[],long l,long u)
{
    //i`m not perfectly familiar with implementing of quick sorting algorithm, but i know that it performs quick
    //so i decided to do some google search about how to implement it correctly
    long j;
    if(l<u)
    {
        j=partArr(items,l,u);
        sorting(items,l,j-1);
        sorting(items,j+1,u);
    }
}

long partArr(struct list items[],long l,long u)
{
    long i,j;
    struct list temp, temp1, temp2;
    temp1.id=items[l].id;
    strcpy(temp1.data, items[l].data);
    i=l;
    j=u+1;
    
    do
    {
        do
            i++;
        while(items[i].id<temp1.id&&i<=u);
        do
            j--;
        while(temp1.id<items[j].id);
        if(i<j)
        {
            temp.id=items[i].id;
            strcpy(temp.data, items[i].data);
            
            items[i].id=items[j].id;
            strcpy(items[i].data, items[j].data);
            
            items[j].id=temp.id;
            strcpy(items[j].data, temp.data);
        }
    }while(i<j);
    items[l].id=items[j].id;
    strcpy( temp2.data, items[j].data);
    strcpy( items[l].data, temp2.data);
    items[j].id=temp1.id;
    strcpy( items[j].data, temp1.data);
    return(j);
}


