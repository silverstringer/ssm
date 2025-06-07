#ifndef SSM_STORAGE_H
#define SSM_STORAGE_H


struct CandleType {

};

namespace CON {

    namespace REALTIME {

        struct Candles {
        };

        namespace SHARES {
            struct Trades {

            };
        }
        namespace FUTURES {
            struct Trade {

            };
        }
    }


    namespace SUPERCANDLES {
        struct TradeStatsInfo {

        };

    }

}


template<typename T>
class Storage {
public:
    Storage() = default;

    void add(T item) {
        m_data.emplace_back(item);
    }

    void remove(T item) {

    }

    bool isContains(T item) {
        auto result = find(first(), last(), item) != last();
        return result;
    }

    std::vector<T> &get() {
        return m_data;
    }

    auto size() {
        return m_data.size();
    }

    auto inline first() const {
        return m_data.begin();
    }

    auto inline last() const {
        return m_data.end();
    }

private:
    std::vector<T> m_data;
};

class TradesInfo {
public:
    explicit TradesInfo(const QString ticker, const QString typeTrades, const QString date) noexcept;

    bool operator==(const TradesInfo &lsh) const {};

    bool operator<(const TradesInfo &lsh) const {};

    bool operator>(const TradesInfo &lsh) const {};
private:
    QString m_date;       // date trades write to storage
    QString m_ticker;     // ticker
    QString m_typeTrades; // sell or buy
};

class CandleInfo {
public:
    explicit CandleInfo(const QString ticker, const CandleType typeCandle, const QString date) noexcept;

    bool operator==(const CandleInfo &lsh) const {};

    bool operator<(const CandleInfo &lsh) const {};

    bool operator>(const CandleInfo &lsh) const {};
private:
    QString m_date; //date candle write to storage
    QString m_ticker;
    CandleType m_type;
};


namespace API::V1 {

    template<typename T>
    concept CandleType = std::is_same_v<T, CON::REALTIME::Candles> ||
                         std::is_same_v<T, CON::SUPERCANDLES::TradeStatsInfo>;

    template<typename T>
    concept TradeType = std::is_same_v<T, CON::REALTIME::SHARES::Trades> ||
                        std::is_same_v<T, CON::REALTIME::FUTURES::Trade>;

    template<typename T, typename Info>
    concept ItemWithInfo = requires(T item, Info info) {
        { info == info } -> std::convertible_to<bool>;
        { info < info } -> std::convertible_to<bool>;
    };

    template<typename T, typename InfoType> requires ItemWithInfo<T, InfoType>
    class UniversalManager {
    public:
        using ItemT = T;
        using InfoT = InfoType;
        using StoragePtr = std::shared_ptr<Storage<T>>;
        using Container = std::map<InfoType, StoragePtr>;

        UniversalManager() = default;

        virtual ~UniversalManager() = default;

        void add(const InfoType &info, const T &item) {
            auto it = m_storage.find(info);
            if (it == m_storage.end()) {
                auto storage = std::make_shared<Storage<T>>();
                storage->add(item);
                m_storage.emplace(info, storage);
            } else {
                if (!it->second->isContains(item)) {
                    it->second->add(item);
                }
            }
        }

        bool remove(const InfoType &info) {
            return m_storage.erase(info) > 0;
        }

        std::optional<StoragePtr> get(const InfoType &info) const {
            auto it = m_storage.find(info);
            return it != m_storage.end() ? std::make_optional(it->second) : std::nullopt;
        }

        auto begin(const InfoType &info) const {
            if (auto storage = get(info); storage && !storage.value()->empty()) {
                return storage.value()->first();
            }
            throw std::runtime_error("Storage is empty or not found");
        }

        auto end(const InfoType &info) const {
            if (auto storage = get(info); storage && !storage.value()->empty()) {
                return storage.value()->last();
            }
            throw std::runtime_error("Storage is empty or not found");
        }

        const Container &getAll() const { return m_storage; }

        size_t count() const { return m_storage.size(); }

        bool empty() const { return m_storage.empty(); }

    private:
        Container m_storage;
    };

    using CandleManager = UniversalManager<
            CON::REALTIME::Candles,
            CandleInfo
    >;

    using SuperCandleManager = UniversalManager<
            CON::SUPERCANDLES::TradeStatsInfo,
            CandleInfo
    >;

    using ShareTradesManager = UniversalManager<
            CON::REALTIME::SHARES::Trades,
            TradesInfo
    >;

    using FuturesTradesManager = UniversalManager<
            CON::REALTIME::FUTURES::Trade,
            TradesInfo
    >;


    template<typename T, typename InfoType>
    class CandleManagerSep {
    public:
        static_assert(CandleType<T>, "This manager is only for candle types");
        // release  as UniversalManager
    };

    template<typename T, typename InfoType>
    class TradeManagerSep {
    public:
        static_assert(TradeType<T>, "This manager is only for trade types");
        //release  as UniversalManager
    };
}

#endif //SSM_STORAGE_H
