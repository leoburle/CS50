#include "helpers.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int avg = 0;

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Calculates the average to each pixel in each chanel
            avg = round((image[h][w].rgbtRed + image[h][w].rgbtGreen + image[h][w].rgbtBlue) / 3.0);
            image[h][w].rgbtRed = avg;
            image[h][w].rgbtGreen = avg;
            image[h][w].rgbtBlue = avg;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a auxiliar RGBTRIPLE
    RGBTRIPLE image_aux[height][width];
    image_aux[height][width] = image[height][width];

    // Copies the value of the original image to the auxiliar
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image_aux[h][w] = image[h][w];
        }
    }

    // Reflect the original image using the auxiliar
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = image_aux[h][width - w - 1];
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a auxiliar RGBTRIPLE
    RGBTRIPLE image_aux[height][width];
    image_aux[height][width] = image[height][width];

    bool isUpperEdge = false;
    bool isBottomEdge = false;
    bool isLeftEdge = false;
    bool isRightEdge = false;
    bool isMiddle = false;

    int avg = 0;
    // Creates an blured auxiliar image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            isUpperEdge = false;
            isBottomEdge = false;
            isLeftEdge = false;
            isRightEdge = false;
            // Checks in which group the each pixel is
            if (h == 0)
            {
                isUpperEdge = true;
                if (w == 0)
                {
                    isLeftEdge = true;
                }
                else if (w == width - 1)
                {
                    isRightEdge = true;
                }
            }
            else if (h == height - 1)
            {
                isBottomEdge = true;
                if (w == 0)
                {
                    isLeftEdge = true;
                }
                else if (w == width - 1)
                {
                    isRightEdge = true;
                }
            }
            else if (w == 0)
            {
                isLeftEdge = true;
            }
            else if (w == width - 1)
            {
                isRightEdge = true;
            }

            // Aply blur according to the groups
            // Pixel on first collumn and first row
            if (isUpperEdge && isLeftEdge)
            {
                image_aux[h][w].rgbtRed = round((image[h + 1][w + 1].rgbtRed + image[h + 1][w].rgbtRed + image[h][w + 1].rgbtRed +
                                                 image[h][w].rgbtRed) / 4.0);

                image_aux[h][w].rgbtGreen = round((image[h + 1][w + 1].rgbtGreen + image[h + 1][w].rgbtGreen + image[h][w + 1].rgbtGreen +
                                                   image[h][w].rgbtGreen) / 4.0);
                image_aux[h][w].rgbtBlue = round((image[h + 1][w + 1].rgbtBlue + image[h + 1][w].rgbtBlue + image[h][w + 1].rgbtBlue +
                                                  image[h][w].rgbtBlue) / 4.0);
                continue;
            }
            // Pixel on first collumn and last row
            else if (isUpperEdge && isRightEdge)
            {
                image_aux[h][w].rgbtRed = round((image[h + 1][w - 1].rgbtRed + image[h + 1][w].rgbtRed + image[h][w - 1].rgbtRed +
                                                 image[h][w].rgbtRed) / 4.0);
                image_aux[h][w].rgbtBlue = round((image[h + 1][w - 1].rgbtBlue + image[h + 1][w].rgbtBlue + image[h][w - 1].rgbtBlue +
                                                  image[h][w].rgbtBlue) / 4.0);
                image_aux[h][w].rgbtGreen = round((image[h + 1][w - 1].rgbtGreen + image[h + 1][w].rgbtGreen + image[h][w - 1].rgbtGreen +
                                                   image[h][w].rgbtGreen) / 4.0);
                continue;
            }
            // Pixel on last collumn and first row
            else if (isBottomEdge && isLeftEdge)
            {
                image_aux[h][w].rgbtRed = round((image[h - 1][w + 1].rgbtRed + image[h - 1][w].rgbtRed + image[h][w + 1].rgbtRed +
                                                 image[h][w].rgbtRed) / 4.0);
                image_aux[h][w].rgbtBlue = round((image[h - 1][w + 1].rgbtBlue + image[h - 1][w].rgbtBlue + image[h][w + 1].rgbtBlue +
                                                  image[h][w].rgbtBlue) / 4.0);
                image_aux[h][w].rgbtGreen = round((image[h - 1][w + 1].rgbtGreen + image[h - 1][w].rgbtGreen + image[h][w + 1].rgbtGreen +
                                                   image[h][w].rgbtGreen) / 4.0);
                continue;
            }
            // Pixel on last collumn and last row
            else if (isBottomEdge && isRightEdge)
            {
                image_aux[h][w].rgbtRed = round((image[h - 1][w - 1].rgbtRed + image[h - 1][w].rgbtRed + image[h][w - 1].rgbtRed +
                                                 image[h][w].rgbtRed)  / 4.0);
                image_aux[h][w].rgbtBlue = round((image[h - 1][w - 1].rgbtBlue + image[h - 1][w].rgbtBlue + image[h][w - 1].rgbtBlue +
                                                  image[h][w].rgbtBlue) / 4.0);
                image_aux[h][w].rgbtGreen = round((image[h - 1][w - 1].rgbtGreen + image[h - 1][w].rgbtGreen + image[h][w - 1].rgbtGreen +
                                                   image[h][w].rgbtGreen) / 4.0);
                continue;
            }
            // Pixels on first row
            else if (isUpperEdge)
            {
                image_aux[h][w].rgbtRed = round((image[h + 1][w + 1].rgbtRed + image[h + 1][w].rgbtRed + image[h + 1][w - 1].rgbtRed + image[h][w
                                                 + 1].rgbtRed + image[h][w].rgbtRed + image[h][w - 1].rgbtRed) / 6.0);
                image_aux[h][w].rgbtBlue = round((image[h + 1][w + 1].rgbtBlue + image[h + 1][w].rgbtBlue + image[h + 1][w - 1].rgbtBlue +
                                                  image[h][w + 1].rgbtBlue + image[h][w].rgbtBlue + image[h][w - 1].rgbtBlue) / 6.0);
                image_aux[h][w].rgbtGreen = round((image[h + 1][w + 1].rgbtGreen + image[h + 1][w].rgbtGreen + image[h + 1][w - 1].rgbtGreen +
                                                   image[h][w + 1].rgbtGreen + image[h][w].rgbtGreen + image[h][w - 1].rgbtGreen) / 6.0);
                continue;
            }
            // Pixels on last row
            else if (isBottomEdge)
            {
                image_aux[h][w].rgbtRed = round((image[h - 1][w + 1].rgbtRed + image[h - 1][w].rgbtRed + image[h - 1][w - 1].rgbtRed + image[h][w
                                                 + 1].rgbtRed + image[h][w].rgbtRed + image[h][w - 1].rgbtRed) / 6.0);
                image_aux[h][w].rgbtBlue = round((image[h - 1][w + 1].rgbtBlue + image[h - 1][w].rgbtBlue + image[h - 1][w - 1].rgbtBlue +
                                                  image[h][w + 1].rgbtBlue + image[h][w].rgbtBlue + image[h][w - 1].rgbtBlue) / 6.0);
                image_aux[h][w].rgbtGreen = round((image[h - 1][w + 1].rgbtGreen + image[h - 1][w].rgbtGreen + image[h - 1][w - 1].rgbtGreen +
                                                   image[h][w + 1].rgbtGreen + image[h][w].rgbtGreen + image[h][w - 1].rgbtGreen) / 6.0);
                continue;
            }
            // Pixels on first collumn
            else if (isLeftEdge)
            {
                image_aux[h][w].rgbtRed = round((image[h][w].rgbtRed + image[h][w + 1].rgbtRed + image[h + 1][w + 1].rgbtRed + image[h +
                                                 1][w].rgbtRed + image[h - 1][w].rgbtRed + image[h - 1][w + 1].rgbtRed) / 6.0);
                image_aux[h][w].rgbtBlue = round((image[h][w].rgbtBlue + image[h][w + 1].rgbtBlue + image[h + 1][w + 1].rgbtBlue + image[h +
                                                  1][w].rgbtBlue + image[h - 1][w].rgbtBlue + image[h - 1][w + 1].rgbtBlue) / 6.0);
                image_aux[h][w].rgbtGreen = round((image[h][w].rgbtGreen + image[h][w + 1].rgbtGreen + image[h + 1][w + 1].rgbtGreen + image[h +
                                                   1][w].rgbtGreen + image[h - 1][w].rgbtGreen + image[h - 1][w + 1].rgbtGreen) / 6.0);
                continue;
            }
            // Pixels on last collumn
            else if (isRightEdge)
            {
                image_aux[h][w].rgbtRed = round((image[h][w].rgbtRed + image[h][w - 1].rgbtRed + image[h + 1][w - 1].rgbtRed + image[h +
                                                 1][w].rgbtRed + image[h - 1][w].rgbtRed + image[h - 1][w - 1].rgbtRed) / 6.0);
                image_aux[h][w].rgbtBlue = round((image[h][w].rgbtBlue + image[h][w - 1].rgbtBlue + image[h + 1][w - 1].rgbtBlue + image[h +
                                                  1][w].rgbtBlue + image[h - 1][w].rgbtBlue + image[h - 1][w - 1].rgbtBlue) / 6.0);
                image_aux[h][w].rgbtGreen = round((image[h][w].rgbtGreen + image[h][w - 1].rgbtGreen + image[h + 1][w - 1].rgbtGreen + image[h +
                                                   1][w].rgbtGreen + image[h - 1][w].rgbtGreen + image[h - 1][w - 1].rgbtGreen) / 6.0);
                continue;
            }
            // Middle pixels
            else
            {
                image_aux[h][w].rgbtRed = round((image[h][w].rgbtRed + image[h][w + 1].rgbtRed + image[h][w - 1].rgbtRed + image[h + 1][w].rgbtRed +
                                                 image[h + 1][w + 1].rgbtRed + image[h + 1][w - 1].rgbtRed + image[h - 1][w].rgbtRed +
                                                 image[h - 1][w + 1].rgbtRed + image[h - 1][w - 1].rgbtRed) / 9.0);
                image_aux[h][w].rgbtBlue = round((image[h][w].rgbtBlue + image[h][w + 1].rgbtBlue + image[h][w - 1].rgbtBlue +
                                                  image[h + 1][w].rgbtBlue + image[h + 1][w + 1].rgbtBlue + image[h + 1][w - 1].rgbtBlue
                                                  + image[h - 1][w].rgbtBlue + image[h - 1][w + 1].rgbtBlue +
                                                  image[h - 1][w - 1].rgbtBlue) / 9.0);
                image_aux[h][w].rgbtGreen = round((image[h][w].rgbtGreen + image[h][w + 1].rgbtGreen + image[h][w - 1].rgbtGreen +
                                                   image[h + 1][w].rgbtGreen + image[h + 1][w + 1].rgbtGreen + image[h + 1][w - 1].rgbtGreen +
                                                   image[h - 1][w].rgbtGreen + image[h - 1][w + 1].rgbtGreen + image[h - 1][w - 1].rgbtGreen) / 9.0);

            }
        }
    }
    // Copies the auxiliar blured image to the original
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = image_aux[h][w];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a auxiliar RGBTRIPLE
    RGBTRIPLE image_aux[height][width];
    image_aux[height][width] = image[height][width];

    bool isUpperEdge = false;
    bool isBottomEdge = false;
    bool isLeftEdge = false;
    bool isRightEdge = false;
    bool isMiddle = false;
    int Gx = 0;
    int Gy = 0;
    int value;

    int avg = 0;
    // Creates an auxiliar image with edges detected
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            isUpperEdge = false;
            isBottomEdge = false;
            isLeftEdge = false;
            isRightEdge = false;
            Gx = 0;
            Gy = 0;
            value = 0;
            // Checks in which group the each pixel is
            if (h == 0)
            {
                isUpperEdge = true;
                if (w == 0)
                {
                    isLeftEdge = true;
                }
                else if (w == width - 1)
                {
                    isRightEdge = true;
                }
            }
            else if (h == height - 1)
            {
                isBottomEdge = true;
                if (w == 0)
                {
                    isLeftEdge = true;
                }
                else if (w == width - 1)
                {
                    isRightEdge = true;
                }
            }
            else if (w == 0)
            {
                isLeftEdge = true;
            }
            else if (w == width - 1)
            {
                isRightEdge = true;
            }

            // Aply Sobel operator according to the groups
            // Pixel on first collumn and first row
            if (isUpperEdge && isLeftEdge)
            {
                // Calculates Gx
                Gx = round(2 * image[h][w + 1].rgbtRed + image[h + 1][w + 1].rgbtRed);
                // Calculates Gy
                Gy = round(2 * image[h + 1][w].rgbtRed + image[h + 1][w + 1].rgbtRed);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtRed = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtRed = 0;
                }
                else
                {
                    image_aux[h][w].rgbtRed = value;
                }
                // Calculates Gx
                Gx = round(2 * image[h][w + 1].rgbtBlue + image[h + 1][w + 1].rgbtBlue);
                // Calculates Gy
                Gy = round(2 * image[h + 1][w].rgbtBlue + image[h + 1][w + 1].rgbtBlue);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtBlue = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtBlue = 0;
                }
                else
                {
                    image_aux[h][w].rgbtBlue = value;
                }
                // Calculates Gx
                Gx = round(2 * image[h][w + 1].rgbtGreen + image[h + 1][w + 1].rgbtGreen);
                // Calculates Gy
                Gy = round(2 * image[h + 1][w].rgbtGreen + image[h + 1][w + 1].rgbtGreen);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtGreen = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtGreen = 0;
                }
                else
                {
                    image_aux[h][w].rgbtGreen = value;
                }
            }
            // Pixel on first collumn and last row
            else if (isUpperEdge && isRightEdge)
            {
                // Calculates Gx
                Gx = round(- 2 * image[h][w - 1].rgbtRed - 1 * image[h + 1][w - 1].rgbtRed);
                // Calculates Gy
                Gy = round(image[h + 1][w - 1].rgbtRed + 2 * image[h + 1][w].rgbtRed);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtRed = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtRed = 0;
                }
                else
                {
                    image_aux[h][w].rgbtRed = value;
                }
                // Calculates Gx
                Gx = round(- 2 * image[h][w - 1].rgbtBlue - 1 * image[h + 1][w - 1].rgbtBlue);
                // Calculates Gy
                Gy = round(image[h + 1][w - 1].rgbtBlue + 2 * image[h + 1][w].rgbtBlue);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtBlue = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtBlue = 0;
                }
                else
                {
                    image_aux[h][w].rgbtBlue = value;
                }

                // Calculates Gx
                Gx = round(- 2 * image[h][w - 1].rgbtGreen - 1 * image[h + 1][w - 1].rgbtGreen);
                // Calculates Gy
                Gy = round(image[h + 1][w - 1].rgbtGreen + 2 * image[h + 1][w].rgbtGreen);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtGreen = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtGreen = 0;
                }
                else
                {
                    image_aux[h][w].rgbtGreen = value;
                }
            }
            // Pixel on last collumn and first row
            else if (isBottomEdge && isLeftEdge)
            {
                // Calculates Gx
                Gx = round(image[h - 1][w + 1].rgbtRed + 2 * image[h][w + 1].rgbtRed);
                // Calculates Gy
                Gy = round(- 2 * image[h - 1][w].rgbtRed - image[h - 1][w + 1].rgbtRed);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtRed = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtRed = 0;
                }
                else
                {
                    image_aux[h][w].rgbtRed = value;
                }
                // Calculates Gx
                Gx = round(image[h - 1][w + 1].rgbtBlue + 2 * image[h][w + 1].rgbtBlue);
                // Calculates Gy
                Gy = round(- 2 * image[h - 1][w].rgbtBlue - image[h - 1][w + 1].rgbtBlue);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtBlue = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtBlue = 0;
                }
                else
                {
                    image_aux[h][w].rgbtBlue = value;
                }
                // Calculates Gx
                Gx = round(image[h - 1][w + 1].rgbtGreen + 2 * image[h][w + 1].rgbtGreen);
                // Calculates Gy
                Gy = round(- 2 * image[h - 1][w].rgbtGreen - image[h - 1][w + 1].rgbtGreen);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtGreen = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtGreen = 0;
                }
                else
                {
                    image_aux[h][w].rgbtGreen = value;
                }
            }
            // Pixel on last collumn and last row
            else if (isBottomEdge && isRightEdge)
            {
                // Calculates Gx
                Gx = round(- 1 * image[h - 1][w - 1].rgbtRed - 2 * image[h][w - 1].rgbtRed);
                // Calculates Gy
                Gy = round(- 1 * image[h - 1][w - 1].rgbtRed - 2 * image[h - 1][w].rgbtRed);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtRed = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtRed = 0;
                }
                else
                {
                    image_aux[h][w].rgbtRed = value;
                }
                // Calculates Gx
                Gx = round(- 1 * image[h - 1][w - 1].rgbtBlue - 2 * image[h][w - 1].rgbtBlue);
                // Calculates Gy
                Gy = round(- 1 * image[h - 1][w - 1].rgbtBlue - 2 * image[h - 1][w].rgbtBlue);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtBlue = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtBlue = 0;
                }
                else
                {
                    image_aux[h][w].rgbtBlue = value;
                }
                // Calculates Gx
                Gx = round(- 1 * image[h - 1][w - 1].rgbtGreen - 2 * image[h][w - 1].rgbtGreen);
                // Calculates Gy
                Gy = round(- 1 * image[h - 1][w - 1].rgbtGreen - 2 * image[h - 1][w].rgbtGreen);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtGreen = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtGreen = 0;
                }
                else
                {
                    image_aux[h][w].rgbtGreen = value;
                }
            }
            // Pixels on first row
            else if (isUpperEdge)
            {
                // Calculates Gx
                Gx = round(- 2 * image[h][w - 1].rgbtRed + 2 * image[h][w + 1].rgbtRed - 1 * image[h + 1][w - 1].rgbtRed + image[h + 1][w +
                           1].rgbtRed);
                // Calculates Gy
                Gy = round(image[h + 1][w - 1].rgbtRed + 2 * image[h + 1][w].rgbtRed + image[h + 1][w + 1].rgbtRed);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtRed = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtRed = 0;
                }
                else
                {
                    image_aux[h][w].rgbtRed = value;
                }
                // Calculates Gx
                Gx = round(- 2 * image[h][w - 1].rgbtBlue + 2 * image[h][w + 1].rgbtBlue - 1 * image[h + 1][w - 1].rgbtBlue + image[h + 1][w +
                           1].rgbtBlue);
                // Calculates Gy
                Gy = round(image[h + 1][w - 1].rgbtBlue + 2 * image[h + 1][w].rgbtBlue + image[h + 1][w + 1].rgbtBlue);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtBlue = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtBlue = 0;
                }
                else
                {
                    image_aux[h][w].rgbtBlue = value;
                }

                // Calculates Gx
                Gx = round(- 2 * image[h][w - 1].rgbtGreen + 2 * image[h][w + 1].rgbtGreen - 1 * image[h + 1][w - 1].rgbtGreen + image[h + 1][w +
                           1].rgbtGreen);
                // Calculates Gy
                Gy = round(image[h + 1][w - 1].rgbtGreen + 2 * image[h + 1][w].rgbtGreen + image[h + 1][w + 1].rgbtGreen);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtGreen = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtGreen = 0;
                }
                else
                {
                    image_aux[h][w].rgbtGreen = value;
                }
            }
            // Pixels on last row
            else if (isBottomEdge)
            {
                // Calculates Gx
                Gx = round(- 1 * image[h - 1][w - 1].rgbtRed + image[h - 1][w + 1].rgbtRed - 2 * image[h][w - 1].rgbtRed + 2 * image[h][w +
                           1].rgbtRed);
                // Calculates Gy
                Gy = round(- 1 * image[h - 1][w - 1].rgbtRed - 2 * image[h - 1][w].rgbtRed - image[h - 1][w + 1].rgbtRed);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtRed = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtRed = 0;
                }
                else
                {
                    image_aux[h][w].rgbtRed = value;
                }
                // Calculates Gx
                Gx = round(- 1 * image[h - 1][w - 1].rgbtBlue + image[h - 1][w + 1].rgbtBlue - 2 * image[h][w - 1].rgbtBlue + 2 * image[h][w +
                           1].rgbtBlue);
                // Calculates Gy
                Gy = round(- 1 * image[h - 1][w - 1].rgbtBlue - 2 * image[h - 1][w].rgbtBlue - image[h - 1][w + 1].rgbtBlue);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtBlue = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtBlue = 0;
                }
                else
                {
                    image_aux[h][w].rgbtBlue = value;
                }
                // Calculates Gx
                Gx = round(- 1 * image[h - 1][w - 1].rgbtGreen + image[h - 1][w + 1].rgbtGreen - 2 * image[h][w - 1].rgbtGreen + 2 * image[h][w +
                           1].rgbtGreen);
                // Calculates Gy
                Gy = round(- 1 * image[h - 1][w - 1].rgbtGreen - 2 * image[h - 1][w].rgbtGreen - image[h - 1][w + 1].rgbtGreen);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtGreen = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtGreen = 0;
                }
                else
                {
                    image_aux[h][w].rgbtGreen = value;
                }
            }
            // Pixels on first collumn
            else if (isLeftEdge)
            {
                // Calculates Gx
                Gx = round(image[h - 1][w + 1].rgbtRed + 2 * image[h][w + 1].rgbtRed + image[h + 1][w + 1].rgbtRed);
                // Calculates Gy
                Gy = round(- 2 * image[h - 1][w].rgbtRed - image[h - 1][w + 1].rgbtRed + 2 * image[h + 1][w].rgbtRed + image[h + 1][w +
                           1].rgbtRed);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtRed = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtRed = 0;
                }
                else
                {
                    image_aux[h][w].rgbtRed = value;
                }
                // Calculates Gx
                Gx = round(image[h - 1][w + 1].rgbtBlue + 2 * image[h][w + 1].rgbtBlue + image[h + 1][w + 1].rgbtBlue);
                // Calculates Gy
                Gy = round(- 2 * image[h - 1][w].rgbtBlue - image[h - 1][w + 1].rgbtBlue + 2 * image[h + 1][w].rgbtBlue + image[h + 1][w +
                           1].rgbtBlue);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtBlue = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtBlue = 0;
                }
                else
                {
                    image_aux[h][w].rgbtBlue = value;
                }

                // Calculates Gx
                Gx = round(image[h - 1][w + 1].rgbtGreen + 2 * image[h][w + 1].rgbtGreen + image[h + 1][w + 1].rgbtGreen);
                // Calculates Gy
                Gy = round(- 2 * image[h - 1][w].rgbtGreen - image[h - 1][w + 1].rgbtGreen + 2 * image[h + 1][w].rgbtGreen + image[h + 1][w +
                           1].rgbtGreen);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtGreen = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtGreen = 0;
                }
                else
                {
                    image_aux[h][w].rgbtGreen = value;
                }
            }
            // Pixels on last collumn
            else if (isRightEdge)
            {
                // Calculates Gx
                Gx = round(- 1 * image[h - 1][w - 1].rgbtRed - 2 * image[h][w - 1].rgbtRed  - 1 * image[h + 1][w - 1].rgbtRed);
                // Calculates Gy
                Gy = round(- 1 * image[h - 1][w - 1].rgbtRed - 2 * image[h - 1][w].rgbtRed + image[h + 1][w - 1].rgbtRed + 2 * image[h +
                           1][w].rgbtRed);
                // Calculates de module of the gradient Vector
                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtRed = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtRed = 0;
                }
                else
                {
                    image_aux[h][w].rgbtRed = value;
                }
                // Calculates Gx
                Gx = round(- 1 * image[h - 1][w - 1].rgbtBlue - 2 * image[h][w - 1].rgbtBlue  - 1 * image[h + 1][w - 1].rgbtBlue);
                // Calculates Gy
                Gy = round(- 1 * image[h - 1][w - 1].rgbtBlue - 2 * image[h - 1][w].rgbtBlue + image[h + 1][w - 1].rgbtBlue + 2 * image[h +
                           1][w].rgbtBlue);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtBlue = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtBlue = 0;
                }
                else
                {
                    image_aux[h][w].rgbtBlue = value;
                }

                // Calculates Gx
                Gx = round(- 1 * image[h - 1][w - 1].rgbtGreen - 2 * image[h][w - 1].rgbtGreen  - 1 * image[h + 1][w - 1].rgbtGreen);
                // Calculates Gy
                Gy = round(- 1 * image[h - 1][w - 1].rgbtGreen - 2 * image[h - 1][w].rgbtGreen + image[h + 1][w - 1].rgbtGreen + 2 * image[h +
                           1][w].rgbtGreen);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtGreen = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtGreen = 0;
                }
                else
                {
                    image_aux[h][w].rgbtGreen = value;
                }

            }
            // Middle pixels
            else
            {
                // Calculates Gx
                Gx = round(- 1 * image[h - 1][w - 1].rgbtRed + image[h - 1][w + 1].rgbtRed - 2 * image[h][w - 1].rgbtRed + 2 * image[h][w +
                           1].rgbtRed - 1 * image[h + 1][w - 1].rgbtRed + image[h + 1][w + 1].rgbtRed);
                // Calculates Gy
                Gy = round(- 1 * image[h - 1][w - 1].rgbtRed - 2 * image[h - 1][w].rgbtRed - image[h - 1][w + 1].rgbtRed + image[h + 1][w -
                           1].rgbtRed + 2 * image[h + 1][w].rgbtRed + image[h + 1][w + 1].rgbtRed);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtRed = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtRed = 0;
                }
                else
                {
                    image_aux[h][w].rgbtRed = value;
                }
                // Calculates Gx
                Gx = round(- 1 * image[h - 1][w - 1].rgbtBlue + image[h - 1][w + 1].rgbtBlue - 2 * image[h][w - 1].rgbtBlue + 2 * image[h][w +
                           1].rgbtBlue - 1 * image[h + 1][w - 1].rgbtBlue + image[h + 1][w + 1].rgbtBlue);
                // Calculates Gy
                Gy = round(- 1 * image[h - 1][w - 1].rgbtBlue - 2 * image[h - 1][w].rgbtBlue - image[h - 1][w + 1].rgbtBlue + image[h + 1][w -
                           1].rgbtBlue + 2 * image[h + 1][w].rgbtBlue + image[h + 1][w + 1].rgbtBlue);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtBlue = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtBlue = 0;
                }
                else
                {
                    image_aux[h][w].rgbtBlue = value;
                }

                // Calculates Gx
                Gx = round(- 1 * image[h - 1][w - 1].rgbtGreen + image[h - 1][w + 1].rgbtGreen - 2 * image[h][w - 1].rgbtGreen + 2 * image[h][w +
                           1].rgbtGreen - 1 * image[h + 1][w - 1].rgbtGreen + image[h + 1][w + 1].rgbtGreen);
                // Calculates Gy
                Gy = round(- 1 * image[h - 1][w - 1].rgbtGreen - 2 * image[h - 1][w].rgbtGreen - image[h - 1][w + 1].rgbtGreen + image[h + 1][w -
                           1].rgbtGreen + 2 * image[h + 1][w].rgbtGreen + image[h + 1][w + 1].rgbtGreen);

                value = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));

                if (value > 255)
                {
                    image_aux[h][w].rgbtGreen = 255;
                }
                else if (value < 0)
                {
                    image_aux[h][w].rgbtGreen = 0;
                }
                else
                {
                    image_aux[h][w].rgbtGreen = value;
                }
            }
        }
    }

    // Copies the 'edged' auxiliar image to the original
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = image_aux[h][w];
        }
    }

    return;
}
