
#include <stdio.h>      // For standard input and output functions
#include <string.h>     // For string manipulation functions
#include <stdlib.h>     // For general utility functions like memory allocation
#include <ctype.h>      // For character type functions (e.g., isspace)

// Define maximum limits for lines, shows, and colors
#define MAX_LINE_LENGTH 1024   // Maximum length of an input line
#define MAX_SHOWS 100          // Maximum number of shows per game
#define MAX_GAMES 1000         // Maximum number of games
#define MAX_COLOR_NAME 10      // Maximum length for color names

// Structure to hold cube counts for a single show
typedef struct {
    int red;        // Number of red cubes in the show
    int green;      // Number of green cubes in the show
    int blue;       // Number of blue cubes in the show
} Show;

// Structure to hold information for a single game
typedef struct {
    int id;                 // Game ID
    Show shows[MAX_SHOWS];  // Array of shows in the game
    int show_count;         // Number of shows in the game
} Game;

// Function to trim leading and trailing whitespace from a string
char* trim_whitespace(char* str) {
    char* end;

    // Trim leading whitespace by advancing the pointer
    while(isspace((unsigned char)*str)) str++;

    // If the string is empty after trimming, return it
    if(*str == 0)
        return str;

    // Set 'end' to point to the last character of the string
    end = str + strlen(str) - 1;

    // Trim trailing whitespace by moving 'end' backwards
    while(end > str && isspace((unsigned char)*end)) end--;

    // Place a null terminator after the last non-space character
    *(end+1) = '\0';

    return str;
}

// Function to parse a single show string and populate a Show structure
void parse_show(char* show_str, Show* show) {
    // Initialize all color counts to zero
    show->red = 0;
    show->green = 0;
    show->blue = 0;

    // Use strtok to split the show string by commas, extracting individual color counts
    char* token = strtok(show_str, ",");

    // Loop through each color count in the show
    while(token != NULL) {
        // Trim any leading/trailing whitespace from the token
        char* trimmed = trim_whitespace(token);

        // Variables to store the parsed count and color name
        int count;
        char color[MAX_COLOR_NAME];

        // Use sscanf to extract the integer count and the color name from the token
        sscanf(trimmed, "%d %s", &count, color);

        // Assign the count to the appropriate color in the Show structure
        if(strcmp(color, "red") == 0) {
            show->red = count;
        }
        else if(strcmp(color, "green") == 0) {
            show->green = count;
        }
        else if(strcmp(color, "blue") == 0) {
            show->blue = count;
        }
        // If the color is not recognized, you can handle it here (optional)

        // Move to the next token (color count)
        token = strtok(NULL, ",");
    }
}

int main() {
    // Open the input file 'day_two_input' in read mode
    FILE* file = fopen("day_two_input.txt", "r");
    if(file == NULL) {  // Check if the file was opened successfully
        printf("Error: Could not open file 'day_two_input'.\n");
        return 1;       // Exit the program with a non-zero status to indicate failure
    }

    char line[MAX_LINE_LENGTH];    // Buffer to store each line from the input file
    Game games[MAX_GAMES];         // Array to store all games
    int game_count = 0;            // Counter for the number of games parsed

    // Read the input file line by line until End of File (EOF) is reached
    while(fgets(line, sizeof(line), file)) {
        // Remove the newline character at the end of the line, if present
        line[strcspn(line, "\n")] = '\0';

        // Skip empty lines to avoid processing them
        if(strlen(line) == 0)
            continue;

        // Example line format:
        // "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green"

        // Find the position of the colon ':' which separates the game ID from its shows
        char* colon_ptr = strchr(line, ':');
        if(colon_ptr == NULL)
            continue;   // If no colon is found, the format is invalid; skip this line

        // Extract the substring containing the game ID (e.g., "Game 1")
        char game_id_str[20];  // Buffer to hold the game ID string
        strncpy(game_id_str, line, colon_ptr - line);  // Copy characters up to the colon
        game_id_str[colon_ptr - line] = '\0';          // Null-terminate the string

        int game_id;  // Variable to store the numerical game ID

        // Use sscanf to extract the integer game ID from the string (e.g., "Game 1" -> 1)
        sscanf(game_id_str, "Game %d", &game_id);

        // Initialize a new game in the games array
        games[game_count].id = game_id;    // Set the game ID
        games[game_count].show_count = 0;   // Initialize the show count to zero

        // The substring after the colon contains all shows, separated by semicolons ';'
        char* shows_str = colon_ptr + 1;    // Pointer to the first character after the colon

        // Use strtok to split the shows string by semicolons, extracting individual shows
        char* show_token = strtok(shows_str, ";");

        // Loop through each show in the current game
        while(show_token != NULL) {
            // Trim any leading/trailing whitespace from the show string
            char* trimmed_show = trim_whitespace(show_token);

            // Parse the show string and populate the corresponding Show structure
            parse_show(trimmed_show, &games[game_count].shows[games[game_count].show_count]);

            // Increment the show count for the current game
            games[game_count].show_count++;

            // Move to the next show in the string
            show_token = strtok(NULL, ";");
        }

        // After processing all shows, increment the game count
        game_count++;

        // Check if we've reached the maximum number of games
        if(game_count >= MAX_GAMES) {
            printf("Warning: Maximum number of games (%d) reached. Some games may not be processed.\n", MAX_GAMES);
            break;  // Exit the loop to prevent array overflow
        }
    }

    // Close the input file as we're done reading from it
    fclose(file);

    // Define the total number of cubes available in the bag for each color
    int total_red = 12;    // Total red cubes
    int total_green = 13;  // Total green cubes
    int total_blue = 14;   // Total blue cubes

    int sum_of_valid_game_ids = 0;  // Variable to accumulate the sum of valid game IDs

    // Iterate through each parsed game to check its validity
    for(int i = 0; i < game_count; i++) {
        int valid = 1;  // Flag to indicate if the current game is valid; assume valid initially

        // Iterate through each show within the current game
        for(int j = 0; j < games[i].show_count; j++) {
            Show current_show = games[i].shows[j];  // Get the current show

            // Check if any color count in the show exceeds the total available cubes
            if(current_show.red > total_red || current_show.green > total_green || current_show.blue > total_blue) {
                valid = 0;  // Mark the game as invalid
                break;      // No need to check further shows in this game
            }
        }

        // If the game is valid (all shows are within cube limits), add its ID to the sum
        if(valid) {
            sum_of_valid_game_ids += games[i].id;
        }
    }

    // Print the final result: the sum of IDs of all valid games
    printf("Sum of valid game IDs: %d\n", sum_of_valid_game_ids);

    return 0;  // Indicate successful program termination
}

