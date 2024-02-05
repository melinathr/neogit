#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <windows.h>
#include <time.h>

#define MAX_NAME_LENGTH 1000
#define MAX_FILENAME_LENGTH 1000
#define MAX_LINE_LENGTH 1000
#define MAX_MESSAGE_LENGTH 1000

#define print fprintf(stdout, "HERE\n")

int neogit_exist();
int file_exists(const char *filename);
int fnmatch(const char *pattern, const char *string);

int run_global_config(int argc , char *argv[]);
int run_lokal_config(int argc , char *argv[]);
void run_alias(char *name, char *commnad, char mode);
bool find_in_alias(char *name, char *command);

int run_init(int argc, char *argv[]);
int creat_configs(char *username, char *email);

int run_add(int argc, char *argv[]);
int add_to_staging(char *filepath , char mode);
void add_directory_to_staging(const char *dir_path);
int check_staging_area(char* filepath);
void run_add_n();
int redo();
bool is_staged(char *filepath);

int run_reset(int argc, char * const argv[]);
int remove_from_staging(char *filepath, char mode);
int run_reset_undo();

int run_commit(char message[]);
int inc_last_commit_ID();
int exists_in_files(char *filepath);
int commit_staged_file(int commit_ID, char* filepath);
int track_file(char *filepath);
bool is_tracked(char *filepath);
int create_commit_file(int commit_ID, char *message);
int find_file_last_commit(char* filepath);

int run_set(int argc,char * const argv[]);
bool shortcut_exist(char name[], char message[]);
int run_commit_s(int argc, char * const argv[]);
int run_replace(int argc, char *argv[]);
int run_remove(int argc, char *argv[]);

int run_branch(int argc, char * const argv[]);
bool branch_exist(char name[]);
void list_branch();

int run_checkout(int argc, char *const argv[]);
int find_file_last_change_before_commit(char *filepath, int commit_ID);
int checkout_file(char *filepath, int commit_ID);
bool is_digit(char *string);
int find_commitID(char *filepath, char* branch);

void run_status(char* name);
void check_status(char* filepath);
bool is_change(char* filepath);

void print_log(char* filepath);
int run_log(int argc, char *argv[]);

int run_tag(int argc, char * const argv[]);
bool tag_exist(char name[]);
int create_tag_file(char *name, char *message, int commit_ID);

int run_grep(int argc, char * const argv[]);
void find_word(char* filepath, char* word, char mode);

int run_diff(char *file1, char *file2, int line1_start, int line1_end, int line2_start, int line2_end);
void remove_spaces(char *str);
void compare_commits(int commitID1, int commitID2);

int run_merge(char* branch1, char* branch2);
int find_HEAD(char* branch);

int run_revert(int argc, char * const argv[]);
char* find_message(int commitID);
int replace(char* mainfile, char* filepath);


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
    }while (strcmp(tmp_cwd, "D:\\") != 0);
    
    if (chdir(cwd) != 0) return 1;

    if(exists){ 
        //not exists
        return 0;
    }
    else{
        //exists
        return 1;
    }
}

int file_exists(const char *filename) {
    return access(filename, F_OK) != -1;
}

int run_global_config(int argc , char *argv[]){
    FILE *file = fopen("D:\\uni\\mabani\\neogit\\globalconfig", "a+");

    char line1[MAX_LINE_LENGTH];
    fgets(line1, sizeof(line1), file);
    char line2[MAX_LINE_LENGTH];
    fgets(line2, sizeof(line2), file);

    if(! strcmp(argv[3], "user.name")){
        if(strstr(line1, "username:")){
            fclose(file);
            file = fopen("D:\\uni\\mabani\\neogit\\globalconfig", "w");
            fprintf(file,"%s", line2);
        }
        if(strstr(line2, "username:")){
            fclose(file);
            file = fopen("D:\\uni\\mabani\\neogit\\globalconfig", "w");
            fprintf(file,"%s", line1);
        }
        fprintf(file, "username: %s\n", argv[4]);
    }
    else if(! strcmp(argv[3], "user.email")){
        if(strstr(line1, "email:")){
            fclose(file);
            file = fopen("D:\\uni\\mabani\\neogit\\globalconfig", "w");
            fprintf(file,"%s", line2);
        }
        if(strstr(line2, "email:")){
            fclose(file);
            file = fopen("D:\\uni\\mabani\\neogit\\globalconfig", "w");
            fprintf(file,"%s", line1);
        }
        fprintf(file, "email: %s\n", argv[4]);
    }
    else {
        perror("invalid command");
        return 1;
    }

    fclose(file);

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
    if (mkdir(".neogit/tags") != 0) return 1;

    file = fopen(".neogit/staging", "w");
    fclose(file);

    file = fopen(".neogit/tracks", "w");
    fclose(file);

    file = fopen(".neogit/reset", "w");
    fclose(file);

    file = fopen(".neogit/shortcut", "w");
    fclose(file);

    file = fopen(".neogit/branch", "w");
    fprintf(file, "master\n");
    fclose(file);

    file = fopen(".neogit/tag", "w");
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

    FILE *configfile = fopen("D:\\uni\\mabani\\neogit\\globalconfig", "r+");

    char username[1000];
    char email[1000];

    fscanf(configfile, "username: %s\nemail: %s", username, email);
    creat_configs(username, email);
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
    else if(strcmp(argv[2], "-redo") == 0){
        if(argc > 3){
            perror("invalid command!");
            return 1;
        }
        return redo();
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
    fprintf(stdout, "%s adds to staging area\n", filepath);
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

int redo()
{
    FILE *resetfile = fopen(".neogit/reset", "r");
    FILE *stagingfile = fopen(".neogit/staging", "a");
    if (resetfile == NULL || stagingfile == NULL) {
        perror("Error opening file");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), resetfile) != NULL){
        int length = strlen(line);

        // remove '\n'
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }

        if(!is_staged(line)){
            fprintf(stagingfile, "%s\n", line);
        }
    }
    fclose(resetfile);
    fclose(stagingfile);

    resetfile = fopen(".neogit/reset", "w");
    fclose(resetfile);

    fprintf(stdout, "redo done!");
    return 0;
}

bool is_staged(char *filepath) {
    FILE *file = fopen(".neogit/staging", "r");
    if (file == NULL) {
        perror("Error opening file");
        return false;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        int length = strlen(line);

        // Remove '\n'
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }

        if (strcmp(line, filepath) == 0) {
            fclose(file);
            return true;
        }
    }

    fclose(file);
    return false;
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

int wildcard_match(const char *str, const char *pattern) {
    while (*str) {
        if (*pattern == '*') {
            do {
                ++pattern;
            } while (*pattern == '*');

            if (*pattern != '\0' && *pattern != *str) {
                return 0;
            }

            while (*str && *str != *pattern) {
                ++str;
            }
        } else if (*str == *pattern) {
            ++str;
            ++pattern;
        } else {
            return 0;
        }
    }
    while (*pattern == '*') {
        ++pattern;
    }

    return *pattern == '\0';
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

    if(strcmp(argv[2], "-undo") == 0){
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
        perror("error opening staging file");
        fclose(stagingfile);
        return 1;
    }

    int lineCount = 0;
    char ch;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), stagingfile) != NULL){
        lineCount++;
    }

    if (lineCount == 0) {
        printf("staging file is empty\n");
        return 1;
    }

    fseek(stagingfile, 0, SEEK_SET);
    FILE *tmp_file = fopen(".neogit/tmp_staging", "w");
    if (tmp_file == NULL) return 1;

    for (int i = 0; i < lineCount - 1; i++) {
        fgets(line, sizeof(line), stagingfile);
        fprintf(tmp_file, "%s", line);
    }
    
    FILE *resetfile = fopen(".neogit/reset", "a");
    if (resetfile == NULL) {
        perror("error opening reset file");
        fclose(resetfile);
        return 1;
    }

    fgets(line, sizeof(line), stagingfile);
    fprintf(resetfile, "%s", line);
    
    remove(".neogit/staging");
    rename(".neogit/tmp_staging", ".neogit/staging");
    fprintf(stdout, "reset -undo done\n");
    return 0;
}

