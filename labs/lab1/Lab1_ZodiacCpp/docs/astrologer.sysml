package Astrologer {

    doc /*
     * Модель системы «Астролог» (вариант 12)
     * в текстовой нотации SysML v2.
     */

    private import ScalarValues::*;

    attribute def InputData {
        attribute dayText : String;
        attribute monthText : String;
        attribute yearText : String;
    }

    attribute def ZodiacSign {
        attribute name : String;
        attribute element : String;
        attribute planet : String;
        attribute description : String;
    }

    port def ModelChangedPort {
        out item modelChanged;
    }

    part def ZodiacModel {
        attribute day : Integer;
        attribute month : Integer;
        attribute year : Integer;
        attribute signs : ZodiacSign[12];
        port notify : ModelChangedPort;
    }

    part def MainWindow {
        attribute lastInput : InputData;
        port observe : ~ModelChangedPort;
    }

    part def InputDialog;

    part def ZodiacController;

    part def Application {
        part zodiacModel : ZodiacModel;
        part mainWindow : MainWindow;
        part inputDialog : InputDialog;
        part zodiacController : ZodiacController;

        // Активная модель: уведомление идет от модели к представлению.
        connect zodiacModel.notify to mainWindow.observe;
    }

    action def DetermineZodiacSign {
        in input : InputData;

        first start;
        then action parse;        // преобразовать текст в числа
        then action validate;     // проверить существование даты
        then action findSign;     // определить знак Зодиака
        then action notify;       // оповестить наблюдателей
        then done;
    }

    part variant12 : Application;
}
