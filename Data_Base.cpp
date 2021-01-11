#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

void Print_Set(const set<string> &x)
{
    for (const auto &item : x)
    {
        cout << item << endl;
    }
}

vector<string> Split(string &command, char drop)
{
    stringstream ss(command);
    vector<string> result;
    string temp;
    while (getline(ss, temp, drop))
    {
        result.push_back(temp);
    }
    return result;
}

class Date
{
public:
    Date(int &y, int &m, int &d) : year(y), month(m), day(d) {}

    int GetYear() const
    {
        return year;
    }
    int GetMonth() const
    {
        return month;
    }
    int GetDay() const
    {
        return day;
    }

private:
    int year = 0;
    int month = 0;
    int day = 0;
};

Date ParseDate(string &date)
{
    stringstream stream(date);
    bool flag = true;

    int year = 0;
    flag = flag && (stream >> year);
    flag = flag && (stream.peek() == '-');
    stream.ignore(1);

    int month = 0;
    flag = flag && (stream >> month);
    flag = flag && (stream.peek() == '-');
    stream.ignore(1);

    int day = 0;
    flag = flag && (stream >> day);
    flag = flag && stream.eof();

    if (!flag)
    {
        throw logic_error("Wrong date format: " + date);
    }

    if (month < 1 || month > 12)
    {
        throw out_of_range("Month value is invalid: " + to_string(month));
    }

    if (day < 1 || day > 31)
    {
        throw out_of_range("Day value is invalid: " + to_string(day));
    }

    return Date(year, month, day);
}

bool operator<(const Date &lhs, const Date &rhs)
{
    if (lhs.GetYear() == rhs.GetYear())
    {
        if (lhs.GetMonth() == rhs.GetMonth())
        {
            return lhs.GetDay() < rhs.GetDay();
        }
        else
        {
            return lhs.GetMonth() < rhs.GetMonth();
        }
    }
    else
    {
        return lhs.GetYear() < rhs.GetYear();
    }
};

class Database
{
public:
    void AddEvent(const Date &date, const string &event)
    {
        eventday[date];
        map<Date, set<string>>::iterator it;
        it = eventday.find(date);
        it->second.insert(event);
    }

    bool DeleteEvent(const Date &date, const string &event)
    {
        if (eventday.find(date) == eventday.end())
        {
            return false;
        }
        map<Date, set<string>>::iterator it;
        it = eventday.find(date);
        if (!it->second.count(event))
        {
            return false;
        }
        else
        {
            it->second.erase(event);
            return true;
        }
    }

    int DeleteDate(const Date &date)
    {
        map<Date, set<string>>::iterator it;
        it = eventday.find(date);
        if (it == eventday.end())
        {
            return 0;
        }
        else
        {
            int N = it->second.size();
            eventday.erase(date);
            return N;
        }
    }

    void Find(const Date &date)
    {
        map<Date, set<string>>::iterator it;
        it = eventday.find(date);
        if (it == eventday.end())
        {
        }
        else
        {
            Print_Set(it->second);
        }
    }

    void Print() const
    {
        for (auto &item : eventday)
        {
            for (auto &item2 : item.second)
            {
                cout << setw(4) << setfill('0') << item.first.GetYear() << "-"
                     << setw(2) << setfill('0') << item.first.GetMonth() << "-"
                     << setw(2) << setfill('0') << item.first.GetDay() << " ";
                cout << item2 << endl;
            }
        }
    }

private:
    map<Date, set<string>> eventday;
};

int main()
{
    Database db;

    string command;
    while (getline(cin, command))
    {
        vector<string> command_vector = Split(command, ' ');
        if (!command_vector.empty())
        {
            try
            {
                if (command_vector[0] == "Add")
                {
                    Date dat = ParseDate(command_vector[1]);
                    db.AddEvent(dat, command_vector[2]);
                }
                else if (command_vector[0] == "Find")
                {
                    Date dat = ParseDate(command_vector[1]);
                    db.Find(dat);
                }
                else if (command_vector[0] == "Del" && command_vector.size() == 3)
                {
                    Date dat = ParseDate(command_vector[1]);
                    if (db.DeleteEvent(dat, command_vector[2]))
                    {
                        cout << "Deleted successfully" << endl;
                    }
                    else
                    {
                        cout << "Event not found" << endl;
                    }
                }
                else if (command_vector[0] == "Del" && command_vector.size() == 2)
                {
                    Date dat = ParseDate(command_vector[1]);
                    cout << "Deleted " << db.DeleteDate(dat) << " events" << endl;
                }
                else if (command_vector[0] == "Print")
                {
                    db.Print();
                }
                else
                {
                    cout << "Unknown command: " << command_vector[0] << endl;
                }
            }
            catch (exception &ex)
            {
                cout << ex.what() << endl;
                //return 0;
            }
        }
    }

    return 0;
}