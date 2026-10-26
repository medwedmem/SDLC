#include <windows.h>

#include "controller/ZodiacController.h"
#include "model/ZodiacModel.h"
#include "view/MainWindow.h"

// Точка входа: создание и связывание компонентов MVC.
int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int showCommand) {
    ZodiacModel model;
    MainWindow view;
    if (!view.create(instance, showCommand)) {
        return 1;
    }
    model.addListener(&view);  // представление подписывается на активную модель
    ZodiacController controller(model, view);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        if (!IsDialogMessageW(view.handle(), &msg)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
    return static_cast<int>(msg.wParam);
}
