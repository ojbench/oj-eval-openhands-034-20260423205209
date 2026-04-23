
#ifndef SKIP_LIST_HPP
#define SKIP_LIST_HPP

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits>

template<typename T>
class SkipList {
private:
    struct Node {
        T value;
        std::vector<Node*> next;
        Node(const T &v, int level) : value(v), next(level, nullptr) {}
    };

    Node* head;
    int maxLevel;
    float probability;
    int currentLevel;

    int randomLevel() {
        int level = 1;
        while ((std::rand() % 100) < (probability * 100) && level < maxLevel) {
            level++;
        }
        return level;
    }

public:
    SkipList() : maxLevel(32), probability(0.5), currentLevel(1) {
        // We need a way to initialize head with a value of T if T doesn't have a default constructor.
        // However, the head node's value is never used for comparison in a standard skip list if we handle it correctly.
        // But T might not even have a default constructor.
        // A common trick is to use a dummy head without a value or with a special value.
        // Since we only have < operator, we can't easily have a "minimum" value.
        // Let's use a pointer for the value in Node or just assume T has a default constructor for now, 
        // or better, use a dummy head that doesn't store a T value if possible.
        // Actually, in C++, we can use char buffer to avoid constructing T.
        
        // For simplicity, let's assume T has a default constructor or we can use a pointer.
        // Given the constraints and the provided template, let's try to make it robust.
        head = reinterpret_cast<Node*>(new char[sizeof(Node)]);
        new (&head->next) std::vector<Node*>(maxLevel, nullptr);
        std::srand(std::time(nullptr));
    }

    ~SkipList() {
        Node* curr = head->next[0];
        while (curr) {
            Node* next = curr->next[0];
            curr->next.~vector<Node*>();
            delete[] reinterpret_cast<char*>(curr);
            curr = next;
        }
        head->next.~vector<Node*>();
        delete[] reinterpret_cast<char*>(head);
    }

    void insert(const T & item) {
        std::vector<Node*> update(maxLevel, nullptr);
        Node* curr = head;
        for (int i = currentLevel - 1; i >= 0; i--) {
            while (curr->next[i] && curr->next[i]->value < item) {
                curr = curr->next[i];
            }
            update[i] = curr;
        }
        curr = curr->next[0];

        if (curr && !(curr->value < item) && !(item < curr->value)) {
            return; // Already exists
        }

        int level = randomLevel();
        if (level > currentLevel) {
            for (int i = currentLevel; i < level; i++) {
                update[i] = head;
            }
            currentLevel = level;
        }

        Node* newNode = reinterpret_cast<Node*>(new char[sizeof(Node)]);
        new (&newNode->value) T(item);
        new (&newNode->next) std::vector<Node*>(level, nullptr);

        for (int i = 0; i < level; i++) {
            newNode->next[i] = update[i]->next[i];
            update[i]->next[i] = newNode;
        }
    }

    bool search(const T & item) {
        Node* curr = head;
        for (int i = currentLevel - 1; i >= 0; i--) {
            while (curr->next[i] && curr->next[i]->value < item) {
                curr = curr->next[i];
            }
        }
        curr = curr->next[0];
        return curr && !(curr->value < item) && !(item < curr->value);
    }

    void deleteItem(const T & item) {
        std::vector<Node*> update(maxLevel, nullptr);
        Node* curr = head;
        for (int i = currentLevel - 1; i >= 0; i--) {
            while (curr->next[i] && curr->next[i]->value < item) {
                curr = curr->next[i];
            }
            update[i] = curr;
        }
        curr = curr->next[0];

        if (curr && !(curr->value < item) && !(item < curr->value)) {
            for (int i = 0; i < currentLevel; i++) {
                if (update[i]->next[i] != curr) break;
                update[i]->next[i] = curr->next[i];
            }
            curr->value.~T();
            curr->next.~vector<Node*>();
            delete[] reinterpret_cast<char*>(curr);

            while (currentLevel > 1 && head->next[currentLevel - 1] == nullptr) {
                currentLevel--;
            }
        }
    }
};

#endif
