#pragma once

#include <vector>

#include "IModelListener.h"
#include "ZodiacSign.h"

// Активная модель: хранит дату рождения, определяет знак Зодиака
// и сама уведомляет подписанные представления об изменениях.
class ZodiacModel {
public:
    static constexpr int MIN_YEAR = 1900;

    void addListener(IModelListener* listener);
    void removeListener(IModelListener* listener);

    // Проверяет дату рождения, определяет знак и оповещает наблюдателей.
    // При недопустимой дате выбрасывает std::invalid_argument.
    void setBirthDate(int day, int month, int year);

    bool isCalculated() const { return calculated_; }
    int day() const { return day_; }
    int month() const { return month_; }
    int year() const { return year_; }
    const ZodiacSign& sign() const { return zodiacSigns()[signIndex_]; }

    static int daysInMonth(int month, int year);

private:
    static int findSign(int day, int month);
    void fireModelChanged();

    std::vector<IModelListener*> listeners_;
    int day_ = 0, month_ = 0, year_ = 0;
    int signIndex_ = 0;
    bool calculated_ = false;
};
