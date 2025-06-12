#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
using namespace std;
mutex m;


struct Dron{
    int id;
    mutex m;
    bool waiting;
    bool altura;
};

void despegar(int id, bool waiting){
    while(true){
        if(waiting){
                    m.lock();
                    cout<<"Dron " <<id<< " esperando para despegar..."<<endl;
                    waiting = false;
                    m3.unlock();
                } 
        else if(try_lock(m1,m2,m3)==-1 ){
            cout <<"Dron "<<id<<" despegando...\n";
            this_thread::sleep_for(chrono::milliseconds(500));
            cout<<"Dron " <<id<< " alcanzó altura de 10m"<<endl;
            m1.unlock();m2.unlock();m3.unlock();
            break;
        }
    }

    
}

int main(){
    vector<thread> drones;
    for(int i = 0; i <5; i++){
        drones.emplace_back(despegar, i, true);
    }
    for(auto& d: drones) d.join();
    /*
    thread dron1(despegar,1,true);
    thread dron2(despegar,2,true);
    thread dron3(despegar,3,true);
    thread dron4(despegar,4,true);
    thread dron5(despegar,5,true);
    
    dron1.join();dron2.join();dron3.join();dron4.join();dron5.join();
    */
    return 0;
}