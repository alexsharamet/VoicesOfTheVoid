#include <utility>

#include "ui/mainPageModel.h"

#include "logic/network.h"
#include "utils/config.h"

namespace UI {
    MainPageModel::MainPageModel(Logic::Network *network)
        : m_network(network) {

        auto unsupportableError = [this](const QString &text) {
            qDebug() << "Unsupportable error: " << text;
            Q_EMIT gotError("Network error: check connection and reopen app \nErrorType: " + text);
        };
        connect(m_network.get(), &Logic::Network::gotNetworkError, this, [unsupportableError](const QString &error) {
            qDebug() << QString("Network error: ") + error;
            unsupportableError("Network error");
        });
        connect(m_network.get(), &Logic::Network::gotWrongResponseFormat, this, [unsupportableError] {
            qDebug() << "Wrong response format";
            unsupportableError("Choose another server in settings");
        });
        connect(m_network.get(), &Logic::Network::gotUnknownError, this, [unsupportableError] {
            qDebug() << "Unknown error";
            unsupportableError("Choose another server in settings");
        });
        connect(m_network.get(), &Logic::Network::gotWrongRequestsArgs, this, [unsupportableError] {
            qDebug() << "Wrong requests args";
            unsupportableError("Choose another server in settings");
        });
        connect(m_network.get(), &Logic::Network::gotWrongServerVersion, this, [unsupportableError] {
            qDebug() << "Wrong server version";
            unsupportableError("Choose another server in settings");
        });

        connect(m_network.get(), &Logic::Network::gotUserIsNotExist, this, &UI::MainPageModel::onUserIsNotExist);
        connect(m_network.get(), &Logic::Network::gotNameIsAlreadyExist, this, &UI::MainPageModel::onNameIsAlreadyExist);
        connect(m_network.get(), &Logic::Network::gotUserIsBusy, this, &UI::MainPageModel::onUserIsBusy);

        connect(m_network.get(), &Logic::Network::gotRegister, this, &UI::MainPageModel::onRegister);
        connect(m_network.get(), &Logic::Network::gotAuth, this, &UI::MainPageModel::onAuth);
        connect(m_network.get(), &Logic::Network::gotSend, this, &UI::MainPageModel::onSend);
        connect(m_network.get(), &Logic::Network::gotTune, this, &UI::MainPageModel::onTune);
        connect(m_network.get(), &Logic::Network::gotBoost, this, &UI::MainPageModel::onBoost);
    }

    void MainPageModel::auth() {
        auto id = Utils::Config::instance().getId();
        if (id.isNull()) {
            registerUser();
        } else {
            authUser(id);
        }
    }

    bool MainPageModel::validateState(bool existId) {
        if (m_network.isNull()) {
            return false;
        }

        if (existId && m_id.isEmpty()) {
            return false;
        }

        if (m_currentAction != Action::None) {
            return false;
        }

        return true;
    }

    void MainPageModel::registerUser() {
        if (!validateState(false)) {
            return;
        }
        m_currentAction = Action::Register;
        m_network->registerClient(QUuid::createUuid().toString(), Utils::Config::instance().getVersion());
    }

    void MainPageModel::authUser(const QString &name) {
        if (!validateState(false)) {
            return;
        }
        m_currentAction = Action::Auth;
        m_network->authClient(name, Utils::Config::instance().getVersion());
    }

    void MainPageModel::message(const QString &text) {
        if (!validateState()) {
            return;
        }
        m_currentAction = Action::Send;
        m_network->send(m_id, text);
    }

    void MainPageModel::tune() {
        if (!validateState()) {
            return;
        }
        m_currentAction = Action::Tune;
        m_network->tune(m_id);
    }

    void MainPageModel::boost() {
        if (!validateState()) {
            return;
        }
        m_currentAction = Action::Boost;
        m_network->boost(m_id);
    }

    bool MainPageModel::getDebug() const {
       return Utils::Config::instance().getDebug();
    }

    void MainPageModel::onUserIsNotExist() {
        auto action = m_currentAction;
        m_currentAction = Action::None;
        switch (action) {
            case Action::Auth: {
                qDebug() << "auth failed";
                Utils::Config::instance().setId(nullptr);
                Utils::Config::instance().save();
                //TODO: emit current history drop
                registerUser();
                break;
            }
            case Action::Register:
                qDebug() << "response in wrong state";
                break;
            default: {
                qDebug() << "reopen app";
            }
        }
    }

    void MainPageModel::onNameIsAlreadyExist() {
        auto action = m_currentAction;
        m_currentAction = Action::None;
        switch (action) {
            case Action::Register: {
                qDebug() << "register retry";
                registerUser();
                break;
            }
            default: {
                Q_EMIT gotError("Error occurred: reopen app");
            }
        }
    }

    void MainPageModel::onUserIsBusy() {
        auto action = m_currentAction;
        m_currentAction = Action::None;
    }

    void MainPageModel::onAuth(QString name, StrategyType type) {
        m_id = std::move(name);
        m_currentAction = Action::None;
        qDebug() << "auth successful";
        Q_EMIT gotTune(QString::fromStdString(toString(type)));
    }

    void MainPageModel::onRegister(QString name, StrategyType type) {
        m_id = std::move(name);
        Utils::Config::instance().setId(m_id);
        Utils::Config::instance().save();

        m_currentAction = Action::None;
        qDebug() << "register successful";

        Q_EMIT gotTune(QString::fromStdString(toString(type)));
    }

    void MainPageModel::onSend(QString answer, bool finished) {
        m_currentAction = Action::None;
        Q_EMIT gotMessage(std::move(answer));
        if (finished) {
            Q_EMIT gotFinished();
        }
    }

    void MainPageModel::onTune(StrategyType type) {
        m_currentAction = Action::None;
        Q_EMIT gotTune(QString::fromStdString(toString(type)));
    }

    void MainPageModel::onBoost() {
        m_currentAction = Action::None;
        Q_EMIT gotBoost();
    }
}// namespace UI