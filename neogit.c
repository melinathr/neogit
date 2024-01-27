#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <windows.h>

#define MAX_FILENAME_LENGTH 1000
#define MAX_COMMIT_MESSAGE_LENGTH 2000
#define MAX_LINE_LENGTH 1000
#define MAX_MESSAGE_LENGTH 1000

#define print fprintf(stdout, "i = %d , argc = %d\n", i, argc)


int neogit_exist();
int file_exists(const char *filename);
int fnmatch(const char *pattern, const char *string);

int run_global_config(int argc , char *argv[]);
int run_lokal_config(int argc , char *argv[]);

int run_init(int argc, char *argv[]);
int creat_configs(char *username, char *email);

int run_add(int argc, char *argv[]);
int add_to_staging(char *filepath , char mode);
void add_directory_to_staging(const char *dir_path);
int check_staging_area(char* filepath);
void run_add_n();

int run_reset(int argc, char * const argv[]);
int remove_from_staging(char *filepath, char mode);
int run_reset_undo();


int neogit_exist()
{
    char cwd[1024];
    if(getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Error getting current directory");
        return 1;    
    }

    struct dirent *entry;
    char tmp_cwd[1024];
    bool exists = false;

    do{
        DIR *dir = opendir(".");
        if(dir == NULL){
            perror("Error opening current directory");
            return 1;
        }
        while((entry = readdir(dir)) != NULL){
            if(entry->d_type == DT_DIR && strcmp(entry->d_name, ".neogit") == 0){
                exists = true;
                break;
            }    
        }
        closedir(dir);

        if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL) return 1;

        if (strcmp(tmp_cwd, "/") != 0){
            if (chdir("..") != 0) return 1;
        }
    }while (strcmp(tmp_cwd, "/") != 0);
    
    if (chdir(cwd) != 0) return 1;

    if(exists){ 
        //exists
        return 0;
    }
    else{
        //not exists
        return 1;
    }
}

int file_exists(const char *filename) {
    return access(filename, F_OK) != -1;
}

int run_global_config(int argc , char *argv[]){
    FILE *file = fopen("~/.config", "w");

    if(! strcmp(argv[3], "user.name")){
        fprintf(file, "username: %s\n", argv[4]);
    }
    else if(! strcmp(argv[3], "user.email")){
        fprintf(file, "email: %s\n", argv[4]);
    }
    else {
        perror("invalid command");
        return 1;
    }

    fclose(file);

    if(neogit_exist == 0)
    {
        FILE *file = fopen(".neogit/config", "r+");
        if (file == NULL) {
            perror("Error opening file");
            return 1;
        }

        if(! strcmp(argv[3], "user.name")){
            fprintf(file, "username: %s\n", argv[4]);
        }
        else if(! strcmp(argv[3], "email.name")){
            fprintf(file, "email: %s\n", argv[4]);
        } 
        fclose(file);
    }

    printf("succesfully config(global)");
    return 0;
}

int run_lokal_config(int argc , char *argv[]){
    if(neogit_exist == 0)
    {
        FILE *file = fopen(".neogit/config", "w");
        if (file == NULL) {
            perror("Error opening file");
            return 1;
        }

        if(! strcmp(argv[3], "user.name")){
            fprintf(file, "username: %s\n", argv[4]);
        }
        else if(! strcmp(argv[3], "email.name")){
            fprintf(file, "email: %s\n", argv[4]);
        }
        fclose(file); 
    }
    else{
        perror("neogit repository has not initialized yet");
        return 1;
    }
    fprintf(stdout, "succesfully config(local)");
    return 0;
}

int creat_configs(char *username, char *email)
{
    FILE *file = fopen(".neogit/config", "w");
    if(file == NULL) {
        perror("Error getting config directory");
        return 1;
    }

    fprintf(file, "username: %s\n", username);
    fprintf(file, "email: %s\n", email);
    fprintf(file, "last_commit_ID: %d\n", 0);
    fprintf(file, "current_commit_ID: %d\n", 0);
    fprintf(file, "branch: %s", "master");

    fclose(file);

    if (mkdir(".neogit/commits") != 0) return 1;
    if (mkdir(".neogit/files") != 0) return 1;

    file = fopen(".neogit/staging", "w");
    fclose(file);

    file = fopen(".neogit/tracks", "w");
    fclose(file);

    file = fopen(".neogit/reset", "w");
    fclose(file);

    return 0;
}

