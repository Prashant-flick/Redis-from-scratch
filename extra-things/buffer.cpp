#include<iostream>
#include<chrono>
#include<thread>

using namespace std;

int main() {
    // unbuffed
    fprintf(stderr, "error"); // stderr is unbuffed - stdout will not work as unbuffed  
    fprintf(stdout, "stdout");

    // Fully Buffered - will be stored until next line or memory full
    printf("A");
    printf("B");
    printf("C");
    printf("D");
    printf("E");
    this_thread::sleep_for(chrono::seconds(5));
    printf("\n");

    // buffer is stored until next line then print - LINE BUFFERED
    printf("Hello");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    printf("\n");
}