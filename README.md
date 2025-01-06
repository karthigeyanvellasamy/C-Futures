# C-Futures

## Overview
This project demonstrates how to adapt the concept of Java Futures to C programming. It provides a lightweight implementation for asynchronous task execution using threads, mutexes, and condition variables.

## Features
- **Asynchronous Task Execution:** Submit tasks to be executed in parallel.
- **Result Retrieval:** Retrieve task results once they are completed.
- **Thread-Safe Design:** Uses mutexes and condition variables to ensure safe concurrent access.

## Usage Instructions
1. **Clone the repository:**
   ```bash
   git clone https://github.com/<your-username>/C-Futures.git
   cd C-Futures/src
   ```

2. **Compile the Code:**
   ```bash
   gcc -o future future.c -lpthread
   ```

3. **Run the Program:**
   ```bash
   ./future
   ```

## Example
The program includes an example task that doubles an integer value asynchronously. The output will look like:
```
Result: 20
```

## License
This project is licensed under the MIT License.

## Acknowledgments
This implementation is part of a technical series developed under the ownership of Karthigeyan, with contributions and guidance from the project owner.