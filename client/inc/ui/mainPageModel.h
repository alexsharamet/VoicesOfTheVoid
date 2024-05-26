#pragma once

#include <QObject>
#include <QPointer>
#include <QString>

namespace Logic {
    class Network;
}

namespace UI {
    enum class Action {
        None,
        Auth,
        Register,
        Send,
        Tune,
        Boost
    };

    class MainPageModel : public QObject {
        Q_OBJECT

    Q_SIGNALS:
        void gotMessage(QString);
        void gotTune();
        void gotBoost();

    public:
        explicit MainPageModel(Logic::Network *network);

        void auth();
        void message(const QString &text);
        void tune();
        void boost();

    private:
        [[nodiscard]] bool validateState(bool existId = true);
        void registerUser();
        void authUser(const QString &name);

    private:
        void onAuth(QString name);
        void onRegister(QString name);
        void onSend(QString answer);
        void onTune();
        void onBoost();

    private://NETWORK errors
        void onUserIsNotExist();
        void onNameIsAlreadyExist();
        void onUserIsBusy();

    private:
        Action m_currentAction{Action::None};
        QPointer<Logic::Network> m_network;
        QString m_id;
    };
}// namespace UI