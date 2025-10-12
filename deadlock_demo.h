// deadlock_demo.h

#ifndef DEADLOCK_DEMO_H
#define DEADLOCK_DEMO_H

// OS Concept: This module is designed to demonstrate a classic deadlock
// scenario (circular wait) for educational purposes.

namespace DeadlockDemo {
    // Intentionally causes a deadlock by having two threads
    // lock two mutexes in opposite orders.
    void demonstrateDeadlock();
}

#endif // DEADLOCK_DEMO_H