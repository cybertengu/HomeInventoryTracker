// ConsoleApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

#define MAX_LINE_LENGTH 1000
#define MAX_PRODUCT_NAME 256

int main()
{
    printf("Scan a barcode (for testing, use 048500001745):\n");
    char barcode[13];
    int result = scanf_s("%s", barcode, (int)sizeof(barcode));
    barcode[sizeof(barcode) - 1] = '\0';

    FILE* csvFile;
    char line[MAX_LINE_LENGTH];

    std::ifstream ifs("products.csv", std::ifstream::in);
    if (!ifs.good())
    {
        return 1;
    }

    bool foundProduct = false;
    char firstToken[MAX_PRODUCT_NAME];
    while (ifs.getline(line, MAX_LINE_LENGTH))
    {
        //printf(line);
        char* namePtr = &line[0];
        int counter = 0;
        int result = strncmp(line, barcode, sizeof(barcode) - 1); // TODO(David): Need to set size as a variable, so I don't compute it over and over.
        while (!result && counter < (sizeof(barcode) - 1))
        {
            firstToken[counter++] = namePtr[0];
            ++namePtr;
        }
        firstToken[counter] = '\0';
        //printf("%s\n", firstToken);

        if (!result)
        {
            printf("Found entry: %s", line);
            foundProduct = true;
            break;
        }
    }

    ifs.close();

    if (foundProduct)
    {
        char productName[MAX_LINE_LENGTH];
        char amount[MAX_LINE_LENGTH];
        bool lookingForProductName = 1;
        bool lookingForAmount = 0;
        char* namePtr = &line[sizeof(barcode)];
        int counter = 0;

        while (namePtr[0] != '\0')
        {
            if (namePtr[0] == ',')
            {
                if (lookingForProductName)
                {
                    productName[counter] = '\0';
                    lookingForAmount = 1;
                    lookingForProductName = 0;
                }
                counter = 0;
                ++namePtr;
                continue;
            }

            if (lookingForProductName)
            {
                productName[counter++] = namePtr[0];
            }
            else if (lookingForAmount)
            {
                amount[counter++] = namePtr[0];
            }
            ++namePtr;
        }
        amount[counter] = '\0';

        printf("\n1: %s 2: %s 3: %s", barcode, productName, amount);

        std::ifstream filein("products.csv"); //File to read from
        std::ofstream fileout("fileout.csv"); //Temporary file
        if (!filein || !fileout)
        {
            printf("Error opening file!");
            return 1;
        }

        char currentLine[MAX_LINE_LENGTH];
        char tempLine[MAX_LINE_LENGTH];
        // TODO(David): I need to figure out how to get the amount and increment by one.
        char newLine[MAX_LINE_LENGTH] = "048500001745,Tropicana Orange Juice,2\0";
        while (filein.getline(currentLine, MAX_LINE_LENGTH))
        {
            int result = strncmp(line, currentLine, sizeof(currentLine));
            if (!result)
            {
                // TODO(David): I need to figure out how long the line is.
                fileout.write(newLine, 37);
                fileout.write("\n", 1);
                //found = true;
            }
            else
            {
                // TODO(David): I need to figure out how long the line is.
                fileout.write(currentLine, 27);
                fileout.write("\n", 1);
            }
        }
        filein.close();
        fileout.close();

        // TODO(David): I need to delete the old file and rename the new file since it has been updated.
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file