#pragma once
#pragma once
#include <iostream>
#include <vector>

using namespace std;

class RoutesArray {
private:
    Route** routes;
    int size;
    int capacity;

    void ensureCapacity() {
        if (size >= capacity) {
            capacity *= 2;
            Route** new_routes = new Route * [capacity];
            for (int i = 0; i < size; ++i) {
                new_routes[i] = routes[i];
            }
            delete[] routes;
            routes = new_routes;
        }
    }
public:
    RoutesArray() : routes(nullptr), size(0), capacity(2) {
        routes = new Route * [capacity];
    }

    RoutesArray(const RoutesArray& other) : routes(nullptr), size(other.size), capacity(other.capacity) {
        routes = new Route * [capacity];
        for (int i = 0; i < size; ++i) {
            routes[i] = other.routes[i]->clone();
        }
    }

    RoutesArray(RoutesArray&& other) noexcept : routes(other.routes), size(other.size), capacity(other.capacity) {
        other.routes = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    RoutesArray& operator=(const RoutesArray& other) {
        if (this != &other) {
            for (size_t i = 0; i < size; ++i) {
                delete routes[i];
            }
            delete[] routes;

            size = other.size;
            capacity = other.capacity;
            routes = new Route * [capacity];
            for (size_t i = 0; i < size; ++i) {
                routes[i] = other.routes[i]->clone();
            }
        }
        return *this;
    }

    RoutesArray& operator=(RoutesArray&& other) noexcept {
        if (this != &other) {
            for (size_t i = 0; i < size; ++i) {
                delete routes[i];
            }
            delete[] routes;

            routes = other.routes;
            size = other.size;
            capacity = other.capacity;

            other.routes = nullptr;
            other.size = 0;
            other.capacity = 0;
        }
        return *this;
    }

    ~RoutesArray() {
        for (int i = 0; i < size; ++i) {
            delete routes[i];
        }
        delete[] routes;
    }

    void add(Route* route) {
        ensureCapacity();
        routes[size++] = route;
    }

    void remove(size_t index) {
        if (index < size) {
            delete routes[index];
            for (int i = index; i < size - 1; ++i) {
                routes[i] = routes[i + 1];
            }
            --size;
        }
    }

    void print() const {
        for (int i = 0; i < size; ++i) {
            cout << *routes[i] << std::endl;
        }
    }

    void clear() {
        for (int i = 0; i < size; ++i) {
            delete routes[i];
        }
        size = 0;
    }
};