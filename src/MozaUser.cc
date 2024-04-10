//
// Created by Jayce on 2024/3/18.
//

#include "MozaUser.h"

namespace moza::component
{

MozaUser::MozaUser(QObject* parent) : QObject(parent) { }

const QString& MozaUser::getFirstName() const { return m_firstName; }

void MozaUser::setFirstName(const QString& newFirstName)
{
    if (m_firstName == newFirstName)
        return;
    m_firstName = newFirstName;
    emit firstNameChanged();
}

const QString& MozaUser::getLastName() const { return m_lastName; }

void MozaUser::setLastName(const QString& newLastName)
{
    if (m_lastName == newLastName)
        return;
    m_lastName = newLastName;
    emit lastNameChanged();
}

const QString& MozaUser::getNickname() const { return m_nickname; }

void MozaUser::setNickname(const QString& newNickname)
{
    if (m_nickname == newNickname)
        return;
    m_nickname = newNickname;
    emit nicknameChanged();
}

}