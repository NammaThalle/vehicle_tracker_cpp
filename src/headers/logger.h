#ifndef LOGGER_H
#define LOGGER_H

#include <mutex>
#include <chrono>
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

#define LOG(level, message) Logger::getInstance().log(level, message, __FILE__) 

class Logger 
{
    private:
        Logger() : logFilePath("logs/vehicle_tracking.log") {}

        // To make the class singleton delete the copy or 
        // instantiation functionalities of the Logger
        Logger(Logger const&) = delete;
        void operator=(Logger const&) = delete;

        ofstream logFile;
        string logFilePath;
        
        // Mutex for thread safety
        mutex mtx; 

    public:
        static Logger& getInstance() 
        {
            static Logger instance; 
            
            // Instantiated on first use
            return instance;
        }

        // Log a message with a specified log level
        void log(const std::string& level, const std::string& message, const std::string& file);

        // Set the log file path (optional)
        void setLogFilePath(const string& filePath);

        // Helper function to get the current timestamp
        string getCurrentTimestamp() const;
};

#endif // LOGGER_H