#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>  
//
int extract_value(char *line) {
    int first_digit = -1;
    int last_digit = -1;

    for (int i = 0; line[i] != '\0'; i++) {
        if (isdigit(line[i])) {
            if (first_digit == -1) {
                first_digit = line[i] - '0';  
            }
            last_digit = line[i] - '0';  
        }
    }

    if (first_digit != -1 && last_digit != -1) {
        return first_digit * 10 + last_digit;
    }
    return 0; }

int main() {
    FILE *file = fopen("day_one_input.txt", "r");
    if (!file) {
        printf("Error opening file!\n");
        return 1;
    }

    char line[100];  
    int total_sum = 0;

    while (fgets(line, sizeof(line), file)) {
        total_sum += extract_value(line);
    }

    fclose(file);

    printf("The sum of the calibration values is: %d\n", total_sum);
    return 0;
}