int run_init(int argc, char *argv[])
{
    char cwd[1024];
    if(getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Error getting current directory");
        return 1;    
    }

    struct dirent *entry;
    char tmp_cwd[1024];
    bool exists = false;
    do{
        // find .neogit
        DIR *dir = opendir(".");
        if(dir == NULL){
            perror("Error opening current directory");
            return 1;
        }
        while((entry = readdir(dir)) != NULL){
            if(entry->d_type == DT_DIR && strcmp(entry->d_name, ".neogit") == 0){
                exists = true;
                break;
            }    
        }
        closedir(dir);

        if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL) return 1;

        if (strcmp(tmp_cwd, "/") != 0){
            if (chdir("..") != 0) return 1;
        }
    }while (strcmp(tmp_cwd, "D:\\") != 0);
    
    
    if (chdir(cwd) != 0) return 1;

    if(exists){ 
        fprintf(stdout, "neogit repository has already initialized");
        return 1;
    }
    else{
        if (mkdir(".neogit") != 0){
            perror("Error creating .neogit directory");
            return 1;
        }
    }

    // FILE *file = fopen(".neogit/config", "r");
    // if (file == NULL) {
    //     perror("you have not config yet");
    //     return 1;
    // }
    return creat_configs("mel", "example@gmail.com");
    fprintf(stdout,"neogit repository initialized\n");
    return 0;
}

int run_add(int argc, char *argv[])
{
    if (argc < 3){
        perror("please specify a file");
        return 1;
    }
    if(neogit_exist == 0)
    {
        perror("neogit repository has not initialized yet");
        return 1;
    }

    if(strcmp(argv[2], "-f") == 0){
        for(int i = 3; i < argc; i++)
        {
            if(strchr(argv[i], '*') != NULL)
                add_to_staging(argv[i], 'w');
            else
                add_to_staging(argv[i], 'n');
        }
    }
    else if(strcmp(argv[2], "-n") == 0){
        run_add_n();
        return 0;
    }
    else if(strchr(argv[2], '*') != NULL){
        //w : wildcard
        return add_to_staging(argv[2], 'w');
    }
    else{
        // n : normal
        return add_to_staging(argv[2], 'n');
    }
}

int add_to_staging(char *filepath , char mode)
{
    FILE *stagingfile = fopen(".neogit/staging", "a+");
    if(stagingfile == NULL) {
        perror("Error getting staging directory");
        return 1;
    }

    //check if the file is already in the staging area
    char line[MAX_LINE_LENGTH];
    while(fgets(line, sizeof(line), stagingfile) != NULL)
    {
        int length = strlen(line);

        // remove '\n'
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }

        if(mode == 'n')
            if (strcmp(filepath, line) == 0){
                fclose(stagingfile);
                return 0;
            } 
        else if(mode == 'w')
            if (fnmatch(filepath, line) == 0){
                fclose(stagingfile);
                return 0;
            } 
    }
    fclose(stagingfile);

    stagingfile = fopen(".neogit/staging", "a");
    if (stagingfile == NULL) {
        perror("Error opening staging file");
        return 1;
    }

    //check if the file exits
    if(mode == 'n')
        if(!file_exists(filepath)){
            perror("file does not exist");
            fclose(stagingfile);
            return 1;
        }
        else{
            struct stat path_stat;
            stat(filepath, &path_stat);

            if (S_ISDIR(path_stat.st_mode)) {
                add_directory_to_staging(filepath);
            } 
            else {
                fprintf(stagingfile, "%s\n", filepath);
            }
        }
            
    else{
        DIR *dir = opendir(".");
        if (dir == NULL) {
            perror("Error opening current directory");
            fclose(stagingfile);
            return 1;
        }
        struct dirent *entry;
        bool found = false;
        while ((entry = readdir(dir)) != NULL) {
            if (fnmatch(filepath, entry->d_name) == 0) {
                fprintf(stagingfile, "%s\n", entry->d_name);
                found = true;
            }
        }
        if(! found){
            perror("file does not exist");
            fclose(stagingfile);
            closedir(dir);
            return 1;
        }
        closedir(dir);
    }
    fclose(stagingfile);
    fprintf(stdout, "adds to staging area\n");
    return 0;
}

