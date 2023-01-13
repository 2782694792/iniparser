#include "INI.h"

#include <sstream>
using std::stringstream;
#include <fstream>
// mbstowcs
#include <stdlib.h>
// to_wide_string
#include <codecvt>
#include <locale>
// UTF8ToGB
#include <windows.h>
// file
#include <stdio.h>

#include <iostream>

using std::logic_error;

inline std::string UTF8ToGB(const char *str)
{
    std::string result;
    WCHAR *strSrc;
    LPSTR szRes;

    // 获得临时变量的大小
    int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    strSrc = new WCHAR[i + 1];
    MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

    // 获得临时变量的大小
    i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
    szRes = new CHAR[i + 1];
    WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

    result = szRes;
    delete[] strSrc;
    delete[] szRes;

    return result;
}

inline std::wstring String2WString(const char *str)
{
    std::wstring wstr;
    if (str != NULL)
    {
        std::string s(str);
        wstr.assign(s.begin(), s.end());
    }
    return wstr;
}

inline std::string WString2String(const wchar_t *wstr)
{
    std::string str;
    if (wstr != NULL)
    {
        std::wstring s(wstr);
        str.assign(s.begin(), s.end());
    }
    return str;
}

// convert string to wstring
inline std::wstring to_wide_string(const std::string &input)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(input);
}

// convert wstring to string
inline std::string to_byte_string(const std::wstring &input)
{
    // std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(input);
}

inline char *trim(std::string &str)
{
    if (str == "")
    {
        return (char *)str.c_str();
    }

    str.erase(0, str.find_first_not_of(" \r\n"));
    str.erase(str.find_last_not_of(" \r\n") + 1);

    return (char *)str.c_str();
}

Value::Value() : _value("")
{
}

Value::Value(int value)
{
    *this = value;
}

Value::Value(double value)
{
    *this = value;
}

Value::Value(bool value)
{
    *this = value;
}

Value::Value(const char *value) : _value(value)
{
}

Value::Value(const string &value) : _value(value)
{
}

Value::~Value()
{
}

Value &Value::operator=(int value)
{
    stringstream ss;
    ss << value;
    _value = ss.str();
    return *this;
}

Value &Value::operator=(double value)
{
    stringstream ss;
    ss << value;
    _value = ss.str();
    return *this;
}

Value &Value::operator=(bool value)
{
    _value = value ? "true" : "false";
    return *this;
}

Value &Value::operator=(const char *value)
{
    _value = value;
    return *this;
}

Value &Value::operator=(const string &value)
{
    _value = value;
    return *this;
}

Value::operator bool()
{
    return _value == "true";
}

Value::operator int()
{
    return atoi(_value.c_str());
}

Value::operator double()
{
    return atof(_value.c_str());
}

Value::operator string()
{
    return _value;
}

INI::INI()
{
}

INI::INI(const char *filename)
{
    m_fileName = filename;
}

INI::INI(const string &filename)
{
    m_fileName = filename;
}

INI::~INI()
{
    clear();
}

void INI::clear()
{
    if (m_ini != NULL)
    {
        for (auto it = m_ini->begin(); it != m_ini->end(); it++)
        {
            for (auto it2 = (*it).second.begin();
                 it2 != (*it).second.end(); it2++)
            {
                (*it).second.erase(it2);
            }
            m_ini->erase(it);
        }
        delete m_ini;
        m_ini = NULL;
    }
}

bool INI::load()
{
    // std::locale chs("chs");
    // std::wifstream ifs(m_fileName);
    // ifs.imbue(chs);
    // std::wcout.imbue(chs);
    // std::wstring line;
    // while (std::getline(ifs, line))
    // {
    //     std::wcout << line << std::endl;
    // }

    std::ifstream fin(m_fileName);
    if (fin.fail())
    {
        return false;
    }

    MAPINI ini;
    string line = "";
    string sectionname;
    int pos;

    while (std::getline(fin, line))
    {
        line = trim(line);

        if (line == "")
        {
            continue;
        }

        if (line[0] == '[') // 节点
        {
            pos = line.find_first_of(']');
            // mbtowc(sectionname, temp.c_str(), sizeof(sectionname) - 1);
            // sectionname = to_wide_string(temp).c_str();
            sectionname = line.substr(1, pos - 1);
            sectionname = trim(sectionname);

            ini[sectionname] = MULTIKV();
        }
        else if (line[0] == ';') // 注释
        {
            continue;
        }
        else // 节点值
        {
            if (line.find_first_of('=') > 0)
            {
                pos = line.find_first_of('=');
                // const wchar_t *keyname = L"";
                // mbstowcs(keyname, temp.c_str(), sizeof(keyname) - 1);
                // keyname = to_wide_string(temp).c_str();
                std::string keyname = line.substr(0, pos);
                keyname = trim(keyname);

                string value = line.substr(pos + 1, line.length() - pos);
                value = trim(value);

                ini[sectionname].insert({keyname, value});
            }
            else
            {
                throw logic_error("not found the symbol of \'=\' in parameter!");
                return false;
            }
        }
    }

    m_ini = new MAPINI(ini);
    fin.close();

    return true;
}

bool INI::load(const char *filename)
{
    m_fileName = filename;

    return load();
}

Value &INI::get(const char *section, const char *key)
{
    auto kv = (*m_ini)[section];

    if (kv.count(key) <= 0)
    {
        throw logic_error("not found the value!");
    }

    return (*m_ini)[section].find(key)->second;
}

void INI::set(const char *section, const char *key, const Value &value)
{
    (*m_ini)[section].insert({key, value});
}

bool INI::has(const char *section)
{
    return (*m_ini).find(section) != (*m_ini).end();
}

bool INI::has(const char *section, const char *key)
{
    auto it = (*m_ini).find(section);
    if (it != (*m_ini).end())
    {
        return it->second.find(key) != it->second.end();
    }

    return false;
}

void INI::remove(const char *section)
{
    if (has(section))
    {
        (*m_ini).erase(section);
    }
}

void INI::remove(const char *section, const char *key)
{
    if (has(section))
    {
        if (has(section, key))
        {
            (*m_ini)[section].erase(key);
        }
    }
}

const std::string INI::str()
{
    if (m_ini == NULL)
    {
        throw logic_error("the ini is null!");
    }

    stringstream ss;

    MAPINI::iterator firstit = (*m_ini).begin();

    for (MAPINI::iterator it = firstit; it != (*m_ini).end(); it++)
    {
        ss << "[" << it->first << "]" << std::endl;

        MULTIKV::iterator secondit = it->second.begin();

        for (MULTIKV::iterator it2 = secondit; it2 != it->second.end(); it2++)
        {
            ss << it2->first << "=" << it2->second << std::endl;
        }
    }

    return ss.str();
}

void INI::show()
{
    std::cout << str() << std::endl;
}

bool INI::save(const char *filename)
{
    std::ofstream ofs(filename);
    if (ofs.fail())
    {
        return false;
    }

    ofs << str();
    ofs.close();

    return true;
}

MULTIKV &INI::operator[](const char *section)
{
    if (section == NULL)
    {
        throw logic_error("current section value is null!");
    }

    if (m_ini == NULL)
    {
        m_ini = new MAPINI();
    }

    return (*m_ini)[section];
}

MAPINI &INI::operator=(const MAPINI &ini)
{
    clear();
    *m_ini = ini;
    return *m_ini;
}

std::ostream &operator<<(std::ostream &cout, const Value &value)
{
    cout << value._value;

    return cout;
}