int run_commit(char message[]) {
    if(neogit_exist == 0)
    {
        perror("neogit repository has not initialized yet");
        return 1;
    }

    FILE *stagingfile = fopen(".neogit/staging", "r");
    if (stagingfile == NULL){
        perror("staging file is empty");
    }

    if(strlen(message) > 72){
        perror("message is too long");
        return 1;
    }

    int commit_ID = inc_last_commit_ID();
    if (commit_ID == -1){
        perror("error getting new commit id");
        return 1; 
    }
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), stagingfile) != NULL){
        int length = strlen(line);

        // remove '\n'
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }

        //add to files directory
        if (exists_in_files(line) != 0) {
            char dir_path[MAX_FILENAME_LENGTH];
            strcpy(dir_path, ".neogit/files/");
            strcat(dir_path, line);
            if (mkdir(dir_path) != 0) 
            {
                perror("error making dir");
                return 1;
            }
        }
        fprintf(stdout ,"successfully commit %s\n", line);
        commit_staged_file(commit_ID, line);
        track_file(line);
    } 
    fclose(stagingfile);

    // free staging
    stagingfile = fopen(".neogit/staging", "w");
    if (stagingfile == NULL) {
        perror("error opening staging file");
        return 1;
    }
    fclose(stagingfile);

    time_t currentTime;
    time(&currentTime);
    struct tm *localTime = localtime(&currentTime);
    char timeString[50];
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", localTime);

    create_commit_file(commit_ID, message);
    fprintf(stdout, "commit successfully with commit ID %d\n", commit_ID);
    fprintf(stdout, "your message: %s\ntime: %s", message, timeString);
    return 0;
}

int exists_in_files(char *filepath){
    DIR *dir = opendir(".neogit/files");
    struct dirent *entry;
    if (dir == NULL) {
        perror("Error opening current directory");
        return 1;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, filepath) == 0)
            return 0;
    }
    closedir(dir);

    return 1;
}

int inc_last_commit_ID() {
    FILE *file = fopen(".neogit/config", "r");
    if (file == NULL) {
        perror("error opening staging file");
        return -1;
    }
    
    FILE *tmp_file = fopen(".neogit/tmp_config", "w");
    if (tmp_file == NULL) {
        perror("error opening temporary staging file");
        return -1;
    }

    int last_commit_ID;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, "last_commit_ID", 14) == 0) {
            sscanf(line, "last_commit_ID: %d\n", &last_commit_ID);
            last_commit_ID++;
            fprintf(tmp_file, "last_commit_ID: %d\n", last_commit_ID);
        } else fprintf(tmp_file, "%s", line);
    }

    fclose(file);
    fclose(tmp_file);
    
    remove(".neogit/config");
    rename(".neogit/tmp_config", ".neogit/config");

    return last_commit_ID;
    //returns new commit_ID
}

int commit_staged_file(int commit_ID, char* filepath)
{
    FILE *read_file, *write_file;
    char read_path[MAX_FILENAME_LENGTH];
    strcpy(read_path, filepath);
    char write_path[MAX_FILENAME_LENGTH];
    strcpy(write_path, ".neogit/files/");
    strcat(write_path, filepath);
    strcat(write_path, "/");
    char tmp[10];
    sprintf(tmp, "%d", commit_ID);
    strcat(write_path, tmp);

    read_file = fopen(read_path, "r");
    if (read_file == NULL) {
        perror("error opening read file");
        return 1;
    }

    write_file = fopen(write_path, "w");
    if (write_file == NULL) {
        perror("error opening write file");
        return 1;
    }

    char buffer;
    buffer = fgetc(read_file);
    while(buffer != EOF) {
        fputc(buffer, write_file);
        buffer = fgetc(read_file);
    }
    fclose(read_file);
    fclose(write_file);

    return 0;
}

int track_file(char *filepath)
{
    if (is_tracked(filepath))
        return 0;
    FILE *file = fopen(".neogit/tracks", "a");
    if (file == NULL) 
    {
        perror("error opening tracks file");
        return 1;
    }
    fprintf(file, "%s\n", filepath);
    fclose(file);
    return 0;
}

bool is_tracked(char *filepath) {
    FILE *fileExistenceCheck = fopen(".neogit/tracks", "r");
    if (fileExistenceCheck == NULL) {
        perror("Error checking file existence");
        return false;
    }
    fclose(fileExistenceCheck);

    FILE *file = fopen(".neogit/tracks", "r");
    if (file == NULL) {
        perror("Error opening file");
        return false;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        int length = strlen(line);

        // Remove '\n'
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }

        if (strcmp(line, filepath) == 0) {
            fclose(file);
            return true;
        }
    }

    fclose(file);
    return false;
}

int create_commit_file(int commit_ID, char *message) {
    char commit_filepath[MAX_FILENAME_LENGTH];
    strcpy(commit_filepath, ".neogit/commits/");
    char tmp[10];
    sprintf(tmp, "%d", commit_ID);
    strcat(commit_filepath, tmp);

    FILE *file = fopen(commit_filepath, "w");
    if (file == NULL) {
        perror("error opening commit filepath");
        return 1;
    }
    fprintf(file, "commitID: %d\n", commit_ID);
    fprintf(file, "message: %s\n", message);

    time_t currentTime;
    time(&currentTime);
    struct tm *localTime = localtime(&currentTime);
    char timeString[50];
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S\n", localTime);
    fprintf(file, "time: %s", timeString);  


    char name[MAX_NAME_LENGTH], email[MAX_NAME_LENGTH], branch[MAX_NAME_LENGTH];
    FILE *configfile= fopen(".neogit/config", "r+");
    if (configfile == NULL) {
        perror("error opening config file");
        fclose(configfile);
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), configfile) != NULL) {
        int length = strlen(line);
        // remove '\n'
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }
        if(strstr(line, "branch:")) 
            sscanf(line ,"branch: %[^\n]", branch);
        else if(strstr(line, "username:"))
            sscanf(line,"username: %[^\n]", name);
        else if(strstr(line, "email:"))
            sscanf(line, "email: %[^\n]", email);
    }
    fclose(configfile);
    fprintf(file, "branch: %s\n",branch);
    fprintf(file, "username: %s\n",name);
    fprintf(file, "email: %s\n",email);

    fprintf(file, "files:\n");
    DIR *dir = opendir(".");
    struct dirent *entry;
    if (dir == NULL) {
        perror("Error opening current directory");
        return 1;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && is_tracked(entry->d_name)) {
            int file_last_commit_ID = find_file_last_commit(entry->d_name);
            //fprintf(file, "%s %d\n", entry->d_name, file_last_commit_ID);
            fprintf(file, "%s\n", entry->d_name);
        }
    }
    closedir(dir); 
    fclose(file);
    return 0;
}

int find_file_last_commit(char* filepath) {
    char filepath_dir[MAX_FILENAME_LENGTH];
    strcpy(filepath_dir, ".neogit/files/");
    strcat(filepath_dir, filepath);

    int max = -1;
    
    DIR *dir = opendir(filepath_dir);
    struct dirent *entry;
    if (dir == NULL) return 1;

    while((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            int tmp = atoi(entry->d_name);
            max = max > tmp ? max: tmp;
        }
    }
    closedir(dir);

    return max;
}

