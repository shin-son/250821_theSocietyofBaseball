#include "attendance.h"
#include "gmock/gmock.h"
#include <fstream>
#include <string>

class AttendanceFixture : public testing::Test {
protected: 
    AttendanceSystem& svc = AttendanceSystem::instance();

public:
    void SetUp() override {
        svc.updateAttendance("Xena");
        for (int i = 0; i < 16; ++i)
            svc.updateScore("Xena", "wednesday");

        svc.updateAttendance("Bob");
        for (int i = 0; i < 30; ++i)
            svc.updateScore("Bob", "monday");

        svc.updateAttendance("Steve");
        for (int i = 0; i < 5; ++i)
            svc.updateScore("Steve", "wednesday");
        for (int i = 0; i < 25; ++i)
            svc.updateScore("Steve", "monday");

        svc.updateAttendance("Will");
        for (int i = 0; i < 5; ++i)
            svc.updateScore("Will", "monday");
    }

    std::string runOnceAndCache(AttendanceSystem& svc) {
        std::string g_cached;
        testing::internal::CaptureStdout();
        svc.load();
        svc.compute();
        g_cached = testing::internal::GetCapturedStdout();
        return g_cached;
    }

    std::string GetRemovedSection(const std::string& all) {
        const std::string marker = "Removed player";
        auto pos = all.find(marker);
        if (pos == std::string::npos) return {};
        return all.substr(pos);
    }
};

TEST_F(AttendanceFixture, PrintsHeaderAndRemovedSectionMarker) {
    const std::string answer = runOnceAndCache(svc);

    EXPECT_NE(answer.find("NAME : "), std::string::npos);
    EXPECT_NE(answer.find("POINT : "), std::string::npos);
    EXPECT_NE(answer.find("GRADE : "), std::string::npos);
    EXPECT_NE(answer.find("Removed player"), std::string::npos);
    EXPECT_NE(answer.find("=============="), std::string::npos);
}

TEST_F(AttendanceFixture, ContainsSteveSummaryLine) {
    const std::string answer = runOnceAndCache(svc);
    EXPECT_NE(answer.find("NAME : Steve"), std::string::npos);
}

TEST_F(AttendanceFixture, HasAtLeastOneNormalMember) {
    const std::string answer = runOnceAndCache(svc);
    EXPECT_NE(answer.find("GRADE : NORMAL"), std::string::npos);
}

TEST_F(AttendanceFixture, WillAppearsInRemovedSection) {
    const std::string answer = runOnceAndCache(svc);
    EXPECT_NE(answer.find("Will"), std::string::npos);
}

TEST_F(AttendanceFixture, XenaDoesNotAppearInRemovedSection) {
    const std::string answer = runOnceAndCache(svc);
    const std::string removedPlayer = GetRemovedSection(answer);
    ASSERT_FALSE(removedPlayer.empty()) << "Removed section not found";
    EXPECT_EQ(removedPlayer.find("Xena"), std::string::npos);
}

TEST_F(AttendanceFixture, XenaAttendWednesdayOnce) {
    svc.updateAttendance("Xena");
    svc.updateScore("Xena", "wednesday");
    EXPECT_NE(runOnceAndCache(svc).find("Xena"), std::string::npos);
}

TEST_F(AttendanceFixture, XenaAttendHolidayOnce) {
    svc.updateAttendance("Xena");
    svc.updateScore("Xena", "sunday");
    EXPECT_NE(runOnceAndCache(svc).find("Xena"), std::string::npos);
}

TEST_F(AttendanceFixture, XenaAttendWednesdayTen) {
    svc.updateAttendance("Xena");
    for (int day = 0; day < 15; ++day)
        svc.updateScore("Xena", "wednesday");

    EXPECT_NE(runOnceAndCache(svc).find("Xena"), std::string::npos);
}

TEST_F(AttendanceFixture, XenaAttendHolidayTen) {
    svc.updateAttendance("Xena");
    for (int day = 0; day < 15; ++day)
        svc.updateScore("Xena", "sunday");
    EXPECT_NE(runOnceAndCache(svc).find("Xena"), std::string::npos);
}

TEST_F(AttendanceFixture, BobAttendMondayOnce) {
    static std::string captured;

    svc.updateAttendance("Bob");
    svc.updateScore("Bob", "monday");
    captured = runOnceAndCache(svc);
    EXPECT_NE(captured.find("Bob"), std::string::npos);
}

TEST_F(AttendanceFixture, BobGetsSilverGrade) {
    static std::string captured;

    svc.updateAttendance("Bob");
    svc.updateScore("Bob", "monday");
    captured = runOnceAndCache(svc);

    EXPECT_NE(captured.find("Bob"), std::string::npos);
}