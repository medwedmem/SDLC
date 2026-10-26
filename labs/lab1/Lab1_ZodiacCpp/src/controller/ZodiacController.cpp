#include "ZodiacController.h"

#include <windows.h>

#include <cwchar>
#include <stdexcept>
#include <string>

#include "../model/ZodiacModel.h"
#include "../view/InputDialog.h"
#include "../view/MainWindow.h"

namespace {
std::wstring fromUtf8(const char* text) {
    const int size = MultiByteToWideChar(CP_UTF8, 0, text, -1, nullptr, 0);
    std::wstring result(size, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, text, -1, result.data(), size);
    result.resize(size - 1);
    return result;
}
}  // namespace

ZodiacController::ZodiacController(ZodiacModel& model, MainWindow& view)
    : model_(model), view_(view) {
    view_.setInputAction([this] { onInputData(); });
}

void ZodiacController::onInputData() {
    std::optional<InputData> input = view_.lastInput();
    while (true) {
        input = InputDialog::show(view_.handle(), input);
        if (!input) {
            return;  // пользователь отменил ввод
        }
        view_.setLastInput(*input);  // запоминаем последние данные в главной форме
        try {
            const int day = parseNumber(input->dayText, "День");
            const int month = parseNumber(input->monthText, "Месяц");
            const int year = parseNumber(input->yearText, "Год");
            model_.setBirthDate(day, month, year);
            return;
        } catch (const std::invalid_argument& ex) {
            view_.showError(fromUtf8(ex.what()));
        }
    }
}

int ZodiacController::parseNumber(const std::wstring& text, const char* field) {
    const size_t first = text.find_first_not_of(L" \t");
    if (first == std::wstring::npos) {
        throw std::invalid_argument(std::string("Поле «") + field + "» не заполнено.");
    }
    const wchar_t* begin = text.c_str() + first;
    wchar_t* end = nullptr;
    const long value = std::wcstol(begin, &end, 10);
    while (end && (*end == L' ' || *end == L'\t')) ++end;
    if (end == begin || *end != L'\0') {
        throw std::invalid_argument(std::string("Поле «") + field + "» должно содержать целое число.");
    }
    return static_cast<int>(value);
}
