#include "logic/network.h"

#include <QJsonDocument>
#include <QJsonObject>

#include "error_codes.h"

namespace Logic {
    Network::Network(QObject *parent)
        : QObject(parent) {
        m_network = new QNetworkAccessManager(this);
        m_network->setTransferTimeout(std::chrono::seconds(6000));//TODO get from config
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
            if (handleServerError(content)) {
                handler(content);
            }
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

    bool Network::handleServerError(const QString &body) {
        const auto doc = QJsonDocument::fromJson(body.toUtf8());
        if (!doc.isObject()) {
            Q_EMIT gotWrongResponseFormat();
            return false;
        }
        const auto obj = doc.object();
        if (!obj.contains("error")) {
            return true;//NO ERROR
        }

        const auto error = obj["error"].toInt(-1);
        if (error == -1) {
            Q_EMIT gotUnknownError();
            return false;
        }

        const auto err = static_cast<ERROR_CODE>(error);
        switch (err) {
            case ERROR_CODE::UNKNOWN:
                Q_EMIT gotUnknownError();
                break;
            case ERROR_CODE::OK:
                //NO Error
                break;
            case ERROR_CODE::WRONG_REQUEST_ARGS:
                Q_EMIT gotWrongRequestsArgs();
                break;
            case ERROR_CODE::USER_IS_NOT_EXIST:
                Q_EMIT gotUserIsNotExist();
                break;
            case ERROR_CODE::THIS_NAME_IS_ALREADY_EXIST:
                Q_EMIT gotNameIsAlreadyExist();
                break;
            case ERROR_CODE::USER_IS_BUSY:
                Q_EMIT gotUserIsBusy();
                break;
            case ERROR_CODE::WRONG_CLIENT_VERSION:
                Q_EMIT gotWrongServerVersion();
                break;
        }

        return false;
    }

    void Network::registerClient(const QString &name, int version) {
        QJsonObject body;
        body["id"] = name;
        body["name"] = name;
        body["version"] = version;

        createPostRequest("/register", body, [this, name](const QString &body) {
            auto obj = QJsonDocument::fromJson(body.toUtf8()).object();
            auto res = obj["response"].toObject();
            auto strategy = res["strategy"].toInt();
            Q_EMIT gotRegister(name, static_cast<StrategyType>(strategy));
        });
    }

    void Network::authClient(const QString &name, int version) {
        QJsonObject body;
        body["id"] = name;
        body["version"] = version;

        createPostRequest("/auth", body, [this, name](const QString &body) {
            auto obj = QJsonDocument::fromJson(body.toUtf8()).object();
            auto res = obj["response"].toObject();
            auto strategy = res["strategy"].toInt();
            Q_EMIT gotAuth(name, static_cast<StrategyType>(strategy));
        });
    }

    void Network::send(const QString &name, const QString &instruction) {
        QJsonObject body;
        body["id"] = name;
        body["instruction"] = instruction;

        createPostRequest("/send", body, [this, name](const QString &body) {
            auto obj = QJsonDocument::fromJson(body.toUtf8()).object();
            auto res = obj["response"].toObject();
            QString message = res["message"].toString();
            bool finished = res["finished"].toBool();

            Q_EMIT gotSend(message, finished);
        });
    }

    void Network::tune(const QString &name) {
        QJsonObject body;
        body["id"] = name;

        createPostRequest("/tune", body, [this, name](const QString &body) {
            auto obj = QJsonDocument::fromJson(body.toUtf8()).object();
            auto res = obj["response"].toObject();
            auto strategy = res["strategy"].toInt();
            Q_EMIT gotTune(static_cast<StrategyType>(strategy));
        });
    }

    void Network::boost(const QString &name) {
        QJsonObject body;
        body["id"] = name;

        createPostRequest("/boost", body, [this, name](const QString &) {
            Q_EMIT gotBoost();
        });
    }
}// namespace Logic