#include "attendance.h"
#include "gmock/gmock.h"
#include <fstream>
#include <string>

class AttendanceFixture : public testing::Test {
public:
    std::string& getAttendanceAnswer() {
        static bool isDone = false;
        static std::string captured;

        if (isDone == false) {
            std::ifstream fin("attendance_answer.txt");
            captured.assign((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
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
};

TEST_F(AttendanceFixture, PrintsHeaderAndRemovedSectionMarker) {
    const std::string& answer = getAttendanceAnswer();
    EXPECT_NE(answer.find("NAME : "), std::string::npos);
    EXPECT_NE(answer.find("POINT : "), std::string::npos);
    EXPECT_NE(answer.find("GRADE : "), std::string::npos);
    EXPECT_NE(answer.find("Removed player"), std::string::npos);
    EXPECT_NE(answer.find("=============="), std::string::npos);
}

TEST_F(AttendanceFixture, ContainsUmarSummaryLine) {
    const std::string& answer = getAttendanceAnswer();
    EXPECT_NE(answer.find("NAME : Umar, POINT : 48, GRADE : SILVER"), std::string::npos);
}

TEST_F(AttendanceFixture, ContainsZaneSummaryLine) {
    const std::string& answer = getAttendanceAnswer();
    EXPECT_NE(answer.find("NAME : Zane, POINT : 1, GRADE : NORMAL"), std::string::npos);
}

TEST_F(AttendanceFixture, HasAtLeastOneNormalMember) {
    const std::string& answer = getAttendanceAnswer();
    EXPECT_NE(answer.find("GRADE : NORMAL"), std::string::npos);
}

TEST_F(AttendanceFixture, ZaneAppearsInRemovedSection) {
    const std::string& answer = getAttendanceAnswer();
    const std::string removedPlayer = GetRemovedSection(answer);
    ASSERT_FALSE(removedPlayer.empty()) << "Removed section not found";
    EXPECT_NE(removedPlayer.find("Zane"), std::string::npos);
}

TEST_F(AttendanceFixture, BobAppearsInRemovedSection) {
    const std::string& answer = getAttendanceAnswer();
    const std::string removedPlayer = GetRemovedSection(answer);
    ASSERT_FALSE(removedPlayer.empty()) << "Removed section not found";
    EXPECT_NE(removedPlayer.find("Bob"), std::string::npos);
}

TEST_F(AttendanceFixture, XenaDoesNotAppearInRemovedSection) {
    const std::string& answer = getAttendanceAnswer();
    const std::string removedPlayer = GetRemovedSection(answer);
    ASSERT_FALSE(removedPlayer.empty()) << "Removed section not found";
    EXPECT_EQ(removedPlayer.find("Xena"), std::string::npos);
}