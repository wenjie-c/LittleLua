#include "../src/LittleLua.cpp"
#include "settings.cpp"
#include <LittleLua.hpp>
#include <cstddef>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

const char *path1 = "example1.lua";
const char *path2 = "example2.lua";
const char *path3 = "example3.lua";
const char *path4 = "example4.lua";
const char *path5 = "example5.lua";
const char *path6 = "example6.lua";
const char *path7 = "example7.lua";

TEST(LuaTest, PrintTest) {
  LittleLua lua;
  lua.init();
  ASSERT_NO_THROW(lua.execute("msg = 'Hello world!'\nprint(msg)"));
  lua.close();
}

TEST(LuaTest, GettersTest) {
  int width, height;
  string name;
  LittleLua lua;
  lua.init();
  ASSERT_NO_THROW(lua.executeFile(path1));
  // getting each value linearly assure us to push the wished value onto the top
  ASSERT_NO_THROW(width = lua.getInteger("width"));
  ASSERT_NO_THROW(height = lua.getInteger("height"));
  ASSERT_NO_THROW(name = lua.getString("name"));
  ASSERT_EQ(width, 42);
  ASSERT_EQ(height, 23);
  ASSERT_EQ(name.compare("Walter White"), 0);
  lua.close();
}

TEST(LuaTest, GetterArrayTest) {
  vector<int> numbers;
  vector<string> fruits;
  LittleLua lua;
  lua.init();
  ASSERT_NO_THROW(lua.executeFile(path1));
  ASSERT_NO_THROW(numbers = lua.getIntegerVector("numbers"));
  for (int i = 0; i < numbers.size(); i++) {
    cout << i << " : " << numbers.at(i) << endl;
  }

  ASSERT_NO_THROW(fruits = lua.getStringVector("fruits"));
  for (int i = 0; i < fruits.size(); i++) {
    cout << i << " : " << fruits.at(i) << endl;
  }

  // lua.dumpstack();

  lua.close();
}

TEST(LuaTest, TableTest) {
  LittleLua lua;
  string name;
  string surname;
  vector<double> numbers;
  vector<string> cities;

  int age;
  double weight;

  lua.init();
  try {
    ASSERT_NO_THROW(lua.executeFile(path1));
  } catch (LuaException e) {
    cerr << e.what() << endl;
  }
  ASSERT_NO_THROW(lua.openTable("mytable"));
  ASSERT_NO_THROW(name = lua.getStringFromTable("name"));
  ASSERT_NO_THROW(surname = lua.getStringFromTable("surname"));
  ASSERT_NO_THROW(age = lua.getIntegerFromTable("age"));
  ASSERT_NO_THROW(weight = lua.getNumberFromTable("weight"));

  // lua.dumpstack();

  ASSERT_NO_THROW(numbers = lua.getNumberVectorFromTable("real"));
  for (int i = 0; i < numbers.size(); i++) {
    cout << i << " : " << numbers.at(i) << endl;
  }

  // lua.dumpstack();

  ASSERT_NO_THROW(cities = lua.getStringVectorFromTable("cities"));
  for (int i = 0; i < cities.size(); i++) {
    cout << i << " : " << cities.at(i) << endl;
  }

  ASSERT_TRUE(name.compare("Alan") == 0);
  ASSERT_TRUE(surname.compare("Donnor") == 0);
  ASSERT_EQ(age, 67);
  ASSERT_EQ(weight, 69.5f);

  // lua.dumpstack();

  lua.close();
}

TEST(LuaTest, DeserializeTest) {
  LittleLua lua;
  lua.init();
  ASSERT_NO_THROW(lua.executeFile(path2));
  Settings s = lua.deserializate<Settings>("example");
  // ASSERT_NO_THROW(s = lua.deserializate<Settings>("example"));
  cout << s.toString() << endl;
}

TEST(LuaTest, SetterTest) {
  LittleLua lua;
  lua.init();
  constexpr int code = 67;
  char name[] = "Bobby";
  lua.setInteger("code", code);
  lua.setString("name", name);
  ASSERT_NO_THROW(lua.executeFile(path3));
  lua.close();
}

TEST(LuaTest, TableSetterTest) {
  LittleLua lua;
  lua.init();

  ASSERT_NO_THROW(lua.executeFile(path4));
  ASSERT_NO_THROW(lua.openTable("example2"));
  ASSERT_NO_THROW(lua.setIntegerFromTable("width", 800));
  ASSERT_NO_THROW(lua.setIntegerFromTable("height", 600));
  ASSERT_NO_THROW(lua.setBoolFromTable("debug", false));
  ASSERT_NO_THROW(lua.setStringFromTable("title", "Barcelona"));

  ASSERT_EQ(lua.getIntegerFromTable("width"), 800);
  ASSERT_EQ(lua.getIntegerFromTable("height"), 600);
  ASSERT_EQ(lua.getBoolFromTable("debug"), false);
  ASSERT_TRUE(string("Barcelona").compare(lua.getStringFromTable("title")) ==
              0);
  lua.close();
}

