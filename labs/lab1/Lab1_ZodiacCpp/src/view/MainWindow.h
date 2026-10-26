#pragma once

#include <windows.h>

#include <functional>
#include <optional>
#include <string>

#include "../model/IModelListener.h"
#include "InputDialog.h"

// Главная форма. Является наблюдателем активной модели: при изменении
// модели сама считывает из нее данные и обновляет метки.
class MainWindow : public IModelListener {
public:
    bool create(HINSTANCE instance, int showCommand);
    HWND handle() const { return hwnd_; }

    void setInputAction(std::function<void()> action) { inputAction_ = std::move(action); }
    const std::optional<InputData>& lastInput() const { return lastInput_; }
    void setLastInput(const InputData& input) { lastInput_ = input; }
    void showError(const std::wstring& message) const;

    void modelChanged(const ZodiacModel& model) override;

private:
    static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT handleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    void createControls();

    HWND hwnd_ = nullptr;
    HWND inputButton_ = nullptr;
    HWND dateLabel_ = nullptr;
    HWND signLabel_ = nullptr;
    HWND periodLabel_ = nullptr;
    HWND elementLabel_ = nullptr;
    HWND planetLabel_ = nullptr;
    HWND descriptionLabel_ = nullptr;
    std::function<void()> inputAction_;
    // Последние введенные данные хранятся в главной форме.
    std::optional<InputData> lastInput_;
};
