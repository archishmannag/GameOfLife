#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstdio>
#include <random>
#include <iomanip>
#include <thread>
#include <chrono>
#include "kbhit.h"

class GameOfLife
{
private:
    const int rows, cols, baseSpeed = 50'000, maxTime = 400'000, minTime = 800, displayTime = 5'000'000;
    int speed = baseSpeed, Spawned = 0;
    float factor = 1.0;
    bool **gridOld, **gridNew, isPaused = false, reset = true, isSpawned = false;
    std::string name, *spawnText;

    void spawn()
    {
        display();
        std::ifstream file("config.txt");
        std::istringstream lineSubString;
        std::string line, spawnCode, name, length, width, inputSpawnCode;
        int len, wid;
        std::cin >> inputSpawnCode;
        while (!file.eof())
        {
            std::getline(file, line);
            if (line[0] == '#' || line.empty())
                continue;
            lineSubString.str(line);
            lineSubString >> spawnCode;
            if (spawnCode.compare(inputSpawnCode) != 0)
            {
                while (!line.empty() && !file.eof())
                {
                    std::getline(file, line);
                }
                continue;
            }
            else
            {
                break;
            }
        }
        if (file.eof())
        {
            spawnText = new std::string("No creature found with name: ");
            return;
        }
        lineSubString >> name >> length >> width;
        this->name = name;
        len = std::stoi(length);
        wid = std::stoi(width);
        for (int i = 0; i < wid; i++)
        {
            std::getline(file, line);
            for (int j = 0; j < len; j++)
            {
                gridOld[i + rows / 2][j + cols / 2] = line[j] == '1';
            }
        }
    }

    void input()
    {
        if (_kbhit())
        {
            int ch = getchar();
            switch (ch)
            {
            case 'q':
                exit(0);
                break;
            case 's':
                isSpawned = true;
                spawn();
                Spawned = displayTime / speed;
                isSpawned = false;
                break;
            case 'c':
                for (int i = 0; i < rows; i++)
                    for (int j = 0; j < cols; j++)
                        gridOld[i][j] = false;
                break;
            case '+':
                if (speed > minTime)
                    factor *= 2;
                break;
            case '-':
                if (speed < maxTime)
                    factor /= 2;
                break;
            case ' ':
                isPaused = !isPaused;
                break;
            case 'r':
                reset = true;
                break;
            }
        }
    }

    void evaluate()
    {
        int count;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                count = 0;
                if (!(i == 0 || j == 0 || i == rows - 1 || j == cols - 1))
                {
                    if (gridOld[i - 1][j - 1])
                        count++;
                    if (gridOld[i - 1][j])
                        count++;
                    if (gridOld[i - 1][j + 1])
                        count++;
                    if (gridOld[i][j - 1])
                        count++;
                    if (gridOld[i][j + 1])
                        count++;
                    if (gridOld[i + 1][j - 1])
                        count++;
                    if (gridOld[i + 1][j])
                        count++;
                    if (gridOld[i + 1][j + 1])
                        count++;

                    if (count > 3 || count < 2)
                        gridNew[i][j] = false;
                    else
                        gridNew[i][j] = gridOld[i][j] || (count == 3);
                }
            }
        }
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                gridOld[i][j] = gridNew[i][j];
        for (int i = 0; i < rows; i++)
        {
            gridOld[i][0] = gridOld[i][cols - 2];
            gridOld[i][cols - 1] = gridOld[i][1];
        }
        for (int i = 0; i < cols; i++)
        {
            gridOld[0][i] = gridOld[rows - 2][i];
            gridOld[rows - 1][i] = gridOld[1][i];
        }
    }

    void display()
    {
        system("clear");
        std::cout
            << "Speed: " << std::setprecision(4) << factor << "x\n"
            << (isSpawned
                    ? "Enter the spawn code..."
                    : "Press s to spawn known creatures.\tPress r to reset.\tPress c to clear board.\tPress q to quit.")
            << std::endl;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (i == 0 && j == 0)
                    std::cout << u8"\u250C"; //                                     Unicode character '┌'
                else if (i == 0 && j == cols - 1)
                    std::cout << u8"\u2510"; //                                     Unicode character '┐'
                else if (i == rows - 1 && j == 0)
                    std::cout << u8"\u2514"; //                                     Unicode character '└'
                else if (i == rows - 1 && j == cols - 1)
                    std::cout << u8"\u2518"; //                                     Unicode character '┘'
                else if (i == 0 || i == rows - 1)
                    std::cout << u8"\u2500"; //                                     Unicode character '─'
                else if (j == 0 || j == cols - 1)
                    std::cout << u8"\u2502"; //                                     Unicode character '│'
                else
                {
                    if (gridNew[i][j])
                        std::cout << u8"\u25A0"; //                                 Unicode character '■'
                    else
                        std::cout << ' ';
                }
            }
            std::cout << std::endl;
        }
        if (Spawned)
        {
            std::cout << "Spawned: " << name << std::endl;
            Spawned--;
        }
    }

    void runGame()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 1);
        while (true)
        {
            input();
            if (!isPaused)
                evaluate();
            display();
            speed = int(baseSpeed / factor);
            std::this_thread::sleep_for(std::chrono::microseconds(speed));
            if (reset)
            {
                reset = false;
                for (int i = 0; i < rows; i++)
                    for (int j = 0; j < cols; j++)
                        gridOld[i][j] = dis(gen);
            }
        }
    }

public:
    GameOfLife(int r, int c) : rows(r), cols(c)
    {
        gridOld = new bool *[rows];
        for (int i = 0; i < rows; i++)
        {
            gridOld[i] = new bool[cols];
        }
        gridNew = new bool *[rows];
        for (int i = 0; i < rows; i++)
        {
            gridNew[i] = new bool[cols];
        }
        GameOfLife::runGame();
    };

    ~GameOfLife()
    {
        for (int i = 0; i < rows; i++)
        {
            delete gridNew[i];
        }
        delete gridNew;
        for (int i = 0; i < rows; i++)
        {
            delete gridOld[i];
        }
        delete gridOld;
    };
};