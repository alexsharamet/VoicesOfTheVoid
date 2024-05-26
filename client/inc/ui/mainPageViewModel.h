#pragma once

#include <QObject>
#include <QQuickView>
#include <QString>

#include "mainPageModel.h"

namespace UI {
    class MainPageViewModel : public QObject {
        Q_OBJECT
        Q_PROPERTY(QString input READ input NOTIFY inputChanged)

    Q_SIGNALS:
        void inputChanged();

    public://QML_CALL
        [[nodiscard]] QString input() const;
        Q_INVOKABLE void setMessage(const QString& text);
        Q_INVOKABLE void tuneButtonCicked();
        Q_INVOKABLE void boostButtonClicked();

    public:
        explicit MainPageViewModel(QQuickView &view, const QString& name, QObject *parent = nullptr);
        void setModel(MainPageModel *model);

    private:
        QString m_input;

        MainPageModel *m_model{nullptr};
    };
}// namespace UI