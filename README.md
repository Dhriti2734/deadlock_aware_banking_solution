# Deadlock Aware Banking System

A comprehensive multithreaded banking system demonstrating **Operating System concepts** including CPU scheduling, deadlock prevention using Banker's Algorithm, and thread synchronization.

## 🎯 Project Overview

This C++17 project implements a complete banking system with:
- **Multi-threaded transaction processing**
- **CPU Scheduling algorithms** (FCFS & Priority with Aging)
- **Banker's Algorithm** for deadlock prevention
- **Mutex-based synchronization**
- **File-based logging**
- **Interactive CLI interface**

## 🏗️ Architecture

### Core Components

```
┌─────────────────────────────────────────────────────────┐
│                   Banking System (CLI)                  │
└────────────────────┬────────────────────────────────────┘
                     │
        ┌────────────┼────────────┐
        │            │            │
┌───────▼──────┐ ┌──▼────────┐ ┌─▼──────────────┐
│   Scheduler  │ │ Deadlock  │ │  Transaction   │
│ (FCFS/Prio)  │ │  Handler  │ │    Manager     │
│              │ │ (Banker's │ │ (3 Workers)    │
│   + Aging    │ │Algorithm) │ │                │
└──────┬───────┘ └─────┬─────┘ └────────┬───────┘
       │               │                 │
       └───────────────┴─────────────────┘
                       │
            ┌──────────┴──────────┐
            │                     │
       ┌────▼────┐          ┌────▼────┐
       │ Accounts│          │  Logger │
       │ (Mutex) │          │ (File)  │
       └─────────┘          └─────────┘
```

## 📋 Features

### 1. Account Management
- Multiple bank accounts with thread-safe operations
- Mutex protection for concurrent access
- Balance tracking and validation

### 2. Transaction Processing
- **Deposit**: Add money to account
- **Withdraw**: Remove money from account
- **Transfer**: Move money between accounts
- Atomic operations with proper synchronization

### 3. CPU Scheduling
#### FCFS (First-Come-First-Served)
- Simple queue-based scheduling
- Fair processing order

#### Priority Scheduling
- Three priority levels: LOW, MEDIUM, HIGH
- **Aging mechanism**: Prevents starvation by automatically increasing priority of waiting transactions
- Dynamic priority adjustments

### 4. Deadlock Prevention
#### Banker's Algorithm Implementation
- Safe state detection before resource allocation
- Prevents circular wait conditions
- Transaction retry mechanism for unsafe states

#### Resource Ordering
- Accounts always locked in ascending ID order
- Eliminates one of the four Coffman conditions

### 5. Synchronization
- `std::mutex` for account protection
- Thread-safe lock tracking
- Atomic transaction execution

### 6. Logging System
- Timestamped transaction logs
- Thread-safe file writing
- Persistent storage in `transaction_log.txt`

### 7. Interactive CLI
- User-friendly menu system
- Real-time status display
- Runtime policy switching
- Comprehensive demo modes

## 🚀 Getting Started

### Prerequisites
- **Windows 11** (or Windows 10)
- **MinGW-w64 GCC** (version 7.0 or higher)
- **C++17** support

### Installation

1. **Verify GCC installation:**
   ```bash
   g++ --version
   ```

2. **Place all files in the same directory**

3. **Build the project using one of these methods:**

#### Method 1: Using build.bat (Recommended)
```bash
build.bat
```

#### Method 2: Single command
```bash
g++ -std=c++17 -O2 -o BankingSystem.exe Account.cpp Transaction.cpp Logger.cpp DeadlockHandler.cpp Scheduler.cpp TransactionManager.cpp BankingSystem.cpp main.cpp -pthread
```

#### Method 3: Using Makefile
```bash
make
```

### Running the Program
```bash
BankingSystem.exe
```

## 📖 Usage Guide

### Main Menu Options

```
=========================================
   Deadlock Aware Banking System
=========================================

[Accounts]
- Account 101 (Balance: $1000)
- Account 102 (Balance: $1000)
- Account 103 (Balance: $500)

[Current Scheduler: FCFS]

[Main Menu]
1. Deposit
2. Withdraw
3. Manual Transfer
4. Run Concurrent Test (Simulate 5 random transfers)
5. Run Deadlock Demonstration
6. Change Scheduling Policy
7. Retry Failed Transactions
8. View Transaction Log
9. Exit
```

### Demo Scenarios

#### Concurrent Test (Option 4)
Automatically generates and executes 5 random transfers using the currently selected scheduling policy.

#### Deadlock Demonstration (Option 5)
**Mode 1: Naive Locking (Shows Deadlock)**
- Disables Banker's Algorithm
- Creates circular wait: T1 locks A→needs B, T2 locks B→needs A
- **⚠️ Console will freeze** (demonstrates actual deadlock)
- Press `Ctrl+C` to terminate

