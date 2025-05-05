#include <gtest/gtest.h>
#include <wx/datetime.h>
#include <fstream>
#include <filesystem>
#include "../src/Model/XMLParser.h"

// Locazioni assolute dei miei file, cambiare queste per cambiare file test
const std::string validFile = "/home/giacomo/Scrivania/UNIFI/1_anno/Taskify/test/test_valid.xml";
const std::string invalidFile = "/home/giacomo/Scrivania/UNIFI/1_anno/Taskify/test/test_invalid.xml";

TEST(XMLParserTest, AddTaskIncreasesSize) {
    XMLParser parser;
    std::string dueDate = "2025-10-10";
    parser.addTask("Titolo", "Descrizione", dueDate);
    EXPECT_EQ(parser.GetTasks().size(), 1);
}

TEST(XMLParserTest, RemoveTaskReducesSize) {
    XMLParser parser;
    std::string dueDate = "2025-10-10";
    parser.addTask("Uno", "D", dueDate);
    parser.addTask("Due", "D", dueDate);
    parser.removeTask(0);
    EXPECT_EQ(parser.GetTasks().size(), 1);
    EXPECT_EQ(parser.GetTasks()[0]->GetTitle(), "Due");
}

TEST(XMLParserTest, ClearTasksEmptiesVector) {
    XMLParser parser;
    parser.addTask("Uno", "D", "2025-10-10");
    parser.clearTasks();
    EXPECT_TRUE(parser.GetTasks().empty());
}

TEST(XMLParserTest, GetTaskByIndexReturnsCorrectTask) {
    XMLParser parser;
    parser.addTask("Task1", "Desc", "2025-10-10");
    Task* t = parser.getTaskByIndex(0);
    ASSERT_NE(t, nullptr);
    EXPECT_EQ(t->GetTitle(), "Task1");
}

TEST(XMLParserTest, SetTaskStatusChangesStatus) {
    XMLParser parser;
    parser.addTask("Task1", "Desc", "2025-10-10");
    parser.setTaskStatus(true, 0);
    EXPECT_TRUE(parser.GetTasks()[0]->IsCompleted());
}

TEST(XMLParserTest, SetTaskDateChangesDate) {
    XMLParser parser;
    parser.addTask("Task1", "Desc", "2025-10-10");

    wxDateTime newDate(1, wxDateTime::Jan, 2030); // 2030-01-01
    parser.setTaskDate(newDate, 0);
    EXPECT_EQ(parser.GetTasks()[0]->GetDueDate(), "2030-01-01");
}

TEST(XMLParserTest, OrderTasksByStatusCorrectlySorts) {
    XMLParser parser;
    parser.addTask("T1", "D", "2025-01-01");
    parser.addTask("T2", "D", "2025-01-01");
    parser.setTaskStatus(true, 0);
    parser.setTaskStatus(false, 1);

    parser.orderTasksByStatus();
    EXPECT_FALSE(parser.GetTasks()[0]->IsCompleted());
    EXPECT_TRUE(parser.GetTasks()[1]->IsCompleted());
}

TEST(XMLParserTest, OrderTasksByDateCorrectlySorts) {
    XMLParser parser;
    parser.addTask("Late", "D", "2025-12-01");
    parser.addTask("Early", "D", "2025-01-01");

    parser.orderTasksByDate();
    EXPECT_EQ(parser.GetTasks()[0]->GetTitle(), "Early");
    EXPECT_EQ(parser.GetTasks()[1]->GetTitle(), "Late");
}

TEST(XMLParserTest, GetTasksReturnsAllTasks) {
    XMLParser parser;
    parser.addTask("T1", "Desc", "2025-12-12");
    parser.addTask("T2", "Desc", "2025-12-13");
    const auto& tasks = parser.GetTasks();
    EXPECT_EQ(tasks.size(), 2);
    EXPECT_EQ(tasks[0]->GetTitle(), "T1");
}

// ========== LOAD FROM FILE ==========
TEST(XMLParserTest, LoadFromValidFileSucceeds) {
    XMLParser parser;
    bool loaded = parser.loadFromFile(validFile);
    EXPECT_TRUE(loaded);
    EXPECT_GT(parser.GetTasks().size(), 0); // Deve aver caricato almeno un task
}

TEST(XMLParserTest, LoadFromInvalidFileFails) {
    XMLParser parser;
    bool loaded = parser.loadFromFile(invalidFile);
    EXPECT_FALSE(loaded);
    EXPECT_EQ(parser.GetTasks().size(), 0); // Nessun task caricato
}

// ========== SAVE TO FILE ==========
TEST(XMLParserTest, SaveToFileWritesXMLSuccessfully) {
    XMLParser parser;
    parser.addTask("T1", "Description", "2025-10-10");
    std::string tempFile = "temp_test_output.xml";

    bool success = parser.saveToFile(tempFile);
    EXPECT_TRUE(success);

    std::ifstream file(tempFile);
    EXPECT_TRUE(file.is_open());
    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    EXPECT_FALSE(content.empty());

    std::filesystem::remove(tempFile);
}