#include "fetch_passwords_task.h"
#include <ctype.h>

static const char* TAG = "FETCH_PASSWORDS_TASK";
static TaskHandle_t fetch_passwords_task_handle;

FILE* csvFile;

char** splitString(const char* str, const char* separator, int* numTokens) {
    // Count the number of tokens
    int count = 1;
    const char* ptr = str;
    while ((ptr = strstr(ptr, separator)) != NULL) {
        count++;
        ptr += strlen(separator);
    }

    // Allocate memory for array of strings
    char** tokens = (char**)malloc(count * sizeof(char*));
    if (tokens == NULL) {
        ESP_LOGE(TAG, "Memory allocation failed!");
        exit(EXIT_FAILURE);
    }

    // Split the string
    char* token = strtok((char*)str, separator);
    int i = 0;
    while (token != NULL) {
        tokens[i] = strdup(token);
        if (tokens[i] == NULL) {
            ESP_LOGE(TAG, "Memory allocation failed!");
            exit(EXIT_FAILURE);
        }
        i++;
        token = strtok(NULL, separator);
    }

    *numTokens = count;
    return tokens;
}

char* trim(char* str) {
    char* end;

    // Trim leading whitespace
    while (isspace((unsigned char)*str)) {
        str++;
    }

    if (*str == '\0') { // If string contains only whitespace
        return str;
    }

    // Trim trailing whitespace
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }

    // Null terminate the trimmed string
    *(end + 1) = '\0';

    return str;
}

char* concatStrings(const char* str1, const char* str2) {
    // Calculate the total length of the concatenated string
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t totalLen = len1 + len2 + 1; // +1 for the null terminator

    // Allocate memory for the concatenated string
    char* result = (char*)malloc(totalLen);
    if (result == NULL) {
        ESP_LOGE(TAG, "Memory allocation failed!");
        exit(EXIT_FAILURE);
    }

    // Copy the first string into the result
    strcpy(result, str1);

    // Concatenate the second string onto the end of the result
    strcat(result, str2);

    return result;
}

void writeToFile(char* path, char* str) {
    FILE* file = fopen(path, "w");
    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to create file: %s.", path);
        return;
    }

    fprintf(file, "%s", str);
    fclose(file);

    ESP_LOGI(TAG, "File \"%s\" created successfully.", path);
}

void writeToBinaryFile(const char* path, const uint8_t* data, size_t size) {
    // Open the file in binary write mode
    FILE* file = fopen(path, "wb");
    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to open file");
        exit(-1);
    }

    // Write the data to the file
    size_t bytes_written = fwrite(data, sizeof(uint8_t), size, file);
    if (bytes_written != size) {
        ESP_LOGE(TAG, "Failed to write data to file");
        fclose(file);
        exit(-1); // Return an error code
    }

    // Close the file
    fclose(file);
}

void writeIntToBinaryFile(const char* path, const int data) {
    // Open the file in binary write mode
    FILE* file = fopen(path, "wb");
    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to open file");
        exit(-1);
    }

    // Write the data to the file
    size_t bytes_written = fwrite(&data, sizeof(int), 1, file);
    if (bytes_written != 1) {
        ESP_LOGE(TAG, "Failed to write data to file");
        fclose(file);
        exit(-1); // Return an error code
    }

    // Close the file
    fclose(file);
}

void readFromFile(char* path) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to create file: %s.", path);
        return;
    }

    char str[100];
    fgets(str, 100, file);

    ESP_LOGI(TAG, "Inside file: %s", str);

    fclose(file);
}

size_t readFromBinaryFile(const char* path, uint8_t* output, size_t max_size) {
    // Open the file in binary read mode
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to open file");
        return 0; // Return 0 to indicate failure
    }

    // Read the data from the file into the output array
    size_t bytes_read = fread(output, sizeof(uint8_t), max_size, file);

    // Close the file
    fclose(file);

    return bytes_read; // Return the number of bytes read
}

void readIntFromBinaryFile(const char* path, int* output) {
    // Open the file in binary read mode
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to open file");
        exit(-1);
    }

    // Read the data from the file into the output array
    fread(output, sizeof(int), 1, file);

    // Close the files
    fclose(file);
}

void fetch_passwords_task() {

    char* line = (char*)malloc(1024);
    fgets(line, 1024, csvFile);

    while (fgets(line, 1024, csvFile)) {
        ESP_LOGI(TAG, "%s", line);
        int numItems;
        char** items = splitString(line, ",", &numItems);

        for (int i = 0; i < numItems; i++) {
            items[i] = trim(items[i]);
        }

        char* name = items[0];
        char* url = items[1];
        char* userName = items[2];
        char* passwd = items[3];

        char* namePath = concatStrings("/spiffs/", name);
        namePath = concatStrings(namePath, "/");

        char* userNamePath = concatStrings(namePath, userName);
        userNamePath = concatStrings(userNamePath, "/");

        char* urlPath = concatStrings(userNamePath, "URL");
        writeToFile(urlPath, url);

        char* passwdPath = concatStrings(userNamePath, "Password");
        writeToFile(passwdPath, passwd);
    }
    free(line);
    finish_fetch_passwords_task();
}

void start_fetch_passwords_task() {
    ESP_LOGI(TAG, "Reading passwords.csv");

    csvFile = fopen(USB_MSC_PASSWD_FILE_PATH, "r");
    if (csvFile == NULL) {
        ESP_LOGE(TAG, "Failed to open passwords.csv");
        return;
    }

    xTaskCreatePinnedToCore(fetch_passwords_task, "FETCH_PASSWORDS",
                            FETCH_PASSWORDS_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY,
                            &fetch_passwords_task_handle, 0);
}

void finish_fetch_passwords_task() {
    fclose(csvFile);
    ESP_LOGI(TAG, "Fetch password complate");

    vTaskDelete(fetch_passwords_task_handle);
}