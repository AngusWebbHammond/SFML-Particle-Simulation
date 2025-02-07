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
    std::vector<std::vector<Node *>> grid;

    void createGrid(int rows, int columns)
    {
        for (int i = 0; i < rows; i++)
        {
            std::vector<Node *> tempGrid = {};
            for (int j = 0; j < columns; j++)
            {
                tempGrid.emplace_back(nullptr);
            }
            grid.emplace_back(tempGrid);
        }
    }

    void addNode(sf::Vector2i index, Node *node)
    {
        if (grid[index.x][index.y] == nullptr)
        {
            grid[index.x][index.y] = node;
        }
        else
        {
            for (Node *nPtr = grid[index.x][index.y]; nPtr != nullptr; nPtr = nPtr->ptr)
            {
                if (nPtr->ptr == nullptr)
                {
                    nPtr->ptr = node;
                    break;
                }
            }
        }
    }

    void removeNode(sf::Vector2i index, Node *node)
    {
        if (grid[index.x][index.y] == node)
        {
            grid[index.x][index.y] = node->ptr;
            node->ptr = nullptr;
        }
        else
        {
            for (Node *nPtr = grid[index.x][index.y]; nPtr != nullptr; nPtr = nPtr->ptr)
            {
                if (nPtr->ptr == node)
                {
                    nPtr->ptr = node->ptr;
                    node->ptr = nullptr;
                    break;
                }
            }
        }
    }

    void moveNode(sf::Vector2i prevIndex, sf::Vector2i newIndex, Node *node)
    {
        removeNode(prevIndex, node);
        addNode(newIndex, node);
    }

    sf::Vector2i getIndex(sf::Vector2f pos)
    {
        int floorX = floor(pos.x / 10);
        int floorY = floor(pos.y / 10);

        return {floorX, floorY};
    }

    void printGrid()
    {
        std::cout << "[";
        for (auto list : grid)
        {
            for (auto *node : list)
            {
                if (node == nullptr)
                {
                    std::cout << node << ", ";
                }
                else
                {
                    if (node->ptr == nullptr)
                    {
                        std::cout << node << " : Index: " << node->index << ", ";
                    }
                    else
                    {
                        std::cout << "[";
                        Node *nodePtr = node;
                        while (nodePtr != nullptr)
                        {
                            std::cout << nodePtr << " : Index: " << nodePtr->index << ", ";
                            nodePtr = nodePtr->ptr;
                        }

                        std::cout << "], ";
                    }
                }
            }
            std::cout << "\n";
        }
        std::cout << "]\n";
    }
};