#include "ui/mainPageViewModel.h"
#include <QQmlContext>
#include <utility>

namespace UI {
    MainPageViewModel::MainPageViewModel(QQuickView &view, const QString &name, QObject *parent)
        : QObject(parent) {
        view.rootContext()->setContextProperty(name, this);
    }

    void MainPageViewModel::setModel(MainPageModel *model) {
        if (m_model) {
            return;
        }
        m_model = model;
        connect(model, &UI::MainPageModel::gotMessage, this, [this](QString text) {
            m_input = std::move(text);
            Q_EMIT inputChanged();
            Q_EMIT changeSpinnerState(false);
        });
        connect(model, &UI::MainPageModel::gotTune, this, [this](QString strategy) {
            m_strategy = std::move(strategy);
            Q_EMIT strategyChanged();
            Q_EMIT changeSpinnerState(false);
        });
        connect(model, &UI::MainPageModel::gotBoost, this, [this] {
            m_input = "Boost";
            Q_EMIT inputChanged();
            Q_EMIT changeSpinnerState(false);
        });
        connect(model, &UI::MainPageModel::gotError, this, &MainPageViewModel::showErrorDialog);
        connect(model, &UI::MainPageModel::gotFinished, this, [this] {
            Q_EMIT openDialog("Game finished");
            tuneButtonCicked();
        });
    }

    void MainPageViewModel::showErrorDialog(QString text) {
        Q_EMIT openDialog(std::move(text));
    }

    QString MainPageViewModel::input() const {
        return m_input;
    }

    QString MainPageViewModel::strategy() const {
        return m_strategy;
    }

    bool MainPageViewModel::debug() const {
        return m_model->getDebug();
    }

    void MainPageViewModel::setMessage(const QString &text) {
        Q_EMIT changeSpinnerState(true);
        m_model->message(text);
        qDebug() << "On Send clicked";
    }

    void MainPageViewModel::tuneButtonCicked() {
        Q_EMIT changeSpinnerState(true);
        m_model->tune();
        qDebug() << "On Tune clicked";
    }

    void MainPageViewModel::boostButtonClicked() {
        Q_EMIT changeSpinnerState(true);
        m_model->boost();
        qDebug() << "On Boost clicked";
    }
}// namespace UI