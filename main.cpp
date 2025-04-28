#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include "headerL4.h"

using namespace std;

class EuropeCars
{
protected:
    string brand;
    string country;
    int start_year;

public:
    EuropeCars(const string& brand, const string& country, int start_year)
        : brand(brand), country(country), start_year(start_year) {}

    virtual ~EuropeCars() {}

    virtual void output_parametrs_1() const
    {
        cout << "Brand: " << brand << endl;
    }

    virtual void output_parametrs_2() const
    {
        cout << "Country: " << country << endl;
    }

    virtual void output_parametrs_3() const
    {
        cout << "Start of production: " << start_year << endl;
    }

    virtual void start_year_before_2000() const
    {
        if (start_year < 2000)
        {
            cout << "The car began to be produced before 2000." << endl;
        }
    }

    virtual void output_parametrs() const = 0;

    virtual void change_country() {}
    virtual void year_del_2() {}
    virtual void input_country() {}
};

class Skoda_Superb : public EuropeCars
{
private:
    string color;

public:
    Skoda_Superb(const string& brand, const string& country, int start_year, const string& color)
        : EuropeCars(brand, country, start_year), color(color) {}

    void output_parametrs() const override
    {
        output_parametrs_1();
        output_parametrs_2();
        output_parametrs_3();
        cout << "\tColor: " << color << endl;
    }

    void change_country() override
    {
        country = "Russia";
        cout << "The country has changed to: " << country << endl;
    }
};

class Audi_TT : public EuropeCars
{
private:
    string platform;

public:
    Audi_TT(const string& brand, const string& country, int start_year, const string& platform)
        : EuropeCars(brand, country, start_year), platform(platform) {}

    void output_parametrs() const override
    {
        output_parametrs_1();
        output_parametrs_2();
        output_parametrs_3();
        cout << "\tPlatform: " << platform << endl;
    }

    void year_del_2() override
    {
        cout << "Start of production divided by 2: " << start_year / 2 << endl;
    }
};

class Seat_Leon_FR : public EuropeCars
{
private:
    bool diesel_engine;

public:
    Seat_Leon_FR(const string& brand, const string& country, int start_year, bool diesel_engine)
        : EuropeCars(brand, country, start_year), diesel_engine(diesel_engine) {}

    void output_parametrs() const override
    {
        output_parametrs_1();
        output_parametrs_2();
        output_parametrs_3();
        cout << "\tIs the engine diesel (1 - yes, 0 - no)?  " << diesel_engine << endl;
    }

    void input_country() override
    {
        cout << "Enter the country of manufacture: ";
        cin >> country;
    }
};

EuropeCars* create_EuropeCars(int type)
{
    switch (type)
    {
    case 0:
        return new Skoda_Superb("Skoda Superb", "Czech Republic", 2001, "Black");
    case 1:
        return new Audi_TT("Audi TT", "Germany", 1998, "MQB");
    case 2:
        return new Seat_Leon_FR("Seat Leon FR", "Spain", 2005, true);
    default:
        return nullptr;
    }
}

template <typename Iterator>
void traverseContainer(Iterator& it)
{
    for (it.First(); !it.IsDone(); it.Next())
    {
        it.GetCurrent()->output_parametrs();
        it.GetCurrent()->year_del_2();
    }
}

int main()
{
    IContainer<EuropeCars*>* cont_stack = new ContainerStack<EuropeCars*>();
    IContainer<EuropeCars*>* cont_list = new ContainerList<EuropeCars*>();

    auto* stack = dynamic_cast<ContainerStack<EuropeCars*>*>(cont_stack);
    auto* list = dynamic_cast<ContainerList<EuropeCars*>*>(cont_list);

    for (int i = 0; i < 5; i++)
    {
        int numb = rand() % 3;
        stack->Push(create_EuropeCars(numb));

        numb = rand() % 3;
        list->add_element(create_EuropeCars(numb));
    }

    cout << "Stack Container (STL Iterator Adapter):" << endl;
    STLAdapter<EuropeCars*> stlAdapterStack(stack->GetIterator());
    traverseContainer(stlAdapterStack);

    cout << "\nList Container (STL Iterator Adapter):" << endl;
    STLAdapter<EuropeCars*> stlAdapterList(list->GetIterator());
    traverseContainer(stlAdapterList);

    delete cont_stack;
    delete cont_list;

    return 0;
}
