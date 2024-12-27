#include "channel_model.hpp"

ChannelModel::ChannelModel(const Epg epg) : epg(epg) { }

int ChannelModel::rowCount(const QModelIndex& parent = QModelIndex()) const {
    return epg.channel_vec.size();
}

int ChannelModel::columnCount(const QModelIndex& parent = QModelIndex()) const {
    return 10;
}

QVariant ChannelModel::headerData(int section,
        Qt::Orientation orientation,
        int role = Qt::DisplayRole) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Vertical) {
            return QString::fromStdString(epg.channel_vec[section]->num_string());
        } else {
            return QVariant(section);
        }
    }

    return QVariant();
}

QVariant ChannelModel::data(const QModelIndex &index, int role = Qt::DisplayRole) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        return QString::fromStdString(epg.channel_vec[index.row()]->programs[index.column()].title);
    }

    return QVariant();
}
