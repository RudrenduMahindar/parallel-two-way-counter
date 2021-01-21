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
condition_variable cv;
bool line_inp_ready = false;

queue<string> inp_queue;
queue<string> adder_queue1;
queue<string> adder_queue2;

void move_input_to_adder1()
{
    while(1)
    {
        unique_lock<mutex> u1(m); //calling contructor starts critical section, locks mutex
        //thread wakes up after line input is available or when no more lines to read (otherwise thread may get stuck when line input adder exits without any notification) 
        cv.wait(u1, [](){return (line_inp_ready || line_reading_ends);});//wait for notification, then evaluate guard condition
        line_inp_ready = false;//reset this flag after taking out each line from input queue

        if(!inp_queue.empty())
        {
            string line = inp_queue.front();
            inp_queue.pop();
            adder_queue1.push(line);
            cout<<"t2 added line to adder_queue1\n";
        }

        if((inp_queue.empty()) && (line_reading_ends == true))
        {
            cout<<"t2 exits\n";
            break;
        }
    }
}

void move_input_to_adder2()
{
    while(1)
    {
        unique_lock<mutex> u1(m);
        cv.wait(u1, []() { return (line_inp_ready || line_reading_ends); }); //wait for notification
        line_inp_ready = false;

        if(!inp_queue.empty())
        {
            string line = inp_queue.front();
            inp_queue.pop();
            adder_queue2.push(line);
            cout<<"t3 added line to adder_queue2\n";
        }

        if((inp_queue.empty()) && (line_reading_ends == true))
        {
            cout<<"t3 exits\n";
            break;
        }
    }
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
            inp_queue.push(read_line); //shared queue
            line_inp_ready = true;
            cv.notify_all();
            //cout << read_line << "\n"; //print the data of the string
            m.unlock();
        }
        m.lock();
        line_reading_ends = true;
        cv.notify_all(); //last notification to wake up threads from cv queue
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
