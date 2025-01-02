#include "epg_model.hpp"

#include <iostream>

EpgModel::EpgModel(const Epg epg) : epg(epg) { }

int EpgModel::rowCount(const QModelIndex& parent) const {
    return epg.channel_vec.size();
}

int EpgModel::columnCount(const QModelIndex& parent) const {
    return 200;
}

QVariant EpgModel::headerData(int section,
        Qt::Orientation orientation,
        int role = Qt::DisplayRole) const {

    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Vertical) {
            return QString::fromStdString(epg.channel_vec[section]->num_string());
        } else if (orientation == Qt::Horizontal) {
            std::time_t result = std::time(nullptr);
            std::tm *tm = std::localtime(&result);
            tm->tm_min = (section + (tm->tm_min / 30)) * 30;
            tm->tm_sec = 0;
            result = std::mktime(tm);

            std::ostringstream time_string;
            time_string << std::put_time(tm, "%H:%M %p");

            return QString::fromStdString(time_string.str());
        }
    }

    return QVariant();
}

QVariant EpgModel::data(const QModelIndex &index, int role = Qt::DisplayRole) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.column() >= epg.channel_vec[index.row()]->programs.size()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        return QString::fromStdString(epg.channel_vec[index.row()]->programs[index.column()].title);
    }

    if (role == Qt::UserRole) {
        return epg.channel_vec[index.row()]->programs[index.column()].duration;
    }

    if (role == Qt::UserRole + 1) {
        return epg.channel_vec[index.row()]->programs[index.column()].int_start;
    }

    return QVariant();
}
