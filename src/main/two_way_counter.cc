#include <iostream>
#include <fstream>
#include <future>
#include <string>
#include <queue>
#include <vector>
#include <mutex>
#include <thread>
#include "src/lib/utility.h"
using namespace std;

queue<string> inp_queue;
queue<string> adder_queue1;
queue<string> adder_queue2;

void move_input_to_adder1()
{
    cout<<"adder1"<<"\n";
}

void move_input_to_adder2()
{
    cout<<"adder2"<<"\n";
}

void read_input_lines()
{
    fstream file_obj;
    file_obj.open("/home/mahindar/multithreading_cpp/src/main/input.txt",ios::in); //open a file to perform read operation using file object
    if (file_obj.is_open()){   //checking whether the file is open
        string read_line;
        while(getline(file_obj, read_line)) //read data from file object and put it into string.
        {
            inp_queue.push(read_line);
            cout << read_line << "\n"; //print the data of the string
        }
        file_obj.close();
    }
}



int main(){
    thread t1(read_input_lines);
    thread t2(move_input_to_adder1);
    thread t3(move_input_to_adder2);
    
    t1.join();
    t2.join();
    t3.join();
    return 0;
}