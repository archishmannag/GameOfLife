#include <iostream>
#include "GameOfLife.cpp"

int main(int argc, char const *argv[])
{
    int rows, cols;
    std::string input;
    system("clear");
    std::cout
        << "Conway's Game of Life\n"
        << "Press 'q' to quit\n"
        << "Press '+' to increase speed\n"
        << "Press '-' to decrease speed\n"
        << "Press 'space' to pause and unpause\n"
        << "Known creatures can be spawned!\n\n\n"
        << "Run default board size? (Y/n/q):";
    fflush(stdout);
    std::cin >> input;
    if (input.compare("q") == 0 || input.compare("Q") == 0)
    {
        return 0;
    }
    else if (input.compare("n") == 0 || input.compare("N") == 0)
    {
        std::cout << "Enter the number of rows and columns in the grid: ";
        std::cin >> rows >> cols;
        new GameOfLife(rows, cols);
    }
    else if (input.compare("y") == 0 || input.compare("Y") == 0 || input.empty())
    {
        // new GameOfLife();
    }
    return 0;
}