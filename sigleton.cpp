#include <iostream>
#include <string>
#include <mutex>
using namespace std;

#if 0
/* 饿汉式 */
class Person {
private:
    Person() {
        name = "我是饿汉式";
    };
    Person(Person&) {};
public:
    static Person* getInstance() {
        return instance;
    }
    const string& getName() const { return name;}
private:
    static Person* instance;
    string name;
};
Person* Person::instance = new Person;

#elif 0
/* 懒汉式 */
class Person {
private:
    Person() { name = "我是懒汉式";}
    Person(Person&) {};
public:
    static Person* getInstance() {
        if (instance == nullptr)
           instance = new Person;
        return instance;
    }
    const string& getName() const {return name;}

     /* 加上资源销毁类 */
    class Gg {
    public:
        ~Gg() {
            if (instance != nullptr)
                delete instance;
        }
    };
    static Gg gg;
private:
    string name;
    static Person* instance;
};
Person* Person::instance = NULL;

#else 
/* 懒汉式加双校验 线程安全 */
class Person {
private:
    Person() { name = "我是线程安全的懒汉式";}
    Person(Person&) {};
public:
    static Person* getInstance() {
        if (instance == nullptr) {
            mut.lock();
            if (instance == nullptr)
                instance = new Person;
            mut.unlock();
        }
        return instance;
    }
    const string& getName() const {return name;}

    /* 加上资源销毁类 */
    class Gg {
    public:
        ~Gg() {
            if (instance != nullptr)
                delete instance;
        }
    };
    static Gg gg;
private:
    string name;
    static Person* volatile instance;
    static mutex mut;
};
Person* volatile Person::instance = nullptr;
mutex Person::mut;
#endif


int main(int argc, char const *argv[])
{
    cout << Person::getInstance()->getName() << endl;
    return 0;
}
