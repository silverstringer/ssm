#ifndef LOGGER_H
#define LOGGER_H

#include <vector>
#include <sstream>
#include <iterator>
#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <type_traits>
#include <utility>
#include <typeinfo>
#include <filesystem>
#include <iomanip>
#include <ctime>
#include <chrono>


#define DEFAULT_DIR "/.ssm"

template< typename VALUE, typename SFINAE = void >
struct StrongTypedefValidator {};


template<>
struct [[maybe_unused]] StrongTypedefValidator< std::string, std::enable_if< true >::type > final
{
    bool operator ()( const std::string & value ) const noexcept
    {
        return !value.empty();
    }
};


template<>
struct [[maybe_unused]] StrongTypedefValidator< QString, std::enable_if< true >::type > final
{
    bool operator ()( const QString & value ) const noexcept
    {
        return !value.isNull() && !value.isEmpty();
    }
};

std::string inline  getDateTime ()
{
    using std::chrono::system_clock;
    std::time_t tt = system_clock::to_time_t (system_clock::now());
    std::stringstream output_stream;
    struct std::tm * ptm = std::localtime(&tt);
    output_stream << std::put_time(ptm,"%F %T");
    return output_stream.str();
}

namespace Common::Logger {

typedef  std::vector<std::string> FieldType ;
typedef  std::vector<QString> FieldQType ;

    enum LogLevel
    {
        Trace,
        Debug,
        Info,
        Warning,
        Error,
        Fatal
    };

    enum Type {
        None,
        Console,
        File,
        CSV
    };

    std::string  static levelToString(Logger::LogLevel logLevel)
    {
        switch (logLevel)
        {
            case Trace:
                return "Trace";
            case Debug:
                return "Debug";
            case Info:
                return "Info";
            case Warning:
                return "Warning";
            case Error:
                return "Error";
            case Fatal:
                return "Fatal";
        }

        return std::string{};
    }

    static Logger::LogLevel levelFromString(const std::string& str)
    {

        LogLevel result = Debug;

        if (str == std::string("trace"))
            result = Trace;
        else if (str == std::string("debug"))
            result = Debug;
        else if (str == std::string("info"))
            result = Info;
        else if (str == std::string("warning"))
            result = Warning;
        else if (str == std::string("error"))
            result = Error;
        else if (str == std::string("fatal"))
            result = Fatal;

        return result;
    }

class ConsoleLogger;

/**
 * @brief The Logger class
 * @note usage default type string, QString, const char * ...
 * @todo refact base class or funct with param pack move to library?
 */
class Logger {
public:
    virtual ~Logger() = default;

    /**
     *@brief get first element from pack params
     */
    template <typename Tp, typename... Types>
    constexpr decltype(auto) front(Tp&& first, Types&&...) noexcept
    {
        return std::forward<Tp>(first);
    }

    template <typename... Types>
    auto prepareData(Types... data)->decltype(FieldType()) {

        return {data...};
    }

    template <typename... Types>
    auto prepareData(std::vector<std::string> data)->decltype(FieldType()) {
        return data;
    }

    template <typename... Types>
    auto prepareData(QString data)->decltype(FieldType()) {
        return {data.toStdString()};
    }

    //    template <typename... Types>
    //    auto prepareData(QString data ...)->decltype(FieldQType()) {
    //        return {data.toStdString()};
    //    }

    template<typename... Types>
    void show(Types... data) {
        auto message =  Logger::prepareData(data...);
        std::cout<<getDateTime() <<"\t";
        std::cout<<"[";
        for(const auto &var:message)
            std::cout<<"\t" <<var;
        std::cout<<"]";
        std::cout<<"\n";
    }

    std::string virtual    getName() { return "Base Logger";}
    Type virtual  getType() { return Type::None;}

};


class FileLogger final: public Logger {
public:
    FileLogger() = default;
    template <typename... T>
    [[maybe_unused]] void add(T... data)  { }
    std::string inline  getName() { return "FileLogger"; }
    Type inline getType() override{ return Type::File;}
};

class ConsoleLogger final: public Logger {
public:

    static ConsoleLogger * p_instanse;

    static ConsoleLogger * LoggerInstance() {
     if(p_instanse)
         p_instanse = new ConsoleLogger();
        return p_instanse;
    }