int run_set(int argc,char * const argv[])
{
    if(neogit_exist == 0)
    {
        perror("neogit repository has not initialized yet");
        return 1;
    }

    char message[MAX_MESSAGE_LENGTH];
    char name[MAX_MESSAGE_LENGTH];
    strcpy(message, argv[3]);
    strcpy(name, argv[5]);

    if(shortcut_exist(name, message)){
        perror("shortcut with this name has already exist");
        return 1;
    }

    FILE *shortcutfile;
    shortcutfile = fopen(".neogit/shortcut", "a");
    if(shortcutfile == NULL){
        perror("error opening shortcut file");
        return 1;
    }
    fprintf(shortcutfile, "%s %s\n", name, message);
    fclose(shortcutfile);
    fprintf(stdout, "set done successfully");
    return 0;
}

bool shortcut_exist(char name[], char message[])
{
    FILE *shortcutFile = fopen(".neogit/shortcut", "r");
    if (shortcutFile == NULL) {
        perror("error opening shortcut file");
        fclose(shortcutFile);
        return false;
    }
    char shrct_name[MAX_NAME_LENGTH];
    char shrct_message[MAX_MESSAGE_LENGTH];

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), shortcutFile) != NULL) {
        int length = strlen(line);

        // remove '\n'
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }

        sscanf(line, "%s %[\n]",shrct_name, shrct_message);
        if(strcmp(name, shrct_name) == 0) {
            fclose(shortcutFile);
            strcpy(message, shrct_message);
            return true;
        }   
    }

    fclose(shortcutFile);
    return false;
}

int run_commit_s(int argc, char * const argv[])
{
    if(argc < 4)
    {
        perror("please use the correct format");
        return 1;
    }
    if(neogit_exist == 0)
    {
        perror("neogit repository has not initialized yet");
        return 1;
    }

    char message[MAX_NAME_LENGTH];
    if(! shortcut_exist(argv[3], message)){
        perror("shortcut do not exist");
        return 1;
    }

    return run_commit(message);
}

int run_replace(int argc, char *argv[])
{
    if(argc < 6)
    {
        perror("please use the correct format");
        return 1;
    }
    if(neogit_exist == 0)
    {
        perror("neogit repository has not initialized yet");
        return 1;
    }

    char message[MAX_NAME_LENGTH] ;
    strcpy(message, argv[3]);
    char shrct_name[MAX_NAME_LENGTH];
    char shrct_message[MAX_MESSAGE_LENGTH];
    if(! shortcut_exist(argv[5], message)){
        perror("shortcut do not exist");
        return 1;
    }

    FILE *shortcutFile = fopen(".neogit/shortcut", "r");
    if (shortcutFile == NULL) {
        perror("error opening file");
        return 1;
    }
    FILE *tmp_file = fopen(".neogit/tmp_shortcut", "w");
    if (tmp_file == NULL) {
        perror("error opening file");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), shortcutFile) != NULL) {
        int length = strlen(line);

        // remove '\n'
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }

        sscanf(line, "%s %s",shrct_name, shrct_message);
        if(strcmp(argv[5], shrct_name) == 0) {
            fprintf(tmp_file, "%s %s\n", argv[5], argv[3]);
        }
        else{
            fputs(line, tmp_file);
            fputs("\n", tmp_file);
        }
    }

    fclose(shortcutFile);
    fclose(tmp_file);

    remove(".neogit/shortcut");
    rename(".neogit/tmp_shortcut", ".neogit/shortcut");

    fprintf(stdout, "replace done successfully");
    return 0;
}

int run_remove(int argc, char *argv[])
{
    if(argc < 4)
    {
        perror("please use the correct format");
        return 1;
    }
    if(neogit_exist == 0)
    {
        perror("neogit repository has not initialized yet");
        return 1;
    }

    char message[MAX_NAME_LENGTH];
    char shrct_name[MAX_NAME_LENGTH];
    char shrct_message[MAX_MESSAGE_LENGTH];
    if(! shortcut_exist(argv[3], message)){
        perror("shortcut do not exist");
        return 1;
    }

    FILE *shortcutFile = fopen(".neogit/shortcut", "r");
    if (shortcutFile == NULL) {
        perror("error opening file");
        return 1;
    }
    FILE *tmp_file = fopen(".neogit/tmp_shortcut", "w");
    if (tmp_file == NULL) {
        perror("error opening file");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), shortcutFile) != NULL) {
        int length = strlen(line);

        // remove '\n'
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }

        sscanf(line, "%s %s",shrct_name, shrct_message);
        if(strcmp(argv[3], shrct_name) != 0) {
            fputs(line, tmp_file);
            fputs("\n", tmp_file);
        }
    }

    fclose(shortcutFile);
    fclose(tmp_file);

    remove(".neogit/shortcut");
    rename(".neogit/tmp_shortcut", ".neogit/shortcut");

    fprintf(stdout, "remove done successfully");
    return 0;
}

void run_alias(char *name, char *command, char mode)
{
    FILE *configfile;
    if(find_in_alias(name, command)){
        perror("this name is already an alias");
        return;
    }
    if(mode == 'l'){
        if(neogit_exist == 0)
        {
            configfile = fopen(".neogit/config", "a+");
            if (configfile == NULL) {
                perror("Error opening config file");
                fclose(configfile);
                return;
            }
        }
        else{
            perror("neogit repository has not initialized yet");
            fclose(configfile);
            return;
        }
    }
    else if(mode == 'g'){
        configfile = fopen("D:\\uni\\mabani\\neogit\\globalconfig", "a+");
    }

    fprintf(configfile, "alias: %s %s\n", name , command);
    fprintf(stdout, "alias done successfully");
    fclose(configfile);
}

bool find_in_alias(char *name, char *command)
{
    char alias_name[MAX_NAME_LENGTH];
    char alias_command[MAX_NAME_LENGTH];

    FILE *config_local = fopen(".neogit/config", "r");
    if (config_local == NULL) {
        perror("error opening shortcut file");
        fclose(config_local);
        return false;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), config_local) != NULL) {
        int length = strlen(line);
        // remove '\n'
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }
        if(strstr(line, "alias:")){
            sscanf(line, "alias: %s %[^\n]",alias_name, alias_command);
            if(strcmp(name, alias_name) == 0) {
                fclose(config_local);
                strcpy(command, alias_command);
                return true;
            }   
        }
    }
    FILE *config_global = fopen("globalconfig", "r");
    if (config_global == NULL) {
        perror("error opening global file");
        fclose(config_global);
        return false;
    }

    while (fgets(line, sizeof(line), config_global) != NULL) {
        int length = strlen(line);
        // remove '\n'
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }
        if(strstr(line, "alias:")){
            sscanf(line, "alias: %s %[\n]",alias_name, alias_command);
            if(strcmp(name, alias_name) == 0) {
                fclose(config_global);
                strcpy(command, alias_command);
                return true;
            }   
        }
    }
    fclose(config_global);

    return false;
}

bool command_valid(char* command){
    if(!strcmp(command, "config") || !strcmp(command, "init") || !strcmp(command, "add") || !strcmp(command, "reset")
    || !strcmp(command, "status") || !strcmp(command, "commit") || !strcmp(command, "set") || !strcmp(command, "replace")
    || !strcmp(command, "remove") || !strcmp(command, "log") || !strcmp(command, "branch") || !strcmp(command, "checkout") )
    {
        return true;
    }
    return false;
}

