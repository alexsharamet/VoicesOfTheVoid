#include "utils/config.h"

#include <QUuid>

namespace Utils {
    Config &Config::instance() {
        static Config config;
        return config;
    }

    void Config::load() {
        if (m_settings) {
            return;
        }

        m_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "VoicesOfTheVoid", "client");
        auto id = m_settings->value("id");
        if (id.isNull()) {
            m_id = QString();
        } else {
            m_id = id.toString();
        }
        auto version = m_settings->value("version");
        if (version.isNull()) {
            m_version = 0;
        } else {
            m_version = version.toInt();
        }
        m_host = m_settings->value("host").toString();
        m_port = m_settings->value("port").toInt();
    }

    void Config::save() {
        m_settings->setValue("version", m_version);
        m_settings->setValue("id", m_id);
        m_settings->setValue("host", m_host);
        m_settings->setValue("port", m_port);
        m_settings->sync();
    }

    int Config::getVersion() const {
        return m_version;
    }

    void Config::setVersion(int version) {
        m_version = version;
    }

    QString Config::getId() const {
        return m_id;
    }

    void Config::setId(QString id) {
        m_id = id;
    }

    QString Config::getHost() const {
        return m_host;
    }

    void Config::setHost(QString host) {
        m_host = host;
    }

    int Config::getPort() const {
        return m_port;
    }

    void Config::setPort(int port) {
        m_port = port;
    }

}// namespace Utils