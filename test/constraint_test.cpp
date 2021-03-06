#include "gtest/gtest.h"

#include "tcframe/constraint.hpp"

using tcframe::Constraint;
using tcframe::ConstraintsCollector;
using tcframe::Subtask;

TEST(ConstraintTest, Construction) {
    int X;
    Constraint constraint([&] { return X > 7; }, "X > 7");

    EXPECT_EQ("X > 7", constraint.getDescription());

    X = 42;
    EXPECT_TRUE(constraint.isSatisfied());

    X = 1;
    EXPECT_FALSE(constraint.isSatisfied());
}

TEST(ConstraintsCollectorTest, WithSubtasks) {
    ConstraintsCollector collector;

    collector.newSubtask();
    collector.addConstraint([] { return 1 + 1 == 2; }, "1 + 1 == 2");

    collector.newSubtask();
    collector.addConstraint([] { return 1 < 2; }, "1 < 2");
    collector.addConstraint([] { return 3 == 3; }, "3 == 3");

    vector<Subtask*> subtasks = collector.collectSubtasks();

    ASSERT_EQ(2, subtasks.size());

    EXPECT_EQ(1, subtasks[0]->getId());
    EXPECT_EQ(1, subtasks[0]->getConstraints().size());

    EXPECT_EQ(2, subtasks[1]->getId());
    EXPECT_EQ(2, subtasks[1]->getConstraints().size());
}

TEST(ConstraintsCollectorTest, WithoutSubtasks) {
    ConstraintsCollector collector;

    collector.addConstraint([] { return 1 + 1 == 2; }, "1 + 1 == 2");
    collector.addConstraint([] { return 1 < 2; }, "1 < 2");
    collector.addConstraint([] { return 3 == 3; }, "3 == 3");

    vector<Subtask*> subtasks = collector.collectSubtasks();

    ASSERT_EQ(1, subtasks.size());

    EXPECT_EQ(-1, subtasks[0]->getId());
    EXPECT_EQ(3, subtasks[0]->getConstraints().size());
}