int run_checkout(int argc, char * const argv[])
{
    if (argc < 3) {
        perror("invalid command");
        return 1;
    }

    if(neogit_exist == 0)
    {
        perror("neogit repository has not initialized yet");
        return 1;
    }
    

    DIR *dir = opendir(".");
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && is_tracked(entry->d_name)) {
            if (is_digit(argv[2])){
                int commit_ID = atoi(argv[2]);

                if(find_file_last_change_before_commit(entry->d_name, commit_ID) == -1){
                    continue;
                }
                if(is_change(entry->d_name)){
                    printf("file %s changed\n", entry->d_name);
                    continue;
                }
                checkout_file(entry->d_name, find_file_last_change_before_commit(entry->d_name, commit_ID));
            }
            else if(!strcmp(argv[2], "HEAD")){
                char branch[MAX_NAME_LENGTH];
                FILE *file = fopen(".neogit/config", "r+");
                char line[MAX_LINE_LENGTH];
                for(int i = 0; i < 5; i++){
                    fgets(line, sizeof(line), file);
                }
                sscanf(line, "branch: %s", branch);
                checkout_file(entry->d_name, find_HEAD(branch));
            }
            else if(argv[2][0] == 'H' && argv[2][1] == 'E' && argv[2][2] == 'A' && argv[2][3] == 'D' && argv[2][4] == '-'){
                int x;
                sscanf(argv[2], "HEAD-%d", x);
                char branch[MAX_NAME_LENGTH];
                FILE *file = fopen(".neogit/config", "r+");
                char line[MAX_LINE_LENGTH];
                for(int i = 0; i < 5; i++){
                    fgets(line, sizeof(line), file);
                }
                sscanf(line,"branch: %s", branch);
                checkout_file(entry->d_name, find_HEAD(branch) - x);
            }
            else{
                char branch[MAX_MESSAGE_LENGTH];
                strcpy(branch, argv[2]);
                if(argc > 3){
                    for(int i = 3; i < argc; i++){
                        strcat(branch, " ");
                        strcat(branch, argv[i]);
                    }
                }
                if(find_commitID(entry->d_name, branch) == -1){
                    perror("wrong commit id");
                    continue;
                }
                if(is_change(entry->d_name)){
                    printf("file %s changed\n", entry->d_name);
                    continue;
                }
                checkout_file(entry->d_name, find_commitID(entry->d_name, branch));
            }
        }
    }
        closedir(dir);

    fprintf(stdout,"checkout done successfully");
    return 0;
}

int find_file_last_change_before_commit(char *filepath, int commit_ID) {
    char filepath_dir[MAX_FILENAME_LENGTH];
    strcpy(filepath_dir, ".neogit/files/");
    strcat(filepath_dir, filepath);

    int max = -1;
    
    DIR *dir = opendir(filepath_dir);
    struct dirent *entry;
    if (dir == NULL) {
        perror("error opening file");
        return 1;
    }

    while((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            int tmp = atoi(entry->d_name);
            if (tmp > max && tmp <= commit_ID) {
                max = tmp;
            }
        }
    }
    closedir(dir);

    return max;
}

int checkout_file(char *filepath, int commit_ID) {
    char src_file[MAX_FILENAME_LENGTH];
    strcpy(src_file, ".neogit/files/");
    strcat(src_file, filepath);
    char tmp[10];
    sprintf(tmp, "/%d", commit_ID);
    strcat(src_file, tmp);

    FILE *read_file = fopen(src_file, "r");
    if (read_file == NULL) {
        perror("error opening file");
        return 1;
    }
    FILE *write_file = fopen(filepath, "w");
    if (write_file == NULL) {
        perror("error opening file");
        return 1;
    }
    
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), read_file) != NULL) {
        fprintf(write_file, "%s", line);
    }
    
    fclose(read_file);
    fclose(write_file);

    return 0;
}
bool is_digit(char *string)
{
    while (*string != '\0')
    {
        if (*string > '9' || *string < '0')
            return false;
        string++;
    }
    return true;
}

int find_commitID(char *filepath, char* branch)
{
    int max = -1;
    struct dirent *entry;
    DIR *dir = opendir(".neogit/commits");
    if(dir == NULL){
        perror("Error opening commits directory");
        return -1;
    }

    int commitID;
    while((entry = readdir(dir)) != NULL){
        char path[MAX_FILENAME_LENGTH];
        strcpy(path, ".neogit/commits/");
        strcat(path, entry->d_name);
        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")){
            continue;
        }

        FILE *file = fopen(path, "r");
        char line[MAX_LINE_LENGTH];
        while (fgets(line, sizeof(line), file) != NULL) {
            int length = strlen(line);

            // remove '\n'
            if (length > 0 && line[length - 1] == '\n') {
                line[length - 1] = '\0';
            }

            if (strstr(line, "branch:") && strstr(line, branch))
            {
                commitID = atoi(entry->d_name);
            } 
            if (strstr(line, filepath))
            {
                if(max < commitID) max = commitID;
            } 
        }
    }
    closedir(dir);

    return max;
}

int run_branch(int argc, char * const argv[])
{
    if(neogit_exist == 0)
    {
        perror("neogit repository has not initialized yet");
        return 1;
    }

    char name[MAX_NAME_LENGTH];
    strcpy(name, argv[2]);
    if(argc > 3){
        for(int i = 3; i < argc; i++){
            strcat(name, " ");
            strcat(name, argv[i]);
        }
    }

    if(branch_exist(name)){
        perror("branch already exist!");
        return 1;
    }

    FILE *branchfile = fopen(".neogit/branch", "a");
    if (branchfile == NULL) {
        perror("error opening branch file");
        fclose(branchfile);
        return 1;
    }

    fprintf(branchfile, "%s\n", name);
    fclose(branchfile);

    FILE *configfile= fopen(".neogit/config", "r+");
    if (configfile == NULL) {
        perror("error opening config file");
        fclose(configfile);
        return 1;
    }
    FILE *tmpconfig = fopen(".neogit/temp_config", "w");
    if (tmpconfig == NULL) {
        perror("error opening temp file");
        fclose(tmpconfig);
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), configfile) != NULL) {
        int length = strlen(line);
        // remove '\n'
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }

        if(strstr(line, "branch:")) {
            fprintf(tmpconfig, "branch: %s\n", name);
        }
        else{
            fprintf(tmpconfig, "%s\n", line);
        }
    }
    fclose(configfile);
    fclose(tmpconfig);
    
    if (remove(".neogit/config") != 0) {
        perror("Error removing file");
        return 1;
    }

    rename(".neogit/temp_config", ".neogit/config");
    fprintf(stdout,"branch creat successfully");
    return 0;
}

bool branch_exist(char name[])
{
    FILE *branchfile = fopen(".neogit/branch", "r");
    if (branchfile == NULL) {
        perror("error opening branch file");
        fclose(branchfile);
        return false;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), branchfile) != NULL) {
        int length = strlen(line);

        // remove '\n'
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }

        if(strcmp(line, name) == 0) {
            fclose(branchfile);
            return true;
        }   
    }

    fclose(branchfile);
    return false;
}

void list_branch()
{
    FILE *branchfile = fopen(".neogit/branch", "r");
    if (branchfile == NULL) {
        perror("error opening branch file");
        fclose(branchfile);
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), branchfile) != NULL) {
        int length = strlen(line);

        fprintf(stdout, "%s", line);
    }
    fclose(branchfile);
}

void run_status(char* name)
{
    if(neogit_exist == 0)
    {
        perror("neogit repository has not initialized yet");
        return;
    }

    struct dirent *entry;
    DIR *dir = opendir(name);
    if(dir == NULL){
        perror("Error opening current directory");
        return;
    }
    while((entry = readdir(dir)) != NULL){
        check_status(entry->d_name);    
    }
    closedir(dir);
}

void check_status(char* filepath)
{
    bool is_staged = false;
    if(check_staging_area(filepath) == 0) is_staged = true;

    char x = '-', y = ' ';
    if(is_staged || is_tracked(filepath)) x = '+';
    if(!is_staged && !is_tracked(filepath)) return;
    if(is_staged && !is_tracked(filepath)) y = 'A';
    if(is_change(filepath)) y = 'M';
    if((is_staged || is_tracked(filepath)) && !file_exists(filepath)) y = 'D';
    fprintf(stdout, "%s %c%c\n", filepath, x, y);
}

