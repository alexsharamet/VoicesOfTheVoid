#pragma once

#include <strategies.h>

#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>

namespace Logic {
    class Network : public QObject {
        Q_OBJECT

    Q_SIGNALS:
        void gotNetworkError(QString);
        void gotWrongServerVersion();
        void gotWrongResponseFormat();
        void gotUnknownError();
        void gotWrongRequestsArgs();
        void gotUserIsNotExist();
        void gotNameIsAlreadyExist();
        void gotUserIsBusy();

        void gotRegister(QString id, StrategyType type);
        void gotAuth(QString id, StrategyType type);
        void gotSend(QString answer);
        void gotTune(StrategyType type);
        void gotBoost();

    public:
        explicit Network(QObject *parent = nullptr);
        void setConnection(QString host, int port);

    public://Requests
        void registerClient(const QString &name, int version);
        void authClient(const QString &name, int version);
        void send(const QString &name, const QString &instruction);
        void tune(const QString &name);
        void boost(const QString &name);

    private:
        bool handleServerError(const QString &body);
        void handleError(QNetworkReply *reply);
        void createPostRequest(QString pattern, const QJsonObject &body, std::function<void(QString)> handler);

    private:
        QString m_host;
        int m_port;
        QNetworkAccessManager *m_network;
    };
}// namespace Logic