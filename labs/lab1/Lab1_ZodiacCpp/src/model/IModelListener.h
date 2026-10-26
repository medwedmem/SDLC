#pragma once

class ZodiacModel;

// Интерфейс наблюдателя активной модели. Представление реализует его,
// чтобы получать уведомления об изменении состояния модели.
class IModelListener {
public:
    virtual ~IModelListener() = default;
    virtual void modelChanged(const ZodiacModel& model) = 0;
};
