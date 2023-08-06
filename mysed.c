#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 2048

void substitute_string(char* target, char* substitution, char* buffer, size_t buffer_size) {
    size_t target_len = strlen(target);
    size_t substitution_len = strlen(substitution);

    char* pos = buffer;
    while ((pos = strstr(pos, target)) != NULL) {
        size_t prefix_len = pos - buffer;
        memmove(pos + substitution_len, pos + target_len, buffer_size - (pos - buffer) - target_len);
        memcpy(pos, substitution, substitution_len);
        pos += substitution_len;
        buffer_size -= target_len - substitution_len;
    }
}

void print_lines_in_range(int start, int end, char* filename) {
    FILE * file_ptr = fopen(filename, "r");
    char buffer[BUFFER_SIZE];
    int line_number = 1;
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file_ptr)) > 0) {
        char * line = strtok(buffer, "\n");
        while (line != NULL) {
            if (line_number >= start && line_number <= end) {
                printf("%s\n", line);
            }
            line = strtok(NULL, "\n");
            line_number++;
        }
    }

    fclose(file_ptr);
}

void print_lines_pattern(char* pattern, char* filename) {
    FILE * file_ptr = fopen(filename, "r");
    char buffer[BUFFER_SIZE];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file_ptr)) > 0) {
        char * line = strtok(buffer, "\n");
        while (line != NULL) {
            if (strstr(line, pattern) != NULL) {
                printf("%s\n", line);
            }
            line = strtok(NULL, "\n");
        }
    }

    fclose(file_ptr);
}

void double_space(char* filename) {
    FILE * file_ptr = fopen(filename, "r");
    char buffer[BUFFER_SIZE];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file_ptr)) > 0) {
        char * line = strtok(buffer, "\n");
        while (line != NULL) {
            printf("%s\n", line);
            printf("\n");
            line = strtok(NULL, "\n");
        }
    }

    fclose(file_ptr);
}

void strip_trailing_blanks(char* filename) {
    FILE * file_ptr = fopen(filename, "r");
    char buffer[BUFFER_SIZE];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file_ptr)) > 0) {
        char * line = strtok(buffer, "\n");
        while (line != NULL) {
            if (strcmp(line, "") != 0) {
                printf("%s\n", line);
            }
            line = strtok(NULL, "\n");
        }
    }

    fclose(file_ptr);
}


void parse(char* argv[]) {
    char * option = argv[1];
    char * filename;

    if (strcmp(option, "-n") == 0){
        char * range_or_pattern = argv[2];
        // output lines in range
        if (strchr(range_or_pattern, ',') != NULL) {
            // Ranges
            int start, end;
            sscanf(range_or_pattern, "%d,%dp", &start, &end);
            filename = argv[3];
            print_lines_in_range(start, end, filename);
        }
        // output only lines containing the specifc pattern
        else {
            char * pattern = argv[2];
            filename = argv[3];
            print_lines_pattern(pattern, filename);
        }   
    }
    else if (strcmp(option, "-G") == 0) {
        // Double space
        filename = argv[2];
        double_space(filename);
    }
    else if (strcmp(option, "/^$/d")) {
        // Strip trailing blank liens
        strip_trailing_blanks(filename);
    }
    else {
        // Substitution
        FILE * file_ptr = fopen(argv[2], "r+");
        char * target = strtok(argv[1], "/");
        char * substitution = strtok(NULL, "/");
        filename = argv[2];
        printf("here");
        char buffer[BUFFER_SIZE];
        size_t bytes_read;

        while ((bytes_read = fread(buffer, 1, sizeof(buffer), file_ptr)) > 0) {
            substitute_string(target, substitution, buffer, bytes_read);
            fseek(file_ptr, -bytes_read, SEEK_CUR); // Move the file pointer back to the beginning of the read buffer
            fwrite(buffer, 1, bytes_read, file_ptr); // Write the modified buffer back to the file
        }

        fclose(file_ptr);
    }

}

int main (int argc, char* argv[]){
    parse(argv);
    return 0;
}