#include <gtest/gtest.h>
#include "../src/Model/Task.h" // adatta il path

TEST(TaskTest, ConstructorAndGetters) {
    Task task("Titolo", "Descrizione", "2025-12-31", true);

    EXPECT_EQ(task.GetTitle(), "Titolo");
    EXPECT_EQ(task.GetDescription(), "Descrizione");
    EXPECT_EQ(task.GetDueDate(), "2025-12-31");
    EXPECT_TRUE(task.IsCompleted());
}

TEST(TaskTest, Setters) {
    Task task;
    task.SetTitle("Nuovo Titolo");
    task.SetDescription("Nuova Descrizione");
    task.SetDueDate("2026-01-01");
    task.SetCompleted(true);

    EXPECT_EQ(task.GetTitle(), "Nuovo Titolo");
    EXPECT_EQ(task.GetDescription(), "Nuova Descrizione");
    EXPECT_EQ(task.GetDueDate(), "2026-01-01");
    EXPECT_TRUE(task.IsCompleted());
}
