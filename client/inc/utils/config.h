#pragma once
#include <QSettings>
#include <QString>

namespace Utils {
    class Config {
    public:
        static Config &instance();

        void load();
        void save();

        QString getId() const;
        void setId(QString id);

        QString getHost() const;
        void setHost(QString host);

        int getPort() const;
        void setPort(int port);

    private:
        Config() = default;

    private:
        QSettings* m_settings;
        QString m_id;
        QString m_host;
        int m_port;
    };
}// namespace Utils