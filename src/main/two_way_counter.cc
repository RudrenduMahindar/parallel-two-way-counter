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

bool line_reading_ends = false;
mutex m;
queue<string> inp_queue;
queue<string> adder_queue1;
queue<string> adder_queue2;

void move_input_to_adder1()
{
    //cout<<"adder1"<<"\n";
    //lock_guard<mutex> guard(m); //calling contructor starts critical section, locks mutex
    while(1)
    {
        m.lock();
        
        if(!inp_queue.empty())
        {
            string line = inp_queue.front();
            inp_queue.pop();
            adder_queue1.push(line);
            cout<<"t2 added line to adder_queue1\n";
        }
        if((inp_queue.empty()) && (line_reading_ends==true))
        {
            cout<<"t2 exits\n";
            m.unlock();
            break;
        }
        m.unlock();
    }
}

void move_input_to_adder2()
{
    //cout<<"adder2"<<"\n";
}

void read_input_lines()
{
    fstream file_obj;
    file_obj.open("/home/mahindar/multithreading_cpp/src/main/input.txt",ios::in); //open a file to perform read operation using file object
    if (file_obj.is_open()){   //checking whether the file is open
        string read_line;
        while(getline(file_obj, read_line)) //read data from file object and put it into string.
        {
            //critical section
            m.lock();
            cout<<"t1 adds line"<<"\n";
            inp_queue.push(read_line);
            //cout << read_line << "\n"; //print the data of the string
            m.unlock();
        }
        m.lock();
        line_reading_ends = true;
        cout<<"t1 exits"<<"\n";
        m.unlock();
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