#include <sstream>
#include "logger.h"

using namespace std;

string Logger::getCurrentTimestamp() const {
    auto now = chrono::system_clock::now();
    auto in_time_t = chrono::system_clock::to_time_t(now);

    stringstream ss;
    ss << put_time(localtime(&in_time_t),"%Y-%m-%d %H:%M:%S"); 

    return ss.str();
}

void Logger::log(const string& level, const string& message, const string& file) {
    lock_guard<mutex> lock(mtx);

    string timestamp = getCurrentTimestamp();

    // Extract the filename from the full path (same logic as before)
    size_t lastSlashPos = file.find_last_of("/\\");
    string filename = (lastSlashPos != string::npos) ? file.substr(lastSlashPos + 1) : file;

    // Determine if log should go to console or file
    bool logToConsole = (level == "INFO" || level == "WARN" || level == "ERROR" || level == "FATAL");

    if (logToConsole) {
        // Log to console
        cout << timestamp << " [" << filename << "] [" << level << "] : " << message << endl;
    }

    // Always log to file (if file path is set)
    if (!logFilePath.empty()) {
        if (!logFile.is_open()) {
            logFile.open(logFilePath, ios_base::app);
        }
        if (logFile.is_open()) {
            logFile << timestamp << " [" << filename << "] [" << level << "] : " << message << endl;
        }
    }
}

void Logger::setLogFilePath(const string& filePath) {
    // Acquire lock    
    lock_guard<mutex> lock(mtx); 
    logFilePath = filePath;
    if (logFile.is_open()) {
        // Close the old file if open
        logFile.close(); 
    }
}