void add_directory_to_staging(const char *dir_path) {
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("Error opening directory");
        return ;
    }
    FILE *stagingfile = fopen(".neogit/staging", "a+");
    if(stagingfile == NULL) {
        perror("Error getting staging directory");
        closedir(dir);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char entry_path[MAX_FILENAME_LENGTH];
        snprintf(entry_path, MAX_FILENAME_LENGTH, "%s/%s", dir_path, entry->d_name);

        struct stat entry_stat;
        stat(entry_path, &entry_stat);

        if (S_ISDIR(entry_stat.st_mode)) {
            add_directory_to_staging(entry_path);
        } else {
            // add files to staging
            add_to_staging(entry_path, 'n');
        }
    }
    fprintf(stagingfile, "%s\n", dir_path);
    fclose(stagingfile);
    closedir(dir);
}

void run_add_n()
{
    struct dirent *entry;
    DIR *dir = opendir(".");
    if(dir == NULL){
        perror("Error opening current directory");
        return;
    }
    while((entry = readdir(dir)) != NULL){
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        } 
        char file_path[MAX_FILENAME_LENGTH];
        snprintf(file_path, MAX_FILENAME_LENGTH, "%s", entry->d_name);
            
        if(check_staging_area(file_path) == 0){
            fprintf(stdout, "%s\t\tstaging\n", file_path);
        }
        else{
            fprintf(stdout, "%s\t\tnot staging\n", file_path);
        }
    }
    closedir(dir);
}

int check_staging_area(char* filepath)
{
    FILE *stagingfile = fopen(".neogit/staging", "a+");
    if(stagingfile == NULL) {
        perror("Error getting staging directory");
        return 1;
    }
    //check if the file is already in the staging area
    char line[MAX_LINE_LENGTH];
    while(fgets(line, sizeof(line), stagingfile) != NULL)
    {
        int length = strlen(line);

        // remove '\n'
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }

        if (strcmp(filepath, line) == 0) 
            return 0;
    }
    fclose(stagingfile);
    return 1;
}

int fnmatch(const char *pattern, const char *string) {
    while (*pattern != '\0' && *string != '\0') {
        if (*pattern == '*') {
            // Handle '*' as a wildcard
            while (*pattern == '*') {
                pattern++;
            }
            if (*pattern == '\0') {
                return 0;
            }
            while (*string != '\0' && *string != *pattern) {
                string++;
            }
        } else if (*pattern == '*' || *pattern == *string) {
            pattern++;
            string++;
        } else {
            // characters do not match
            return 1;
        }
    }
    return (*pattern == '\0' && *string == '\0') ? 0 : 1;
}

int run_reset(int argc, char *const argv[]) {
    if (argc < 3){
        perror("please specify a file");
        return 1;
    }
    if(neogit_exist == 0)
    {
        perror("neogit repository has not initialized yet");
        return 1;
    }

    if(strcmp(argv[2], "undo") == 0){
        run_reset_undo();
    }
    else if(strcmp(argv[2], "-f") == 0) {
        for(int i = 3; i < argc; i++)
        {
            if(strchr(argv[i], '*') != NULL){
                remove_from_staging(argv[i], 'w');
            }
            else{
                remove_from_staging(argv[i], 'n');
            }   
        }
    }
    else if(strchr(argv[2], '*') != NULL){
        //w : wildcard
        return remove_from_staging(argv[2], 'w');
    }
    else{
        // n : normal
        return remove_from_staging(argv[2], 'n');
    }
}

