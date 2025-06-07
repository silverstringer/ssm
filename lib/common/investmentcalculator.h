#ifndef SSM_INVESTMENTCALCULATOR_H
#define SSM_INVESTMENTCALCULATOR_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <limits>
#include <algorithm>
#include <thread>
#include <mutex>
#include <future>


#define MINSTEP 10
#define MAXSTEP 20

class InvestmentStrategy {
private:
    int currentLots;
    double currentAvgPrice;
    double targetAvgPrice;
    double availableBudget;
    double priceStep;
    double maxPurchasePercent;
    bool autoStepEnabled;

public:
    InvestmentStrategy(int lots, double avgPrice, double targetPrice,
                       double budget = std::numeric_limits<double>::max(),
                       double step = 0.0, double maxPercent = 0.3)
            : currentLots(lots), currentAvgPrice(avgPrice), targetAvgPrice(targetPrice),
              availableBudget(budget), priceStep(step), maxPurchasePercent(maxPercent) {
        autoStepEnabled = (priceStep <= 0.0);
        if (autoStepEnabled) {
            priceStep = std::max(0.01, (currentAvgPrice - targetAvgPrice) * 0.05);
        }
    }

    struct Result {
        std::vector<std::tuple<double, int, double>> purchases;
        double finalAvgPrice;
        double totalCost;
        int totalLots;
        double priceStepUsed;
    };

    Result calculate() const {
        Result result;
        result.priceStepUsed = priceStep;
        double remainingBudget = availableBudget;
        double tempLots = currentLots;
        double tempAvgPrice = currentAvgPrice;
        double currentMarketPrice = currentAvgPrice * 0.95;
        bool hasBudget = (availableBudget < std::numeric_limits<double>::max());

        int maxSteps = autoStepEnabled ? MINSTEP : static_cast<int>((currentAvgPrice - targetAvgPrice) / priceStep) + 1;
        maxSteps = std::min(maxSteps, MAXSTEP);

        for (int step = 0; step < maxSteps; ++step) {
            if (tempAvgPrice <= targetAvgPrice + 0.001) break;
            if (hasBudget && remainingBudget <= 0) break;
            if (currentMarketPrice <= 0) break;

            auto [lotsToBuy, cost] = calculateOptimalLots(tempLots, tempAvgPrice,
                                                          currentMarketPrice, remainingBudget);

            if (lotsToBuy > 0) {
                result.purchases.emplace_back(currentMarketPrice, lotsToBuy, cost);
                remainingBudget -= cost;
                tempAvgPrice = (tempLots * tempAvgPrice + lotsToBuy * currentMarketPrice) / (tempLots + lotsToBuy);
                tempLots += lotsToBuy;
            }

            currentMarketPrice -= priceStep;
            if (currentMarketPrice < targetAvgPrice * 0.9) {
                currentMarketPrice = targetAvgPrice * 0.9;
            }
        }

        result.finalAvgPrice = tempAvgPrice;
        result.totalCost = availableBudget - remainingBudget;
        result.totalLots = tempLots;
        return result;
    }

private:
    std::pair<int, double> calculateOptimalLots(double currentLots, double currentAvg,
                                                double price, double remainingBudget) const {
        if (price >= currentAvg) return {0, 0};

        bool hasBudget = (remainingBudget < std::numeric_limits<double>::max());
        double maxPossibleByBudget = hasBudget ? remainingBudget / price : std::numeric_limits<double>::max();

        double maxByPortfolioSize = currentLots * maxPurchasePercent;
        double requiredForTarget = currentLots * (currentAvg - targetAvgPrice) / (targetAvgPrice - price);

        int lotsToBuy = static_cast<int>(std::min({
                                                          maxPossibleByBudget,
                                                          maxByPortfolioSize,
                                                          std::max(1.0, requiredForTarget * 0.8)
                                                  }));

        if (lotsToBuy <= 0) return {0, 0};

        double totalCost = lotsToBuy * price;
        return {lotsToBuy, totalCost};
    }
};

