#pragma once
#include <QSettings>
#include <QString>

namespace Utils {
    class Config {
    public:
        static Config &instance();

        void load();
        void save();

        int getVersion() const;
        void setVersion(int version);

        QString getId() const;
        void setId(QString id);

        QString getHost() const;
        void setHost(QString host);

        int getPort() const;
        void setPort(int port);

        bool getDebug() const;

    private:
        Config() = default;

    private:
        QSettings *m_settings;
        int m_version;
        QString m_id;
        QString m_host;
        int m_port;
        bool m_debug;
    };
}// namespace Utils