bool is_change(char* filepath)
{
    char file[MAX_FILENAME_LENGTH];
    sprintf(file, ".neogit/files/%s", filepath);

    struct dirent *entry;
    DIR *dir = opendir(file);
    if(dir == NULL){
        perror("Error opening commits directory");
        return true;
    }

    int last_commit;
    while((entry = readdir(dir)) != NULL){
        last_commit = atoi(entry->d_name);
    }
    closedir(dir);

    sprintf(file, ".neogit/files/%s/%d", filepath, last_commit);
    FILE *commited_file = fopen(file, "r");
    if (commited_file == NULL) {
        perror("error opening commited file");
        fclose(commited_file);
        return true;
    }
    FILE *current_file = fopen(filepath, "r");
    if (current_file == NULL) {
        perror("error opening curent file");
        fclose(current_file);
        return true;
    }

    char line1[MAX_LINE_LENGTH];
    char line2[MAX_LINE_LENGTH];
    while (fgets(line1, sizeof(line1), commited_file) != NULL && fgets(line2, sizeof(line2), current_file) != NULL) {
        if (strncmp(line1, line2, MAX_LINE_LENGTH) != 0) {
            fclose(current_file);
            fclose(commited_file);
            return true;
        }
    }

    fclose(current_file);
    fclose(commited_file);
    return false;
}

void print_log(char* filepath)
{
    FILE *file = fopen(filepath, "r+");
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL){
        fprintf(stdout, "%s", line);
    }
    fprintf(stdout, "\n");
    fclose(file);
}

int run_log(int argc, char *argv[])
{
    if(neogit_exist == 0)
    {
        perror("neogit repository has not initialized yet");
        return 1;
    }

    struct dirent *entry;
    DIR *dir = opendir(".neogit/commits");
    if(dir == NULL){
        perror("Error opening current directory");
        return 1;
    }
    char filepath[MAX_FILENAME_LENGTH];
    if(argc == 2){
        while((entry = readdir(dir)) != NULL){
            sprintf(filepath, ".neogit/commits/%s", entry->d_name);
            print_log(filepath);
        }
    }
    else if(argc == 4 && !strcmp(argv[2], "-n")){
        if(!(is_digit(argv[3]))){
            perror("invalid input");
            return 1;
        }
        int numberoffiles= atoi(argv[3]);
        int last_commit_id;
        FILE *file = fopen(".neogit/config", "r+");
        char line[MAX_LINE_LENGTH];
        for(int i = 0; i < 3; i++){
            fgets(line, sizeof(line), file);
        }
        sscanf(line,"last_commit_ID: %d", &last_commit_id);
        if(numberoffiles > last_commit_id || numberoffiles <= 0){
            perror("invalid input");
            return 1;
        }
        for(int i = 0; i < last_commit_id - numberoffiles + 2; i++){
            entry = readdir(dir);
        }
        while((entry = readdir(dir)) != NULL){
            sprintf(filepath, ".neogit/commits/%s", entry->d_name);
            print_log(filepath);
        }
    }
    else if(!strcmp(argv[2], "-branch")){
        char branch[MAX_NAME_LENGTH];
        strcpy(branch, argv[3]);
        if(argc > 3){
            for(int i = 4; i < argc; i++){
                strcat(branch, " ");
                strcat(branch, argv[i]);
            }
        }
        if(!branch_exist){
            perror("branch not exist");
            return 1;
        }
        
        DIR *dir = opendir(".neogit/commits");
        if(dir == NULL){
            perror("Error opening current directory");
            return 1;
        }
        char line[MAX_LINE_LENGTH];
        char filepath[MAX_FILENAME_LENGTH];
        struct dirent *entry;
        while((entry = readdir(dir)) != NULL){
            sprintf(filepath, ".neogit/commits/%s",entry->d_name);
            FILE *file = fopen(filepath, "r+");
            while (fgets(line, sizeof(line), file) != NULL) {
                if(strstr(line, branch) && strstr(line, "branch:")) {
                    fclose(file);
                    print_log(filepath);
                }
            }
            fclose(file);
        }
        closedir(dir);
        return 0;
    }
    else if(!strcmp(argv[2], "-author")){
        char author[MAX_NAME_LENGTH];
        strcpy(author, argv[3]);

        DIR *dir = opendir(".neogit/commits");
        if(dir == NULL){
            perror("Error opening current directory");
            return 1;
        }
        char line[MAX_LINE_LENGTH];
        char filepath[MAX_FILENAME_LENGTH];
        struct dirent *entry;
        while((entry = readdir(dir)) != NULL){
            sprintf(filepath, ".neogit/commits/%s",entry->d_name);
            FILE *file = fopen(filepath, "r+");
            while (fgets(line, sizeof(line), file) != NULL) {
                if(strstr(line, author) && strstr(line, "username:")) {
                    printf("%s\n", line);
                    fclose(file);
                    print_log(filepath);
                }
            }
            fclose(file);
        }
        closedir(dir);
        return 0;
    }
    else if(!strcmp(argv[2], "-search")){
        for(int i = 3; i < argc; i++){
        char mode = 'n';
        if(strchr(argv[i], '*') != NULL) mode = 'w';
        
        DIR *dir = opendir(".neogit/commits");
        if(dir == NULL){
            perror("Error opening current directory");
            return -1;
        }
        char line[MAX_LINE_LENGTH];
        char filepath[MAX_FILENAME_LENGTH];
        char message[MAX_MESSAGE_LENGTH];
        struct dirent *entry;
        while((entry = readdir(dir)) != NULL){
            sprintf(filepath, ".neogit/commits/%s",entry->d_name);
            FILE *file = fopen(filepath, "r+");

            fgets(line, sizeof(line), file);
            fgets(line, sizeof(line), file);

            sscanf(line ,"message: %[^\n]",message);
            if(mode == 'n' && strstr(message, argv[i])){
                fclose(file);
                print_log(filepath);
            }
            if(mode == 'w' && fnmatch(argv[i], message) == 0){
                fclose(file);
                print_log(filepath);
            }
            fclose(file);
        }
        closedir(dir);
    }
    }
    else if(!strcmp(argv[2], "-since") || !strcmp(argv[2], "-before")){
        struct tm timeinfo = {0};
        sscanf(argv[3], "%d-%d-%d",
           &timeinfo.tm_year, &timeinfo.tm_mon, &timeinfo.tm_mday);
        sscanf(argv[4], "%d:%d:%d",
           &timeinfo.tm_hour, &timeinfo.tm_min, &timeinfo.tm_sec);

        timeinfo.tm_year -= 1900;
        timeinfo.tm_mon--;
        time_t input_time = mktime(&timeinfo);

        DIR *dir = opendir(".neogit/commits");
        if(dir == NULL){
            perror("Error opening current directory");
            return 1;
        }
        char line[MAX_LINE_LENGTH];
        char filepath[MAX_FILENAME_LENGTH];
        struct dirent *entry;
        while((entry = readdir(dir)) != NULL){
            sprintf(filepath, ".neogit/commits/%s",entry->d_name);
            FILE *file = fopen(filepath, "r+");
            while (fgets(line, sizeof(line), file) != NULL) {
                if(strstr(line, "time:")) {
                    struct tm file_timeinfo = {0};
                sscanf(line, "time: %d-%d-%d %d:%d:%d",
                    &file_timeinfo.tm_year, &file_timeinfo.tm_mon, &file_timeinfo.tm_mday,
                    &file_timeinfo.tm_hour, &file_timeinfo.tm_min, &file_timeinfo.tm_sec);
                file_timeinfo.tm_year -= 1900;
                file_timeinfo.tm_mon--;
                time_t file_time = mktime(&file_timeinfo);
                    if(!strcmp(argv[2], "-since") && file_time >= input_time){
                        print_log(filepath);
                    }
                    if(!strcmp(argv[2], "-before") && file_time <= input_time){
                        print_log(filepath);
                    }
                }
            }
            fclose(file);
        }
        closedir(dir);
    }
    return 0;
}

