#include "dca.h"

namespace Strategy {

dca::dca(double assest, double price, double goal_price) {
    this->assets = assest;
    this->price = price;
    this->goal_price = goal_price;
}

void dca::clear()
{
    assets = 0;
    price = 0;
    goal_price = 0;
    goal_range.clear();
}

DiffPercent::DiffPercent(double depo, double percent, double counterMonth, double refill)
{
    deposit = depo;
    percent = percent;
    step = counterMonth;
    refill = refill;
}

void DiffPercent::convertPercentage()
{
    double Coeff {0};
    auto getCoefficient = [&](){
    for( auto i = 1; i < 10; i++)
    {
        if(isBoundRange(i * 100 , (i + 1) *100 , (int)percent))
        {
            Coeff = 1 + percent / 100;
        }
    }
    };

    getCoefficient();


   if(isBoundRange(0, 99, static_cast<int>(percent)))
       percent = 1 + percent / 100;
   if(percent == 100)
       percent = 2;
   if(Coeff != 0 )
       percent= Coeff;

//   assert(percent >100 && "Error!Percent can't larger 100");

}

void DiffPercent::clear()
{
    deposit = 0;
    percent = 0;
    step = 0;
}

}