**Mode 2: Banker's Algorithm (Prevents Deadlock)**
- Runs same scenario with deadlock prevention
- Detects unsafe state
- Prevents system freeze
- Transactions complete successfully

### Scheduling Policy Switch (Option 6)
Toggle between:
- **FCFS**: Simple queue, fair ordering
- **Priority**: Three priority levels with aging to prevent starvation

## 🔬 Technical Details

### Banker's Algorithm Flow
```
1. Transaction requests resources (accounts)
2. System checks current state
3. Simulates allocation
4. Checks if safe sequence exists
5. If SAFE → Grant resources
6. If UNSAFE → Transaction waits/retries
```

### Aging Mechanism
```
IF transaction_wait_time > 5 seconds:
    IF priority == LOW:
        priority = MEDIUM
    ELIF priority == MEDIUM:
        priority = HIGH
```

### Lock Ordering Strategy
```cpp
// Always lock in ascending ID order
vector<int> accounts = {102, 101};  // Requested
sort(accounts.begin(), accounts.end());  // → {101, 102}
// Lock 101 first, then 102
```

## 📁 Project Structure

```
Deadlock-Aware-Banking-System/
├── Account.h / .cpp           # Account management
├── Transaction.h / .cpp       # Transaction types and tracking
├── Logger.h / .cpp            # File logging (Singleton)
├── DeadlockHandler.h / .cpp   # Banker's Algorithm
├── Scheduler.h / .cpp         # FCFS & Priority scheduling
├── TransactionManager.h / .cpp # Multi-threaded processor
├── BankingSystem.h / .cpp     # Main system & CLI
├── main.cpp                   # Entry point
├── build.bat                  # Build script
├── Makefile                   # Make build file
├── COMPILATION_GUIDE.txt      # Detailed build instructions
└── README.md                  # This file
```

## 🧪 Testing

### Test Case 1: Normal Operation
```
1. Run program
2. Select "Manual Transfer" (Option 3)
3. Transfer $200 from 101 to 102
4. Verify balances update correctly
5. Check transaction_log.txt for entry
```

### Test Case 2: Concurrent Transactions
```
1. Select "Run Concurrent Test" (Option 4)
2. Observe 5 transfers executing
3. Verify all complete successfully
4. Check final balances are consistent
```

### Test Case 3: Deadlock Prevention
```
1. Select "Run Deadlock Demonstration" (Option 5)
2. Choose Mode 2 (Banker's Algorithm)
3. Observe safe state checks
4. Verify transactions complete without freezing
```

### Test Case 4: Priority Scheduling
```
1. Change policy to Priority (Option 6)
2. Run concurrent test
3. Observe high-priority transactions execute first
4. Verify aging mechanism activates for waiting transactions
```

## 🐛 Troubleshooting

### Compilation Errors
| Error | Solution |
|-------|----------|
| `g++ not recognized` | Add MinGW bin to PATH |
| `undefined reference` | Ensure all .cpp files are included |
| `C++17 not supported` | Update GCC to version 7.0+ |
| `ctime_s not found` | Use Unix compiler or modify Logger.cpp |

### Runtime Issues
| Issue | Solution |
|-------|----------|
| Program freezes in Demo Mode 1 | Expected behavior! Press Ctrl+C |
| Log file empty | Check file permissions |
| Transactions fail | Verify account balances are sufficient |

## 📊 Performance Characteristics

- **Worker Threads**: 3 (configurable)
- **Scheduling Overhead**: O(n log n) for Priority, O(1) for FCFS
- **Lock Acquisition**: O(k) where k = accounts needed
- **Banker's Algorithm**: O(n²) complexity
- **Throughput**: ~10-20 transactions/second (with simulated delays)

## 🎓 Learning Objectives

This project demonstrates:
✅ **Process Synchronization** (mutexes, locks)
✅ **CPU Scheduling** (FCFS, Priority, Aging)
✅ **Deadlock Prevention** (Banker's Algorithm, Resource Ordering)
✅ **Multithreading** (worker threads, thread pools)
✅ **Race Condition Prevention** (atomic operations, critical sections)
✅ **Design Patterns** (Singleton Logger, Observer pattern)

## 📜 License

This project is created for educational purposes to demonstrate Operating System concepts.

## 👨‍💻 Author

Created as a comprehensive OS project demonstrating:
- Thread synchronization
- CPU scheduling algorithms
- Deadlock prevention techniques
- Concurrent programming in C++17

---

**Note**: This is an educational project. For production banking systems, use established frameworks, databases, and security measures.

## 🔗 References

- Silberschatz, Galvin, Gagne - *Operating System Concepts*
- Banker's Algorithm (Dijkstra, 1965)
- C++17 Threading Standard Library
- MinGW-w64 Documentation