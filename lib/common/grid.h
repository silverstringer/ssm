#ifndef SSM_GRID_H
#define SSM_GRID_H

#include <vector>
#include <iostream>
#include <optional>


namespace Common {

    template<typename Cell>
    class Grid {
        using DataType = std::vector<Cell>;
    public:
        Grid(size_t column, size_t row) : m_column{column}, m_row{row} {
            m_data.resize(column * row);
        }

        auto begin() { return m_data.begin(); }

        auto end() { return m_data.end(); }

        auto &at(size_t x, size_t y) { return m_data[x * m_column + y]; }

        auto &value(size_t x, size_t y) { return m_data[x * m_column + y]; }

        auto &operator[](size_t index) {
            if (index < m_data.size())
                return m_data[index];
            else
                return std::nullopt;
        }

        auto getRow(size_t y) {
            auto left = begin() + m_column * y;
            auto right = left + m_column;
            return std::make_pair(left, right);
        }

        void print() const {
            for (std::size_t i = 0; i < m_data.size(); i++) {
                std::cout << m_data[i] << "\t";
                if ((i + 1) % m_column == 0) { std::cout << "\n"; }
            }
        }

        void setDataGrid(DataType &data) {
            m_data = data;
        }

        auto getSizeGrid() const {
            return std::make_pair(m_column, m_row);
        }

        auto byColumn(const int column) {
            DataType data_column;
            for (auto i = 0; i < m_data.size() / m_column; i++)
                data_column.push_back(at(i, column));

            return data_column;
        }

        //todo: usage multimap
        auto byColumns(const int col1, const int col2) {
            std::map<Cell, Cell> data_columns;
            for (size_t rows = 0; rows < getSizeGrid().second; rows++)
                data_columns.insert(std::make_pair(this->value(rows, col1), this->value(rows, col2)));
            return data_columns;
        }

    protected:
        template<typename ...Ts>
        [[maybe_unused]]
        auto &addRow(Ts ... data) {
            (m_data.push_back(data), ...);
            m_row++;
            return *this;
        }

        /**
         *
         * @tparam T add one row
         * @param row
         * @return
         */
        template<typename T>
        auto &addRow(std::initializer_list<T> row) {
            m_data.insert(m_data.end(), row.begin(), row.end());
            m_row++;
            return *this;
        }

    private:
        DataType m_data{};
        size_t m_column{};
        size_t m_row{};
    };
}

#endif //SSM_GRID_H
