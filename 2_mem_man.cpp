#include <iostream>
#include <iomanip>
#include <climits>   // for INT_MAX
using namespace std;

int main() {
    int bsize, psize;
    cout << "Enter number of memory blocks: ";
    cin >> bsize;
    cout << "Enter number of processes: ";
    cin >> psize;

    int blocks[20], process[20], flag[20];
    cout << "Enter block sizes:\n";
    for (int i = 0; i < bsize; i++) {
        cin >> blocks[i];
        flag[i] = 0; // mark all free
    }

    cout << "Enter process sizes:\n";
    for (int i = 0; i < psize; i++)
        cin >> process[i];

    int choice;
    cout << "\n1. First Fit\n2. Best Fit\n3. Worst Fit\nEnter your choice: ";
    cin >> choice;

    cout << "\nBlock No" << setw(15) << "Block Size" << setw(25) << "Process Size\n";

    if (choice == 1) {
        // ---------------- FIRST FIT ----------------
        for (int i = 0; i < psize; i++) {
            bool allocated = false;
            for (int j = 0; j < bsize; j++) {
                if (process[i] <= blocks[j] && flag[j] == 0) {
                    cout << setw(5) << j + 1 << setw(15) << blocks[j]
                         << setw(25) << process[i] << endl;
                    flag[j] = 1;
                    allocated = true;
                    break;
                }
            }
            if (!allocated)
                cout << "Process " << i + 1 << " (" << process[i] << ") not allocated\n";
        }
    }

    else if (choice == 2) {
        // ---------------- BEST FIT ----------------
        for (int i = 0; i < psize; i++) {
            int best = -1;
            int minDiff = INT_MAX;

            for (int j = 0; j < bsize; j++) {
                if (flag[j] == 0 && process[i] <= blocks[j]) {
                    int diff = blocks[j] - process[i];
                    if (diff < minDiff) {
                        minDiff = diff;
                        best = j;
                    }
                }
            }

            if (best != -1) {
                cout << setw(5) << best + 1 << setw(15) << blocks[best]
                     << setw(25) << process[i] << endl;
                flag[best] = 1;
            } else {
                cout << "Process " << i + 1 << " (" << process[i] << ") not allocated\n";
            }
        }
    }

    else if (choice == 3) {
        // ---------------- WORST FIT ----------------
        for (int i = 0; i < psize; i++) {
            int worst = -1;
            int maxDiff = -1;

            for (int j = 0; j < bsize; j++) {
                if (flag[j] == 0 && process[i] <= blocks[j]) {
                    int diff = blocks[j] - process[i];
                    if (diff > maxDiff) {
                        maxDiff = diff;
                        worst = j;
                    }
                }
            }

            if (worst != -1) {
                cout << setw(5) << worst + 1 << setw(15) << blocks[worst]
                     << setw(25) << process[i] << endl;
                flag[worst] = 1;
            } else {
                cout << "Process " << i + 1 << " (" << process[i] << ") not allocated\n";
            }
        }
    }

    else {
        cout << "Invalid choice!\n";
    }

    cout << "\nAllocation completed.\n";
    return 0;
}
