#include <gtest/gtest.h>
#include "../src/XMLParser.h"
#include <wx/wx.h>

class XMLParserTest : public ::testing::Test {
protected:
    XMLParser parser;

    void SetUp() override {
        parser.addToTaskList({"Task 1", "Descrizione 1", false});
        parser.addToTaskList({"Task 2", "Descrizione 2", true});
    }

    void TearDown() override {

    }
};

TEST_F(XMLParserTest, OpenValidFile) {
    EXPECT_TRUE(parser.openFile("/home/giacomo/Scrivania/UNIFI/1_anno/Taskify/test/test_valid.xml"));
}

TEST_F(XMLParserTest, OpenInvalidFile) {
    EXPECT_FALSE(parser.openFile("/home/giacomo/Scrivania/UNIFI/1_anno/Taskify/test/test_invalid.xml"));
}

TEST_F(XMLParserTest, ParseXMLCorrectly) {
    EXPECT_TRUE(parser.openFile("/home/giacomo/Scrivania/UNIFI/1_anno/Taskify/test/test_valid.xml"));

    parser.parseXML();
    auto tasks = parser.getTaskList();

    ASSERT_EQ(tasks.size(), 2);
    EXPECT_EQ(std::get<0>(tasks[0]), "Task 1");
    EXPECT_EQ(std::get<1>(tasks[1]), "Descrizione 2");
    EXPECT_TRUE(std::get<2>(tasks[1]));
}

TEST_F(XMLParserTest, SerializeXML) {
    parser.serializeXML();
    EXPECT_TRUE(parser.saveToFile("../test/test_output.xml"));

    XMLParser newParser;
    EXPECT_TRUE(newParser.openFile("../test/test_output.xml"));
    newParser.parseXML();
    auto tasks = newParser.getTaskList();

    ASSERT_EQ(tasks.size(), 2);
    EXPECT_EQ(std::get<0>(tasks[0]), "Task 1");
    EXPECT_FALSE(std::get<2>(tasks[0]));  // Task 1 non è completato
}

TEST_F(XMLParserTest, AddTask) {
    parser.addToTaskList({"Task 3", "Descrizione 3", false});
    auto tasks = parser.getTaskList();
    ASSERT_EQ(tasks.size(), 3);
    EXPECT_EQ(std::get<0>(tasks[2]), "Task 3");
}

TEST_F(XMLParserTest, SetTaskList) {
    std::vector<std::tuple<std::string, std::string, bool>> newTasks = {
        {"Nuovo Task", "Nuova Desc", true}
    };
    parser.setTaskList(newTasks);
    auto tasks = parser.getTaskList();
    ASSERT_EQ(tasks.size(), 1);
    EXPECT_EQ(std::get<0>(tasks[0]), "Nuovo Task");
    EXPECT_TRUE(std::get<2>(tasks[0]));
}