//
// Created by Jayce on 2024/3/18.
//

#pragma once

#include <QObject>
#include <QString>
#include <QtQml>

namespace moza::component
{

class MozaUser : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString firstName READ getFirstName WRITE setFirstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString lastName READ getLastName WRITE setLastName NOTIFY lastNameChanged)
    Q_PROPERTY(QString nickname READ getNickname WRITE setNickname NOTIFY nicknameChanged)
public:
    explicit MozaUser(QObject* parent = nullptr);

    [[nodiscard]] const QString& getFirstName() const;
    void setFirstName(const QString& newFirstName);

    [[nodiscard]] const QString& getLastName() const;
    void setLastName(const QString& newLastName);

    [[nodiscard]] const QString& getNickname() const;
    void setNickname(const QString& newNickname);

signals:
    void firstNameChanged();
    void lastNameChanged();
    void nicknameChanged();

private:
    QString m_firstName;
    QString m_lastName;
    QString m_nickname;
};

}