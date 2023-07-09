#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 100

int main() {
    FILE *file;
    char line[MAX_LINE_LENGTH];

    // Open the file for reading
    file = fopen("hello2_rand.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    char line_data[100]={0};
    char line_data2[100]={0};
    fgets(line_data, sizeof(line_data), file);

    printf("%s\n",line_data);

    // Read and process each line
    while (fgets(line_data2, sizeof(line_data2), file) != NULL) {
        // Use sscanf to parse the values from the line
        sscanf(line, "%s",line_data2);
        if(strcmp(line_data2,line_data)==0)
        {
            printf("same\n");
        }

        }


    // Close the file
    fclose(file);

    return 0;
}