#include<string>
#include<iostream>
#include<map>

using namespace std;
struct person
{
    string name;
    int age;

    person(){

    }

    person(string name, int age)
    {
        this->name = name;
        this->age = age;
    }

    void setName(string name){

        this->name=name;
    }

    string getName(){

        return this->name;
    }

    void setAge(int age){

        this->age=age;
    }

    int getAge(){

        return this->age;
    }

    bool operator < (const person& p) const//运算符左侧对象为调用对象，右侧对象为传参对象
    {
        return this->name < p.name;
    }
};