int run_tag(int argc, char * const argv[])
{
    if(neogit_exist == 0)
    {
        perror("neogit repository has not initialized yet");
        return 1;
    }
        
    if(argc == 2){
        struct dirent *entry;
        DIR *dir = opendir(".neogit/tags");
        if(dir == NULL){
            perror("Error opening current directory");
            return 1;
        }
        while((entry = readdir(dir)) != NULL){
            if(entry->d_type == DT_REG)
                fprintf(stdout, "%s\n", entry->d_name);   
        }
        closedir(dir);
    }
    else if(strcmp(argv[2] ,"-a") == 0){
        if(! strcmp(argv[argc - 1] ,"-f")){
            char file[MAX_FILENAME_LENGTH];
            sprintf(file, ".neogit/tags/%s", argv[3]);
            if (remove(file) != 0) {
                perror("Error removing tag file");
                return 1;
            }

            FILE *tagfile = fopen(".neogit/tag", "r");
            FILE *tmp_file = fopen(".neogit/tmp_tag", "w");
            if (tmp_file == NULL || tagfile == NULL) {
                perror("error opening file");
                return 1;
            }
            char line[MAX_LINE_LENGTH];
            while (fgets(line, sizeof(line), tagfile) != NULL) {
                int length = strlen(line);
                if(strstr(line, argv[3]) == 0)
                    fputs(line, tmp_file);
            }

            fclose(tagfile);
            fclose(tmp_file);

            if (remove(".neogit/tag") != 0) {
                perror("Error removing old tag file");
                return 1;
            }
            if (rename(".neogit/tmp_tag", ".neogit/tag") != 0) {
                perror("Error renaming temporary file");
                return 1;
            }
        }
        else if(tag_exist(argv[3])){
            perror("tag already exist");
            return 1;
        }
        FILE *tagFile = fopen(".neogit/tag", "a");
        fprintf(tagFile ,"%s\n", argv[3]);
        fclose(tagFile);

        if(argc > 6 && strcmp(argv[4], "-m") == 0 && strcmp(argv[6], "-c") == 0){
            create_tag_file(argv[3], argv[5], atoi(argv[7]));
        }
        else if(argc > 6 && strcmp(argv[4], "-m") == 0){
            int last_ID;
            FILE *config_file = fopen(".neogit/config", "r");
            char line[MAX_LINE_LENGTH];
            for(int i = 0; i < 3; i++){
                fgets(line, sizeof(line), config_file);   
            }
            sscanf(line,"last_commit_ID: %d", &last_ID);
            create_tag_file(argv[3], argv[5], last_ID);
        }
        else if(argc > 4 && strcmp(argv[4], "-c") == 0){
            create_tag_file(argv[3], "no message", atoi(argv[5]));
        }
        else if(argc == 4){
            int last_ID;
            FILE *config_file = fopen(".neogit/config", "r");
            char line[MAX_LINE_LENGTH];
            for(int i = 0; i < 3; i++){
                fgets(line, sizeof(line), config_file);   
            }
            sscanf(line,"last_commit_ID: %d", &last_ID);
            create_tag_file(argv[3], "no message", last_ID);
        }
        else{
            perror("invalid command");
            return 1;
        }
        fprintf(stdout, "tag create successfully");
        return 0;
    }
    else if(strcmp(argv[2] ,"show") == 0){
        char filepath[MAX_FILENAME_LENGTH];
    
        sprintf(filepath, ".neogit/tags/%s", argv[3]);

        FILE *tag_file = fopen(filepath, "r");
        char line[MAX_LINE_LENGTH];
        while(fgets(line, sizeof(line), tag_file) != NULL)
        {
            fprintf(stdout, "%s", line);
        }
    }

}

bool tag_exist(char name[])
{
    FILE *tagfile = fopen(".neogit/tag", "r");
    if (tagfile == NULL) {
        perror("error opening tag file");
        fclose(tagfile);
        return false;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), tagfile) != NULL) {
        int length = strlen(line);

        // remove '\n'
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }

        if(strcmp(line, name) == 0) {
            fclose(tagfile);
            return true;
        }   
    }

    fclose(tagfile);
    return false;
}

int create_tag_file(char *name, char *message, int commit_ID) {
    char tag_filepath[MAX_FILENAME_LENGTH];
    strcpy(tag_filepath, ".neogit/tags/");
    sprintf(tag_filepath,".neogit/tags/%s", name);

    FILE *tag_file = fopen(tag_filepath, "w");
    if (tag_file == NULL) {
        perror("error opening commit filepath");
        return 1;
    }
    fprintf(tag_file, "tag %s\n", name);
    fprintf(tag_file, "commitID: %d\n", commit_ID);
    fprintf(tag_file, "message: %s\n", message);

    time_t currentTime;
    time(&currentTime);
    struct tm *localTime = localtime(&currentTime);
    char timeString[50];
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S\n", localTime);
    fprintf(tag_file, "time: %s", timeString);

    char filepath[MAX_FILENAME_LENGTH];
    sprintf(filepath, ".neogit/commits/%d", commit_ID);

    FILE *commit_file = fopen(filepath, "r");
    char line[MAX_LINE_LENGTH];
    for(int i = 0; i < 5; i++){
        fgets(line, sizeof(line), commit_file);   
    }
    int length = strlen(line);
    if (length > 0 && line[length - 1] == '\n') {
        line[length - 1] = '\0';
    }
    char username[MAX_NAME_LENGTH], email[MAX_NAME_LENGTH];
    sscanf(line, "username: %s", username);
    fgets(line, sizeof(line), commit_file);
    sscanf(line, "email: %s", email);
    
    fprintf(tag_file, "Author:: %s <%s>", username, email);
    fclose(tag_file);
    return 0;
}

int run_grep(int argc, char * const argv[]){
    if(neogit_exist == 0)
    {
        perror("neogit repository has not initialized yet");
        return 1;
    }

    if(!file_exists(argv[3])){
        perror("file does not exist");
        return 1;
    }

    if(argc > 6 && !strcmp(argv[6], "-c")){
        char filepath[MAX_FILENAME_LENGTH];
        sprintf(filepath, ".neogit/files/%s/%s", argv[3], argv[7]);
        if(argc == 9 && !strcmp(argv[8], "-n"))
            find_word(filepath, argv[5], 'l');
        else if(argc == 8)
            find_word(filepath, argv[5], 'n');
            else{
                perror("invalid command");
                return 1;
            }
    }
    else if(argc > 6 && !strcmp(argv[6], "-n")){
        find_word(argv[3], argv[5], 'l');
    }
    else if(argc == 6){
        find_word(argv[3], argv[5], 'n');
    }
    else{
        perror("invalid command");
        return 1;
    }

    return 0;
}

void find_word(char* filepath, char* word, char mode)
{
    bool is_found = false;
    FILE *file = fopen(filepath, "r");
    char line[MAX_LINE_LENGTH];
    int num_line = 0;
    while (fgets(line, sizeof(line), file) != NULL){
        int length = strlen(line);
        num_line++;

        if(strstr(line, word)){
            is_found = true;
            if(mode == 'l')
                fprintf(stdout, "\033[33m%d \033[0m", num_line);
            char* token = strtok (line," ");
            while (token != NULL)
            {
                if(! strcmp(token, word)){
                    fprintf(stdout, "\033[35m%s \033[0m", token);
                }
                else{
                    fprintf(stdout, "%s ", token);
                }
                token = strtok(NULL, " ");
            }
        }
    }

    if(!is_found)
        fprintf(stdout, "nothing found!\n");
}

