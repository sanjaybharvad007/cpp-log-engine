#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <chrono>
#include "SafeQueue.h"

void log_consumer(int id, SafeQueue<std::string>& q) {
    std::string logLine;
    while (q.pop(logLine)) {
        if (logLine.find("ERROR") != std::string::npos) { 
            // Simulate error processing
        }
    }
}

void log_producer(SafeQueue<std::string>& q) {
    for (int i = 0; i < 1000000; i++) {
        std::string log = "Log Entry " + std::to_string(i) + " [INFO] System normal";
        if (i % 100 == 0) log = "Log Entry " + std::to_string(i) + " [ERROR] Connection fail";
        q.push(log);
    }
    q.markFinished();
}

int main() {
    SafeQueue<std::string> queue;
    std::cout << "Starting Log Engine..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    std::thread producer(log_producer, std::ref(queue));
    std::vector<std::thread> consumers;
    for(int i=0; i<4; i++) consumers.emplace_back(log_consumer, i, std::ref(queue));

    producer.join();
    for(auto& t : consumers) t.join();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Processed 1M logs in " << elapsed.count() << " seconds." << std::endl;
    return 0;
}
