#include "logic/network.h"

#include <QJsonDocument>
#include <QJsonObject>

namespace Logic {
    Network::Network(QObject *parent)
        : QObject(parent) {
        m_network = new QNetworkAccessManager(this);
    }

    void Network::createPostRequest(QString pattern, const QJsonObject &body, std::function<void(QString)> handler) {
        auto url = QString("http://") + m_host + QString(":") + QString::number(m_port) + pattern;
        QNetworkRequest request(QUrl{url});
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        auto reply = m_network->post(request, QJsonDocument(body).toJson());
        QObject::connect(reply, &QNetworkReply::finished, this, [this, reply, handler = std::move(handler)] {
            if (reply->error() != QNetworkReply::NoError) {
                handleError(reply);
                return;
            }

            auto content = QString::fromUtf8(reply->readAll());
            reply->deleteLater();
            handler(content);
        });
    }

    void Network::setConnection(QString host, int port) {
        m_host = host;
        m_port = port;
    }

    void Network::handleError(QNetworkReply *reply) {
        auto err = reply->errorString();
        reply->deleteLater();
        Q_EMIT gotNetworkError(err);
    }

    void Network::registerClient(const QString &name) {
        QJsonObject body;
        body["name"] = name;

        createPostRequest("/register", body, [this, name](const QString &body) {
            auto obj = QJsonDocument::fromJson(body.toUtf8()).object();
            bool res = obj["answer"].toBool();

            Q_EMIT gotRegister(name, res);
        });
    }

    void Network::authClient(const QString &name) {
        QJsonObject body;
        body["name"] = name;

        createPostRequest("/auth", body, [this, name](const QString &body) {
            auto obj = QJsonDocument::fromJson(body.toUtf8()).object();
            bool res = obj["answer"].toBool();

            Q_EMIT gotAuth(name, res);
        });
    }

    void Network::send(const QString &name, const QString &instruction) {
        QJsonObject body;
        body["usr"] = name;
        body["instruction"] = instruction;

        createPostRequest("/send", body, [this, name](const QString &body) {
            auto obj = QJsonDocument::fromJson(body.toUtf8()).object();
            auto res = obj["answer"].toString();

            Q_EMIT gotSend(res);
        });
    }
}// namespace Logic