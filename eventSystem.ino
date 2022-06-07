
class event{
  public:
    int id;
    long starting_time;
    int index;
    int port;
    int duration;
    int while_event;
    int delay_time;
    bool repeat;
    void define(int port_, int duration_, int while_event_ = HIGH, int delay_time_ = 0, bool repeat_ = false){
      this->port = port_;
      this->duration = duration_;
      this->while_event = while_event_;
      this->delay_time = delay_time_;
      this->repeat = repeat_;
    }
    
};

class functionEvent{
  public:
    int id;
    long starting_time;
    int index;
    void (*func);
    int delay_time;
    bool repeat;
    void define(void func_(), int delay_time_ = 0, bool repeat_ = false){
      this->func = (*func_);
      this->delay_time = delay_time_;
      this->repeat = repeat_;
    }


};

event allEvents[10];
int event_amount = 0;
int last_id = 0;

void AssignEvent(event new_event){
  new_event.starting_time = millis();
  new_event.id = last_id;
  last_id++;
  new_event.index = event_amount;
  allEvents[event_amount] = new_event;
  event_amount++;
}

void StopRepeating(event event_){
  for (byte k=0; k < event_amount; k++){
    if (allEvents[k].id == event_.id){
      allEvents[k].repeat = false;  
    }  
  }  
}

event led;

void setup() {
  pinMode(A0, OUTPUT);
  
  Serial.begin(9600);
  led.define(A0, 500, 160, 500, true);
  AssignEvent(led);
}

void loop() {
  Serial.println(millis());
  RunEvents();
  if(millis() > 5000){
    StopRepeating(led);
  }
}

void RunEvents() {

  for (byte i=0; i < event_amount; i++){
    event current_event = allEvents[i];
    long current_time = millis();
    long time_past = current_time - current_event.starting_time;
    
    if (time_past - current_event.delay_time < current_event.duration && time_past - current_event.delay_time > 0){
      analogWrite(current_event.port, current_event.while_event);
    }
    
    else if(time_past - current_event.delay_time > current_event.duration){
      if(!current_event.repeat){
        event_amount--;
        analogWrite(current_event.port, 0);
        for (byte j=current_event.index; j < event_amount; j++){
          allEvents[j] = allEvents[j+1];
          allEvents[j].index = j;
        }
      }
      
      else{
        allEvents[i].starting_time = millis();
        analogWrite(current_event.port, 0);
      }
    }
  }
  
}
