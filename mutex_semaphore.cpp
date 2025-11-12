#include <iostream>
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>

using namespace std;

// ---------------------------------------
// Custom Semaphore Implementation
// ---------------------------------------
class MySemaphore {
private:
    int permits;
    mutex mtx;
    condition_variable cv;

public:
    MySemaphore(int initial) : permits(initial) {}

    void acquire() {
        unique_lock<mutex> lock(mtx);
        while (permits <= 0) {
            cv.wait(lock);
        }
        permits--;
    }

    void release() {
        unique_lock<mutex> lock(mtx);
        permits++;
        cv.notify_one();
    }

    int availablePermits() {
        unique_lock<mutex> lock(mtx);
        return permits;
    }
};

// ---------------------------------------
// Producer-Consumer Utility using MySemaphore
// ---------------------------------------
class Utility {
private:
    MySemaphore mutexSem{1};
    MySemaphore full{0};
    MySemaphore empty;

    int capacity;
    queue<int> buffer;

public:
    Utility(int capacity) : capacity(capacity), empty(capacity) {}

    vector<int> getBuffer() {
        vector<int> buf;
        queue<int> temp = buffer;
        while (!temp.empty()) {
            buf.push_back(temp.front());
            temp.pop();
        }
        return buf;
    }

    void produce(int val) {
        if (empty.availablePermits() == 0) {
            cout << "Buffer is FULL! Waiting for consumer..." << endl;
            return;
        }

        empty.acquire();
        mutexSem.acquire();

        buffer.push(val);
        cout << "Produced: " << val << endl;

        mutexSem.release();
        full.release();
    }

    void consume() {
        if (full.availablePermits() == 0) {
            cout << "Buffer is EMPTY! Waiting for producer..." << endl;
            return;
        }

        full.acquire();
        mutexSem.acquire();

        int value = buffer.front();
        buffer.pop();
        cout << "Consumed: " << value << endl;

        mutexSem.release();
        empty.release();
    }
};

// ---------------------------------------
// Main Program
// ---------------------------------------
int main() {
    int bufferSize;
    cout << "Enter Buffer Size: ";
    cin >> bufferSize;

    Utility utility(bufferSize);
    int produceValue = 1;

    while (true) {
        cout << "\nMENU -> \n";
        cout << "1. Produce\n";
        cout << "2. Consume\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                utility.produce(produceValue++);
                break;
            case 2:
                utility.consume();
                break;
            case 3:
                cout << "Exiting..." << endl;
                return 0;
            default:
                cout << "Invalid choice!" << endl;
        }

        // Display current buffer
        vector<int> buf = utility.getBuffer();
        cout << "Current Buffer: [ ";
        for (int x : buf) cout << x << " ";
        cout << "]" << endl;
    }
}
