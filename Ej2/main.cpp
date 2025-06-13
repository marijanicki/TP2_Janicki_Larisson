#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
using namespace std;
mutex coutmtx;

array<mutex,5> zonas;

void despegar(int id, bool waiting, bool altura){
    while(true){
        if(waiting){
                    coutmtx.lock();
                    cout<<"Dron " <<id<< " esperando para despegar..."<<endl;
                    waiting = false;
                    coutmtx.unlock();
                } 
        else if(try_lock(zonas[(id-1)%5],zonas[id])==-1 ){
            coutmtx.lock();
            cout <<"Dron "<<id<<" despegando...\n";
            coutmtx.unlock();
            this_thread::sleep_for(chrono::milliseconds(500));
            coutmtx.lock();
            cout<<"Dron " <<id<< " alcanzó altura de 10m"<<endl;
            coutmtx.unlock();
            zonas[(id-1)%5].unlock();zonas[id].unlock();
            break;
        }

    } 
}
//las zonas son el recurso compartido
int main(){
    vector<thread> threads;
    for(int i = 0; i <5; i++){
        threads.emplace_back(despegar,i,true);
    }
    for(auto& d: threads) d.join();

    return 0;
}