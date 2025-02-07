#include <iostream>
#include <SFML/Graphics.hpp>

struct Node;

struct Node
{
    int index;
    Node *ptr;

    Node(int index_) : index{index_}, ptr{nullptr} {};
};

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
                tempGrid.emplace_back(tempVector);
            }
            grid.emplace_back(tempGrid);
        }
    }

    int addNode(sf::Vector2i index, int value)
    {
        int valueIndex = grid[index.x][index.y].size() + 1;
        grid[index.x][index.y].emplace_back(value);
        return valueIndex;
    }

    void removeNode(sf::Vector2i index, int valueIndex)
    {
        grid[index.x][index.y].erase(grid[index.x][index.y].begin() + valueIndex);
    }

    int moveNode(sf::Vector2i prevIndex, sf::Vector2i newIndex, int valueIndex, int value)
    {
        // std::cout << "NewIndex: [" << newIndex.x << ", " << newIndex.y << "]\n";
        // std::cout << "PrevIndex: [" << prevIndex.x << ", " << prevIndex.y << "]\n";
        removeNode(prevIndex, valueIndex);

        int indexValue = addNode(newIndex, value);
        // std::cout << "IndexVal: " << indexValue << "\n";

        return indexValue;
    }

    sf::Vector2i getIndex(sf::Vector2f pos, sf::Vector2i gridSize, sf::Vector2f windowSize)
    {
        int floorX = floor(pos.x / (windowSize.x / gridSize.x));
        int floorY = floor(pos.y / (windowSize.y / gridSize.y));

        // std::cout << "Index: " << floorX << ", " << floorY << "\n";

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