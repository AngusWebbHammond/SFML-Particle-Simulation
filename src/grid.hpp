#include <iostream>
#include <SFML/Graphics.hpp>

struct Grid
{
    std::vector<std::vector<std::vector<int>>> grid;

    void createGrid(int rows, int columns)
    {
        for (int i = 0; i < rows; i++)
        {
            std::vector<std::vector<int>> tempGrid = {};
            for (int j = 0; j < columns; j++)
            {
                std::vector<int> tempVector = {};
                tempGrid.push_back(tempVector);
            }
            grid.push_back(tempGrid);
        }
    }

    int addNode(sf::Vector2i index, int value)
    {
        int valueIndex = grid[index.x][index.y].size() + 1;
        grid[index.x][index.y].push_back(value);
        return valueIndex;
    }

    void removeNode(sf::Vector2i index, int valueIndex)
    {
        grid[index.x][index.y].erase(grid[index.x][index.y].begin() + valueIndex);
    }

    int moveNode(sf::Vector2i prevIndex, sf::Vector2i newIndex, int valueIndex, int value)
    {
        removeNode(prevIndex, valueIndex);
        int indexValue = addNode(newIndex, value);
        return indexValue;
    }

    sf::Vector2i getIndex(sf::Vector2f pos, sf::Vector2i gridSize, sf::Vector2f windowSize)
    {
        int floorX = floor(pos.x / (windowSize.x / (gridSize.x + 1)));
        int floorY = floor(pos.y / (windowSize.y / (gridSize.y + 1)));

        return {floorX, floorY};
    }

    void printGrid()
    {
        std::cout << "[";
        for (auto list : grid)
        {
            for (auto list1 : list)
            {
                if (list1.empty())
                {
                    std::cout << "0, ";
                }
                else
                {
                    std::cout << "[";

                    for (int i : list1)
                    {
                        std::cout << i << ", ";
                    }

                    std::cout << "], ";
                }
            }
            std::cout << "\n";
        }
        std::cout << "]\n";
    }
};