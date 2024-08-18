#include <random>
#include <ctime>
#include <stdexcept>
#include <iostream>

using namespace std;

// A single bag node that holds an item and a pointer to the next node.
template<typename ItemType>
class BagNode {
public:
    ItemType item; // The item stored in this node
    BagNode<ItemType>* next; // Pointer to the next node
    
    // Constructor that sets the item and next node pointer.
    BagNode(const ItemType& anItem, BagNode<ItemType>* nextNode = nullptr) : item(anItem), next(nextNode) {}
};

// The main class for our grab bag
template<typename ItemType>
class GrabBag {
private:
    BagNode<ItemType>* headPtr; // Points to the first node in the list
    int itemCount; // How many items are in the bag
    std::minstd_rand randgen; // The random number generator

public:
    // Default constructor that initializes an empty bag
    GrabBag() : headPtr(nullptr), itemCount(0) {
        std::random_device rd; // Device to generate a random seed
        randgen.seed(rd()); // Set the seed for our random generator
    }

    // Constructor that uses a specific seed for the random generator
    explicit GrabBag(unsigned int seed) : headPtr(nullptr), itemCount(0), randgen(seed) {}

    // Adds a new item to the bag
    bool add(const ItemType& newEntry) {
        try {
            BagNode<ItemType>* newNode = new BagNode<ItemType>(newEntry, headPtr); // Create a new node with the item
            headPtr = newNode; // New node becomes the first node
            itemCount++; // Increase the count of items
            return true;
        } catch (const std::bad_alloc& e) {
            std::cerr << "Failed to add new entry: " << e.what() << std::endl; // If memory allocation fails, print an error
            return false;
        }
    }

    // Removes and returns a random item from the bag
    ItemType grab() {
        if (itemCount == 0) {
            throw std::logic_error("Attempted to grab from an empty bag."); // Can't grab from an empty bag
        }
        try {
            int index = randgen() % itemCount; // Pick a random item index
            BagNode<ItemType>* nodeToRemove = nullptr;
            ItemType item;
            if (index == 0) { // If it's the first item, adjust the head pointer
                nodeToRemove = headPtr;
                headPtr = headPtr->next;
            } else { // Otherwise, find the item to remove
                BagNode<ItemType>* prevNode = getNodeAt(index - 1);
                nodeToRemove = prevNode->next;
                prevNode->next = nodeToRemove->next;
            }
            item = nodeToRemove->item; // Save the item to return
            delete nodeToRemove; // Free the removed node
            itemCount--; // Decrease the item count
            return item; // Return the removed item
        } catch (const std::exception& e) {
            std::cerr << "Error during grab: " << e.what() << std::endl; // Print any errors
            throw; // Re-throw the exception
        }
    }

    // Checks if the bag is empty
    bool isEmpty() const {
        return itemCount == 0;
    }

    // Returns the current size of the bag
    int getCurrentSize() const {
        return itemCount;
    }

    // Counts how many times a given item appears in the bag
    int getFrequencyOf(const ItemType& item) const {
        int frequency = 0;
        BagNode<ItemType>* currentNode = headPtr;
        while (currentNode != nullptr) { // Loop through all nodes
            if (currentNode->item == item) { // If the item matches, logic to increase the frequency
                frequency++;
            }
            currentNode = currentNode->next; // Move to the next node
        }
        return frequency;
    }

    // Destructor that clears the bag
    ~GrabBag() {
        empty();
    }

    // Removes all items from the bag
    void empty() {
        try {
            BagNode<ItemType>* nodeToDelete = headPtr;
            while (headPtr != nullptr) { // Loop until all nodes are deleted
                headPtr = headPtr->next; // Move the head pointer to the next node
                delete nodeToDelete; // Delete the current node
                nodeToDelete = headPtr; // Update the node to delete
            }
            itemCount = 0; // Reset the item count
        } catch (const std::exception& e) {
            std::cerr << "Exception during clear: " << e.what() << std::endl; // To print any errors
        }
    }

private:
    // Helper function to get the node at a certain position
    BagNode<ItemType>* getNodeAt(int position) const {
        BagNode<ItemType>* curPtr = headPtr;
        for (int skip = 0; skip < position && curPtr != nullptr; skip++) {
            curPtr = curPtr->next; // Move to the next node
        }
        return curPtr; // Return the node at the position
    }
};

// g++ -o main main.cpp -std=c++11
//  ./main