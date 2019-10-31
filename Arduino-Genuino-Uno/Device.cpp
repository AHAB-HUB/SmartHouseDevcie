#include "Device.h"



int Device::getId(){
  return id;
}

char* Device::getName(){
  return name;
}

char* Device::getStatus(){
  return state;
}

void Device::setID(int id1){
    id = id1;
}
  
void Device::setName(char* name1){
    name = name1;
}
  
void Device::setStatus(char* state1){
  state = state1;
}

 void Device::operator=(const Device f) const{
  setID( f.getId() );
  setName( f.getName() );
  setStatus( f.getStatus() );
}
