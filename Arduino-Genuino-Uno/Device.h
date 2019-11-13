#ifndef Device_H
#define Device_H


class Device{

  public: 
  Device( int ID = 0, char* name1 = " ", char* state1= "  ") : id(ID), name(name1), state(state1){};
  int getId();
  char* getName();
  char* getStatus();
  void setID(int id1);
  void setName(char* name1);
  void setStatus(char* state1);
  void operator=(const Device f) const;
    
  private:
  int id;
  char* name;
  char* state;
  };
  
#endif // !Device_H
