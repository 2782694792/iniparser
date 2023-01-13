#include "INI.h"

#include <iostream>

int main(int argc, char const *argv[])
{
    Value v11("123");
    Value v12(12);
    Value v13(45.21);
    Value v14(false);

    Value v21 = "1233";
    Value v22 = 12;
    Value v23 = 12.34;
    Value v24 = false;

    INI i11("students");

    INI i21("..//testFile//INITest.conf");
    i21.load();

    MULTIKV a = i21["ui"];
    auto a2 = a.find("username");
    Value a3 = a2->second;

    std::string str = i21.get("ui", "username");
    a3 = i21.get("ui", "username");

    i21.set("ui2", "username2", true);
    bool b = i21.get("ui2", "username2");

    b = i21.has("ui2", "username2");

    i21.remove("ui2", "username2");
    i21.remove("ui2");

    i21.show();

    i21.save("..//testFile//test.ini");

    return 0;
}
