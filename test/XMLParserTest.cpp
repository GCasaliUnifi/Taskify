#include <gtest/gtest.h>
#include "../src/Model/XMLParser.h"
#include <wx/wx.h>

// Locazioni assolute dei miei file, cambiare queste per cambiare file test.
const wxString invalidFile = "/home/giacomo/Scrivania/UNIFI/1_anno/Taskify/test/test_invalid.xml";
const wxString validFile = "/home/giacomo/Scrivania/UNIFI/1_anno/Taskify/test/test_valid.xml";
const wxString outputFile = "/home/giacomo/Scrivania/UNIFI/1_anno/Taskify/test/test_output.xml";

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
    EXPECT_TRUE(parser.openFile(validFile));
}

TEST_F(XMLParserTest, OpenInvalidFile) {
    EXPECT_FALSE(parser.openFile(invalidFile));
}

TEST_F(XMLParserTest, ParseXMLCorrectly) {
    EXPECT_TRUE(parser.openFile(validFile));

    parser.parseXML();
    auto tasks = parser.getTaskList();

    ASSERT_EQ(tasks.size(), 2);
    EXPECT_EQ(std::get<0>(tasks[0]), "Task 1");
    EXPECT_EQ(std::get<1>(tasks[1]), "Descrizione 2");
    EXPECT_TRUE(std::get<2>(tasks[1]));
}

TEST_F(XMLParserTest, SerializeXML) {
    parser.serializeXML();
    EXPECT_TRUE(parser.saveToFile(outputFile));

    XMLParser newParser;
    EXPECT_TRUE(newParser.openFile(outputFile));
    newParser.parseXML();
    auto tasks = newParser.getTaskList();

    ASSERT_EQ(tasks.size(), 2);
    EXPECT_EQ(std::get<0>(tasks[0]), "Task 1");
    EXPECT_FALSE(std::get<2>(tasks[0]));  // Task 1 test_output non è completed=true
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