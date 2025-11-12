import java.util.*;

public class CustomSemaphoreDemo {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Enter Buffer Size: ");
        int bufferSize = sc.nextInt();

        Utility utility = new Utility(bufferSize);
        int produceValue = 1;

        while (true) {
            System.out.println("\nMENU -> ");
            System.out.println("1. Produce");
            System.out.println("2. Consume");
            System.out.println("3. Exit");
            System.out.print("Enter your choice: ");
            int choice = sc.nextInt();

            try {
                switch (choice) {
                    case 1:
                        utility.produce(produceValue++);
                        break;
                    case 2:
                        utility.consume();
                        break;
                    case 3:
                        System.out.println("Exiting...");
                        System.exit(0);
                        break;
                    default:
                        System.out.println("Invalid choice!");
                }
            } catch (Exception e) {
                e.printStackTrace();
            }

            System.out.println("Current Buffer: " + utility.getBuffer());
        }
    }
}

// ---------------------------------------
// Custom Semaphore Implementation
// ---------------------------------------
class MySemaphore {
    private int permits;

    public MySemaphore(int initial) {
        this.permits = initial;
    }

    // acquire() -- wait until permit is available
    public synchronized void acquire() throws InterruptedException {
        while (permits <= 0) {
            wait();  // block until someone releases
        }
        permits--;
    }

    // release() -- signal that one permit is available
    public synchronized void release() {
        permits++;
        notify(); // wake up one waiting thread
    }

    // just for debugging messages
    public synchronized int availablePermits() {
        return permits;
    }
}

// ---------------------------------------
// Producer-Consumer Utility using MySemaphore
// ---------------------------------------
class Utility {
    private final MySemaphore mutex = new MySemaphore(1);
    private final MySemaphore full = new MySemaphore(0);
    private final MySemaphore empty;

    private final int capacity;
    private final Queue<Integer> buffer;

    public Utility(int capacity) {
        this.capacity = capacity;
        this.buffer = new LinkedList<>();
        this.empty = new MySemaphore(capacity);
    }

    public List<Integer> getBuffer() {
        return new ArrayList<>(buffer);
    }

    public void produce(int i) throws Exception {
        if (empty.availablePermits() == 0){
            System.out.println("Buffer is FULL! Waiting for consumer...");
            return ;
        }
            

        empty.acquire();
        mutex.acquire();

        buffer.add(i);
        System.out.println("Produced: " + i);

        mutex.release();
        full.release();
    }

    public void consume() throws Exception {
        if (full.availablePermits() == 0){
            System.out.println("Buffer is EMPTY! Waiting for producer...");
            return ;
        }

        full.acquire();
        mutex.acquire();

        int value = buffer.poll();
        System.out.println("Consumed: " + value);

        mutex.release();
        empty.release();
    }
}