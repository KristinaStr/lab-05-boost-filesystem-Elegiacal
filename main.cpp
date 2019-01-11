//
//  main.cpp
//  lab_35 (filesystem)
//
//  Created by Артём Песоцкий on 25/11/2018.
//  Copyright © 2018 Артём Песоцкий. All rights reserved.
//

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <exception>
#include <boost/filesystem.hpp>

class Holder
{
    std::string broker;
    size_t files;
    std::string date;
public:
    Holder() = default;
    Holder(const std::string& b, const std::string& d,
           const size_t n = 1)
    : broker(b)
    , files(n)
    , date(d)
    {}
    Holder& update(const std::string& dateNew)
    {
        files += 1;
        if (dateNew > date)
        {
            date = dateNew;
        }
        return *this;
    }
    const std::string getBroker() const
    {
        return broker;
    }
    size_t getFiles() const
    {
        return files;
    }
    const std::string GetLastDate() const
    {
        return date;
    }
};
// Checks
bool oldCheck(const std::string& filename)
{
    return filename.find("old") == std::string::npos;
}
bool formatCheck(const std::string& filename)
{
    if (filename.find("balance") == 0 && filename.size() == 25)
    {
        int client;
        client = atoi(filename.substr(8, 8).c_str());
        int date = atoi(filename.substr(17, 8).c_str());
        if (client > 0 && date > 0)
        {
            return true;
        }
    }
    return false;
}
// Output functions
void printFiles(std::vector<std::pair<std::string, std::string>> files)
{
    for (auto& pair : files)
    {
        std::cout << pair.first << " " << pair.second << std::endl;
    }
    std::cout << std::endl;
}
void print(const std::map<std::string, Holder>& map)
{
    for (auto& pair : map) {
        std::cout << "broker : " << pair.second.getBroker() << std::endl;
        std::cout << "client : " << pair.first << std::endl;
        std::cout << "files : " << pair.second.getFiles() << std::endl;
        std::cout << "date : " << pair.second.GetLastDate() << std::endl;
    }
}

std::map<std::string, Holder> Browse(const boost::filesystem::path& dir)
{
    std::map<std::string, Holder> clients;
    std::vector<std::pair<std::string, std::string>> files;
    for (const boost::filesystem::directory_entry& directoryIterator
         : boost::filesystem::directory_iterator{dir})
    {
        if (boost::filesystem::is_directory(directoryIterator.path()))
        {
            std::map<std::string, Holder> dir = Browse(directoryIterator);
            for (auto& it : dir)
            {
                clients[it.first] = it.second;
            }
        } else {
            std::string filename = directoryIterator.path().stem().string();
            if (oldCheck(filename) && formatCheck(filename))
            {
                std::pair<std::string,
                std::string> pair;
                pair.first = directoryIterator.path().
                parent_path().stem().string();  //  broker
                std::string account = directoryIterator.
                path().stem().string().substr(8, 8);
                std::string data = directoryIterator.
                path().stem().string().substr(17, 8);
                if (clients.find(account) == clients.end()) {
                    clients[account] = Holder(pair.first, data);
                } else {
                    clients[account] = clients[account].update(data);
                }
                pair.second = directoryIterator.path().stem().string()
                + directoryIterator.path().extension().string();
                files.push_back(pair);
            }
        }
    }
    printFiles(files);
    return clients;
}

int main(int args, char* argv[])
{
    boost::filesystem::path p;
    switch (args)
    {
        case 1:
            p = boost::filesystem::current_path();
            break;
        case 2:
            p = boost::filesystem::path{argv[1]};
            break;
        default:
            throw std::logic_error("Error");
            break;
    }
    auto answ = Browse(p);
    print(answ);
    return 0;
}
