#include <cstddef>
#include <gtest/gtest.h>
#include <LittleLua.hpp>
#include <iostream>


using namespace std;

const char * path1 = "example1.lua";

TEST(LuaTest, PrintTest)
{
    LittleLua lua;
    lua.init();
    ASSERT_NO_THROW(lua.execute("msg = 'Hello world!'\nprint(msg)"));
    lua.close();
}

TEST(LuaTest, GettersTest)
{
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
  ASSERT_EQ(name.compare("Walter White"),0);
  lua.close();
}

TEST(LuaTest, GetterArrayTest)
{
  vector<int> numbers;
  LittleLua lua;
  lua.init();
  ASSERT_NO_THROW(lua.executeFile(path1));
  ASSERT_NO_THROW(numbers = lua.getIntegerArray("numbers"));
  for(int i = 0; i < numbers.size(); i++) {
    cout << i << " : "<< numbers.at(i) << endl;
  }
}

int main(int argc, char ** argv)
{
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}