int run_diff(char *file1, char *file2, int line1_start, int line1_end, int line2_start, int line2_end)
{
    if(neogit_exist == 0)
    {
        perror("neogit repository has not initialized yet");
        return 1;
    }

    if(!file_exists(file1) || !file_exists(file1)){
        perror("files not exists\n");
        return 1;
    }

    FILE *fp1 = fopen(file1, "r");
    FILE *fp2 = fopen(file2, "r");
    if (fp1 == NULL || fp2 == NULL) {
        perror("Error opening files.\n");
        return 1;
    }

    char line1[1000], line2[1000];
    int curline1 = 1, curline2 = 1;

    bool diff_found = false;

    while (fgets(line1, sizeof(line1), fp1) != NULL && fgets(line2, sizeof(line2), fp2) != NULL) {
        char cpy_line1[1000], cpy_line2[1000];
        strcpy(cpy_line1, line1);
        strcpy(cpy_line2, line2);

        if(strcmp(line1, "\n") == 0){
            while (strcmp(line1, "\n") == 0){
                fgets(line1, sizeof(line1), fp1);
                curline1++;
            }
        }
        if(strcmp(line2, "\n") == 0){
            while (strcmp(line2, "\n") == 0){
                fgets(line2, sizeof(line2), fp2);
                curline2++;
            }
        }

        remove_spaces(line1);
        remove_spaces(line2);

        // remove '\n'
        int length = strlen(line1);
        if (length > 0 && line1[length - 1] == '\n') {
            line1[length - 1] = '\0';
        }
        length = strlen(line1);
        if (length > 0 && line1[length - 1] == '\n') {
            line1[length - 1] = '\0';
        }

        if ((curline1 >= line1_start && curline1 <= line1_end) || (line1_start == 0 && line1_end == 0)) {
            if ((curline2 >= line2_start && curline2 <= line2_end) || (line2_start == 0 && line2_end == 0)) {
                if (strcmp(line1, line2) != 0) {
                    fprintf(stdout,"\033[34m%s Line %d : %s\033[0m",file1 ,curline1, cpy_line1);
                    fprintf(stdout ,"\033[32m%s Line %d : %s\n\033[0m",file2 , curline2, cpy_line2);
                    diff_found = true;
                }
            }
            curline2++;
        }
        curline1++;
    }
    if(! diff_found)
        fprintf(stdout,"files match");
    fclose(fp1);
    fclose(fp2);
    return 0;
}

