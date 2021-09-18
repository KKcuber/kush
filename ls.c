#include "headers.h"

char *checkpermissions(char *filepath)
{
    struct stat filedetails;
    if (stat(filepath, &filedetails) == -1)
    {
        perror("error in ls");
        return NULL;
    }

    char *permissions;
    permissions = (char *)malloc(11 * sizeof(char));
    mode_t mode = filedetails.st_mode;
    if (S_ISDIR(mode))
        permissions[0] = 'd';
    else if (S_ISLNK(mode))
        permissions[0] = 'l';
    else
        permissions[0] = '-';
    permissions[1] = (mode & S_IRUSR) ? 'r' : '-';
    permissions[2] = (mode & S_IWUSR) ? 'w' : '-';
    permissions[3] = (mode & S_IXUSR) ? 'x' : '-';
    permissions[4] = (mode & S_IRGRP) ? 'r' : '-';
    permissions[5] = (mode & S_IWGRP) ? 'w' : '-';
    permissions[6] = (mode & S_IXGRP) ? 'x' : '-';
    permissions[7] = (mode & S_IROTH) ? 'r' : '-';
    permissions[8] = (mode & S_IWOTH) ? 'w' : '-';
    permissions[9] = (mode & S_IXOTH) ? 'x' : '-';
    permissions[10] = '\0';

    // totalBlock = statbuf.st_blocks/2;
    return permissions;
}

int isDirectory(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
   {
       return 0;
   }
   return S_ISDIR(statbuf.st_mode);
}

//A Filter function for scandir which checks whether a file is dotfile or not
int dotFilter(const struct dirent *file)
{
    if (file->d_name[0] == '.')
        return 0;
    return 1;
}

void ls(int numTokens)
{
    //flags for l and a
    int lflag = 0, aflag = 0;
    char *dirlist[10000];
    int index = 0;
    for (int i = 1; i < numTokens; i++)
    {
        if (token[i][0] == '-')
        {
            for (int j = 1; j < strlen(token[i]); j++)
            {
                if (token[i][j] == 'l')
                    lflag = 1;
                else if (token[i][j] == 'a')
                    aflag = 1;
            }
        }
        else
        {
            if (strcmp(token[i], "~") == 0)
            {
                dirlist[index] = tempHome;
                index++;
            }
            dirlist[index] = token[i];
            index++;
        }
    }
    if(index == 0)
    {
        dirlist[0] = cwd;
        index++;
    }
  
    for(int dirIndex = 0; dirIndex<index; dirIndex++)
    {
        if(index != 1)
            printf("%s:\n", dirlist[dirIndex]);

        if (!isDirectory(dirlist[dirIndex]))
        {
            const char ch = '/';
            char * fileName = strrchr(dirlist[dirIndex], ch);
            if(!lflag)
            {
                if(fileName != NULL)
                    printf("%s\n", fileName+1);
                else
                    printf("%s\n", dirlist[dirIndex]);
            }
            else
            {
                char *permissions = checkpermissions(dirlist[dirIndex]);
                struct stat filedetails;
                int stat_status = stat(dirlist[dirIndex], &filedetails);
                if (stat_status == -1)
                {
                    perror("error in ls");
                }
                int hardlink_number = filedetails.st_nlink;

                //getting the owner and group names
                char *owner = getpwuid(filedetails.st_uid)->pw_name;
                char *group = getgrgid(filedetails.st_gid)->gr_name;

                int size = filedetails.st_size;
                char *time = (char *)malloc(100 * sizeof(char));
                strftime(time, 100, "%b %d %H:%M", localtime(&(filedetails.st_ctime)));
                const char ch = '/';
                char * fileName = strrchr(dirlist[dirIndex], ch);
                printf("%s\t%d\t%s\t%s\t%d\t%s\t%s\n", permissions, hardlink_number, owner, group, size, time, (fileName != NULL) ? fileName+1 : dirlist[dirIndex]);
                free(time);
                free(permissions);
            }
            continue;
        }

        struct dirent **listOfFiles;
        int numberOfFiles = 0;

        if (aflag)
        {
            numberOfFiles = scandir(dirlist[dirIndex], &listOfFiles, NULL, alphasort);
        }
        else
        {
            numberOfFiles = scandir(dirlist[dirIndex], &listOfFiles, dotFilter, alphasort);
        }
        if (numberOfFiles == -1)
        {
            perror("error in ls");
            return;
        }

        if (lflag)
        {
            int totalBlocks = 0;
            for (int i = 0; i < numberOfFiles; i++)
            {
                char *filepath = (char *)malloc(4096 * sizeof(char));
                strcpy(filepath, dirlist[dirIndex]);
                strcat(filepath, "/");
                strcat(filepath, listOfFiles[i]->d_name);
                struct stat filedetails;
                stat(filepath, &filedetails);
                totalBlocks += (filedetails.st_blocks/2);
            }
            printf("total %d\n", totalBlocks);
            for (int i = 0; i < numberOfFiles; i++)
            {
                char *filepath = (char *)malloc(4096 * sizeof(char));
                strcpy(filepath, dirlist[dirIndex]);
                strcat(filepath, "/");
                strcat(filepath, listOfFiles[i]->d_name);

                //getting the file permissions
                char *permissions = checkpermissions(filepath);
                struct stat filedetails;

                int stat_status = stat(filepath, &filedetails);
                if (stat_status == -1)
                {
                    perror("error in ls");
                }
                int hardlink_number = filedetails.st_nlink;

                //getting the owner and group names
                char *owner = getpwuid(filedetails.st_uid)->pw_name;
                char *group = getgrgid(filedetails.st_gid)->gr_name;

                int size = filedetails.st_size;
                char *time = (char *)malloc(100 * sizeof(char));
                strftime(time, 100, "%b %d %H:%M", localtime(&(filedetails.st_ctime)));

                printf("%s\t%d\t%s\t%s\t%d\t%s\t%s\n", permissions, hardlink_number, owner, group, size, time, listOfFiles[i]->d_name);
                free(filepath);
                free(time);
                free(permissions);
            }
        }
        else
        {
            for (int i = 0; i < numberOfFiles; i++)
            {
                printf("%s ", listOfFiles[i]->d_name);
            }
            printf("\n");
        }
        printf("\n");
    }
}