#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
using namespace std;
mutex coutmtx;

array<mutex,5> zonas;

struct Dron{
    int id;
    bool waiting = true;
    bool isFlying = false;
};

void despegar(Dron* d, vector<Dron>* data_drones){
    while(true){
        if(d->waiting){
                    coutmtx.lock();
                    cout<<"Dron " <<d->id<< " esperando para despegar..."<<endl;
                    d->waiting = false;
                    coutmtx.unlock();
                } 
        else if(try_lock(zonas[(d->id-1)%5],zonas[d->id])==-1 ){
            coutmtx.lock();
            cout <<"Dron "<<d->id<<" despegando...\n";
            coutmtx.unlock();
            this_thread::sleep_for(chrono::milliseconds(500));
            coutmtx.lock();
            cout<<"Dron " <<d->id<< " alcanzó altura de 10m"<<endl;
            
            coutmtx.unlock();
            (*data_drones)[(d->id-1)%5].isFlying = true;
            (*data_drones)[d->id].isFlying = true;
            zonas[(d->id-1)%5].unlock();zonas[d->id].unlock();
            break;
        }
    else if((*data_drones)[(d->id-1)%5].isFlying  ||  ( (*data_drones)[d->id].isFlying )){
        
    }

    } 
}
//las zonas son el recurso compartido
int main(){
    vector<thread> drones;
    vector<Dron> data_drones;
    for (int i = 0; i < 5; i++) {
        data_drones.emplace_back(i, true, false); 
    }
    for(int i = 0; i <5; i++){
        drones.emplace_back(despegar,&data_drones[i], &data_drones);
    }
    for(auto& dron: drones) dron.join();

    return 0;
}