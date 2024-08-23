#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 512

void update_file_name(char *file_name, int counter);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./recover file\n");
        return 1;
    }
    // Open the file
    char *infile = argv[1];
    FILE *file = fopen(infile, "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 1;
    }

    // Declare the buffer
    uint8_t buffer[BLOCK_SIZE];
    // Loop through the file to check
    bool condition = false;
    int counter = 0;
    char output_name[8];
    int len;      // Use for checking error
    int len_read; // Use for contiue inner loop
    FILE *output;
    while (fread(&buffer, BLOCK_SIZE, 1, file) == 1)
    {
        // Check the start of the block is jpeg
        condition = buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
                    buffer[3] >= 0xe0 && buffer[3] <= 0xef;
        if (!condition)
        {
            continue;
        }
        bool newfile = true;
        // Stuff to do for first loop
        output = fopen("000.jpg", "w");
        condition = false;
        do
        {
            // Write to the new file
            len = fwrite(&buffer, BLOCK_SIZE, 1, output);
            if (len != 1)
            {
                printf("Error: writting to new file\n");
                return 1;
            }
            // Read new block
            condition = false;
            len_read = fread(&buffer, BLOCK_SIZE, 1, file);
            // if (len != 1)
            // {
            //     printf("Error: reading source file\n");
            //     return 1;
            // }
            condition = buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
                        buffer[3] >= 0xe0 && buffer[3] <= 0xef;

            if (condition)
            {
                fclose(output);
                // Create a new name
                counter++;
                len = snprintf(output_name, sizeof(output_name), "%0*i.jpg", 3, counter);
                if (len < 0 || len >= sizeof(output_name))
                {
                    printf("Error: string truncation occurred.\n");
                    return 1;
                }
                // Open that file
                output = fopen(output_name, "w");
                if (output == NULL)
                {
                    printf("Could not open %s.\n", output_name);
                    return 1;
                }
                printf("I found %s\n", output_name);
            }
        }
        while (len_read == 1);
        fclose(output);
    }
    fclose(file);
}

// void update_file_name(char *file_name, int counter)
// {
// sprintf(file_name, "00%
// }

// #include <stdio.h>

// int main() {
//     char byte = 0xAB;
//     char firstFourBits = (byte & 0xF0) >> 4;

//     printf("Original byte: 0x%02X\n", (unsigned char) byte);
//     printf("First 4 bits: 0x%01X\n", firstFourBits);

//     return 0;
// }
