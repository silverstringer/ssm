#include <sstream>
#include <fstream>
#include <sstream>

#include <iostream>
#include <string>
#include <vector>
#include <utility>

class CSVRead final   {
    using string = std::string;
public:
   explicit CSVRead(string filename, char delimeter = ';') : m_filename(filename), m_delimeter(delimeter)
   {
        if(existsFile()) {
        } else {
            std::cerr<<"File not exists!";
        }
    }

    string const &operator[](std::size_t index) const {
        return m_data[index];
    }

    std::size_t size() const {
        return m_data.size();
    }

    void readNextRow(std::istream &str) {
        string line;
        std::getline(str, line);
        std::stringstream lineStream(line);
        string cell;

        while (std::getline(lineStream, cell, m_delimeter))
        {
            m_data.push_back(cell);
        }

        if (!lineStream && cell.empty()) {
            m_data.push_back("");
        }
    }

    string inline getFilename() const
    {
        return m_filename;
    }
    char inline getDelimeter() const
    {
      return m_delimeter;
    }


    std::vector<string> getData() const
    {
        return m_data;
    }

    friend std::istream &operator>>(std::istream &str, CSVRead &data)
    {
        data.readNextRow(str);
        return str;
    }
    friend void readCSV(CSVRead &csv)
    {
        std::string filename = csv.getFilename();
        std::ifstream file(filename);
        while (file >> csv) {
            csv.m_counterRows++;
        }
    }

    /**
     * @brief
     * @return column x rows
     */
    std::pair<int, int > getSizeGrid()
    {
       if(m_counterRows > 0)
       m_counterColumn = this->size()/m_counterRows;
       return std::make_pair(m_counterColumn, m_counterRows);
    }

protected:
    inline bool existsFile()
    {
        std::ifstream f(m_filename.c_str());
        return f.good();
    }

    int  m_counterRows {0}, m_counterColumn {0};

private:
    std::vector<string> m_data;
    string m_filename;
    char m_delimeter;
};


static std::vector<std::vector<std::string>> read_csv(std::string filename) {
    std::vector<std::vector<std::string>> result;
    std::ifstream infile(filename);
    std::string line;
    while (getline(infile, line)) {
        std::vector<std::string> res_line;
        size_t pos;
        std::string item;
        while ((pos = line.find(";")) != std::string::npos) {
            item = line.substr(0, pos);
            res_line.push_back(item);
            line.erase(0, pos + 1);
        }
        res_line.push_back(line);
        result.push_back(res_line);
    }

    return result;
}

//static std::vector <std::map<string, string>> read_csv_map(string filename) {
//    std::vector<std::map<string, string>> result;
//
//    auto csv = read_csv(filename);
//
//    if (csv.size() < 2)
//        return result;
//
//    auto names = csv[0];
//    for (int i = 1; i < csv.size(); i++) {
//        std::map <string, string> line;
//        for (int j = 0; j < csv[i].size(); j++) {
//            //check column name
//            if (j >= names.size())
//                elog("no name for column " + to_string(j) +
//                     " line " + to_string(i) +
//                     " file " + filename);
//            line[names[j]] = csv[i][j];
//        }
//        result.push_back(line);
//    }
//
//    return result;
//};





//int main() {
//
//    CSVRead reader_csv("/tmp/log.csv");
//    readCSV(reader_csv);
//    int column = 3;
//
////    for(auto counter = 0 ; counter < reader.size() ; counter++)
////    std::cout<<"Item " <<counter << " : "<<reader[counter] <<"\n";
//
//    auto grid = Grid<string>(column, reader_csv.getRow / column);
////
//    std::vector<string> r_data{std::move(reader_csv.getData())};
//    grid.setDataGrid(r_data);
//
////    for (auto i = 0; i < reader.size() / column; i++)
////        std::cout << " Line: " << i << ": \t" << grid.at(i, 2) << "\n";
//
//
//   const std::string  proto = "cdcdcdcd";
//   int counter_tx_packet { 0 }, counter_rx_packet {0} , counter_rx_empty_packet {0};


//   for(auto &item: grid.byColumn(2))
//        if (item.find(proto) != std::string::npos) {
////            std::cout << "Line: " << item << '\n';
//            counter_tx_packet++;
//        }
//   else
//        {
//            counter_rx_packet++;
//            if(item.empty())
//                counter_rx_empty_packet++;
//        }
//
//   std::cout<<"Common statistics: \n";
//   std::cout<<"Counter TX packets: " <<counter_tx_packet <<" \n";
//   std::cout<<"Counter RX packets: " <<counter_rx_packet <<" \n";
//   std::cout<<"Counter Empty RX  packets[!?]: " << counter_rx_empty_packet <<" \n";
//   std::cout<<"Counter TX/RX packets: " << grid.getSizeGrid().second <<" \n";
//   //algo work glue [bad packets]
//   auto gluePackets = 311;
//    double glue_packets_in_percentage =(double)gluePackets * 100 / counter_rx_packet;
//   std::cout<<"Good  packets: " << counter_rx_packet - gluePackets <<"\t [" <<100 - (double)gluePackets * 100 / counter_rx_packet  <<"%]"<< "\n";
//   std::cout<<"Glue  packets: " << gluePackets <<" \t ["<<glue_packets_in_percentage  <<"%] \n";
//
//   std::cout<<"By type packets: ";
//
//
//
////    auto r = grid.get_row(48);
////    auto &val = grid.at(0,2);
////    std::cout<<" \n Line 2: " << val <<" \n";
////    std::cout<<" \n Line 3: " << grid.at(1,2) <<" \n";
////    std::cout<<" \n Line 4: " << grid.at(2,2) <<" \n";
//
////    grid.print_grid();
//
////    auto csv_vec = read_csv("/tmp/log.csv");
////    std::cout<<" \t Counter field : " <<csv_vec.size();
//
//}


