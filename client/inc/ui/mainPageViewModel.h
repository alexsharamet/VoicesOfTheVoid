#pragma once

#include <QObject>
#include <QQuickView>
#include <QString>

namespace UI {
    class MainPageViewModel : public QObject {
        Q_OBJECT
        Q_PROPERTY(QString input READ input WRITE setInput NOTIFY inputChanged)
        Q_PROPERTY(QString output READ output WRITE setOutput NOTIFY outputChanged)

    Q_SIGNALS:
        void inputChanged();
        void outputChanged();

    public:
        explicit MainPageViewModel(QQuickView &view, QString name, QObject *parent = nullptr);

        [[nodiscard]] QString input() const;
        void setInput(const QString &text);

        [[nodiscard]] QString output() const;
        void setOutput(const QString &text);

    private:
        QString m_input;
        QString m_output;
    };
}// namespace UI