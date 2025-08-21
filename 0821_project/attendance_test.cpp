#include "attendance.h"
#include "gmock/gmock.h"
#include <fstream>
#include <string>

namespace {
    std::string& RunProgramOnceAndGetOutput() {
        static bool isDone = false;
        static std::string captured;

        if (isDone == false) {
            std::ifstream fin("attendance_weekday_500.txt");

            testing::internal::CaptureStdout();
            load();
            compute();
            captured = testing::internal::GetCapturedStdout();
            isDone = true;
        }
        return captured;
    }

    std::string GetRemovedSection(const std::string& all) {
        const std::string marker = "Removed player";
        auto pos = all.find(marker);
        if (pos == std::string::npos) return {};
        return all.substr(pos);
    }
}

TEST(Attendance, PrintsHeaderAndRemovedSectionMarker) {
    const std::string& answer = RunProgramOnceAndGetOutput();
    EXPECT_NE(answer.find("NAME : "), std::string::npos);
    EXPECT_NE(answer.find("POINT : "), std::string::npos);
    EXPECT_NE(answer.find("GRADE : "), std::string::npos);
    EXPECT_NE(answer.find("Removed player"), std::string::npos);
    EXPECT_NE(answer.find("=============="), std::string::npos);
}

TEST(Attendance, ContainsUmarSummaryLine) {
    const std::string& answer = RunProgramOnceAndGetOutput();
    EXPECT_NE(answer.find("NAME : Umar, POINT : 48, GRADE : SILVER"), std::string::npos);
}

TEST(Attendance, ContainsZaneSummaryLine) {
    const std::string& answer = RunProgramOnceAndGetOutput();
    EXPECT_NE(answer.find("NAME : Zane, POINT : 1, GRADE : NORMAL"), std::string::npos);
}

TEST(Attendance, HasAtLeastOneNormalMember) {
    const std::string& answer = RunProgramOnceAndGetOutput();
    EXPECT_NE(answer.find("GRADE : NORMAL"), std::string::npos);
}

TEST(Attendance, ZaneAppearsInRemovedSection) {
    const std::string& answer = RunProgramOnceAndGetOutput();
    const std::string removedPlayer = GetRemovedSection(answer);
    ASSERT_FALSE(removedPlayer.empty()) << "Removed section not found";
    EXPECT_NE(removedPlayer.find("Zane"), std::string::npos);
}

TEST(Attendance, BobAppearsInRemovedSection) {
    const std::string& answer = RunProgramOnceAndGetOutput();
    const std::string removedPlayer = GetRemovedSection(answer);
    ASSERT_FALSE(removedPlayer.empty()) << "Removed section not found";
    EXPECT_NE(removedPlayer.find("Bob"), std::string::npos);
}

TEST(Attendance, XenaDoesNotAppearInRemovedSection) {
    const std::string& answer = RunProgramOnceAndGetOutput();
    const std::string removedPlayer = GetRemovedSection(answer);
    ASSERT_FALSE(removedPlayer.empty()) << "Removed section not found";
    EXPECT_EQ(removedPlayer.find("Xena"), std::string::npos);
}