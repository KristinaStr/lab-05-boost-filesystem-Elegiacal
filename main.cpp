//
//  main.cpp
//  lab_35 (filesystem)
//
//  Created by Артём Песоцкий on 25/11/2018.
//  Copyright © 2018 Артём Песоцкий. All rights reserved.
//

#include <iostream>
#include <boost/filesystem.hpp>
#include <vector>
#include <map>
#include <algorithm>
#include <exception>

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
    ~Holder() = default;
    Holder& update(const std::string& d)
    {
        files += 1;
        if (d > date)
        {
            date = d;
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
    const std::string getDate() const
    {
        return date;
    }
};
// Checks
bool oldCheck(const std::string& fn)
{
    return fn.find("old") == std::string::npos;
}
bool formatCheck(const std::string& fn)
{
    if (fn.find("balance") == 0 && fn.size() == 25)
    {
        int client;
        client = atoi(fn.substr(8, 8).c_str());
        int date = atoi(fn.substr(17, 8).c_str());
        if (client > 0 && date > 0)
        {
            return true;
        }
    }
    return false;
}
// Printing
void filesPrint(std::vector<std::pair<std::string, std::string>> files)
{
    for (auto& pair : files)
    {
        std::cout << pair.first << " " << pair.second << std::endl;
    }
    std::cout << std::endl;
}
void Print(const std::map<std::string, Holder>& m)
{
    for (auto& pair : m)
    {
        std::cout << "broker : " << pair.second.getBroker() << std::endl;
        std::cout << "client : " << pair.first << std::endl;
        std::cout << "files : " << pair.second.getFiles() << std::endl;
        std::cout << "date : " << pair.second.getDate() << std::endl;
    }
}
// Browsing logic
std::map<std::string, Holder> Browse(const boost::filesystem::path& dir)
{
    std::map<std::string, Holder> clients;
    std::vector<std::pair<std::string, std::string>> files;
    for (const boost::filesystem::directory_entry& dIterator
         : boost::filesystem::directory_iterator{dir})
    {
        if (boost::filesystem::is_directory(dIterator.path()))
        {
            std::map<std::string, Holder> dir = Browse(dIterator);
            for (auto& it : dir)
            {
                clients[it.first] = it.second;
            }
        } else {
            std::string filename = dIterator.path().stem().string();
            if (oldCheck(filename) && formatCheck(filename))
            {
                std::pair<std::string, std::string> p;
                p.first = dIterator.path().
                parent_path().stem().string();
                std::string account = dIterator.
                path().stem().string().substr(8, 8);
                std::string data = dIterator.
                path().stem().string().substr(17, 8);
                if (clients.find(account) == clients.end())
                {
                    clients[account] = Holder(p.first, data);
                } else {
                    clients[account] = clients[account].update(data);
                }
                p.second = dIterator.path().stem().string()
                + dIterator.path().extension().string();
                files.push_back(p);
            }
        }
    }
    filesPrint(files);
    return clients;
}
// Main
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
    Print(answ);
    return 0;
}
