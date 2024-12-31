#pragma once

#include <QtWidgets>

#include "epg.hpp"

class EpgModel : public QAbstractTableModel {
  private:
    const Epg epg;

  public:
    EpgModel(const Epg epg);

    int rowCount(const QModelIndex&) const override;
    int columnCount(const QModelIndex&) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role) const override;
};
