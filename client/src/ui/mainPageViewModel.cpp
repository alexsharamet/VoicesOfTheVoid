#include "ui/mainPageViewModel.h"
#include <QQmlContext>

namespace UI {
    MainPageViewModel::MainPageViewModel(QQuickView &view, QString name, QObject *parent)
        : QObject(parent) {
        view.rootContext()->setContextProperty(name, this);
    }

    QString MainPageViewModel::input() const {
        return m_input;
    }

    void MainPageViewModel::setInput(const QString &text) {
        m_input = text;
        Q_EMIT inputChanged();
    }

    QString MainPageViewModel::output() const {
        return m_output;
    }

    void MainPageViewModel::setOutput(const QString &text) {
        m_output = text;
        Q_EMIT outputChanged();
    }
}// namespace UI