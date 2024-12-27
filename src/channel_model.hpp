#pragma once

#include <QtWidgets>

#include "epg.hpp"

class ChannelModel : public QAbstractTableModel {
  private:
    const Epg epg;

  public:
    ChannelModel(const Epg epg);

    int rowCount(const QModelIndex&) const;
    int columnCount(const QModelIndex&) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
};
