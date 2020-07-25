class Singleton
{
public:
  static Singleton& get(){
    static Singleton instance;
    return instance;
  }

  Singleton(const Singleton&) = delete;

  

private:
  Singleton() { }
  static instance = 

};

class Test : public Singleton
{

};