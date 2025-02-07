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
    std::vector<Node *> grid;

    void createGrid(int rows, int columns)
    {
        for (int i = 0; i < rows * columns; i++)
        {
            grid.emplace_back(nullptr);
        }
    }

    void addNode(int indexGrid, Node *node)
    {
        if (grid[indexGrid] == nullptr)
        {
            grid[indexGrid] = node;
        }
        else
        {
            for (Node *nPtr = grid[indexGrid]; nPtr != nullptr; nPtr = nPtr->ptr)
            {
                if (nPtr->ptr == nullptr)
                {
                    nPtr->ptr = node;
                    break;
                }
            }
        }
    }

    void removeNode(int indexGrid, Node *node)
    {
        if (grid[indexGrid] == node)
        {
            grid[indexGrid] = node->ptr;
            node->ptr = nullptr;
        }
        else
        {
            for (Node *nPtr = grid[indexGrid]; nPtr != nullptr; nPtr = nPtr->ptr)
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

    void moveNode(int prevIndex, int newIndex, Node *node)
    {
        removeNode(prevIndex, node);
        addNode(newIndex, node);
    }

    int getIndex(sf::Vector2f pos)
    {
        float xVal = pos.x;
        float yVal = pos.y;

        int floorX = floor(xVal / 20);
        int floorY = floor(yVal / 20);

        int index = floorX + floorY * (19);

        return index;
    }

    void printGrid()
    {
        std::cout << "[";
        for (auto *node : grid)
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
        std::cout << "]\n";
    }
};