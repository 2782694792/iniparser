#pragma once

#ifndef __INI_H
#define __INI_H
#endif

#include <string>
#include <map>

using std::map;
using std::multimap;
using std::string;

class Value
{

private:
    string _value;

public:
    Value();
    Value(int value);
    Value(double value);
    Value(bool value);
    Value(const char *value);
    Value(const string &value);
    ~Value();

    Value &operator=(int value);
    Value &operator=(double value);
    Value &operator=(bool value);
    Value &operator=(const char *value);
    Value &operator=(const string &value);
    friend std::ostream &operator<<(std::ostream &cout, const Value &value);

    operator bool();
    operator int();
    operator double();
    operator string();
};

// [section...] name=value,name2=value, ....
typedef multimap<string, Value> MULTIKV;
typedef map<string, MULTIKV> MAPINI;

class INI
{
private:
    string m_fileName;

    MAPINI *m_ini;

public:
    INI();
    INI(const char *filename);
    INI(const string &filename);
    ~INI();

    void clear();

    MULTIKV &operator[](const char *section);
    MAPINI &operator=(const MAPINI &ini);

public:
    bool load();
    bool load(const char *filename);

    Value &get(const char *section, const char *key);
    void set(const char *section, const char *key, const Value &value);
    bool has(const char *section);
    bool has(const char *section, const char *key);

    void remove(const char *section);
    void remove(const char *section, const char *key);

    const std::string str();
    void show();
    bool save(const char *filename);
};