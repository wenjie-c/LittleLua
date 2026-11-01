#include <LittleLua.hpp>
#include "../src/LittleLua.cpp"
#include <cstddef>
#include <gtest/gtest.h>
#include <iostream>
#include "settings.cpp"

using namespace std;

const char *path1 = "example1.lua";
const char *path2 = "example2.lua";

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

  lua.close();
}

TEST(LuaTest, TableTest) {
  LittleLua lua;
  string name;
  string surname;

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

  ASSERT_TRUE(name.compare("Alan") == 0);
  ASSERT_TRUE(surname.compare("Donnor") == 0);
  ASSERT_EQ(age, 67);
  ASSERT_EQ(weight, 69.5f);

  lua.close();
}

TEST(LuaTest, DeserializeTest)
{
  LittleLua lua;
  lua.init();
  ASSERT_NO_THROW(lua.executeFile(path2));
  Settings s = lua.deserializate<Settings>("example");
  // ASSERT_NO_THROW(s = lua.deserializate<Settings>("example"));
  cout << s.toString() << endl;
}

// This only works if "more" were a global variable, but it is a field from this table.
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
