#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through each row of the image
    for (int i = 0; i < height; i++)
    {
        // Iterate through each pixel in the row
        for (int j = 0; j < width; j++)
        {
            // Calculate the average value of the red, green, and blue components of the pixel
            // The average value will give us a grayscale representation of the pixel
            int average = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            // Set the red, green, and blue components of the pixel to the calculated average
            // This will result in a grayscale version of the pixel
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    // Return the grayscale image
    return;
}


// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through each row of the image
    for (int i = 0; i < height; i++)
    {
        // Iterate through each pixel in the current row
        for (int j = 0; j < width; j++)
        {
            // Get the original RGB values of the pixel
            int originalRed = image[i][j].rgbtRed;
            int originalGreen = image[i][j].rgbtGreen;
            int originalBlue = image[i][j].rgbtBlue;

            // Apply the sepia transformation to the pixel's RGB values
            int sepiaRed = round(0.393 * originalRed + 0.769 * originalGreen + 0.189 * originalBlue);
            int sepiaGreen = round(0.349 * originalRed + 0.686 * originalGreen + 0.168 * originalBlue);
            int sepiaBlue = round(0.272 * originalRed + 0.534 * originalGreen + 0.131 * originalBlue);

            // Ensure that the sepia values are within the range of 0 to 255
            // Set the pixel's RGB values to the sepia values
            image[i][j].rgbtRed = fmin(255, sepiaRed);
            image[i][j].rgbtGreen = fmin(255, sepiaGreen);
            image[i][j].rgbtBlue = fmin(255, sepiaBlue);
        }
    }
    // Return the modified image with the sepia filter applied
    return;
}


// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through each row of the image
    for (int i = 0; i < height; i++)
    {
        // Iterate through the first half of the pixels (columns) of the image
        for (int j = 0; j < width / 2; j++)
        {
            // Create a temporary variable to store the pixel value
            RGBTRIPLE temp[height][width];

            // Store the pixel value of the left half in the temporary variable
            temp[i][j] = image[i][j];

            // Reflect the pixel value of the right half to the left half (horizontally)
            // Swap the pixel at [i][j] with the corresponding pixel on the other side, i.e., [i][width - (j + 1)]
            image[i][j] = image[i][width - (j + 1)];

            // Restore the original pixel value from the temporary variable to the right half
            image[i][width - (j + 1)] = temp[i][j];
        }
    }
    return;
}


// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a temporary image array to store the blurred pixels
    RGBTRIPLE temp[height][width];

    // Iterate through each pixel in the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Initialize variables to store the sum of RGB values and the number of neighboring pixels
            float sumBlue = 0;
            float sumGreen = 0;
            float sumRed = 0;
            float counter = 0;

            // Iterate through the surrounding pixels (3x3 grid) including the current pixel
            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    // Check if the surrounding pixel is outside the image boundary and skip it
                    if (i + x < 0 || i + x > height - 1)
                    {
                        continue;
                    }

                    if (j + y < 0 || j + y > width - 1)
                    {
                        continue;
                    }

                    // Accumulate the sum of RGB values from the surrounding pixels
                    sumBlue += image[i + x][j + y].rgbtBlue;
                    sumGreen += image[i + x][j + y].rgbtGreen;
                    sumRed += image[i + x][j + y].rgbtRed;
                    // Increment the counter for each valid surrounding pixel
                    counter++;
                }
            }

            // Calculate the average of RGB values from the surrounding pixels and round it
            temp[i][j].rgbtBlue = round(sumBlue / counter);
            temp[i][j].rgbtGreen = round(sumGreen / counter);
            temp[i][j].rgbtRed = round(sumRed / counter);
        }
    }

    // Copy the blurred pixels from the temporary array back to the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
        }
    }

    return;
}
