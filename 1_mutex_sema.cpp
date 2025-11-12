#include <iostream>
#include <queue>
#include <semaphore.h>
#include <mutex>
using namespace std;

queue<int> buffer;
const int BUFFER_SIZE = 5;

mutex m;
sem_t emptySlots;   // counts free spaces
sem_t filledSlots;  // counts items available

void producer(int item) {
    sem_wait(&emptySlots); // wait if buffer is full

    m.lock();
    buffer.push(item);
    cout << "Produced: " << item << endl;
    m.unlock();

    sem_post(&filledSlots); // signal item added
}

void consumer() {
    sem_wait(&filledSlots); // wait if buffer is empty

    m.lock();
    int item = buffer.front();
    buffer.pop();
    cout << "  Consumed: " << item << endl;
    m.unlock();

    sem_post(&emptySlots); // signal space freed
}

int main() {
    sem_init(&emptySlots, 0, BUFFER_SIZE); // start with 5 empty slots
    sem_init(&filledSlots, 0, 0);          // start with 0 items

    // ------------------------------
    // Replace threading with a menu
    // ------------------------------
    int choice, item = 1;
    while (true) {
        cout << "\nMENU ->\n";
        cout << "1. Produce\n";
        cout << "2. Consume\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (buffer.size() == BUFFER_SIZE) {
                    cout << "Buffer is FULL! Cannot produce now.\n";
                } else {
                    producer(item++);
                }
                break;

            case 2:
                if (buffer.empty()) {
                    cout << "Buffer is EMPTY! Cannot consume now.\n";
                } else {
                    consumer();
                }
                break;

            case 3:
                cout << "Exiting..." << endl;
                sem_destroy(&emptySlots);
                sem_destroy(&filledSlots);
                return 0;

            default:
                cout << "Invalid choice!\n";
        }

        // Display current buffer
        cout << "Current Buffer: [ ";
        queue<int> temp = buffer;
        while (!temp.empty()) {
            cout << temp.front() << " ";
            temp.pop();
        }
        cout << "]" << endl;
    }
}
