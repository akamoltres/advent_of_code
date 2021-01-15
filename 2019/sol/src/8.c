
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int solve_2019_8_1(char const *input_filename, int width, int height)
{
    const int bufsize = 20000;
    char buffer[bufsize];
    memset(buffer, 0, bufsize * sizeof(char));

    FILE *fp = fopen(input_filename, "r");
    assert(fp != NULL);
    fscanf(fp, "%s\n", buffer);
    fclose(fp);

    int input_length = strlen(buffer);
    int pixels = width * height;
    assert(input_length % pixels == 0);

    int min_zeros = input_length;
    int product = -1;

    for(int layer = 0; layer < input_length / pixels; ++layer)
    {
        int zero_count = 0;
        int one_count = 0;
        int two_count = 0;

        for(int i = 0; i < pixels; ++i)
        {
            zero_count += (buffer[layer * pixels + i] == '0');
            one_count += (buffer[layer * pixels + i] == '1');
            two_count += (buffer[layer * pixels + i] == '2');
        }

        if(zero_count < min_zeros)
        {
            min_zeros = zero_count;
            product = one_count * two_count;
        }
    }

    return product;
}

char *solve_2019_8_2(char const *input_filename, const int width, const int height)
{
    const int bufsize = 20000;
    char buffer[bufsize];
    memset(buffer, 0, bufsize * sizeof(char));

    char *image;
    image = (char *) malloc((width * height + 1) * sizeof(char));
    memset(image, 0, (width * height + 1) * sizeof(char));

    FILE *fp = fopen(input_filename, "r");
    assert(fp != NULL);
    fscanf(fp, "%s\n", buffer);
    fclose(fp);

    int input_length = strlen(buffer);
    int pixels = width * height;
    assert(input_length % pixels == 0);

    for(int layer = 0; layer < input_length / pixels; ++layer)
    {
        if(layer == 0)
        {
            memcpy(image, buffer, pixels * sizeof(char));
        }
        else
        {
            for(int i = 0; i < pixels; ++i)
            {
                if(image[i] == '2')
                {
                    image[i] = buffer[layer * pixels + i];
                }
            }
        }
    }

    return image;
}