TEST(LuaTest, setArrayTest) {
  const char *code1 = "for i = 1, #numbers do\n  print(numbers[i])\nend";
  const char *code2 = "for i = 1, #foods do\n  print(foods[i])\nend";
  const char *code3 =
      "for i = 1, #things.cities do\n  print(things.cities[i])\nend";
  const char *code4 =
      "for i = 1, #things.real do\n  print(things.real[i])\nend";

  LittleLua lua;
  lua.init();
  ASSERT_NO_THROW(lua.setIntegerVector("numbers", {1, 3, 5, 7, 12, 1}));
  ASSERT_NO_THROW(
      lua.setStringVector("foods", {"bread", "burguer", "pasta", "salad"}));
  ASSERT_NO_THROW(lua.execute(code1));
  ASSERT_NO_THROW(lua.execute(code2));
  ASSERT_NO_THROW(lua.executeFile(path5));
  lua.openTable("things");
  ASSERT_NO_THROW(lua.setStringVectorFromTable(
      "cities", {"Barcelona", "Madrid", "Valencia", "Seville",
                 "Bilbao"})); // changing time
  ASSERT_NO_THROW(
      lua.setNumberVectorFromTable("real", {4.2f, 6.7f, 1.999f, 8.88888f}));
  ASSERT_NO_THROW(lua.execute(code3));
  ASSERT_NO_THROW(lua.execute(code4));
  lua.closeTable();
  lua.dumpstack(); // It mus remain empty
  lua.close();
}

TEST(LuaTest, SerializeTest) {
  const char *code1 =
      "print(\"Width: \" .. mySettings.width)\nprint(\"Height: \" .. "
      "mySettings.height)\nprint(\"Debug: \" .. "
      "tostring(mySettings.debug))\nprint(\"Title: \" .. mySettings.title)";

  LittleLua lua;
  lua.init();

  Settings setting(800, 600, true, "Serialization");
  ASSERT_NO_THROW(lua.serialize<Settings>("mySettings", setting));
  ASSERT_NO_THROW(lua.execute(code1));
  lua.close();
}

TEST(LuaTest, LuaToCFunctionTest)
{

  constexpr int a = 3;
  constexpr int b = 4;
  constexpr int n = 15;
  constexpr char firstName[] = "Juan\0";
  constexpr char middleName[] = "Carlos\0";
  constexpr char lastName[] = "Borbón\0";
  int res1 = 0;
  int res2 = 0;
  string res3;

  LittleLua lua;
  lua.init();

  ASSERT_NO_THROW(lua.executeFile(path6));
  ASSERT_NO_THROW(
    res1 = lua.LUA_INT_FUNCTION_BEGIN(sum)
    LUA_ARG_INT(3)
    LUA_ARG_INT(4)
    LUA_FUNCTION_END
  );

  ASSERT_NO_THROW(
    res1 = lua.LUA_INT_FUNCTION_BEGIN(sum)
    LUA_ARG_INT(3)
    LUA_ARG_INT(4)
    LUA_FUNCTION_END
  );

  ASSERT_NO_THROW(
    res2 = lua.LUA_INT_FUNCTION_BEGIN(fibonacci)
    LUA_ARG_INT(n)
    LUA_FUNCTION_END
  );

  ASSERT_NO_THROW(
    res3 = lua.LUA_STRING_FUNCTION_BEGIN(concat)
    LUA_ARG_STRING(firstName)
    LUA_ARG_STRING(middleName)
    LUA_ARG_STRING(lastName)
    LUA_FUNCTION_END
  );

  ASSERT_EQ(res1, 7);
  cout << "Concat result is " << res3 << endl;

  lua.close();


}

TEST(LuaTest, CtoLuaFunctionTest)
{

  vector<int> linspace{1,2,3,4,5,6,7};

  LittleLua lua;
  lua.init();
  
  ASSERT_NO_THROW(
    lua.C_FUNCTION_BEGIN(log)
    string text = C_ARG_STRING;
    cout << "Log from C: " << text << endl;
    C_FUNCTION_END
  );

  ASSERT_NO_THROW(
    lua.C_FUNCTION_BEGIN(sum)
    int b = C_ARG_INT; // The top is the last argument, because we are in a stack and the first argument is pushed first
    int a = C_ARG_INT; 
    int res = a + b;
    C_FUNCTION_RETURN(res)
  );

  ASSERT_NO_THROW(
    lua.C_FUNCTION_BEGIN(multiplyby)
    int m = C_ARG_INT;
    std::for_each(linspace.begin(), linspace.end(), [m](int &i){i *= m;});

    C_FUNCTION_RETURN(linspace)
  );

  lua.executeFile(path7);
  lua.close();
}

// This only works if "more" were a global variable, but it is a field from this
// table.
/* TEST(LuaTest, Table2Test) {
  LittleLua lua;
  string name;
  string food;

  lua.init();
  try {
    ASSERT_NO_THROW(lua.executeFile(path1));
  } catch (LuaException e) {
    cerr << e.what() << endl;
  }
  ASSERT_NO_THROW(lua.openTable("mytable"));
  ASSERT_NO_THROW(name = lua.getStringFromTable("name"));
  lua.openTable("more");
  ASSERT_NO_THROW(food = lua.getStringFromTable("food"));


  ASSERT_TRUE(name.compare("Alan") == 0);
  ASSERT_TRUE(food.compare("cheese") == 0);


  lua.close();
} */

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
