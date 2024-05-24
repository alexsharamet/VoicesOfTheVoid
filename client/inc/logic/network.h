#pragma once

#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>

namespace Logic {
    class Network : public QObject {
        Q_OBJECT

    Q_SIGNALS:
        void gotNetworkError(QString);

        void gotRegister(QString id, bool success);
        void gotAuth(QString id, bool success);
        void gotSend(QString answer);

    public:
        explicit Network(QObject *parent = nullptr);
        void setConnection(QString host, int port);

    public://Requests
        void registerClient(const QString &name);
        void authClient(const QString &name);
        void send(const QString& name, const QString& instruction);

    private:
        void handleError(QNetworkReply *reply);
        void createPostRequest(QString pattern, const QJsonObject &body, std::function<void(QString)> handler);

    private:
        QString m_host;
        int m_port;
        QNetworkAccessManager *m_network;
    };
}// namespace Logic