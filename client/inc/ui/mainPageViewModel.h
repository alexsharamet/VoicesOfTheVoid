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
        Q_PROPERTY(bool debug READ debug)

    Q_SIGNALS:
        void openDialog(QString);
        void inputChanged();
        void strategyChanged();
        void changeSpinnerState(bool running);

    public://QML_CALL
        [[nodiscard]] QString input() const;
        [[nodiscard]] QString strategy() const;
        [[nodiscard]] bool debug() const;
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

        MainPageModel *m_model{nullptr};
    };
}// namespace UI