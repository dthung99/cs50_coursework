#include "helpers.h"
#include <math.h>

// Swap two pixels
void assign_pixel(RGBTRIPLE *target, RGBTRIPLE *source)
{
    target->rgbtBlue = source->rgbtBlue;
    target->rgbtGreen = source->rgbtGreen;
    target->rgbtRed = source->rgbtRed;
}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE byte;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            byte = (BYTE) round(
                ((float) image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3);
            image[i][j].rgbtBlue = byte;
            image[i][j].rgbtGreen = byte;
            image[i][j].rgbtRed = byte;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE byte;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            assign_pixel(&byte, &image[i][j]);
            assign_pixel(&image[i][j], &image[i][width - 1 - j]);
            assign_pixel(&image[i][width - 1 - j], &byte);
        }
    }
    return;
}

// Average the pixels from an array of pixels
RGBTRIPLE average_pixels(int count, RGBTRIPLE pixels[count])
{
    float rgbtBlue = 0, rgbtGreen = 0, rgbtRed = 0;
    for (int i = 0; i < count; i++)
    {
        rgbtBlue += pixels[i].rgbtBlue;
        rgbtGreen += pixels[i].rgbtGreen;
        rgbtRed += pixels[i].rgbtRed;
    }
    rgbtBlue = round(rgbtBlue / count);
    rgbtGreen = round(rgbtGreen / count);
    rgbtRed = round(rgbtRed / count);
    RGBTRIPLE result = {(BYTE) rgbtBlue, (BYTE) rgbtGreen, (BYTE) rgbtRed};
    return result;
}

// Copy a whole image
void copy_image(int height, int width, RGBTRIPLE target[height][width],
                RGBTRIPLE source[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            target[i][j] = source[i][j];
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copied_image[height][width];
    // Copy the original image
    copy_image(height, width, copied_image, image);
    // Go through all pixel in the image
    RGBTRIPLE pixel_array[9];
    RGBTRIPLE average;
    int array_index = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            array_index = 0;
            // Make an array from 9 pixels around i and j
            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    if (i + x >= 0 && j + y >= 0 && i + x < height && j + y < width)
                    {
                        assign_pixel(&pixel_array[array_index], &copied_image[i + x][j + y]);
                        array_index++;
                    }
                }
            }
            // Get the average the assign to the original image
            average = average_pixels(array_index, pixel_array);
            assign_pixel(&image[i][j], &average);
        }
    }
    return;
}

// Cap and round the float
float cap_float(float x)
{
    x = round(x);
    if (x < 0)
    {
        x = -x;
    }
    if (x > 255)
    {
        x = 255;
    }
    return x;
}
// Multiply pixels with an array and sum the results
RGBTRIPLE mul_sum_pixels(int height, int width, RGBTRIPLE pixels[height][width],
                         float filter[height][width])
{
    float rgbtBlue = 0, rgbtGreen = 0, rgbtRed = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            rgbtBlue += (float) pixels[i][j].rgbtBlue * filter[i][j];
            rgbtGreen += (float) pixels[i][j].rgbtGreen * filter[i][j];
            rgbtRed += (float) pixels[i][j].rgbtRed * filter[i][j];
        }
    }
    rgbtBlue = cap_float(rgbtBlue);
    rgbtGreen = cap_float(rgbtGreen);
    rgbtRed = cap_float(rgbtRed);
    RGBTRIPLE result = {(BYTE) rgbtBlue, (BYTE) rgbtGreen, (BYTE) rgbtRed};
    return result;
}

RGBTRIPLE cal_rms_pixel(RGBTRIPLE x, RGBTRIPLE y)
{
    float rgbtBlue = 0, rgbtGreen = 0, rgbtRed = 0;
    rgbtBlue = fmin(
        round(sqrt((((float) x.rgbtBlue) * x.rgbtBlue + ((float) y.rgbtBlue) * y.rgbtBlue))), 255);
    rgbtGreen = fmin(
        round(sqrt((((float) x.rgbtGreen) * x.rgbtGreen + ((float) y.rgbtGreen) * y.rgbtGreen))),
        255);
    rgbtRed =
        fmin(round(sqrt((((float) x.rgbtRed) * x.rgbtRed + ((float) y.rgbtRed) * y.rgbtRed))), 255);
    RGBTRIPLE result = {(BYTE) rgbtBlue, (BYTE) rgbtGreen, (BYTE) rgbtRed};
    return result;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create the sober filter
    float x_filter[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    float y_filter[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    RGBTRIPLE copied_image[height][width];
    // Copy the original image
    copy_image(height, width, copied_image, image);

    RGBTRIPLE pixel_array[3][3];
    RGBTRIPLE rms_pixel;
    RGBTRIPLE zero_pixel = {0, 0, 0};
    // Main loop
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Make an 3x3 array from 9 pixels around i and j
            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    if (i + x >= 0 && j + y >= 0 && i + x < height && j + y < width)
                    {
                        assign_pixel(&pixel_array[1 + x][1 + y], &copied_image[i + x][j + y]);
                    }
                    else
                    {
                        assign_pixel(&pixel_array[1 + x][1 + y], &zero_pixel);
                    }
                }
            }
            // Multiply with sober filter and calculate rms
            // mul_sum_pixels(3, 3, pixel_array, x_filter);
            // mul_sum_pixels(3, 3, pixel_array, y_filter);
            rms_pixel = cal_rms_pixel(mul_sum_pixels(3, 3, pixel_array, x_filter),
                                      mul_sum_pixels(3, 3, pixel_array, y_filter));
            // rms_pixel = mul_sum_pixels(3, 3, pixel_array, x_filter);
            // rms_pixel = mul_sum_pixels(3, 3, pixel_array, y_filter);
            assign_pixel(&image[i][j], &rms_pixel);
        }
    }
    return;
}
