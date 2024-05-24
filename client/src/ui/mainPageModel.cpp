#include <utility>

#include "ui/mainPageModel.h"

#include "logic/network.h"
#include "utils/config.h"

namespace UI {
    MainPageModel::MainPageModel(Logic::Network *network)
        : m_network(network) {
        connect(m_network.get(), &Logic::Network::gotRegister, this, &UI::MainPageModel::onRegister);
        connect(m_network.get(), &Logic::Network::gotAuth, this, &UI::MainPageModel::onAuth);
        connect(m_network.get(), &Logic::Network::gotSend, this, &UI::MainPageModel::gotMessage);
    }

    void MainPageModel::auth() {
        auto id = Utils::Config::instance().getId();
        if (id.isNull()) {
            registerUser();
        } else {
            authUser(id);
        }
    }

    void MainPageModel::registerUser() {
        if (m_network.isNull()) {
            return;
        }
        m_network->registerClient(QUuid::createUuid().toString());
    }

    void MainPageModel::authUser(const QString &name) {
        if (m_network.isNull()) {
            return;
        }
        m_network->authClient(name);
    }

    void MainPageModel::onAuth(QString name, bool success) {
        if (!success) {
            qDebug() << "auth failed";
            Utils::Config::instance().setId(nullptr);
            Utils::Config::instance().save();
            //TODO: emit current history drop
            registerUser();
            return;
        }

        m_id = std::move(name);
        qDebug() << "auth successful";
    }

    void MainPageModel::onRegister(QString name, bool success) {
        if (!success) {
            qDebug() << "register retry";
            registerUser();
            return;
        }

        m_id = std::move(name);
        Utils::Config::instance().setId(m_id);
        Utils::Config::instance().save();

        qDebug() << "register successful";
    }

    void MainPageModel::message(QString text) {
        //TODO: check state emit errors
        m_network->send(m_id, text);
    }
}// namespace UI