int remove_from_staging(char *filepath, char mode)
{
    FILE *stagingfile = fopen(".neogit/staging", "r");
    if (stagingfile == NULL) {
        perror("error opening staging file");
        fclose(stagingfile);
        return 1;
    }
    
    if(mode == 'n')
    {
        if(!file_exists(filepath)){
            perror("file does not exist");
            return 1;
        }
    }

    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("Error opening current directory");
        fclose(stagingfile);
        return 1;
    }
    
    struct dirent *entry;
    bool found = false;
        while ((entry = readdir(dir)) != NULL) {
            if(mode == 'w'){
                if (fnmatch(filepath, entry->d_name) == 0)
                    found = true;
            }
            else if(mode == 'n'){
                if (strcmp(filepath, entry->d_name) == 0)
                    found = true;
            }
        }
        if(! found){
            perror("file does not exist in staging area");
            fclose(stagingfile);
            closedir(dir);
            return 1;
        }

    FILE *tmp_file = fopen(".neogit/tmp_staging", "w");
    if (tmp_file == NULL) return 1;

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), stagingfile) != NULL) {
        int length = strlen(line);

        // remove '\n'
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }

        if(mode == 'n'){
            if (strcmp(filepath, line) != 0) fputs(line, tmp_file);
            fputs("\n", tmp_file);
        }
            
        if(mode == 'w'){
            if(fnmatch(filepath, line) != 0) fputs(line, tmp_file);
            fputs("\n", tmp_file);
        }
    }
    fclose(stagingfile);
    fclose(tmp_file);

    remove(".neogit/staging");
    rename(".neogit/tmp_staging", ".neogit/staging");

    FILE *resetfile = fopen(".neogit/reset", "a");
    if (resetfile == NULL) {
        perror("error opening reset file");
        fclose(resetfile);
        return 1;
    }
    fprintf(resetfile, "%s\n", filepath);

    fprintf(stdout, "reset done\n");
    return 0;
}

int run_reset_undo()
{
    FILE *stagingfile = fopen(".neogit/staging", "r+");
    if (stagingfile == NULL) {
        perror("error opening reset file");
        fclose(stagingfile);
        return 1;
    }

    int lineCount = 0;
    char ch;
    while ((ch = fgetc(stagingfile)) != EOF) {
        if (ch == '\n') {
            lineCount++;
        }
    }

    if (lineCount == 0) {
        printf("staging file is empty\n");
        return 1;
    }

    fseek(stagingfile, 0, SEEK_SET);
    FILE *tmp_file = fopen(".neogit/tmp_staging", "w");
    if (tmp_file == NULL) return 1;

    for (int i = 0; i < lineCount - 1; i++) {
        while (ch = fgetc(stagingfile) != '\n'){
            fprintf(tmp_file, "%c", ch);
        }
        fputs("\n", tmp_file);
    }
    

    FILE *resetfile = fopen(".neogit/reset", "a");
    if (resetfile == NULL) {
        perror("error opening reset file");
        fclose(resetfile);
        return 1;
    }

    while (ch = fgetc(stagingfile) != '\0'){
        fprintf(resetfile, "%c", ch);
    }
    fputs("\n", resetfile);
    
    remove(".neogit/staging");
    rename(".neogit/tmp_staging", ".neogit/staging");
    fprintf(stdout, "reset -undo done\n");
    return 0;
}


void print_command(int argc, char * const argv[]) {
    for (int i = 0; i < argc; i++) {
        fprintf(stdout, "arg[%d] = %s\n", i , argv[i]);
    }
    fprintf(stdout, "\n");
}

int main(int argc , char *argv[])
{
    print_command(argc, argv);
    //just input neogit
    if (argc < 2) {
        fprintf(stdout, "please enter a valid command");
        return 1;
    }

    
    if(! strcmp(argv[1], "config")){
         if(! strcmp(argv[2], "global"))
            return run_global_config(argc , argv);
        else
            return run_lokal_config(argc , argv);
    }
    else if(! strcmp(argv[1], "init")){
        return run_init(argc, argv);
    }
    else if (! strcmp(argv[1], "add")){
        return run_add(argc, argv);
    }
    else if (! strcmp(argv[1], "reset")){
        return run_reset(argc, argv);
    }
}