    template<typename... Types>
    void write(LogLevel logLevel, const char* file, int line, const char* function, Types... data){
        auto logLevelString = levelToString(logLevel);
        ConsoleLogger::LoggerInstance()->show(logLevelString, file, std::to_string(line) , function,  data...);
    }

    std::string inline   getName() { return "ConsoleLogger";   }
    Type inline getType() override{ return Type::Console;}

};

class CSVLogger final : public Logger
{
    using string = std::string;

    string path;
    string fileName;
    string delimeter;

    bool write_enabled;
    bool timestamp_enabled;

    template<typename T>
    void addDatainRow(T first, T last);

public:
    explicit CSVLogger(string directory = (getenv("HOME") != nullptr ? std::string(getenv("HOME")) : "/tmp") + std::string(DEFAULT_DIR),
                       string delm = ";", bool write = true, bool timestamp = false ) :
        path(directory), delimeter(delm), write_enabled(write), timestamp_enabled(timestamp)
    {}


    void inline setWriteEnabled(bool write)
    {
        this->write_enabled = write;
    }

    void inline setPath(string directory)  {
        this->path = directory + "/";
    }
    auto inline getPath () { return path; }
    auto inline getFilename() { return fileName; }
    auto inline isWriteEnabled() { return write_enabled;}

    void inline setFileName(string filename) {

        if(!write_enabled)
            return;

        if(!std::filesystem::exists(path))
            std::filesystem::create_directory(path);

        this->fileName = path + filename;
    }


    /**
     *  @brief write data to file
     *  \note usage container (ex: vector, list, etc., but  not map and triv. type data,)
     *
     */
    template <typename T>
    void addData(const T &data);

    void inline settings( string path = " ", string filename = " ", bool is_write = true, bool is_timestamp = false)
    {
        write_enabled = is_write;
        timestamp_enabled = is_timestamp;

        if(!path.empty())
            setPath(path);

        if(!filename.empty())
            setFileName(filename);
    }

    template <typename... T>
    void add(T... data) {
        FieldType param = Logger::prepareData(data...);
        addData(param);
    }

    template<typename... T>
    void operator()(T... data)
    {
        add(data...);
    }

    string inline getNameLogger ()
    {
        return "CSVLogger";
    }

    Type inline getType() override { return Type::CSV;}
};


template<typename T>
void CSVLogger::addDatainRow(T first, T last)
{
    if(!write_enabled)
        return;

    std::fstream file;

    try {
        file.open(fileName, std::ios::out | std::ios::app);
    }
    catch (std::exception &ex)
    {
        std::cerr<<ex.what() + fileName;
    }

    if(timestamp_enabled) {
        file << getDateTime()<< delimeter;
    }
    for(; first != last; )
    {

        file << *first;
        if(++first != last)
            file << delimeter;
    }
    file << "\n";

    file.close();
}


template <typename T = FieldType>
void CSVLogger::addData(const T &data)
{
    typename T::const_iterator first(data.begin());
    typename T::const_iterator last(data.end());

    if(std::is_arithmetic<T>::value)
        return;

    //check type if  our's container
    if((std::is_same <T, std::vector <std::string>>::value) | (std::is_same <T, std::list <std::string>>::value))
    {
        addDatainRow(first, last);
    }
    else
    {
        return;
    }
}

}

using  ConsoleLogger = Common::Logger::ConsoleLogger;
using  Loglevel = Common::Logger::LogLevel;
#define cuteLogger ConsoleLogger::LoggerInstance()

#define LOG_TRACE(args...)       cuteLogger->write(Loglevel::Trace,     __FILE__, __LINE__, __FUNCTION__, args);
#define LOG_DEBUG (args...)      cuteLogger->write(Loglevel::Debug,     __FILE__, __LINE__, __FUNCTION__, args);
#define LOG_INFO(args...)        cuteLogger->write(Loglevel::Info,      __FILE__, __LINE__, __FUNCTION__, args);
#define LOG_WARNING(args...)     cuteLogger->write(Loglevel::Warning,   __FILE__, __LINE__, __FUNCTION__, args);
#define LOG_ERROR(args...)       cuteLogger->write(Loglevel::Error,     __FILE__, __LINE__, __FUNCTION__, args);



#endif // LOGGER_H
