#pragma once

#include <QObject>
#include <QPointer>
#include <QString>

namespace Logic {
    class Network;
}

namespace UI {
    class MainPageModel : public QObject {
        Q_OBJECT

    Q_SIGNALS:
        void gotMessage(QString);

    public:
        explicit MainPageModel(Logic::Network *network);
        void auth();

        void message(QString text);

    private:
        void registerUser();
        void authUser(const QString& name);

    private:
        void onAuth(QString name, bool success);
        void onRegister(QString name, bool success);

    private:
        QPointer<Logic::Network> m_network;
        QString m_id;
        bool m_auth{};
    };
}// namespace UI