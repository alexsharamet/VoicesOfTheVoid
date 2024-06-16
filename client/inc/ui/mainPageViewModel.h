#pragma once

#include <QObject>
#include <QQuickView>
#include <QString>

#include "mainPageModel.h"

namespace UI {
    class MainPageViewModel : public QObject {
        Q_OBJECT
        Q_PROPERTY(QString input READ input NOTIFY inputChanged)
        Q_PROPERTY(QString strategy READ strategy NOTIFY strategyChanged)
        Q_PROPERTY(QString error READ error NOTIFY errorChanged)

    Q_SIGNALS:
        void openDialog();
        void errorChanged();
        void inputChanged();
        void strategyChanged();
        void changeSpinnerState(bool running);

    public://QML_CALL
        [[nodiscard]] QString error() const;
        [[nodiscard]] QString input() const;
        [[nodiscard]] QString strategy() const;
        Q_INVOKABLE void setMessage(const QString &text);
        Q_INVOKABLE void tuneButtonCicked();
        Q_INVOKABLE void boostButtonClicked();

    public:
        explicit MainPageViewModel(QQuickView &view, const QString &name, QObject *parent = nullptr);
        void setModel(MainPageModel *model);
        void showErrorDialog(QString text);

    private:
        QString m_input;
        QString m_strategy;
        QString m_error;

        MainPageModel *m_model{nullptr};
    };
}// namespace UI