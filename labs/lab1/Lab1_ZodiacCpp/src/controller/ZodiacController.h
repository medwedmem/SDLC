#pragma once

#include <string>

class ZodiacModel;
class MainWindow;

// Контроллер: обрабатывает действия пользователя, проверяет введенные
// данные и изменяет модель. Представление контроллер не обновляет –
// при активной модели это делает сама модель через механизм уведомлений.
class ZodiacController {
public:
    ZodiacController(ZodiacModel& model, MainWindow& view);

    // Преобразует текст в целое число; при ошибке выбрасывает std::invalid_argument.
    static int parseNumber(const std::wstring& text, const char* field);

private:
    void onInputData();  // обработчик кнопки «Узнать свой знак Зодиака»

    ZodiacModel& model_;
    MainWindow& view_;
};