void remove_spaces(char *str)
{
    int i, j = 0;
    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n') {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

void compare_commits(int commitID1, int commitID2)
{
    char filepath1[MAX_FILENAME_LENGTH], filepath2[MAX_FILENAME_LENGTH];
    sprintf(filepath1,".neogit/commits/%d", commitID1);
    FILE *file1 = fopen(filepath1, "r+");

    sprintf(filepath2,".neogit/commits/%d", commitID2);
    FILE *file2 = fopen(filepath2, "r+");

    // if (file1 == NULL || file2 == NULL) {
    //     perror("Error opening files.\n");
    //     return;
    // }
    fseek(file1, sizeof(char), SEEK_SET);
    char line1[MAX_LINE_LENGTH], line2[MAX_LINE_LENGTH];
    for(int i = 0; i < 7; i++){
       fgets(line1, sizeof(line1), file1);
       fgets(line2, sizeof(line2), file2);

        int length = strlen(line1);
        if (length > 0 && line1[length - 1] == '\n') {
            line1[length - 1] = '\0';
        }
        length = strlen(line2);
        if (length > 0 && line2[length - 1] == '\n') {
            line2[length - 1] = '\0';
        }
    }

    while(fgets(line1, sizeof(line1), file1) != NULL){
        while(fgets(line2, sizeof(line2), file2) != NULL){
            int length = strlen(line1);
            if (length > 0 && line1[length - 1] == '\n') {
                line1[length - 1] = '\0';
            }
            length = strlen(line2);
            if (length > 0 && line2[length - 1] == '\n') {
                line2[length - 1] = '\0';
            }

            if (strcmp(line1, line2) == 0){
                char path1[MAX_FILENAME_LENGTH], path2[MAX_FILENAME_LENGTH];
                sprintf(path1,".neogit/files/%s/%d", line1,commitID1);
                sprintf(path2,".neogit/files/%s/%d", line2,commitID2);
                printf("path1: %s\npath2: %s\n", path1, path2);
                run_diff(path1, path2, 0, 0, 0, 0);
            } 
        }
    } 
}

int run_merge(char* branch1, char* branch2)
{
    if(neogit_exist == 0)
    {
        perror("neogit repository has not initialized yet");
        return 1;
    }
    if(!branch_exist(branch1) || !branch_exist(branch2))
    {
        perror("branch names are invalid");
        return 1;
    }

    char path1[MAX_FILENAME_LENGTH], path2[MAX_FILENAME_LENGTH];
    sscanf(path1, ".neogit/commits/%d", find_HEAD(branch1));
    sscanf(path2, ".neogit/commits/%d",find_HEAD(branch2));  
    FILE *file1 = fopen(path1, "r");
    FILE *file2 = fopen(path2, "r");
    if (file1 == NULL || file2 == NULL) {
        perror("Error opening files.\n");
        return 1;
    } 

    char line1[MAX_LINE_LENGTH], line2[MAX_LINE_LENGTH];
    for(int i = 0; i < 7; i++){
       fgets(line1, sizeof(line1), file1);
       fgets(line2, sizeof(line2), file2);
    }
}
int find_HEAD(char* branch)
{
    int max = -1;
    struct dirent *entry;
    DIR *dir = opendir(".neogit/commits");
    if(dir == NULL){
        perror("Error opening commits directory");
        return -1;
    }

    int commitID;
    while((entry = readdir(dir)) != NULL){
        char path[MAX_FILENAME_LENGTH];
        strcpy(path, ".neogit/commits/");
        strcat(path, entry->d_name);

        FILE *file = fopen(path, "r");
        char line[MAX_LINE_LENGTH];
        while (fgets(line, sizeof(line), file) != NULL) {
            int length = strlen(line);

            // remove '\n'
            if (length > 0 && line[length - 1] == '\n') {
                line[length - 1] = '\0';
            }
            if (strstr(line, "branch:") && strstr(line, branch))
            {
                commitID = atoi(entry->d_name);
                if(max < commitID) max = commitID; 
            } 

        }
    }
    closedir(dir);

    return max;
}

int run_revert(int argc, char * const argv[]){
    if(neogit_exist == 0)
    {
        perror("neogit repository has not initialized yet");
        return 1;
    }

    int commitID;
    char message[MAX_MESSAGE_LENGTH];
    
    if(! strcmp(argv[2], "-n")){
        commitID = atoi(argv[argc - 1]);
    }
    else if(strncmp(argv[argc - 1], "HEAD", 4) == 0){
        FILE *configfile = fopen(".neogit/config", "r");
        char line[MAX_LINE_LENGTH];
        for(int i = 0; i < 3; i++){
            fgets(line, sizeof(line), configfile);
        }
        sscanf(line,"last_commit_ID: %d", &commitID);
        int x;
        sscanf(argv[argc - 1], "HEAD-%d", &x);
        commitID -= x;

        if(! strcmp(argv[2], "-m")){
            strcpy(message, argv[3]);
            if(argc > 5){
                for(int i = 4; i < argc - 1; i++){
                    strcat(message, " ");
                    strcat(message, argv[i]);
                }
            }
        }
        else{
            strcpy(message, find_message(commitID));
        }
    }
    else{
        if(! strcmp(argv[2], "-m")){
            strcpy(message, argv[3]);
            if(argc > 5){
                for(int i = 4; i < argc - 1; i++){
                    strcat(message, " ");
                    strcat(message, argv[i]);
                }
            }
        }
        else{
            if(is_digit(argv[2]))
                strcpy(message, find_message(atoi(argv[2])));
            else{
                perror("invalid command");
                return 1;
            }
        }
        commitID = atoi(argv[argc - 1]);
    }

    if(strlen(message) > 72){
        perror("message is too long");
        return 1;
    }
    int new_commit_ID;
    if(strcmp(argv[2], "-n")){
        new_commit_ID = inc_last_commit_ID();
        if (new_commit_ID == -1){
            perror("error getting new commit id");
            return 1; 
        }
    }

    struct dirent *entry;
    char filetoreplace[MAX_FILENAME_LENGTH];
    DIR *dir = opendir(".neogit/files");
    if(dir == NULL){
        perror("Error opening current directory");
        return 1;
    }
    while((entry = readdir(dir)) != NULL){
        sprintf(filetoreplace ,".neogit/files/%s/%d", entry->d_name, commitID);
        if(file_exists(filetoreplace)){
            replace(entry->d_name, filetoreplace);
            if(strcmp(argv[2], "-n")){
                commit_staged_file(new_commit_ID, entry->d_name);
            }
        }
        
    }
    closedir(dir);

    if(! strcmp(argv[2], "-n")){
        fprintf(stdout, "revert done successfully");
        return 0;
    }

    time_t currentTime;
    time(&currentTime);
    struct tm *localTime = localtime(&currentTime);
    char timeString[50];
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", localTime);

    create_commit_file(new_commit_ID, message);
    fprintf(stdout, "commit successfully with commit ID %d\n", new_commit_ID);
    fprintf(stdout, "your message: %s\ntime: %s", message, timeString);
    return 0;
}

char* find_message(int commitID){
    char path[MAX_FILENAME_LENGTH];
    sprintf(path, ".neogit/commits/%d", commitID);
    FILE *commitFile = fopen(path, "r");

    char line[MAX_LINE_LENGTH];
    char message[MAX_MESSAGE_LENGTH];
    while (fgets(line, sizeof(line), commitFile) != NULL) {
        if (strstr(line, "message:")) {
            sscanf(line, "message: %[^\n]", message);
            break;
        }
    }
    fclose(commitFile);
    printf("mess: %s\n\n", message);
    char* result;
    strcpy(result, message);
    return result;
}

int replace(char* mainfile, char* filepath)
{
    FILE *read_file, *write_file;
    char read_path[MAX_FILENAME_LENGTH];
    strcpy(read_path, filepath);
    char write_path[MAX_FILENAME_LENGTH];
    strcpy(write_path, mainfile);

    read_file = fopen(read_path, "r");
    if (read_file == NULL) {
        perror("error opening read file");
        return 1;
    }

    write_file = fopen(write_path, "w");
    if (write_file == NULL) {
        perror("error opening write file");
        return 1;
    }

    char buffer;
    buffer = fgetc(read_file);
    while(buffer != EOF) {
        fputc(buffer, write_file);
        buffer = fgetc(read_file);
    }
    fclose(read_file);
    fclose(write_file);

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
    //coloring prints
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        printf("Error getting console handle\n");
        return -1;
    }
    DWORD consoleMode;
    if (!GetConsoleMode(hConsole, &consoleMode)) {
        printf("Error getting console mode\n");
        return -1;
    }
    if (!SetConsoleMode(hConsole, consoleMode)) {
        printf("Error setting console mode\n");
        return -1;
    }

    print_command(argc, argv);
    //just input neogit
    if (argc < 2) {
        fprintf(stdout, "please enter a valid command");
        return 1;
    }
    char command[MAX_MESSAGE_LENGTH];
    if(neogit_exist() == 0 && find_in_alias(argv[1], command)){
        strcpy(*argv, "neogit ");
        strcat(*argv, command);
    }
    
    if(! strcmp(argv[1], "config")){
        if(! strcmp(argv[2], "global")){
            if(! strcmp(argv[3], "user.name") || ! strcmp(argv[3], "user.email"))
                return run_global_config(argc , argv);
            else if(argv[3][0] == 'a' && argv[3][1] == 'l' && argv[3][2] == 'i' && argv[3][3] == 'a' && argv[3][4] == 's' && argv[3][5] == '.'){
                char command[MAX_MESSAGE_LENGTH];
                strcpy(command, argv[4]);
                if(argc > 5){
                    for(int i = 5; i < argc; i++){
                        strcat(command, " ");
                        strcat(command, argv[i]);
                    }   
                }
                if(!command_valid(argv[5])){
                    perror("invalid command in alias");
                    return 1;
                }
                run_alias(argv[3] + 6, command, 'g');
            }
                
        }   
        else if(! strcmp(argv[2], "user.name") || ! strcmp(argv[2], "user.email"))
            return run_lokal_config(argc , argv);
        else if (argv[2][0] == 'a' && argv[2][1] == 'l' && argv[2][2] == 'i' && argv[2][3] == 'a' && argv[2][4] == 's' && argv[2][5] == '.' && argc == 4){
            char command[MAX_MESSAGE_LENGTH];
            strcpy(command, argv[3]);
            if(argc > 4){
                for(int i = 4; i < argc; i++){
                    strcat(command, " ");
                    strcat(command, argv[i]);
                }   
            }
            if(!command_valid(argv[4])){
                perror("invalid command in alias");
                return 1;
            }
            run_alias(argv[2] + 6, command, 'l');
        }  
        else
            fprintf(stdout, "please enter a valid command");
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
    else if(! strcmp(argv[1], "commit")){
        if(! strcmp(argv[2], "-m")){
            if (argc < 4) {
            perror("please use the correct format");
            return 1;
        }
        char message[MAX_MESSAGE_LENGTH];
        strcpy(message, argv[3]);
        if(argc > 4){
            strcpy(message, "\"");
            for(int i = 3; i < argc; i++){
                strcat(message, " ");
                strcat(message, argv[i]);
            }
            strcat(message, "\"");
        }
            return run_commit(message);
        }
        else if(! strcmp(argv[2], "-s"))
            return run_commit_s(argc, argv);
    }
    else if (strcmp(argv[1], "set") == 0 && strcmp(argv[2], "-m")==0 && strcmp(argv[4], "-s") == 0) {
        return run_set(argc, argv);
    }
    else if (strcmp(argv[1], "replace") == 0 && strcmp(argv[2], "-m")==0 && strcmp(argv[4], "-s") == 0) {
        return run_replace(argc, argv);
    }
    else if (strcmp(argv[1], "remove") == 0 && strcmp(argv[2], "-s")==0) {
        return run_remove(argc, argv);
    }
    else if (!strcmp(argv[1], "checkout")) {
        return run_checkout(argc, argv);
    }
    else if (!strcmp(argv[1], "branch")) {
        if(argc > 2)
            return run_branch(argc, argv);
        else if(argc == 2)
            list_branch();
    }
    else if (!strcmp(argv[1], "status") && argc == 2) {
        run_status(".");
    }
    else if(!strcmp(argv[1], "log")){
        return run_log(argc, argv);
    }
    else if(!strcmp(argv[1], "tag")){
        return run_tag(argc, argv);
    }
    else if(!strcmp(argv[1], "grep")){
        return run_grep(argc, argv);
    }
    else if(!strcmp(argv[1], "diff")){
        if(!strcmp(argv[2], "-f")){
            int line1_start = 0, line1_end = 0, line2_start = 0, line2_end = 0;
            for (int i = 3; i < argc; i++) {
                if (strcmp(argv[i], "-line1") == 0) {
                    sscanf(argv[i + 1], "%d-%d", &line1_start, &line1_end);
                }
                else if (strcmp(argv[i], "-line2") == 0) {
                    sscanf(argv[i + 1], "%d-%d", &line2_start, &line2_end);
                }
            }
            run_diff(argv[3], argv[4], line1_start, line1_end, line2_start, line2_end);
        }
        else if(!strcmp(argv[2], "-c")){
            compare_commits(atoi(argv[3]) , atoi(argv[4]));
        }
        else{
            perror("invalid command");
            return 1;
        }
    }
    else if(!strcmp(argv[1], "merge") && !strcmp(argv[2], "-b")){
        return run_merge(argv[3], argv[4]);
        //not compelete
    }
    else if(!strcmp(argv[1], "revert")){
        return run_revert(argc, argv);
    }
    else{
        perror("invalid command!");
        return 1;
    }
    
    return 0;
}