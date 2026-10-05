#include "calendar.h"

#include <iomanip>
#include <sstream>

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

int daysInMonth(int month, int year) {
    static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) {
        return 29;
    }
    return days[month - 1];
}

int dayOfWeek(int day, int month, int year) {
    static const int offsets[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    if (month < 3) {
        year--;
    }
    return (year + year / 4 - year / 100 + year / 400 + offsets[month - 1] + day) % 7;
}

std::string formatMonth(int month, int year) {
    static const std::string names[] = {"January", "February", "March", "April", "May", "June", "July",
                                        "August", "September", "October", "November", "December"};
    std::ostringstream text;

    std::string title = names[month - 1] + " " + std::to_string(year);
    text << std::string((20 - title.size()) / 2, ' ') << title << "\n";
    text << "Su Mo Tu We Th Fr Sa\n";

    int weekday = dayOfWeek(1, month, year);
    text << std::string(weekday * 3, ' ');
    int days = daysInMonth(month, year);
    for (int day = 1; day <= days; day++) {
        text << std::setw(2) << day;
        weekday++;
        if (weekday == 7 || day == days) {
            text << "\n";
            weekday = 0;
        } else {
            text << " ";
        }
    }
    return text.str();
}
