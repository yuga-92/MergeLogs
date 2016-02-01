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

struct list {
    unsigned long id;
    char data[10000];
};
int openFileAndAddToList(struct list items[], long numberOfLines);
long numberOfEntries();
void sorting(struct list items[], long size);
void writeToFile(struct list items[],long size);

int main(int argc, const char * argv[])
{
    long listSize;
    listSize = numberOfEntries();
    struct list items[listSize];
    openFileAndAddToList(items, listSize);
    printf("Merging...\n");
    sorting(items, listSize);
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

void sorting(struct list items[], long size) {
    long i, j;
    struct list temp;
    
    for (i = 1; i < size; i++) {
        for (j = 0; j < size - 1; j++) {
            if (items[j].id > items[j + 1].id) {
                temp.id = items[j].id;
                strcpy(temp.data, items[j].data);
                items[j].id = items[j + 1].id;
                strcpy(items[j].data, items[j + 1].data);
                items[j + 1].id = temp.id;
                strcpy(items[j + 1].data, temp.data);
            }
        }
    }
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

