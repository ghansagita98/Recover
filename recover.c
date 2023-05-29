#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>
#include <stdint.h>

const int BLOCK_SIZE = 512;

typedef struct JPEG_BLOCK
{
    uint8_t bytes[BLOCK_SIZE]; // contain aall bytes of a block in an array
} jpeg;

void make_filename(int i, string container);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        //check number of Command-Line argument
        printf("usage: ./recover <filename>\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");

    if (input == NULL)
    {
        //check if file is readable.
        printf("Cannot read file.\n");
        return 1;
    }

    jpeg buffer_jpeg;//buffer for reading jpeg blocks

    int onoff = 0;//determine whether program is writing to output or not
    int file_counter = 0;//file number counter
    FILE *output;
    string filename = malloc(8);


    while (fread(&buffer_jpeg, sizeof(uint8_t), BLOCK_SIZE, input) == BLOCK_SIZE)
    {
        if (buffer_jpeg.bytes[0] == 0xff && buffer_jpeg.bytes[1] == 0xd8 && buffer_jpeg.bytes[2] == 0xff
            && buffer_jpeg.bytes[3] >= 0xe0 && buffer_jpeg.bytes[3] <= 0xef && onoff == 0)//
        {
            //find the first jpeg file signature
            make_filename(file_counter, filename);
            output = fopen(filename, "w");
            fwrite(&buffer_jpeg, sizeof(uint8_t), BLOCK_SIZE, output);
            onoff = 1;
        }
        else if (onoff == 1)
        {
            //if the first jpeg file signature is found
            if (buffer_jpeg.bytes[0] == 0xff && buffer_jpeg.bytes[1] == 0xd8 && buffer_jpeg.bytes[2] == 0xff
                && buffer_jpeg.bytes[3] >= 0xe0 && buffer_jpeg.bytes[3] <= 0xef)
            {
                //find another jpeg file signature
                fclose(output);
                file_counter += 1;
                make_filename(file_counter, filename);
                output = fopen(filename, "w");
                fwrite(&buffer_jpeg, sizeof(uint8_t), BLOCK_SIZE, output);
            }
            else
            {
                //no submit50 cs50/problems/2023/x/recover
                fwrite(&buffer_jpeg, sizeof(uint8_t), BLOCK_SIZE, output);
            }
        }
    }
    fclose(output);
    fclose(input);
    free(filename);

    return 0;

}

void make_filename(int i, string container)
{
    //function to make file name
    if (i < 10)
    {
        //if i is 1 digit int
        char buffer[] = {'0', '0', i + '0', '.', 'j', 'p', 'g', '\0'};
        strcpy(container, buffer);

    }
    else if (i >= 100 && i < 1000)
    {
        //if i is 3 digit int
        int a = i / 100;
        int b = (i - (a * 100)) / 10;
        int c = (i - (a * 100) - (b * 10));
        char buffer[] = {a + '0', b + '0',  c + '0', '.', 'j', 'p', 'g', '\0'};
        strcpy(container, &buffer[0]);

    }
    else if (i < 100)
    {
        //if i is 2 digit int
        int a = i / 10;
        int b = i - (a * 10);
        char buffer[] = {'0', a + '0', b + '0', '.', 'j', 'p', 'g', '\0'};
        strcpy(container, &buffer[0]);
    }


}
