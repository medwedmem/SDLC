#include "ZodiacModel.h"

#include <algorithm>
#include <ctime>
#include <stdexcept>

void ZodiacModel::addListener(IModelListener* listener) {
    listeners_.push_back(listener);
}

void ZodiacModel::removeListener(IModelListener* listener) {
    listeners_.erase(std::remove(listeners_.begin(), listeners_.end(), listener),
                     listeners_.end());
}

int ZodiacModel::daysInMonth(int month, int year) {
    static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    const bool leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
    return month == 2 && leap ? 29 : days[month - 1];
}

void ZodiacModel::setBirthDate(int day, int month, int year) {
    const std::time_t now = std::time(nullptr);
    const std::tm* today = std::localtime(&now);
    const int curYear = today->tm_year + 1900, curMonth = today->tm_mon + 1;
    const int curDay = today->tm_mday;

    if (month < 1 || month > 12) {
        throw std::invalid_argument("Месяц должен быть числом от 1 до 12.");
    }
    if (year < MIN_YEAR || year > curYear) {
        throw std::invalid_argument("Год должен быть в диапазоне от 1900 до текущего года.");
    }
    if (day < 1 || day > daysInMonth(month, year)) {
        throw std::invalid_argument("В указанном месяце нет такого дня.");
    }
    if (year == curYear && (month > curMonth || (month == curMonth && day > curDay))) {
        throw std::invalid_argument("Дата рождения не может быть позже текущей даты.");
    }
    day_ = day;
    month_ = month;
    year_ = year;
    signIndex_ = findSign(day, month);
    calculated_ = true;
    fireModelChanged();
}

int ZodiacModel::findSign(int day, int month) {
    const auto& signs = zodiacSigns();
    for (int i = 0; i < static_cast<int>(signs.size()); ++i) {
        const ZodiacSign& s = signs[i];
        if ((month == s.startMonth && day >= s.startDay) ||
            (month == s.endMonth && day <= s.endDay)) {
            return i;
        }
    }
    return 0;
}

void ZodiacModel::fireModelChanged() {
    for (IModelListener* listener : listeners_) {
        listener->modelChanged(*this);
    }
}
