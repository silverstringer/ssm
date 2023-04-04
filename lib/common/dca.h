#ifndef DCA_H
#define DCA_H

#include <utility>
#include <map>
#include <cmath>
#include <assert.h>
#include <iostream>
#include <thread>
#include <vector>
#include <iterator>


namespace Strategy {

template <typename T>
bool isBoundRange(const T low, const  T high, const T value) {
    return ((low <= value) && (value <= high ));
}
    static inline std::vector<std::pair<int,int>> make_sub_range(int range)
    {
        int number_of_elements { range};
        auto num_pthread {std::thread::hardware_concurrency() - 1 };
        auto number_of_period {num_pthread};
        std::vector<std::pair<int,int>> sub_range;

        auto found_count_pthread =[&]() {
            for(auto i = num_pthread; i < num_pthread * 2 ; i++)
            {
                if (number_of_elements % number_of_period == 0)
                    return number_of_period;
                else
                    number_of_period++;
            };

            return number_of_period;
        };

        found_count_pthread();

        for(unsigned int  period = 1; period <= number_of_period; period++)
            sub_range.push_back(std::make_pair(number_of_elements/number_of_period * (period - 1 ), (number_of_elements/number_of_period * period)));

        return sub_range;
    }

//namespace container{}

struct dca {
    double assets {0};
    double price  {0};
    double goal_price {0};
    std::map< double, double> goal_range;

    dca() = default;
    explicit dca(double assets, double price, double goal_price);
    void clear();
};

/**
 * @brief The diffPercent struct
 * @note  calculate for month
 */
struct DiffPercent
{
    DiffPercent(double depo, double percent, double counterMonth);
    double deposit;
    double m_percent;
    double step;

    void convertPercentage();
    void clear ();
};


double static calculateDiffPercent(DiffPercent &dps)
{
    dps.convertPercentage();
    double result = dps.deposit * pow(dps.m_percent, dps.step - 1);
    return result;
}

void static  calculateDCA(dca & res, int min_range = 0, int max_range = 0) {

    auto first_total_sum = res.assets * res.price;
    res.goal_range.clear();

    assert(max_range != 0 && " max_range <= min_range");

    for( ; min_range < max_range ; min_range ++) {
        for(auto range_price = 0; range_price <= res.goal_price; range_price++) {
            auto range_sum = min_range * range_price;

            auto total_sum = range_sum + first_total_sum;
            auto find_down_price = total_sum /(res.assets + min_range);

            auto diff_price = res.goal_price - find_down_price;

            if(diff_price == 0 )
            {
                res.goal_range[min_range] = range_price;
            }
        }
    }
//    for(const auto [range_assets, range_price] : res.goal_range)
//    {
//        std::cout<< "Range assets " <<range_assets << " \t"<<"Range price:" <<range_price <<" \n ";
//    }

}
}

#endif // DCA_H