class InvestmentCalculator {
public:
    static void compareStrategies(int currentLots, double currentAvgPrice,
                                  double targetAvgPrice, double budget) {
        std::vector<std::future<InvestmentStrategy::Result>> futures;
        std::vector<double> stepsToTest = {0.0, 1.0, 2.0,3.0, 4.0,  5.0, 6.0 }; // 0.0 - автошаг

        for (double step : stepsToTest) {
            futures.push_back(std::async(std::launch::async, [=]() {
                InvestmentStrategy strategy(currentLots, currentAvgPrice,
                                            targetAvgPrice, budget, step);
                return strategy.calculate();
            }));
        }

        std::vector<InvestmentStrategy::Result> results;
        for (auto& future : futures) {
            results.push_back(future.get());
        }

        printComparison(results, currentAvgPrice, targetAvgPrice, budget);
    }

private:
    static void printComparison(const std::vector<InvestmentStrategy::Result>& results,
                                double currentAvgPrice, double targetAvgPrice,
                                double budget) {
        std::cout << "\nСравнение стратегий покупки:\n";
        std::cout << "====================================================================\n";
        std::cout << "| Шаг цены | Лотов | Ср.цена | Затраты | Близость к цели | Эффект.\n";
        std::cout << "====================================================================\n";

        for (const auto& result : results) {
            double distanceToTarget = std::abs(result.finalAvgPrice - targetAvgPrice);
            double efficiency = (currentAvgPrice - result.finalAvgPrice) / result.totalCost * 1000;

            std::cout << "| " << std::setw(8) << std::fixed << std::setprecision(2)
                      << (result.priceStepUsed == 0.0 ? "авто" : std::to_string(result.priceStepUsed))
                      << " | " << std::setw(5) << result.totalLots
                      << " | " << std::setw(7) << result.finalAvgPrice
                      << " | " << std::setw(7) << result.totalCost
                      << " | " << std::setw(14) << distanceToTarget
                      << " | " << std::setw(7) << efficiency << "\n";
        }
        std::cout << "====================================================================\n";

        // best strategy
        auto best = std::min_element(results.begin(), results.end(),
                                     [targetAvgPrice](const auto& a, const auto& b) {
                                         return std::abs(a.finalAvgPrice - targetAvgPrice) <
                                                std::abs(b.finalAvgPrice - targetAvgPrice);
                                     });

        std::cout << "\nЛучшая стратегия: шаг "
                  << (best->priceStepUsed == 0.0 ? "авто" : std::to_string(best->priceStepUsed))
                  << " руб.\n";
        printDetailedResult(*best);
    }

    static void printDetailedResult(const InvestmentStrategy::Result& result) {
        std::cout << "\nДетализация лучшей стратегии:\n";
        std::cout << "==================================================\n";
        std::cout << std::setw(12) << "Цена" << std::setw(15) << "Лоты"
                  << std::setw(15) << "Стоимость" << std::setw(20) << "Ср. цена после\n";

        double tempLots = 0;
        double tempAvgPrice = 0;
        double totalLots = 0;
        double totalCost = 0;

        for (const auto& [price, lots, cost] : result.purchases) {
            tempAvgPrice = (tempLots * tempAvgPrice + lots * price) / (tempLots + lots);
            tempLots += lots;
            totalCost += cost;

            std::cout << std::setw(12) << std::fixed << std::setprecision(2) << price
                      << std::setw(15) << lots
                      << std::setw(15) << cost
                      << std::setw(20) << tempAvgPrice << "\n";
        }

        std::cout << "==================================================\n";
        std::cout << "Итоговая средняя цена: " << result.finalAvgPrice << " руб.\n";
        std::cout << "Общие затраты: " << result.totalCost << " руб.\n";
        std::cout << "Общее количество лотов: " << result.totalLots << "\n";
    }
};



#endif //SSM_INVESTMENTCALCULATOR_H
